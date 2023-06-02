#!/usr/bin/env python
# coding: utf-8

from tensorflow.python.client import device_lib
from tensorflow.keras import Sequential
from tensorflow.keras.layers import Dense, ReLU, BatchNormalization, Dropout
from tensorflow.keras.optimizers import Adam
from sklearn.model_selection import train_test_split
from datetime import datetime

from general.util import *

import tensorflow as tf
import pandas as pd
import itertools
import re
import math


# Runs the experiment for generating training data for our ml model
def runTrainingQueriesMlModel(expRootFolder, EXECUTABLE, PATH_TO_PAPI, PATH_TO_TRAINED_ML_MODEL):
    # Setting all experiment parameters
    NUM_TUPLES = [[350 * 1000 * 1000, 10 * 1000 * 1000, 10 * 1000 * 1000, 10 * 1000 * 1000, 10 * 1000 * 1000]]

    JOIN_TYPES = [[0, 1, 3, 4], [0, 3, 2, 2], [0, 1, 2, 2]]
    NUMBER_OF_JOINS = 4
    NUM_REPS_PER_EXPERIMENT = 10
    MAX_VALUE = [1e4]
    WITH_REORDERING = False
    SAMPLE_SEQ = "PAPI_TCM"
    SELECTIVITIES = [[0.01] * NUMBER_OF_JOINS,
                     [0.11] * NUMBER_OF_JOINS,
                     [0.22] * NUMBER_OF_JOINS,
                     [0.33] * NUMBER_OF_JOINS]

    allExperimentParams = list(itertools.product(NUM_TUPLES, JOIN_TYPES, MAX_VALUE, SELECTIVITIES))

    # Creating path to ml model and prolog for any file in runExperiments
    currDateTimeStr = datetime.now().strftime('%Y%m%d_%H%M%S')
    prologStr = f"machine_learning_{currDateTimeStr}_training"


    return runExperiments(allExperimentParams, NUM_REPS_PER_EXPERIMENT, WITH_REORDERING, SAMPLE_SEQ,
                          EXECUTABLE, PATH_TO_PAPI, PATH_TO_TRAINED_ML_MODEL, expRootFolder, prologStr)


# Converts the csv into a dataframe that has all columns necessary for training our model
def convertDataToBinaryClassification(training_data_csv):
    allData = pd.read_csv(training_data_csv)

    allData['NUM_TUPLES'] = allData['NUM_TUPLES'].str.replace(r"\[(\d+),(\d+),\d+,\d+,\d+\]", "\\1", regex=True)

    allData['PAPI_L1_TCM_NORMALIZED'] = allData['PAPI_L1_TCM'] / (allData['NUM_TUPLES'].astype(float))
    allData['PAPI_L2_TCM_NORMALIZED'] = allData['PAPI_L2_TCM'] / (allData['NUM_TUPLES'].astype(float))
    allData['PAPI_L3_TCM_NORMALIZED'] = allData['PAPI_L3_TCM'] / (allData['NUM_TUPLES'].astype(float))
    allData['RUNTIME_NORMALIZED'] = allData['EXEC_TIME_MS'] / allData['NUM_TUPLES'].astype(float)

    # Rewrite join types as these are still the join types for permutation 0
    NUMBER_OF_JOINS = 4
    ALL_PERMUTATIONS_LIST = list(itertools.permutations([x for x in range(NUMBER_OF_JOINS)]))
    JOIN_TYPES_WITH_PERMUTATION = [[] for i in range(NUMBER_OF_JOINS)]
    allData['PERMUTATION'] = allData['PERMUTATION'].astype(int)

    for idx, row in allData.iterrows():
        for joinNumber in range(NUMBER_OF_JOINS):
            curPermutation = row['PERMUTATION']
            joinType = re.search(r"\[(\d+),(\d+),(\d+),(\d+)]", str(row['JOIN_TYPES']))
            JOIN_TYPES_WITH_PERMUTATION[joinNumber].append(joinType.group(ALL_PERMUTATIONS_LIST[curPermutation][joinNumber]+1))

    # Labeling the outermost join type
    for joinNumber in range(NUMBER_OF_JOINS):
        allData[f"JOIN_{joinNumber}_TYPE"] = JOIN_TYPES_WITH_PERMUTATION[joinNumber]
        allData[f"JOIN_{joinNumber}_TYPE"] = allData[f"JOIN_{joinNumber}_TYPE"].astype(int)

    # Converting problem to binary classification problem. All except random are 0 and random is 1
    RANDOM_VALUE = 2
    allData['JOIN_0_TYPE_BINARY'] = allData['JOIN_0_TYPE'] == RANDOM_VALUE
    allData['JOIN_0_TYPE_BINARY'] = allData['JOIN_0_TYPE_BINARY'].astype(int)

    return allData


# Returns all available devices that are of type CPU
def get_available_devices():
    local_device_protos = device_lib.list_local_devices()
    return [x.name for x in local_device_protos if x.device_type == 'CPU']


# Trains the ml model with the data in the training_data_csv and also stores the trained model at model_output_fldr
def trainMlModel(training_data_csv, model_output_fldr):
    allData = convertDataToBinaryClassification(training_data_csv)

    # Taking time for later printing out the duration of training the model
    start_time = datetime.now()

    # Duplicating either all random or not random cases to have a level playing field
    randomDataFrame = allData[allData['JOIN_0_TYPE_BINARY'] == 1]
    notRandomDataFrame = allData[allData['JOIN_0_TYPE_BINARY'] == 0]

    if len(randomDataFrame) > len(notRandomDataFrame):
        missingFactor = math.ceil(len(randomDataFrame) / len(notRandomDataFrame))
        notRandomDataFrame = pd.concat([notRandomDataFrame]*missingFactor, verify_integrity=True, ignore_index=True)
        dropNoItems = len(notRandomDataFrame) - len(randomDataFrame)
        notRandomDataFrame.drop(notRandomDataFrame.tail(dropNoItems).index, inplace=True)

    elif len(randomDataFrame) < len(notRandomDataFrame):
        missingFactor = math.ceil(len(notRandomDataFrame) / len(randomDataFrame))
        randomDataFrame = pd.concat([randomDataFrame]*missingFactor, verify_integrity=True, ignore_index=True)
        dropNoItems = len(randomDataFrame) - len(notRandomDataFrame)
        randomDataFrame.drop(randomDataFrame.tail(dropNoItems).index, inplace=True)

    allData = pd.concat([notRandomDataFrame, randomDataFrame])


    # Training on CPU
    with tf.device(get_available_devices()[0]):

        # Converting data to x and y data and also one hot encoding y data
        xFull = allData[['PAPI_L1_TCM_NORMALIZED', 'PAPI_L2_TCM_NORMALIZED', 'PAPI_L3_TCM_NORMALIZED']]
        yFull = allData['JOIN_0_TYPE_BINARY']
        printHighlight(f"Dataset contains #samples for the binary classes 0 and 1:"
                       f"\n{allData['JOIN_0_TYPE_BINARY'].value_counts()}")

        xTrainFull, xTest, yTrainFull, yTest = train_test_split(xFull, yFull, test_size=0.2, random_state=42)
        xTrain, xValid, yTrain, yValid = train_test_split(xTrainFull, yTrainFull, test_size=0.1, random_state=42)

        xTrainScaled = xTrain
        xValidScaled = xValid
        printHighlight(f"The training set comprises {xTrain.shape[0]} examples.")
        printHighlight(f"The validation set comprises {xValid.shape[0]} examples.")
        printHighlight(f"The test dataset comprises {xTest.shape[0]} examples")

        # Create model and training callback
        callbacks_list = [
            tf.keras.callbacks.EarlyStopping(monitor= 'val_loss', min_delta = .001, patience = 100)
        ]

        inputShape = xTrainScaled.shape[1:]
        outputShape = 1
        model = Sequential()
        model.add(Dense(4, activation="relu", input_shape = inputShape))
        model.add(Dense(outputShape, activation='sigmoid'))

        model.compile(loss='binary_crossentropy', metrics=['accuracy'],
                      optimizer=Adam(learning_rate=0.0001,
                                     beta_1=0.9, beta_2=0.999,
                                     epsilon=1e-07, amsgrad=True))
        model.summary()

        model.fit(xTrainScaled, yTrain, epochs = 1000, batch_size=None,
                  validation_data = (xValidScaled, yValid), verbose = 2, callbacks = callbacks_list)
        model.save(model_output_fldr)
        printSuccess(f"Saved model to: {model_output_fldr}!")

        printSuccess(f"Training ml model took (hh:mm:ss.ms) {datetime.now() - start_time}!")
