#!/usr/bin/env python
# coding: utf-8
from datetime import datetime, timedelta

import itertools
import subprocess
import pandas as pd
import os
import shutil


########################Start of helper functions########################
LOG_FILE = "run_experiments_general.log"

class bcolors:
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OKCYAN = '\033[96m'
    OKGREEN = '\033[92m'
    OKMAGENTA = '\033[35m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'
    UNDERLINE = '\033[4m'


def printWithColor(message, color):
    print("{}{}{}".format(color, message, bcolors.ENDC))


def printSuccess(message):
    printWithColor(message, bcolors.OKGREEN)


def printFail(message):
    printWithColor(message, bcolors.FAIL)


def printHighlight(message):
    printWithColor(message, bcolors.OKCYAN)


def print2File(message, fileName, file=__file__):
    with open(fileName, 'a+') as f:
        f.write("{}".format(message))


def print2Log(message, file=__file__):
    with open(LOG_FILE, 'a+') as f:
        f.write("\n----------------------{}----------------------\n".format(file))
        f.write("{}".format(message))
        f.write("\n----------------------{}----------------------\n".format(file))


def fileFolderExists(file):
    return os.path.exists(file)


def createFolder(folder):
    if not fileFolderExists(folder):
        os.mkdir(folder)
        printHighlight("{} was created!".format(folder))

def removeFolder(folder):
    if fileFolderExists(folder):
        printHighlight(f"Removing {folder}")
        shutil.rmtree(folder)

########################End of helper functions##########################


# Returns a string that can be executed via os.subprocess
def createCmdString(tuples, joinType, curSel, executable, expId, csvFile, maxValue, repsPerExperiment, sampleSeq,
                    withReordering, pathToPapiFolder, pathToTrainedMlModel):

    # Checking if pathToPapiFolder and pathToTrainedMlModel are not empty, as the executable expects a path
    if not pathToPapiFolder:
        pathToPapiFolder = "someDir"

    if not pathToTrainedMlModel:
        pathToTrainedMlModel = "someDir"

    tuplesString = str(tuples).replace(" ", "")
    joinTypesString = str(joinType).replace(" ", "")
    curSelStr = str(curSel).replace(" ", "")
    cmdString = "{} {} {} {} {} {} {} {} {} {} {} {}".format(executable,
                                                             expId,
                                                             csvFile,
                                                             tuplesString,
                                                             maxValue,
                                                             repsPerExperiment,
                                                             sampleSeq,
                                                             curSelStr,
                                                             joinTypesString,
                                                             withReordering,
                                                             pathToPapiFolder,
                                                             pathToTrainedMlModel)

    tuplesString = "\"{}\"".format(tuplesString)
    joinTypesString = "\"{}\"".format(joinTypesString)
    cmdStringCsv = "{} {} {} {} {} {} {} {} {}".format(executable,
                                                       expId,
                                                       csvFile,
                                                       tuplesString,
                                                       maxValue,
                                                       repsPerExperiment,
                                                       withReordering,
                                                       sampleSeq,
                                                       joinTypesString)

    cmdStringCsv = cmdStringCsv.replace(" ", ",")
    return (cmdString, cmdStringCsv)

# Compiles the project via a subprocess.check_call
def compileProject(sourceCodeFldr, releaseFldr, pathToCmake = "/usr/bin/cmake"):
    # Remove folder and then recreate it for an easy way of cleaning the project
    removeFolder(releaseFldr)
    createFolder(releaseFldr)

    # Creating a command for running cmake and one for building the project
    cmakeCmd = f"{pathToCmake} -DCMAKE_BUILD_TYPE=Release -S {sourceCodeFldr} -B {releaseFldr}"
    buildCmd = f"{pathToCmake} --build {releaseFldr} --target apaj"

    try:
        with open('/dev/null', 'a+') as f:
            subprocess.check_call(cmakeCmd.split(), stdout=f, stderr=subprocess.DEVNULL)
            subprocess.check_call(buildCmd.split(), stdout=f, stderr=subprocess.DEVNULL)
    except Exception as e:
        printFail(e)
        exit(1)


# Creates multiple file names from the prologStr
def createFileNames(expRootFolder, prologStr):
    csvFile = os.path.join(expRootFolder, prologStr, f"{prologStr}_csv_file.csv")
    csvFileParams = os.path.join(expRootFolder, prologStr, f"{prologStr}_csv_file_params.csv")
    csvFileJoined = os.path.join(expRootFolder, prologStr, f"{prologStr}_csv_file_joined.csv")
    figureFilename = os.path.join(expRootFolder, prologStr, f"{prologStr}_plot.pdf")
    logFile = os.path.join(expRootFolder, prologStr, f"{prologStr}_log_file.log")

    return csvFile, csvFileParams, csvFileJoined, figureFilename, logFile


# Delete all rows that contain header
def deleteRowsContainHeader(file):
    df = pd.read_csv(file)
    headerAsList = list(df)
    duplicateHeaderIndexes = []
    for i, row in df.iterrows():
        if list(row) == headerAsList:
            duplicateHeaderIndexes.append(i)

    df = df.drop(duplicateHeaderIndexes, axis=0)
    df.to_csv(file, index=False)


# Running all experiments and combines the two csv files into a single one.
# This function returns the joined csv file and the figure name
def runExperiments(ALL_EXPERIMENT_PARAMS, NUM_REPS_PER_EXPERIMENT, WITH_REORDERING, SAMPLE_SEQ,
                   EXECUTABLE, PATH_TO_PAPI, PATH_TO_TRAINED_ML_MODEL, expRootFolder, prologStr):

    TOTAL_EXPERIMENTS = len(list(ALL_EXPERIMENT_PARAMS))
    experimentId = 0

    # Creating file names and writing header to the csv file
    (csvFile, csvFileParams, csvFileJoined, figureFilename, logFile) = createFileNames(expRootFolder, prologStr)
    createFolder(os.path.join(expRootFolder, prologStr))
    print2Log("EXECUTABLE,EXPERIMENT_ID,CSV_FILENAME,NUM_TUPLES,MAX_VALUE,NUM_REPS_PER_EXPERIMENT,WITH_REORDERING,SAMPLE_SEQ,JOIN_TYPES,CYCLE_LENGTH")
    print2File("EXECUTABLE,EXPERIMENT_ID,CSV_FILENAME,NUM_TUPLES,MAX_VALUE,NUM_REPS_PER_EXPERIMENT,WITH_REORDERING,SAMPLE_SEQ,JOIN_TYPES,CYCLE_LENGTH\n", csvFileParams)

    totalSeconds = 0
    for numTuples, joinType, maxValue, sel in list(ALL_EXPERIMENT_PARAMS):


        # Writing the output
        cmdString, cmdStringCsv = createCmdString(numTuples, joinType, sel, EXECUTABLE, experimentId, csvFile,
                                                  maxValue, NUM_REPS_PER_EXPERIMENT, SAMPLE_SEQ, WITH_REORDERING,
                                                  PATH_TO_PAPI, PATH_TO_TRAINED_ML_MODEL)
        print2File("{}\n".format(cmdStringCsv), csvFileParams)

        print("{} [{}/{}]".format(cmdString, (experimentId+1), TOTAL_EXPERIMENTS))

        startTimeStamp = datetime.now()
        # Running the experiment
        try:
            with open("{}".format(logFile), 'a+') as f:
                subprocess.check_call(cmdString.split(), stdout=f, stderr=subprocess.DEVNULL)
        except Exception as e:
            printFail(e)

        experimentId += 1

        delta = datetime.now() - startTimeStamp
        totalSeconds += delta.total_seconds()
        hours = int(totalSeconds // 3600)
        minutes = int(totalSeconds % 3600) // 60
        seconds = int(totalSeconds % 60)

        eta_seconds = (TOTAL_EXPERIMENTS - experimentId) / (float(experimentId) / float(totalSeconds))
        eta_datetime = datetime.now() + timedelta(seconds=eta_seconds)

        eta_hours = int(eta_seconds // 3600)
        eta_minutes = int(eta_seconds % 3600) // 60
        eta_seconds = int(eta_seconds % 60)
        printHighlight("{}: total duration {}h {}m {}s and should be done in approx {}h {}m {}s at {}".format(datetime.now().strftime("%d.%m.%Y %H:%M:%S"), hours, minutes, seconds, eta_hours, eta_minutes, eta_seconds, eta_datetime.strftime("%d.%m.%Y %H:%M:%S")))


    deleteRowsContainHeader(csvFile)
    deleteRowsContainHeader(csvFileParams)

    outputDf = pd.read_csv(csvFile)
    paramDf = pd.read_csv(csvFileParams)

    joinedDf = outputDf.join(paramDf.set_index('EXPERIMENT_ID'), on='EXPERIMENT_ID')
    joinedDf.to_csv(csvFileJoined, index=False)

    return csvFileJoined, figureFilename


# Swaps the param number of Rnd and PRandL1 for a nicer visualization
def createNewParam(x):
    if x < 12:
        return x
    if x < 18:
        return x + 6
    else:
        return x - 6