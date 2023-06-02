#ifndef APAJ_HPC_PERMUTATION_JOIN_HPP
#define APAJ_HPC_PERMUTATION_JOIN_HPP

#include <Tables/DimensionTable.hpp>
#include <Tables/FactTable.hpp>
#include <unistd.h>
#include <vector>

namespace APAJ::PermutationJoin {
    uint64_t permutationJoin(size_t permutation, uint64_t dummyVar, size_t startOffset, size_t stopOffset, 
                             std::vector<size_t> selectivityValues, APAJ::FactTable* factTable, 
                             APAJ::DimensionTable* firstDimension, APAJ::DimensionTable* secondDimension, 
                             APAJ::DimensionTable* thirdDimension, APAJ::DimensionTable* fourthDimension);

uint64_t permutationJoin0(uint64_t dummyVar, size_t startOffset, size_t stopOffset,std::vector<size_t> selectivityValues, APAJ::FactTable* factTable, APAJ::DimensionTable* firstDimension, APAJ::DimensionTable* secondDimension, APAJ::DimensionTable* thirdDimension,APAJ::DimensionTable* fourthDimension);
uint64_t permutationJoin1(uint64_t dummyVar, size_t startOffset, size_t stopOffset,std::vector<size_t> selectivityValues, APAJ::FactTable* factTable, APAJ::DimensionTable* firstDimension, APAJ::DimensionTable* secondDimension, APAJ::DimensionTable* thirdDimension,APAJ::DimensionTable* fourthDimension);
uint64_t permutationJoin2(uint64_t dummyVar, size_t startOffset, size_t stopOffset,std::vector<size_t> selectivityValues, APAJ::FactTable* factTable, APAJ::DimensionTable* firstDimension, APAJ::DimensionTable* secondDimension, APAJ::DimensionTable* thirdDimension,APAJ::DimensionTable* fourthDimension);
uint64_t permutationJoin3(uint64_t dummyVar, size_t startOffset, size_t stopOffset,std::vector<size_t> selectivityValues, APAJ::FactTable* factTable, APAJ::DimensionTable* firstDimension, APAJ::DimensionTable* secondDimension, APAJ::DimensionTable* thirdDimension,APAJ::DimensionTable* fourthDimension);
uint64_t permutationJoin4(uint64_t dummyVar, size_t startOffset, size_t stopOffset,std::vector<size_t> selectivityValues, APAJ::FactTable* factTable, APAJ::DimensionTable* firstDimension, APAJ::DimensionTable* secondDimension, APAJ::DimensionTable* thirdDimension,APAJ::DimensionTable* fourthDimension);
uint64_t permutationJoin5(uint64_t dummyVar, size_t startOffset, size_t stopOffset,std::vector<size_t> selectivityValues, APAJ::FactTable* factTable, APAJ::DimensionTable* firstDimension, APAJ::DimensionTable* secondDimension, APAJ::DimensionTable* thirdDimension,APAJ::DimensionTable* fourthDimension);
uint64_t permutationJoin6(uint64_t dummyVar, size_t startOffset, size_t stopOffset,std::vector<size_t> selectivityValues, APAJ::FactTable* factTable, APAJ::DimensionTable* firstDimension, APAJ::DimensionTable* secondDimension, APAJ::DimensionTable* thirdDimension,APAJ::DimensionTable* fourthDimension);
uint64_t permutationJoin7(uint64_t dummyVar, size_t startOffset, size_t stopOffset,std::vector<size_t> selectivityValues, APAJ::FactTable* factTable, APAJ::DimensionTable* firstDimension, APAJ::DimensionTable* secondDimension, APAJ::DimensionTable* thirdDimension,APAJ::DimensionTable* fourthDimension);
uint64_t permutationJoin8(uint64_t dummyVar, size_t startOffset, size_t stopOffset,std::vector<size_t> selectivityValues, APAJ::FactTable* factTable, APAJ::DimensionTable* firstDimension, APAJ::DimensionTable* secondDimension, APAJ::DimensionTable* thirdDimension,APAJ::DimensionTable* fourthDimension);
uint64_t permutationJoin9(uint64_t dummyVar, size_t startOffset, size_t stopOffset,std::vector<size_t> selectivityValues, APAJ::FactTable* factTable, APAJ::DimensionTable* firstDimension, APAJ::DimensionTable* secondDimension, APAJ::DimensionTable* thirdDimension,APAJ::DimensionTable* fourthDimension);
uint64_t permutationJoin10(uint64_t dummyVar, size_t startOffset, size_t stopOffset,std::vector<size_t> selectivityValues, APAJ::FactTable* factTable, APAJ::DimensionTable* firstDimension, APAJ::DimensionTable* secondDimension, APAJ::DimensionTable* thirdDimension,APAJ::DimensionTable* fourthDimension);
uint64_t permutationJoin11(uint64_t dummyVar, size_t startOffset, size_t stopOffset,std::vector<size_t> selectivityValues, APAJ::FactTable* factTable, APAJ::DimensionTable* firstDimension, APAJ::DimensionTable* secondDimension, APAJ::DimensionTable* thirdDimension,APAJ::DimensionTable* fourthDimension);
uint64_t permutationJoin12(uint64_t dummyVar, size_t startOffset, size_t stopOffset,std::vector<size_t> selectivityValues, APAJ::FactTable* factTable, APAJ::DimensionTable* firstDimension, APAJ::DimensionTable* secondDimension, APAJ::DimensionTable* thirdDimension,APAJ::DimensionTable* fourthDimension);
uint64_t permutationJoin13(uint64_t dummyVar, size_t startOffset, size_t stopOffset,std::vector<size_t> selectivityValues, APAJ::FactTable* factTable, APAJ::DimensionTable* firstDimension, APAJ::DimensionTable* secondDimension, APAJ::DimensionTable* thirdDimension,APAJ::DimensionTable* fourthDimension);
uint64_t permutationJoin14(uint64_t dummyVar, size_t startOffset, size_t stopOffset,std::vector<size_t> selectivityValues, APAJ::FactTable* factTable, APAJ::DimensionTable* firstDimension, APAJ::DimensionTable* secondDimension, APAJ::DimensionTable* thirdDimension,APAJ::DimensionTable* fourthDimension);
uint64_t permutationJoin15(uint64_t dummyVar, size_t startOffset, size_t stopOffset,std::vector<size_t> selectivityValues, APAJ::FactTable* factTable, APAJ::DimensionTable* firstDimension, APAJ::DimensionTable* secondDimension, APAJ::DimensionTable* thirdDimension,APAJ::DimensionTable* fourthDimension);
uint64_t permutationJoin16(uint64_t dummyVar, size_t startOffset, size_t stopOffset,std::vector<size_t> selectivityValues, APAJ::FactTable* factTable, APAJ::DimensionTable* firstDimension, APAJ::DimensionTable* secondDimension, APAJ::DimensionTable* thirdDimension,APAJ::DimensionTable* fourthDimension);
uint64_t permutationJoin17(uint64_t dummyVar, size_t startOffset, size_t stopOffset,std::vector<size_t> selectivityValues, APAJ::FactTable* factTable, APAJ::DimensionTable* firstDimension, APAJ::DimensionTable* secondDimension, APAJ::DimensionTable* thirdDimension,APAJ::DimensionTable* fourthDimension);
uint64_t permutationJoin18(uint64_t dummyVar, size_t startOffset, size_t stopOffset,std::vector<size_t> selectivityValues, APAJ::FactTable* factTable, APAJ::DimensionTable* firstDimension, APAJ::DimensionTable* secondDimension, APAJ::DimensionTable* thirdDimension,APAJ::DimensionTable* fourthDimension);
uint64_t permutationJoin19(uint64_t dummyVar, size_t startOffset, size_t stopOffset,std::vector<size_t> selectivityValues, APAJ::FactTable* factTable, APAJ::DimensionTable* firstDimension, APAJ::DimensionTable* secondDimension, APAJ::DimensionTable* thirdDimension,APAJ::DimensionTable* fourthDimension);
uint64_t permutationJoin20(uint64_t dummyVar, size_t startOffset, size_t stopOffset,std::vector<size_t> selectivityValues, APAJ::FactTable* factTable, APAJ::DimensionTable* firstDimension, APAJ::DimensionTable* secondDimension, APAJ::DimensionTable* thirdDimension,APAJ::DimensionTable* fourthDimension);
uint64_t permutationJoin21(uint64_t dummyVar, size_t startOffset, size_t stopOffset,std::vector<size_t> selectivityValues, APAJ::FactTable* factTable, APAJ::DimensionTable* firstDimension, APAJ::DimensionTable* secondDimension, APAJ::DimensionTable* thirdDimension,APAJ::DimensionTable* fourthDimension);
uint64_t permutationJoin22(uint64_t dummyVar, size_t startOffset, size_t stopOffset,std::vector<size_t> selectivityValues, APAJ::FactTable* factTable, APAJ::DimensionTable* firstDimension, APAJ::DimensionTable* secondDimension, APAJ::DimensionTable* thirdDimension,APAJ::DimensionTable* fourthDimension);
uint64_t permutationJoin23(uint64_t dummyVar, size_t startOffset, size_t stopOffset,std::vector<size_t> selectivityValues, APAJ::FactTable* factTable, APAJ::DimensionTable* firstDimension, APAJ::DimensionTable* secondDimension, APAJ::DimensionTable* thirdDimension,APAJ::DimensionTable* fourthDimension);

} // namespace APAJ::PermutationJoin

#endif //APAJ_HPC_PERMUTATION_JOIN_HPP
