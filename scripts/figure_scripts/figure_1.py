#!/usr/bin/env python
# coding: utf-8

from datetime import datetime
from general.util import *
from general.create_permutations_with_selections import createPermutationFunctions

import os
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
import itertools


def main(expRootFolder):
    # Setting all experiment parameters
    NUM_TUPLES = [[350 * 1000 * 1000, 10 * 1000 * 1000, 10 * 1000 * 1000, 10 * 1000 * 1000, 10 * 1000 * 1000]]

    JOIN_TYPES = [[0, 1, 2, 0]]  # The last value will be ignored as we only run three joins
    NUMBER_OF_JOINS = 3
    NUM_REPS_PER_EXPERIMENT = 10
    MAX_VALUE = [1e4]
    WITH_REORDERING = False
    SAMPLE_SEQ = "PAPI_TCM"
    SELECTIVITIES = [[0.22, 0.22, 0.22, 0.22]]

    sourceCodeFldr = os.path.abspath(os.path.join(__file__, "../../../source_code"))
    releaseFldr = os.path.abspath(os.path.join(expRootFolder, "cmake-build-release"))
    EXECUTABLE = os.path.abspath(os.path.join(releaseFldr, "apaj"))
    PATH_TO_PAPI = os.path.abspath(os.path.join(__file__, "../../../source_code/include/Papi"))
    PATH_TO_TRAINED_ML_MODEL = "" # We can ignore the path, as we do not require the ml model

    # Creating all permutation functions for 3 joins and also compiling the project
    createPermutationFunctions(NUMBER_OF_JOINS)
    compileProject(sourceCodeFldr, releaseFldr)

    # Creating file names and writing header to the csv file
    currDateTimeStr = datetime.now().strftime('%Y%m%d_%H%M%S')
    prologStr = f"figure_1_{currDateTimeStr}"

    # Running the experiment and joining the two csv files on the experiment id
    allExperimentParams = list(itertools.product(NUM_TUPLES, JOIN_TYPES, MAX_VALUE, SELECTIVITIES))
    csvFileJoined, figureFilename = runExperiments(allExperimentParams, NUM_REPS_PER_EXPERIMENT, WITH_REORDERING, SAMPLE_SEQ,
                                                   EXECUTABLE, PATH_TO_PAPI, PATH_TO_TRAINED_ML_MODEL, expRootFolder, prologStr)

    # After we have run some experiments, we remove the build folder to clean up after ourselves
    removeFolder(releaseFldr)

    # Plotting from the csv file similar to Figure 1 in the paper
    sns.set()
    data = pd.read_csv(csvFileJoined)
    data['EXEC_TIME_S'] = data['EXEC_TIME_MS'] / 1000

    colors = ["purple", "purple", "green", "green", "orange", "orange"]
    hatch_pattern = ['/', '/', '\\', '\\', '-', '-']

    plt.figure(figsize=(8, 4))
    sns.barplot(x='PERMUTATION', y='EXEC_TIME_S', data=data,
                palette=colors, hatch=hatch_pattern, width=0.5)
    plt.ylabel('Execution time in s')
    plt.xticks(range(6), ["Permu-\ntation 1", "Permu-\ntation 2", "Permu-\ntation 3",
                          "Permu-\ntation 4", "Permu-\ntation 5", "Permu-\ntation 6"])

    legend_labels = ['Repeated', 'Sequential', 'Random']
    legend_patches = [plt.Rectangle((0, 0), 1, 1, fc=color) for color in colors[::2]]
    plt.legend(legend_patches, legend_labels, loc='upper center', bbox_to_anchor=(0.5, 1.15), ncol=3)
    plt.savefig(figureFilename)
