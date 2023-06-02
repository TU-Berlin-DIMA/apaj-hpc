#!/usr/bin/env python
# coding: utf-8

from datetime import datetime
from general.util import *
from general.create_permutations_with_selections import createPermutationFunctions

import os
import pandas as pd
import seaborn as sns
import matplotlib.pyplot as plt
import itertools


def main(expRootFolder):
    # Setting all experiment parameters
    NUM_TUPLES = [[350 * 1000 * 1000, 10 * 1000 * 1000, 10 * 1000 * 1000, 10 * 1000 * 1000, 10 * 1000 * 1000]]

    JOIN_TYPES = [[0, 1, 2, 4]]
    NUMBER_OF_JOINS = 4
    NUM_REPS_PER_EXPERIMENT = 10
    MAX_VALUE = [1e4]
    WITH_REORDERING = False
    SAMPLE_SEQ = "PAPI_TCM"
    SELECTIVITIES = [ [0.01, 0.01, 0.01, 0.01], [0.5, 0.5, 0.5, 0.5], [0.99, 0.99, 0.99, 0.99] ]

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
    prologStr = f"figure_3_{currDateTimeStr}"

    # Running the experiment and joining the two csv files on the experiment id
    allExperimentParams = list(itertools.product(NUM_TUPLES, JOIN_TYPES, MAX_VALUE, SELECTIVITIES))
    csvFileJoined, figureFilename = runExperiments(allExperimentParams, NUM_REPS_PER_EXPERIMENT, WITH_REORDERING, SAMPLE_SEQ,
                                                   EXECUTABLE, PATH_TO_PAPI, PATH_TO_TRAINED_ML_MODEL, expRootFolder, prologStr)

    # After we have run some experiments, we remove the build folder to clean up after ourselves
    removeFolder(releaseFldr)

    # Plotting from the csv file similar to Figure 3 in the paper
    sns.set()
    data = pd.read_csv(csvFileJoined)

    data['EXEC_TIME_S'] = data['EXEC_TIME_MS'] / 1000
    data['NEW_PERMUTATION'] = data['PERMUTATION'].apply(createNewParam)
    data['NUM_TUPLES'] = data['NUM_TUPLES'].str.replace(r"\[(\d+),(\d+),\d+,\d+,\d+\]", "\\1", regex=True)
    data['NUM_TUPLES'] = data['NUM_TUPLES'].astype(float)
    data['PAPI_L1_TCM_NORMALIZED'] = data['PAPI_L1_TCM'] / (data['NUM_TUPLES'].astype(float))

    colors = ["purple"] * 6 + ["green"] * 6 + ["pink"] * 6 + ["orange"] * 6
    hatch_pattern = ['/'] * 6 + ['\\'] * 6 + ['x'] * 6 + ['-'] * 6

    fig, axes = plt.subplots(nrows=2, ncols=3, figsize=(24, 8),sharey='row')
    for col_index, selectivity in enumerate(sorted(data['Selectivity_0'].unique())):
        row_index = 0

        tmpData = data[data['Selectivity_0'] == selectivity]
        sns.barplot(x='NEW_PERMUTATION', y = 'EXEC_TIME_S', data=tmpData,
                    palette=colors, hatch=hatch_pattern,
                    ax=axes[row_index, col_index])

        sns.barplot(x='NEW_PERMUTATION', y = 'PAPI_L1_TCM_NORMALIZED', data=tmpData,
                    palette=colors, hatch=hatch_pattern,
                    ax=axes[row_index + 1, col_index])

        axes[row_index, col_index].set_xticklabels([])
        axes[row_index, col_index].set_xlabel("")
        axes[row_index + 1, col_index].set_xticklabels([])

        axes[row_index+1, col_index].set_xlabel(f"Permutations with {int(selectivity*100)}% selectivity")
        axes[row_index, col_index].set_ylabel("Execution time in s")
        axes[row_index + 1, col_index].set_ylabel("L1 cache misses per tuple")

        if col_index != 0:
            axes[row_index, col_index].set_ylabel("")
            axes[row_index + 1, col_index].set_ylabel("")

    legend_labels = ['Repeated', 'Sequential', 'Pseudo-Random in L1', 'Random']
    legend_patches = [plt.Rectangle((0, 0), 1, 1, fc=color) for color in colors[::6]]
    fig.legend(legend_patches, legend_labels, loc='upper center', bbox_to_anchor=(0.5, 1.1), ncol=4)

    # Adjust the spacing between subplots
    plt.tight_layout()
    plt.savefig(figureFilename)
