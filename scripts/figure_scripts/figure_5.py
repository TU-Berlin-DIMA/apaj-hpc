#!/usr/bin/env python
# coding: utf-8
from datetime import datetime
from general.util import *
from general.create_permutations_with_selections import createPermutationFunctions

import os
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
import random
import itertools

# Maps the integer to a string representaition
def mapIntToJoinTypeStr(x):
    if x == 0:
        return "Repeated"
    elif x == 1:
        return "Sequential"
    elif x == 2:
        return "Random"
    elif x == 3:
        return "Pseudo-Random in L1"
    elif x == 4:
        return "Pseudo-Random in L2"
    elif x == 5:
        return "Pseudo-Random in L3"
    else:
        return "Error occured!"


def main(expRootFolder):
    # Setting all experiment parameters
    NUM_TUPLES = [[350 * 1000 * 1000, 10 * 1000 * 1000, 10 * 1000 * 1000, 10 * 1000 * 1000, 10 * 1000 * 1000]]

    # For each item, the last value is ignored as we only run three joins
    JOIN_TYPES = [[0, 1, 1, 0], [0, 5, 5, 0], [2, 3, 5, 0], [0, 4, 5, 0],
                  [2, 2, 5, 0], [0, 1, 2, 0], [1, 4, 4, 0], [0, 4, 4, 0],
                  [2, 2, 4, 0], [0, 1, 3, 0], [1, 4, 5, 0], [0, 1, 4, 0],
                  [0, 1, 5, 0], [3, 3, 3, 0], [0, 2, 3, 0], [2, 3, 3, 0],
                  [3, 3, 5, 0], [0, 2, 2, 0], [0, 0, 5, 0], [5, 5, 5, 0],
                  [3, 3, 4, 0], [0, 2, 5, 0], [0, 0, 4, 0], [2, 4, 5, 0],
                  [0, 2, 4, 0], [0, 0, 3, 0], [2, 4, 4, 0], [0, 3, 3, 0],
                  [0, 0, 2, 0], [1, 2, 2, 0], [0, 0, 1, 0], [1, 2, 3, 0],
                  [0, 0, 0, 0], [4, 5, 5, 0], [1, 2, 4, 0], [1, 1, 1, 0],
                  [1, 5, 5, 0], [2, 2, 3, 0], [1, 2, 5, 0], [2, 2, 2, 0],
                  [0, 3, 4, 0], [1, 1, 3, 0], [4, 4, 5, 0], [0, 3, 5, 0],
                  [2, 5, 5, 0], [1, 1, 2, 0], [1, 3, 5, 0], [4, 4, 4, 0],
                  [3, 4, 4, 0], [1, 1, 5, 0], [1, 3, 4, 0], [3, 4, 5, 0],
                  [2, 3, 4, 0], [3, 5, 5, 0], [1, 1, 4, 0], [1, 3, 3, 0]]

    NUMBER_OF_JOINS = 3
    NUM_REPS_PER_EXPERIMENT = 10
    MAX_VALUE = [1e4]
    WITH_REORDERING = False
    SAMPLE_SEQ = "PAPI_TCM"
    SELECTIVITIES = [[0.01] * NUMBER_OF_JOINS, [0.11] * NUMBER_OF_JOINS, [0.22] * NUMBER_OF_JOINS,
                     [0.33] * NUMBER_OF_JOINS, [0.44] * NUMBER_OF_JOINS, [0.55] * NUMBER_OF_JOINS,
                     [0.66] * NUMBER_OF_JOINS, [0.77] * NUMBER_OF_JOINS, [0.88] * NUMBER_OF_JOINS,
                     [0.99] * NUMBER_OF_JOINS]
    NUM_QUERIES = 100

    # Randomly shuffling the data and then get the first NUM_QUERIES
    random.seed(42)
    allExperimentParams = list(itertools.product(NUM_TUPLES, JOIN_TYPES, MAX_VALUE, SELECTIVITIES))
    random.shuffle(allExperimentParams)
    allExperimentParams = allExperimentParams[:NUM_QUERIES]

    sourceCodeFldr = os.path.abspath(os.path.join(__file__, "../../../source_code"))
    releaseFldr = os.path.abspath(os.path.join(expRootFolder, "cmake-build-release"))
    EXECUTABLE = os.path.abspath(os.path.join(releaseFldr, "apaj"))
    PATH_TO_PAPI = os.path.abspath(os.path.join(__file__, "../../../source_code/include/Papi"))
    PATH_TO_TRAINED_ML_MODEL = ""  # We can ignore the path, as we do not require the ml model

    # Creating all permutation functions for 3 joins and also compiling the project
    createPermutationFunctions(NUMBER_OF_JOINS)
    compileProject(sourceCodeFldr, releaseFldr)

    # Creating file names and writing header to the csv file
    currDateTimeStr = datetime.now().strftime('%Y%m%d_%H%M%S')
    prologStr = f"figure_5_{currDateTimeStr}"

    # Running the experiment and joining the two csv files on the experiment id
    csvFileJoined, figureFilename = runExperiments(allExperimentParams, NUM_REPS_PER_EXPERIMENT, WITH_REORDERING, SAMPLE_SEQ,
                                                   EXECUTABLE, PATH_TO_PAPI, PATH_TO_TRAINED_ML_MODEL, expRootFolder, prologStr)

    # After we have run some experiments, we remove the build folder to clean up after ourselves
    removeFolder(releaseFldr)

    # Plotting from the csv file similar to Figure 5 in the paper
    sns.set()
    data = pd.read_csv(csvFileJoined)

    data['EXEC_TIME_S'] = data['EXEC_TIME_MS'] / 1000
    data['NUM_TUPLES'] = data['NUM_TUPLES'].str.replace(r"\[(\d+),(\d+),\d+,\d+,\d+\]", "\\1", regex=True)
    data['NUM_TUPLES'] = data['NUM_TUPLES'].astype(float)
    data['PAPI_L1_TCM_NORMALIZED'] = data['PAPI_L1_TCM'] / (data['NUM_TUPLES'].astype(float))
    data['JOIN_TYPE_OUTER'] = data['JOIN_TYPES'].str.replace(r"\[(\d+),(\d+),\d+,\d+\]", "\\1", regex=True).astype(int)
    data['JOIN_TYPE_OUTER'] = data['JOIN_TYPE_OUTER'].apply(mapIntToJoinTypeStr)

    colors = ["purple", "green", "pink", "yellow", "grey", "orange"]
    xOrderList = ['Repeated', 'Sequential', 'Pseudo-Random in L1', 'Pseudo-Random in L2', 'Pseudo-Random in L3',
                  'Random']
    fig, axes = plt.subplots(nrows=1, ncols=2, figsize=(24, 8))
    sns.boxplot(x='JOIN_TYPE_OUTER', y = 'EXEC_TIME_S', data = data, ax=axes[0], palette=colors, order = xOrderList)
    sns.boxplot(x='JOIN_TYPE_OUTER', y = 'PAPI_L1_TCM_NORMALIZED', data = data, ax=axes[1], palette=colors, order = xOrderList)

    # Removing all xtick labels
    axes[0].set_xticklabels([])
    axes[1].set_xticklabels([])

    # Removing x label
    axes[0].set_xlabel("")
    axes[1].set_xlabel("")

    # Setting the y label
    axes[0].set_ylabel("Execution time in s")
    axes[1].set_ylabel("L1 cache misses per tuple")

    # Adjust the spacing between subplots
    legend_labels = xOrderList
    legend_patches = [plt.Rectangle((0, 0), 1, 1, fc=color) for color in colors]
    fig.legend(legend_patches, legend_labels, loc='upper center', bbox_to_anchor=(0.5, 1), ncol=3)
    plt.savefig(figureFilename)
