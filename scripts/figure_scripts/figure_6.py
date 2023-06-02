#!/usr/bin/env python
# coding: utf-8

from datetime import datetime
from general.util import *
from general.create_permutations_with_selections import createPermutationFunctions
from general.train_ml_model import *

import ast
import random
import os
import itertools
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
import numpy as np

# Combines the two csv files into a dataframe that is then used for plotting Figure 6
def createCombinedDataFrame(csvFileJoinedWithout, csvFileJoinedWith):
    without_reordering = pd.read_csv(csvFileJoinedWithout)
    with_reordering = pd.read_csv(csvFileJoinedWith)

    without_reordering['EXEC_TIME_S'] = without_reordering['EXEC_TIME_MS'] / 1000.0
    with_reordering['EXEC_TIME_S'] = with_reordering['EXEC_TIME_MS'] / 1000.0

    # Dictionaries with the key being the experiment id, thus, all permutations belong to one group
    worstCase = {}
    bestCase = {}
    averageCase = {}
    expId = {}
    worstParams = {}
    bestParams = {}

    ourCaseBestStart = {}
    ourCaseWorstStart = {}
    ourCaseAverage = {}
    selectivity = {}
    joinTypes = {}

    for key, group in without_reordering.groupby(['EXPERIMENT_ID']):
        worstCase[key] = group['EXEC_TIME_S'].max()
        bestCase[key] = group['EXEC_TIME_S'].min()
        worstParams[key] = group['EXEC_TIME_S'].argmax()
        bestParams[key] = group['EXEC_TIME_S'].argmin()
        averageCase[key] = group['EXEC_TIME_S'].mean()
        selectivity[key] = group['Selectivity_0'].mean()
        joinTypes[key] = group.iloc[0]['JOIN_TYPES']

    for key, group in with_reordering.groupby(['EXPERIMENT_ID']):
        ourCaseBestStart[key] = group.iloc[bestParams[key]]['EXEC_TIME_S']
        ourCaseAverage[key] = group['EXEC_TIME_S'].mean()
        ourCaseWorstStart[key] = group.iloc[worstParams[key]]['EXEC_TIME_S']
        expId[key] = key

    # Sort by the average execution time
    sortedOurCaseAverage = {k: v for k, v in sorted(ourCaseAverage.items(), key=lambda item: item[1])}

    # Create a new dataframe containing all necessary rows and columns
    combined_df = pd.DataFrame()
    combined_df['OUR_CASE_AVERAGE'] = sortedOurCaseAverage
    combined_df['OUR_CASE_BEST_START'] = [ourCaseBestStart[key] for key in sortedOurCaseAverage]
    combined_df['OUR_CASE_WORST_START'] = [ourCaseWorstStart[key] for key in sortedOurCaseAverage]
    combined_df['OUR_CASE_AVERAGE'] = [ourCaseAverage[key] for key in sortedOurCaseAverage]
    combined_df['WORST_CASE'] = [worstCase[key] for key in sortedOurCaseAverage]
    combined_df['BEST_CASE'] = [bestCase[key] for key in sortedOurCaseAverage]
    combined_df['AVERAGE_CASE'] = [averageCase[key] for key in sortedOurCaseAverage]
    combined_df['SELECTIVITY'] = [selectivity[key] for key in sortedOurCaseAverage]
    combined_df['OUR_CASE_AVERAGE_NORMALIZED_TO_WORST_CASE'] = 1 / (combined_df['OUR_CASE_AVERAGE'] / combined_df['WORST_CASE'])
    combined_df['OUR_CASE_BEST_START_NORMALIZED_TO_WORST_CASE'] = 1 / (combined_df['OUR_CASE_BEST_START'] / combined_df['WORST_CASE'])
    combined_df['OUR_CASE_WORST_START_NORMALIZED_TO_WORST_CASE'] = 1 / (combined_df['OUR_CASE_WORST_START'] / combined_df['WORST_CASE'])

    combined_df['BEST_CASE_NORMALIZED_TO_WORST_CASE'] = 1 / (combined_df['BEST_CASE'] / combined_df['WORST_CASE'])
    combined_df['WORST_CASE_NORMALIZED_TO_WORST_CASE'] = 1 / (combined_df['WORST_CASE'] / combined_df['WORST_CASE'])

    combined_df['EXPERIMENT_ID'] = [expId[key] for key in sortedOurCaseAverage]
    combined_df['MAXIMUM_SPEEDUP'] = combined_df['WORST_CASE'] / combined_df['BEST_CASE']
    combined_df['AVERAGE_SPEEDUP'] = combined_df['AVERAGE_CASE'] / combined_df['OUR_CASE_AVERAGE']
    combined_df['SPEEDUP_ADAPTIVE_TO_WORST_CASE'] = combined_df['WORST_CASE'] / combined_df['OUR_CASE_AVERAGE']
    combined_df['SPEEDUP_WORST_TO_WORST_CASE'] = combined_df['WORST_CASE'] / combined_df['WORST_CASE']
    combined_df['SPEEDUP_BEST_ADAPTIVE_TO_WORST_CASE'] = combined_df['WORST_CASE'] / combined_df['OUR_CASE_BEST_START']
    combined_df['JOIN_TYPES'] = joinTypes

    return combined_df

# Categorizes the selectivity into four groups and return a string representation of the group
def categorizeSel(x):
    if x < 0.1:
        return "<10%"
    elif x < 0.25:
        return "<25%"
    elif x < 0.5:
        return "<50%"
    else:
        return "Rest"

# Categorizes the memory access into three groups and returns a string representation of the group
def categorizeMemAccess(memAccessString):
    memAccessList = ast.literal_eval(memAccessString)
    randomCount = memAccessList.count(2)
    if randomCount == 0 or randomCount == 1 :
        return "1) More Single/Sequential/PRnd"
    elif randomCount == 2:
        return "2) Equal"
    elif randomCount == 3 or randomCount == 4:
        return "3) More Random"
    else:
        return "TBA"

# Counts the number of random accesses in the string
def countNoRandom(memAccessString):
    memAccessList = ast.literal_eval(memAccessString)
    randomCount = memAccessList.count(2)
    if randomCount == 0 or randomCount == 1 :
        return 1
    elif randomCount == 2:
        return 2
    elif randomCount == 3 or randomCount == 4:
        return 3
    else:
        return "TBA"

# Creates the bar plot datafram from the combined dataframe (combined_df)
def createBarPlotDataFrame(combined_df):
    adaptiveAverage = pd.DataFrame(combined_df[['OUR_CASE_AVERAGE_NORMALIZED_TO_WORST_CASE', 'OUR_CASE_AVERAGE',
                                                'SELECTIVITY', 'EXPERIMENT_ID', 'JOIN_TYPES']])
    adaptiveAverage.rename(columns = {'OUR_CASE_AVERAGE_NORMALIZED_TO_WORST_CASE' : 'Runtime Normalized'}, inplace=True)
    adaptiveAverage.rename(columns = {'OUR_CASE_AVERAGE' : 'Runtime'}, inplace=True)
    adaptiveAverage['RUN_TYPE'] = "Adaptive Average"

    adaptiveBestStart = pd.DataFrame(combined_df[['OUR_CASE_BEST_START_NORMALIZED_TO_WORST_CASE', 'OUR_CASE_BEST_START',
                                                  'SELECTIVITY', 'EXPERIMENT_ID', 'JOIN_TYPES']])
    adaptiveBestStart.rename(columns = {'OUR_CASE_BEST_START_NORMALIZED_TO_WORST_CASE' : 'Runtime Normalized'}, inplace=True)
    adaptiveBestStart.rename(columns = {'OUR_CASE_BEST_START' : 'Runtime'}, inplace=True)
    adaptiveBestStart['RUN_TYPE'] = "Adaptive Best Start"

    adaptiveWorstStart = pd.DataFrame(combined_df[['OUR_CASE_WORST_START_NORMALIZED_TO_WORST_CASE', 'OUR_CASE_WORST_START',
                                                   'SELECTIVITY', 'EXPERIMENT_ID', 'JOIN_TYPES']])
    adaptiveWorstStart.rename(columns = {'OUR_CASE_WORST_START_NORMALIZED_TO_WORST_CASE' : 'Runtime Normalized'}, inplace=True)
    adaptiveWorstStart.rename(columns = {'OUR_CASE_WORST_START' : 'Runtime'}, inplace=True)
    adaptiveWorstStart['RUN_TYPE'] = "Adaptive Worst Start"

    bestCase = pd.DataFrame(combined_df[['WORST_CASE_NORMALIZED_TO_WORST_CASE', 'WORST_CASE',
                                         'SELECTIVITY', 'EXPERIMENT_ID', 'JOIN_TYPES']])
    bestCase.rename(columns = {'WORST_CASE_NORMALIZED_TO_WORST_CASE' : 'Runtime Normalized'}, inplace=True)
    bestCase.rename(columns = {'WORST_CASE' : 'Runtime'}, inplace=True)
    bestCase['RUN_TYPE'] = "Worst Case"

    worstCase = pd.DataFrame(combined_df[['BEST_CASE_NORMALIZED_TO_WORST_CASE', 'BEST_CASE',
                                          'SELECTIVITY', 'EXPERIMENT_ID', 'JOIN_TYPES']])
    worstCase.rename(columns = {'BEST_CASE_NORMALIZED_TO_WORST_CASE' : 'Runtime Normalized'}, inplace=True)
    worstCase.rename(columns = {'BEST_CASE' : 'Runtime'}, inplace=True)
    worstCase['RUN_TYPE'] = "Best Case"

    barPlotData = pd.concat([adaptiveAverage, bestCase, worstCase,adaptiveWorstStart, adaptiveBestStart])
    barPlotData['SEL_GROUP'] = barPlotData['SELECTIVITY'].apply(categorizeSel)
    barPlotData['MEM_ACCESS'] = barPlotData['JOIN_TYPES'].apply(categorizeMemAccess)
    barPlotData['NO_RANDOM'] = barPlotData['JOIN_TYPES'].apply(countNoRandom)
    barPlotData.sort_values(['SEL_GROUP', 'MEM_ACCESS'], inplace=True)
    barPlotData = barPlotData.round(1)

    return barPlotData


def main(expRootFolder):
    # Setting all experiment parameters
    NUM_TUPLES = [[350 * 1000 * 1000, 10 * 1000 * 1000, 10 * 1000 * 1000, 10 * 1000 * 1000, 10 * 1000 * 1000]]
    JOIN_TYPES = [[0, 5, 5, 5], [3, 4, 4, 4], [0, 2, 2, 4], [2, 4, 4, 5], [0, 1, 2, 5], [1, 4, 4, 5], [3, 3, 3, 5],
                  [0, 1, 2, 2], [0, 1, 4, 4], [3, 3, 3, 3], [1, 1, 2, 2], [1, 1, 2, 5], [1, 2, 2, 4], [2, 2, 3, 4],
                  [5, 5, 5, 5], [0, 0, 2, 2], [2, 2, 5, 5], [1, 3, 4, 4], [0, 0, 2, 4], [0, 3, 3, 5], [0, 0, 0, 1],
                  [0, 0, 0, 4], [3, 3, 4, 4], [0, 2, 3, 4], [1, 1, 5, 5], [1, 1, 1, 3], [0, 1, 3, 3], [2, 3, 4, 4],
                  [0, 3, 4, 5], [2, 2, 2, 4], [1, 2, 5, 5], [0, 1, 3, 5], [1, 4, 5, 5], [2, 2, 2, 3], [0, 1, 1, 2],
                  [0, 1, 1, 5], [0, 0, 3, 4], [0, 2, 4, 5], [0, 0, 1, 4], [0, 0, 1, 1], [0, 0, 1, 2], [1, 3, 3, 3],
                  [1, 3, 5, 5], [2, 2, 4, 5], [3, 3, 5, 5], [2, 3, 3, 5], [4, 4, 4, 5], [3, 4, 4, 5], [0, 2, 2, 3],
                  [2, 4, 4, 4], [0, 4, 5, 5], [0, 1, 2, 3], [0, 1, 4, 5], [2, 2, 3, 3], [1, 1, 2, 4], [1, 2, 2, 5],
                  [2, 2, 3, 5], [1, 2, 2, 2], [1, 2, 4, 4], [0, 0, 4, 5], [0, 3, 3, 4], [2, 5, 5, 5], [0, 0, 0, 0],
                  [0, 0, 0, 3], [0, 2, 3, 5], [0, 2, 3, 3], [1, 1, 1, 4], [1, 1, 1, 1], [1, 1, 3, 4], [2, 4, 5, 5],
                  [2, 3, 4, 5], [3, 4, 5, 5], [0, 1, 3, 4], [0, 4, 4, 5], [2, 2, 2, 2], [0, 1, 1, 1], [0, 1, 1, 4],
                  [0, 0, 3, 5], [1, 2, 3, 3], [1, 5, 5, 5], [0, 2, 4, 4], [0, 0, 1, 5], [1, 2, 3, 5], [0, 0, 1, 3],
                  [1, 3, 3, 4], [2, 2, 4, 4], [1, 1, 4, 5], [2, 3, 3, 4], [4, 4, 4, 4], [2, 3, 3, 3], [0, 2, 2, 2],
                  [0, 2, 2, 5], [0, 1, 2, 4], [1, 4, 4, 4], [3, 3, 3, 4], [0, 3, 5, 5], [1, 1, 2, 3], [1, 2, 2, 3],
                  [1, 2, 4, 5], [0, 0, 4, 4], [2, 3, 5, 5], [0, 0, 2, 3], [4, 5, 5, 5], [0, 2, 5, 5], [0, 3, 3, 3],
                  [1, 3, 4, 5], [0, 0, 2, 5], [0, 0, 0, 2], [0, 0, 0, 5], [3, 3, 4, 5], [1, 1, 1, 5], [1, 1, 1, 2],
                  [4, 4, 5, 5], [0, 1, 5, 5], [1, 1, 3, 5], [1, 1, 3, 3], [0, 3, 4, 4], [0, 4, 4, 4], [2, 2, 2, 5],
                  [3, 5, 5, 5], [0, 1, 1, 3], [0, 0, 3, 3], [0, 0, 5, 5], [1, 2, 3, 4], [1, 3, 3, 5], [1, 1, 4, 4]]
    NUMBER_OF_JOINS = 4
    NUM_REPS_PER_EXPERIMENT = 10
    MAX_VALUE = [1e4]
    WITH_REORDERING = False
    SAMPLE_SEQ = "PAPI_TCM"
    SELECTIVITIES = [[i] * NUMBER_OF_JOINS for i in np.linspace(1, 99, 99) / 100]
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

    # Creating all permutation functions for 4 joins and also compiling the project
    createPermutationFunctions(NUMBER_OF_JOINS)
    compileProject(sourceCodeFldr, releaseFldr)

    # Creating path to ml model and prolog for any file in runExperiments
    currDateTimeStr = datetime.now().strftime('%Y%m%d_%H%M%S')
    PATH_TO_TRAINED_ML_MODEL = os.path.abspath(os.path.join(expRootFolder, f"figure_6_ml_model_{currDateTimeStr}"))
    prologStr = f"figure_6_no_reordering_{currDateTimeStr}"

    # # Running the experiment and joining the two csv files on the experiment id
    csvFileJoinedWithout, figureFilenameWithout = runExperiments(allExperimentParams, NUM_REPS_PER_EXPERIMENT, WITH_REORDERING, SAMPLE_SEQ,
                                                                 EXECUTABLE, PATH_TO_PAPI, PATH_TO_TRAINED_ML_MODEL, expRootFolder, prologStr)



    # Training the ml model with the queries described in the paper
    csvFileJoinedTrainingMl, _ = runTrainingQueriesMlModel(expRootFolder, EXECUTABLE, PATH_TO_PAPI, PATH_TO_TRAINED_ML_MODEL)
    trainMlModel(csvFileJoinedTrainingMl, PATH_TO_TRAINED_ML_MODEL)

    # Running the experiment again with join reordering so that we can then later on compare the performance of our approach
    prologStr = f"figure_6_with_reordering_{currDateTimeStr}"
    WITH_REORDERING = True
    csvFileJoinedWith, figureFilenameWith = runExperiments(allExperimentParams, NUM_REPS_PER_EXPERIMENT, WITH_REORDERING, SAMPLE_SEQ,
                                                           EXECUTABLE, PATH_TO_PAPI, PATH_TO_TRAINED_ML_MODEL, expRootFolder, prologStr)

    # Plotting from the csv file similar to Figure 6 in the paper
    combined_df = createCombinedDataFrame(csvFileJoinedWithout, csvFileJoinedWith)
    bar_plot_df = createBarPlotDataFrame(combined_df)

    # Plotting for different selectivity groups
    fig, axes = plt.subplots(nrows=1, ncols=2, figsize=(24, 8))
    sns.barplot(x = 'SEL_GROUP', y = 'Runtime',
                hue = 'RUN_TYPE', errorbar=None,
                data = bar_plot_df, ax = axes[0])
    for bar in axes[0].containers:
        labels = [f'{(v.get_height()):0.2f}' for v in bar]
        axes[0].bar_label(bar, labels=labels, label_type='center')

    # Plotting for different the memory access distributions
    sns.barplot(x = 'MEM_ACCESS', y = 'Runtime',
                hue = 'RUN_TYPE', errorbar=None,
                data = bar_plot_df, ax = axes[1])
    for bar in axes[1].containers:
        labels = [f'{(v.get_height()):0.2f}' for v in bar]
        axes[1].bar_label(bar, labels=labels, label_type='center')

    plt.tight_layout()
    plt.savefig(figureFilenameWith)
    printSuccess(f"Saved figure to {figureFilenameWith}")
