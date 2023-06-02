#!/usr/bin/env python
# coding: utf-8

from datetime import datetime
from general.util import *
from general.create_permutations_with_selections import createPermutationFunctions
from general.train_ml_model import *

import os
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns


def main(expRootFolder):
    # Setting all experiment parameters
    NUM_TUPLES = [[350 * 1000 * 1000, 10 * 1000 * 1000, 10 * 1000 * 1000, 10 * 1000 * 1000, 10 * 1000 * 1000]]
    JOIN_TYPES = [[0, 0, 2, 4]]
    NUMBER_OF_JOINS = 4
    NUM_REPS_PER_EXPERIMENT = 10
    MAX_VALUE = [1e4]
    WITH_REORDERING = False
    SAMPLE_SEQ = "PAPI_TCM"
    SELECTIVITIES = [[0.49] * NUMBER_OF_JOINS]
    allExperimentParams = list(itertools.product(NUM_TUPLES, JOIN_TYPES, MAX_VALUE, SELECTIVITIES))

    sourceCodeFldr = os.path.abspath(os.path.join(__file__, "../../../source_code"))
    releaseFldr = os.path.abspath(os.path.join(expRootFolder, "cmake-build-release"))
    EXECUTABLE = os.path.abspath(os.path.join(releaseFldr, "apaj"))
    PATH_TO_PAPI = os.path.abspath(os.path.join(__file__, "../../../source_code/include/Papi"))

    # Creating all permutation functions for 4 joins and also compiling the project
    createPermutationFunctions(NUMBER_OF_JOINS)
    compileProject(sourceCodeFldr, releaseFldr)

    # Creating path to ml model and prolog for any file in runExperiments
    currDateTimeStr = datetime.now().strftime('%Y%m%d_%H%M%S')
    PATH_TO_TRAINED_ML_MODEL = os.path.abspath(os.path.join(expRootFolder, f"figure_8_ml_model_{currDateTimeStr}"))
    prologStr = f"figure_8_no_reordering_{currDateTimeStr}"

    # Running the experiment and joining the two csv files on the experiment id
    csvFileJoinedWithout, figureFilenameWithout = runExperiments(allExperimentParams, NUM_REPS_PER_EXPERIMENT, WITH_REORDERING, SAMPLE_SEQ,
                                                                 EXECUTABLE, PATH_TO_PAPI, PATH_TO_TRAINED_ML_MODEL, expRootFolder, prologStr)

    # Training the ml model with the queries described in the paper
    csvFileJoinedTrainingMl, _ = runTrainingQueriesMlModel(expRootFolder, EXECUTABLE, PATH_TO_PAPI, PATH_TO_TRAINED_ML_MODEL)
    trainMlModel(csvFileJoinedTrainingMl, PATH_TO_TRAINED_ML_MODEL)

    # Running the experiment again with join reordering so that we can compare the performance of our approach
    prologStr = f"figure_8_with_reordering_{currDateTimeStr}"
    WITH_REORDERING = True
    csvFileJoinedWith, figureFilenameWith = runExperiments(allExperimentParams, NUM_REPS_PER_EXPERIMENT, WITH_REORDERING, SAMPLE_SEQ,
                                                           EXECUTABLE, PATH_TO_PAPI, PATH_TO_TRAINED_ML_MODEL, expRootFolder, prologStr)

    # After we have run some experiments, we remove the build folder to clean up after ourselves
    removeFolder(releaseFldr)

    # Plotting from the csv file similar to Figure 8 in the paper
    without_reordering = pd.read_csv(csvFileJoinedWithout)
    with_reordering = pd.read_csv(csvFileJoinedWith)

    without_reordering['EXEC_TIME_S'] = without_reordering['EXEC_TIME_MS'] / 1000
    with_reordering['EXEC_TIME_S'] = with_reordering['EXEC_TIME_MS'] / 1000

    without_reordering['REORDERING'] = [False for i in range(len(without_reordering))]
    with_reordering['REORDERING'] = [True for i in range(len(with_reordering))]
    tmpData = pd.concat([without_reordering, with_reordering], ignore_index=True)

    tmpData['NEW_PERMUTATION'] = tmpData['PERMUTATION'].apply(createNewParam)
    fig, ax = plt.subplots(figsize=(12, 6))
    sns.lineplot(x='NEW_PERMUTATION', y='EXEC_TIME_S', ax=ax, data=tmpData, hue='REORDERING', marker='o')
    plt.ylim((0, 11))
    plt.tight_layout()
    plt.savefig(figureFilenameWith)
    printSuccess(f"Saved figure to {figureFilenameWith}")
