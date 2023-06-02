#!/usr/bin/env python
# coding: utf-8

import math
import itertools
import os.path
import sys
import re

def createPermutationFunctions(numberOfJoins):
    NUMBER_OF_PERMUTATIONS = math.factorial(numberOfJoins)
    DST_INCLUDE_FILE_NAME = os.path.abspath(os.path.join(__file__, "../../../source_code/include/Join/join_permutations.hpp"))
    DST_SOURCE_FILE_NAME = os.path.abspath(os.path.join(__file__, "../../../source_code/src/Join/join_permutations.cpp"))
    MAIN_CPP_FILE_NAME = os.path.abspath(os.path.join(__file__, "../../../source_code/main.cpp"))
    REGEX_MAIN_CPP_LINE = r"uint64_t NUMBER_OF_JOINS = (\d+);"

    INCLUDE_HEADER = """#ifndef APAJ_HPC_PERMUTATION_JOIN_HPP
    #define APAJ_HPC_PERMUTATION_JOIN_HPP
    
    #include <Tables/DimensionTable.hpp>
    #include <Tables/FactTable.hpp>
    #include <unistd.h>
    #include <vector>
    
    namespace APAJ::PermutationJoin {
        uint64_t permutationJoin(size_t permutation, uint64_t dummyVar, size_t startOffset, size_t stopOffset, 
                                 std::vector<size_t> selectivityValues, APAJ::FactTable* factTable, 
                                 APAJ::DimensionTable* firstDimension, APAJ::DimensionTable* secondDimension, 
                                 APAJ::DimensionTable* thirdDimension, APAJ::DimensionTable* fourthDimension);\n
    """
    
    INCLUDE_FOOTER = """
    } // namespace APAJ::PermutationJoin
    
    #endif //APAJ_HPC_PERMUTATION_JOIN_HPP
    """
    
    with open(DST_INCLUDE_FILE_NAME, 'w') as includeFile:
        includeFile.write(INCLUDE_HEADER)
        for i in range(NUMBER_OF_PERMUTATIONS):
            decString = (f"uint64_t permutationJoin{i}(uint64_t dummyVar, size_t startOffset, size_t stopOffset," +
                         "std::vector<size_t> selectivityValues, " +
                         "APAJ::FactTable* factTable, APAJ::DimensionTable* firstDimension, " +
                         "APAJ::DimensionTable* secondDimension, APAJ::DimensionTable* thirdDimension," +
                         "APAJ::DimensionTable* fourthDimension);\n")
            includeFile.write(decString)

        includeFile.write(INCLUDE_FOOTER)
    
    VOID_LINES = ["((void) factTable);",
                  "((void) firstDimension);",
                  "((void) secondDimension);",
                  "((void) thirdDimension);",
                  "((void) fourthDimension);"]
    
    PERMUTATION_LINES = [
        "if (firstDimension[factTable[factTableCnt].getDimensionKey(1)].getValue() < selectivityValues[0])",
        "if (secondDimension[factTable[factTableCnt].getDimensionKey(2)].getValue() < selectivityValues[1])",
        "if (thirdDimension[factTable[factTableCnt].getDimensionKey(3)].getValue() < selectivityValues[2])",
        "if (fourthDimension[factTable[factTableCnt].getDimensionKey(4)].getValue() < selectivityValues[3])"]
    ALL_PERMUTATION_LINE_LIST = list(itertools.permutations([x for x in range(numberOfJoins)]))
    
    PERMUTATION_OUTPUT_LINES = ["dummyVar += firstDimension[factTable[factTableCnt].getDimensionKey(1)].getValue();",
                                "dummyVar += secondDimension[factTable[factTableCnt].getDimensionKey(2)].getValue();",
                                "dummyVar += thirdDimension[factTable[factTableCnt].getDimensionKey(3)].getValue();",
                                "dummyVar += fourthDimension[factTable[factTableCnt].getDimensionKey(4)].getValue();",
                                "dummyVar += factTable[factTableCnt].getValue();"]
    
    with open(DST_SOURCE_FILE_NAME, 'w') as sourceFile:
        sourceFile.write("#include <Join/join_permutations.hpp>\nnamespace APAJ::PermutationJoin {\n")
    
        sourceFile.write(
            """uint64_t permutationJoin(size_t permutation, uint64_t dummyVar, size_t startOffset, size_t stopOffset, std::vector<size_t> selectivityValues,
                                    APAJ::FactTable* factTable, APAJ::DimensionTable* firstDimension, APAJ::DimensionTable* secondDimension,
                                    APAJ::DimensionTable* thirdDimension, APAJ::DimensionTable* fourthDimension) {\n""")
    
        for i in range(NUMBER_OF_PERMUTATIONS):
            sourceFile.write(
                f"""\t\tif (permutation == {i}) {{
                return permutationJoin{i}(dummyVar, startOffset, stopOffset, selectivityValues, factTable, firstDimension, secondDimension, thirdDimension, fourthDimension);
        \t}}\n""")
    
        # Else just so that no error during compilation happens
        sourceFile.write(
            """\t\telse {
                        return 42;
                \t}\n""")
    
        # End of void permutationJoin(...)
        sourceFile.write("\t}")
    
        # For each permutation / function
        for i in range(NUMBER_OF_PERMUTATIONS):
            sourceFile.write(
                f"""\n\n\tuint64_t permutationJoin{i}(uint64_t dummyVar, size_t startOffset, size_t stopOffset, std::vector<size_t> selectivityValues,
                                                      APAJ::FactTable* factTable, APAJ::DimensionTable* firstDimension, APAJ::DimensionTable* secondDimension,
                                                      APAJ::DimensionTable* thirdDimension, APAJ::DimensionTable* fourthDimension) {{
            for (size_t factTableCnt = startOffset; factTableCnt < stopOffset; ++factTableCnt) {{\n""")
    
            for k in range(numberOfJoins):
                sourceFile.write("\t\t" + "\t" * (k + 1))
                sourceFile.write(f"{PERMUTATION_LINES[ALL_PERMUTATION_LINE_LIST[i][k]]} {{\n")
    
            # After all ifs have been written, output line with three extra tabs (one for function, one for loop and one for new block)
            sourceFile.write(
                "\t" * (numberOfJoins + 3) + f"{PERMUTATION_OUTPUT_LINES[ALL_PERMUTATION_LINE_LIST[i][-1]]}\n")
    
            # Closing all open brackets from ifs
            for k in range(numberOfJoins):
                sourceFile.write("\t\t" + "\t" * ((numberOfJoins - k)))
                sourceFile.write("}\n")
    
            # Closing bracket for for loop
            sourceFile.write("\t\t}\n")
    
            # Returning modified dummyVar value
            sourceFile.write("\t\treturn dummyVar;\n")
    
            # Adding (void) for all unused tables
            for k in range(numberOfJoins + 1, len(VOID_LINES)):
                sourceFile.write("\t\t" + VOID_LINES[k] + "\n")
    
            # End of function
            sourceFile.write("\t}\n\n")
    
        sourceFile.write("\n}")

    with open(MAIN_CPP_FILE_NAME, 'r') as origMain:
        with open(f"/tmp/main.cpp.bak", 'w') as bakMain:
            bakMain.writelines(origMain.readlines())
    
    pattern = re.compile(REGEX_MAIN_CPP_LINE)
    with open(MAIN_CPP_FILE_NAME, 'w') as origMain:
        with open(f"/tmp/main.cpp.bak", 'r') as bakMain:
            for line in bakMain.readlines():
                if pattern.search(line):
                    origMain.write(f"\tuint64_t NUMBER_OF_JOINS = {numberOfJoins};\n")
                else:
                    origMain.write(line)

    
if __name__ == "main":
    numberOfJoins = int(sys.argv[1]) if len(sys.argv) == 2 else 4
    createPermutationFunctions(numberOfJoins)