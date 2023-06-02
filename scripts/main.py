#!/usr/bin/env python
# coding: utf-8

import sys

import general.util as util
import figure_scripts.figure_1 as figure_1
import figure_scripts.figure_3 as figure_3
import figure_scripts.figure_5 as figure_5
import figure_scripts.figure_6 as figure_6
import figure_scripts.figure_8 as figure_8

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Only expects one additional argument, which is the figure number [1, 3, 5, 6, 8]")
        exit(1)

    expRootFolder = "experiments"
    util.createFolder(expRootFolder)

    figNumber = int(sys.argv[1])
    if figNumber == 1:
        util.printHighlight("Running experiments for figure 1!\n")
        figure_1.main(expRootFolder)
    elif figNumber == 3:
        util.printHighlight("Running experiments for figure 3!")
        figure_3.main(expRootFolder)
    elif figNumber == 5:
        util.printHighlight("Running experiments for figure 5!")
        figure_5.main(expRootFolder)
    elif figNumber == 6:
        util.printHighlight("Running experiments for figure 6!")
        figure_6.main(expRootFolder)
    elif figNumber == 8:
        util.printHighlight("Running experiments for figure 8!")
        figure_8.main(expRootFolder)
    else:
        util.printFail(f"Figure number {figNumber} is not in the expected figure numbers [1, 3, 5, 6, 8]")
        exit(1)
