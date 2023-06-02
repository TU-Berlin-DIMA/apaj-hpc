#include <Join/join_permutations.hpp>
namespace APAJ::PermutationJoin {
uint64_t permutationJoin(size_t permutation, uint64_t dummyVar, size_t startOffset, size_t stopOffset, std::vector<size_t> selectivityValues,
                                APAJ::FactTable* factTable, APAJ::DimensionTable* firstDimension, APAJ::DimensionTable* secondDimension,
                                APAJ::DimensionTable* thirdDimension, APAJ::DimensionTable* fourthDimension) {
		if (permutation == 0) {
	    	return permutationJoin0(dummyVar, startOffset, stopOffset, selectivityValues, factTable, firstDimension, secondDimension, thirdDimension, fourthDimension);
		}
		if (permutation == 1) {
	    	return permutationJoin1(dummyVar, startOffset, stopOffset, selectivityValues, factTable, firstDimension, secondDimension, thirdDimension, fourthDimension);
		}
		if (permutation == 2) {
	    	return permutationJoin2(dummyVar, startOffset, stopOffset, selectivityValues, factTable, firstDimension, secondDimension, thirdDimension, fourthDimension);
		}
		if (permutation == 3) {
	    	return permutationJoin3(dummyVar, startOffset, stopOffset, selectivityValues, factTable, firstDimension, secondDimension, thirdDimension, fourthDimension);
		}
		if (permutation == 4) {
	    	return permutationJoin4(dummyVar, startOffset, stopOffset, selectivityValues, factTable, firstDimension, secondDimension, thirdDimension, fourthDimension);
		}
		if (permutation == 5) {
	    	return permutationJoin5(dummyVar, startOffset, stopOffset, selectivityValues, factTable, firstDimension, secondDimension, thirdDimension, fourthDimension);
		}
		if (permutation == 6) {
	    	return permutationJoin6(dummyVar, startOffset, stopOffset, selectivityValues, factTable, firstDimension, secondDimension, thirdDimension, fourthDimension);
		}
		if (permutation == 7) {
	    	return permutationJoin7(dummyVar, startOffset, stopOffset, selectivityValues, factTable, firstDimension, secondDimension, thirdDimension, fourthDimension);
		}
		if (permutation == 8) {
	    	return permutationJoin8(dummyVar, startOffset, stopOffset, selectivityValues, factTable, firstDimension, secondDimension, thirdDimension, fourthDimension);
		}
		if (permutation == 9) {
	    	return permutationJoin9(dummyVar, startOffset, stopOffset, selectivityValues, factTable, firstDimension, secondDimension, thirdDimension, fourthDimension);
		}
		if (permutation == 10) {
	    	return permutationJoin10(dummyVar, startOffset, stopOffset, selectivityValues, factTable, firstDimension, secondDimension, thirdDimension, fourthDimension);
		}
		if (permutation == 11) {
	    	return permutationJoin11(dummyVar, startOffset, stopOffset, selectivityValues, factTable, firstDimension, secondDimension, thirdDimension, fourthDimension);
		}
		if (permutation == 12) {
	    	return permutationJoin12(dummyVar, startOffset, stopOffset, selectivityValues, factTable, firstDimension, secondDimension, thirdDimension, fourthDimension);
		}
		if (permutation == 13) {
	    	return permutationJoin13(dummyVar, startOffset, stopOffset, selectivityValues, factTable, firstDimension, secondDimension, thirdDimension, fourthDimension);
		}
		if (permutation == 14) {
	    	return permutationJoin14(dummyVar, startOffset, stopOffset, selectivityValues, factTable, firstDimension, secondDimension, thirdDimension, fourthDimension);
		}
		if (permutation == 15) {
	    	return permutationJoin15(dummyVar, startOffset, stopOffset, selectivityValues, factTable, firstDimension, secondDimension, thirdDimension, fourthDimension);
		}
		if (permutation == 16) {
	    	return permutationJoin16(dummyVar, startOffset, stopOffset, selectivityValues, factTable, firstDimension, secondDimension, thirdDimension, fourthDimension);
		}
		if (permutation == 17) {
	    	return permutationJoin17(dummyVar, startOffset, stopOffset, selectivityValues, factTable, firstDimension, secondDimension, thirdDimension, fourthDimension);
		}
		if (permutation == 18) {
	    	return permutationJoin18(dummyVar, startOffset, stopOffset, selectivityValues, factTable, firstDimension, secondDimension, thirdDimension, fourthDimension);
		}
		if (permutation == 19) {
	    	return permutationJoin19(dummyVar, startOffset, stopOffset, selectivityValues, factTable, firstDimension, secondDimension, thirdDimension, fourthDimension);
		}
		if (permutation == 20) {
	    	return permutationJoin20(dummyVar, startOffset, stopOffset, selectivityValues, factTable, firstDimension, secondDimension, thirdDimension, fourthDimension);
		}
		if (permutation == 21) {
	    	return permutationJoin21(dummyVar, startOffset, stopOffset, selectivityValues, factTable, firstDimension, secondDimension, thirdDimension, fourthDimension);
		}
		if (permutation == 22) {
	    	return permutationJoin22(dummyVar, startOffset, stopOffset, selectivityValues, factTable, firstDimension, secondDimension, thirdDimension, fourthDimension);
		}
		if (permutation == 23) {
	    	return permutationJoin23(dummyVar, startOffset, stopOffset, selectivityValues, factTable, firstDimension, secondDimension, thirdDimension, fourthDimension);
		}
		else {
                    return 42;
            	}
	}

	uint64_t permutationJoin0(uint64_t dummyVar, size_t startOffset, size_t stopOffset, std::vector<size_t> selectivityValues,
                                                  APAJ::FactTable* factTable, APAJ::DimensionTable* firstDimension, APAJ::DimensionTable* secondDimension,
                                                  APAJ::DimensionTable* thirdDimension, APAJ::DimensionTable* fourthDimension) {
		for (size_t factTableCnt = startOffset; factTableCnt < stopOffset; ++factTableCnt) {
			if (firstDimension[factTable[factTableCnt].getDimensionKey(1)].getValue() < selectivityValues[0]) {
				if (secondDimension[factTable[factTableCnt].getDimensionKey(2)].getValue() < selectivityValues[1]) {
					if (thirdDimension[factTable[factTableCnt].getDimensionKey(3)].getValue() < selectivityValues[2]) {
						if (fourthDimension[factTable[factTableCnt].getDimensionKey(4)].getValue() < selectivityValues[3]) {
							dummyVar += fourthDimension[factTable[factTableCnt].getDimensionKey(4)].getValue();
						}
					}
				}
			}
		}
		return dummyVar;
	}



	uint64_t permutationJoin1(uint64_t dummyVar, size_t startOffset, size_t stopOffset, std::vector<size_t> selectivityValues,
                                                  APAJ::FactTable* factTable, APAJ::DimensionTable* firstDimension, APAJ::DimensionTable* secondDimension,
                                                  APAJ::DimensionTable* thirdDimension, APAJ::DimensionTable* fourthDimension) {
		for (size_t factTableCnt = startOffset; factTableCnt < stopOffset; ++factTableCnt) {
			if (firstDimension[factTable[factTableCnt].getDimensionKey(1)].getValue() < selectivityValues[0]) {
				if (secondDimension[factTable[factTableCnt].getDimensionKey(2)].getValue() < selectivityValues[1]) {
					if (fourthDimension[factTable[factTableCnt].getDimensionKey(4)].getValue() < selectivityValues[3]) {
						if (thirdDimension[factTable[factTableCnt].getDimensionKey(3)].getValue() < selectivityValues[2]) {
							dummyVar += thirdDimension[factTable[factTableCnt].getDimensionKey(3)].getValue();
						}
					}
				}
			}
		}
		return dummyVar;
	}



	uint64_t permutationJoin2(uint64_t dummyVar, size_t startOffset, size_t stopOffset, std::vector<size_t> selectivityValues,
                                                  APAJ::FactTable* factTable, APAJ::DimensionTable* firstDimension, APAJ::DimensionTable* secondDimension,
                                                  APAJ::DimensionTable* thirdDimension, APAJ::DimensionTable* fourthDimension) {
		for (size_t factTableCnt = startOffset; factTableCnt < stopOffset; ++factTableCnt) {
			if (firstDimension[factTable[factTableCnt].getDimensionKey(1)].getValue() < selectivityValues[0]) {
				if (thirdDimension[factTable[factTableCnt].getDimensionKey(3)].getValue() < selectivityValues[2]) {
					if (secondDimension[factTable[factTableCnt].getDimensionKey(2)].getValue() < selectivityValues[1]) {
						if (fourthDimension[factTable[factTableCnt].getDimensionKey(4)].getValue() < selectivityValues[3]) {
							dummyVar += fourthDimension[factTable[factTableCnt].getDimensionKey(4)].getValue();
						}
					}
				}
			}
		}
		return dummyVar;
	}



	uint64_t permutationJoin3(uint64_t dummyVar, size_t startOffset, size_t stopOffset, std::vector<size_t> selectivityValues,
                                                  APAJ::FactTable* factTable, APAJ::DimensionTable* firstDimension, APAJ::DimensionTable* secondDimension,
                                                  APAJ::DimensionTable* thirdDimension, APAJ::DimensionTable* fourthDimension) {
		for (size_t factTableCnt = startOffset; factTableCnt < stopOffset; ++factTableCnt) {
			if (firstDimension[factTable[factTableCnt].getDimensionKey(1)].getValue() < selectivityValues[0]) {
				if (thirdDimension[factTable[factTableCnt].getDimensionKey(3)].getValue() < selectivityValues[2]) {
					if (fourthDimension[factTable[factTableCnt].getDimensionKey(4)].getValue() < selectivityValues[3]) {
						if (secondDimension[factTable[factTableCnt].getDimensionKey(2)].getValue() < selectivityValues[1]) {
							dummyVar += secondDimension[factTable[factTableCnt].getDimensionKey(2)].getValue();
						}
					}
				}
			}
		}
		return dummyVar;
	}



	uint64_t permutationJoin4(uint64_t dummyVar, size_t startOffset, size_t stopOffset, std::vector<size_t> selectivityValues,
                                                  APAJ::FactTable* factTable, APAJ::DimensionTable* firstDimension, APAJ::DimensionTable* secondDimension,
                                                  APAJ::DimensionTable* thirdDimension, APAJ::DimensionTable* fourthDimension) {
		for (size_t factTableCnt = startOffset; factTableCnt < stopOffset; ++factTableCnt) {
			if (firstDimension[factTable[factTableCnt].getDimensionKey(1)].getValue() < selectivityValues[0]) {
				if (fourthDimension[factTable[factTableCnt].getDimensionKey(4)].getValue() < selectivityValues[3]) {
					if (secondDimension[factTable[factTableCnt].getDimensionKey(2)].getValue() < selectivityValues[1]) {
						if (thirdDimension[factTable[factTableCnt].getDimensionKey(3)].getValue() < selectivityValues[2]) {
							dummyVar += thirdDimension[factTable[factTableCnt].getDimensionKey(3)].getValue();
						}
					}
				}
			}
		}
		return dummyVar;
	}



	uint64_t permutationJoin5(uint64_t dummyVar, size_t startOffset, size_t stopOffset, std::vector<size_t> selectivityValues,
                                                  APAJ::FactTable* factTable, APAJ::DimensionTable* firstDimension, APAJ::DimensionTable* secondDimension,
                                                  APAJ::DimensionTable* thirdDimension, APAJ::DimensionTable* fourthDimension) {
		for (size_t factTableCnt = startOffset; factTableCnt < stopOffset; ++factTableCnt) {
			if (firstDimension[factTable[factTableCnt].getDimensionKey(1)].getValue() < selectivityValues[0]) {
				if (fourthDimension[factTable[factTableCnt].getDimensionKey(4)].getValue() < selectivityValues[3]) {
					if (thirdDimension[factTable[factTableCnt].getDimensionKey(3)].getValue() < selectivityValues[2]) {
						if (secondDimension[factTable[factTableCnt].getDimensionKey(2)].getValue() < selectivityValues[1]) {
							dummyVar += secondDimension[factTable[factTableCnt].getDimensionKey(2)].getValue();
						}
					}
				}
			}
		}
		return dummyVar;
	}



	uint64_t permutationJoin6(uint64_t dummyVar, size_t startOffset, size_t stopOffset, std::vector<size_t> selectivityValues,
                                                  APAJ::FactTable* factTable, APAJ::DimensionTable* firstDimension, APAJ::DimensionTable* secondDimension,
                                                  APAJ::DimensionTable* thirdDimension, APAJ::DimensionTable* fourthDimension) {
		for (size_t factTableCnt = startOffset; factTableCnt < stopOffset; ++factTableCnt) {
			if (secondDimension[factTable[factTableCnt].getDimensionKey(2)].getValue() < selectivityValues[1]) {
				if (firstDimension[factTable[factTableCnt].getDimensionKey(1)].getValue() < selectivityValues[0]) {
					if (thirdDimension[factTable[factTableCnt].getDimensionKey(3)].getValue() < selectivityValues[2]) {
						if (fourthDimension[factTable[factTableCnt].getDimensionKey(4)].getValue() < selectivityValues[3]) {
							dummyVar += fourthDimension[factTable[factTableCnt].getDimensionKey(4)].getValue();
						}
					}
				}
			}
		}
		return dummyVar;
	}



	uint64_t permutationJoin7(uint64_t dummyVar, size_t startOffset, size_t stopOffset, std::vector<size_t> selectivityValues,
                                                  APAJ::FactTable* factTable, APAJ::DimensionTable* firstDimension, APAJ::DimensionTable* secondDimension,
                                                  APAJ::DimensionTable* thirdDimension, APAJ::DimensionTable* fourthDimension) {
		for (size_t factTableCnt = startOffset; factTableCnt < stopOffset; ++factTableCnt) {
			if (secondDimension[factTable[factTableCnt].getDimensionKey(2)].getValue() < selectivityValues[1]) {
				if (firstDimension[factTable[factTableCnt].getDimensionKey(1)].getValue() < selectivityValues[0]) {
					if (fourthDimension[factTable[factTableCnt].getDimensionKey(4)].getValue() < selectivityValues[3]) {
						if (thirdDimension[factTable[factTableCnt].getDimensionKey(3)].getValue() < selectivityValues[2]) {
							dummyVar += thirdDimension[factTable[factTableCnt].getDimensionKey(3)].getValue();
						}
					}
				}
			}
		}
		return dummyVar;
	}



	uint64_t permutationJoin8(uint64_t dummyVar, size_t startOffset, size_t stopOffset, std::vector<size_t> selectivityValues,
                                                  APAJ::FactTable* factTable, APAJ::DimensionTable* firstDimension, APAJ::DimensionTable* secondDimension,
                                                  APAJ::DimensionTable* thirdDimension, APAJ::DimensionTable* fourthDimension) {
		for (size_t factTableCnt = startOffset; factTableCnt < stopOffset; ++factTableCnt) {
			if (secondDimension[factTable[factTableCnt].getDimensionKey(2)].getValue() < selectivityValues[1]) {
				if (thirdDimension[factTable[factTableCnt].getDimensionKey(3)].getValue() < selectivityValues[2]) {
					if (firstDimension[factTable[factTableCnt].getDimensionKey(1)].getValue() < selectivityValues[0]) {
						if (fourthDimension[factTable[factTableCnt].getDimensionKey(4)].getValue() < selectivityValues[3]) {
							dummyVar += fourthDimension[factTable[factTableCnt].getDimensionKey(4)].getValue();
						}
					}
				}
			}
		}
		return dummyVar;
	}



	uint64_t permutationJoin9(uint64_t dummyVar, size_t startOffset, size_t stopOffset, std::vector<size_t> selectivityValues,
                                                  APAJ::FactTable* factTable, APAJ::DimensionTable* firstDimension, APAJ::DimensionTable* secondDimension,
                                                  APAJ::DimensionTable* thirdDimension, APAJ::DimensionTable* fourthDimension) {
		for (size_t factTableCnt = startOffset; factTableCnt < stopOffset; ++factTableCnt) {
			if (secondDimension[factTable[factTableCnt].getDimensionKey(2)].getValue() < selectivityValues[1]) {
				if (thirdDimension[factTable[factTableCnt].getDimensionKey(3)].getValue() < selectivityValues[2]) {
					if (fourthDimension[factTable[factTableCnt].getDimensionKey(4)].getValue() < selectivityValues[3]) {
						if (firstDimension[factTable[factTableCnt].getDimensionKey(1)].getValue() < selectivityValues[0]) {
							dummyVar += firstDimension[factTable[factTableCnt].getDimensionKey(1)].getValue();
						}
					}
				}
			}
		}
		return dummyVar;
	}



	uint64_t permutationJoin10(uint64_t dummyVar, size_t startOffset, size_t stopOffset, std::vector<size_t> selectivityValues,
                                                  APAJ::FactTable* factTable, APAJ::DimensionTable* firstDimension, APAJ::DimensionTable* secondDimension,
                                                  APAJ::DimensionTable* thirdDimension, APAJ::DimensionTable* fourthDimension) {
		for (size_t factTableCnt = startOffset; factTableCnt < stopOffset; ++factTableCnt) {
			if (secondDimension[factTable[factTableCnt].getDimensionKey(2)].getValue() < selectivityValues[1]) {
				if (fourthDimension[factTable[factTableCnt].getDimensionKey(4)].getValue() < selectivityValues[3]) {
					if (firstDimension[factTable[factTableCnt].getDimensionKey(1)].getValue() < selectivityValues[0]) {
						if (thirdDimension[factTable[factTableCnt].getDimensionKey(3)].getValue() < selectivityValues[2]) {
							dummyVar += thirdDimension[factTable[factTableCnt].getDimensionKey(3)].getValue();
						}
					}
				}
			}
		}
		return dummyVar;
	}



	uint64_t permutationJoin11(uint64_t dummyVar, size_t startOffset, size_t stopOffset, std::vector<size_t> selectivityValues,
                                                  APAJ::FactTable* factTable, APAJ::DimensionTable* firstDimension, APAJ::DimensionTable* secondDimension,
                                                  APAJ::DimensionTable* thirdDimension, APAJ::DimensionTable* fourthDimension) {
		for (size_t factTableCnt = startOffset; factTableCnt < stopOffset; ++factTableCnt) {
			if (secondDimension[factTable[factTableCnt].getDimensionKey(2)].getValue() < selectivityValues[1]) {
				if (fourthDimension[factTable[factTableCnt].getDimensionKey(4)].getValue() < selectivityValues[3]) {
					if (thirdDimension[factTable[factTableCnt].getDimensionKey(3)].getValue() < selectivityValues[2]) {
						if (firstDimension[factTable[factTableCnt].getDimensionKey(1)].getValue() < selectivityValues[0]) {
							dummyVar += firstDimension[factTable[factTableCnt].getDimensionKey(1)].getValue();
						}
					}
				}
			}
		}
		return dummyVar;
	}



	uint64_t permutationJoin12(uint64_t dummyVar, size_t startOffset, size_t stopOffset, std::vector<size_t> selectivityValues,
                                                  APAJ::FactTable* factTable, APAJ::DimensionTable* firstDimension, APAJ::DimensionTable* secondDimension,
                                                  APAJ::DimensionTable* thirdDimension, APAJ::DimensionTable* fourthDimension) {
		for (size_t factTableCnt = startOffset; factTableCnt < stopOffset; ++factTableCnt) {
			if (thirdDimension[factTable[factTableCnt].getDimensionKey(3)].getValue() < selectivityValues[2]) {
				if (firstDimension[factTable[factTableCnt].getDimensionKey(1)].getValue() < selectivityValues[0]) {
					if (secondDimension[factTable[factTableCnt].getDimensionKey(2)].getValue() < selectivityValues[1]) {
						if (fourthDimension[factTable[factTableCnt].getDimensionKey(4)].getValue() < selectivityValues[3]) {
							dummyVar += fourthDimension[factTable[factTableCnt].getDimensionKey(4)].getValue();
						}
					}
				}
			}
		}
		return dummyVar;
	}



	uint64_t permutationJoin13(uint64_t dummyVar, size_t startOffset, size_t stopOffset, std::vector<size_t> selectivityValues,
                                                  APAJ::FactTable* factTable, APAJ::DimensionTable* firstDimension, APAJ::DimensionTable* secondDimension,
                                                  APAJ::DimensionTable* thirdDimension, APAJ::DimensionTable* fourthDimension) {
		for (size_t factTableCnt = startOffset; factTableCnt < stopOffset; ++factTableCnt) {
			if (thirdDimension[factTable[factTableCnt].getDimensionKey(3)].getValue() < selectivityValues[2]) {
				if (firstDimension[factTable[factTableCnt].getDimensionKey(1)].getValue() < selectivityValues[0]) {
					if (fourthDimension[factTable[factTableCnt].getDimensionKey(4)].getValue() < selectivityValues[3]) {
						if (secondDimension[factTable[factTableCnt].getDimensionKey(2)].getValue() < selectivityValues[1]) {
							dummyVar += secondDimension[factTable[factTableCnt].getDimensionKey(2)].getValue();
						}
					}
				}
			}
		}
		return dummyVar;
	}



	uint64_t permutationJoin14(uint64_t dummyVar, size_t startOffset, size_t stopOffset, std::vector<size_t> selectivityValues,
                                                  APAJ::FactTable* factTable, APAJ::DimensionTable* firstDimension, APAJ::DimensionTable* secondDimension,
                                                  APAJ::DimensionTable* thirdDimension, APAJ::DimensionTable* fourthDimension) {
		for (size_t factTableCnt = startOffset; factTableCnt < stopOffset; ++factTableCnt) {
			if (thirdDimension[factTable[factTableCnt].getDimensionKey(3)].getValue() < selectivityValues[2]) {
				if (secondDimension[factTable[factTableCnt].getDimensionKey(2)].getValue() < selectivityValues[1]) {
					if (firstDimension[factTable[factTableCnt].getDimensionKey(1)].getValue() < selectivityValues[0]) {
						if (fourthDimension[factTable[factTableCnt].getDimensionKey(4)].getValue() < selectivityValues[3]) {
							dummyVar += fourthDimension[factTable[factTableCnt].getDimensionKey(4)].getValue();
						}
					}
				}
			}
		}
		return dummyVar;
	}



	uint64_t permutationJoin15(uint64_t dummyVar, size_t startOffset, size_t stopOffset, std::vector<size_t> selectivityValues,
                                                  APAJ::FactTable* factTable, APAJ::DimensionTable* firstDimension, APAJ::DimensionTable* secondDimension,
                                                  APAJ::DimensionTable* thirdDimension, APAJ::DimensionTable* fourthDimension) {
		for (size_t factTableCnt = startOffset; factTableCnt < stopOffset; ++factTableCnt) {
			if (thirdDimension[factTable[factTableCnt].getDimensionKey(3)].getValue() < selectivityValues[2]) {
				if (secondDimension[factTable[factTableCnt].getDimensionKey(2)].getValue() < selectivityValues[1]) {
					if (fourthDimension[factTable[factTableCnt].getDimensionKey(4)].getValue() < selectivityValues[3]) {
						if (firstDimension[factTable[factTableCnt].getDimensionKey(1)].getValue() < selectivityValues[0]) {
							dummyVar += firstDimension[factTable[factTableCnt].getDimensionKey(1)].getValue();
						}
					}
				}
			}
		}
		return dummyVar;
	}



	uint64_t permutationJoin16(uint64_t dummyVar, size_t startOffset, size_t stopOffset, std::vector<size_t> selectivityValues,
                                                  APAJ::FactTable* factTable, APAJ::DimensionTable* firstDimension, APAJ::DimensionTable* secondDimension,
                                                  APAJ::DimensionTable* thirdDimension, APAJ::DimensionTable* fourthDimension) {
		for (size_t factTableCnt = startOffset; factTableCnt < stopOffset; ++factTableCnt) {
			if (thirdDimension[factTable[factTableCnt].getDimensionKey(3)].getValue() < selectivityValues[2]) {
				if (fourthDimension[factTable[factTableCnt].getDimensionKey(4)].getValue() < selectivityValues[3]) {
					if (firstDimension[factTable[factTableCnt].getDimensionKey(1)].getValue() < selectivityValues[0]) {
						if (secondDimension[factTable[factTableCnt].getDimensionKey(2)].getValue() < selectivityValues[1]) {
							dummyVar += secondDimension[factTable[factTableCnt].getDimensionKey(2)].getValue();
						}
					}
				}
			}
		}
		return dummyVar;
	}



	uint64_t permutationJoin17(uint64_t dummyVar, size_t startOffset, size_t stopOffset, std::vector<size_t> selectivityValues,
                                                  APAJ::FactTable* factTable, APAJ::DimensionTable* firstDimension, APAJ::DimensionTable* secondDimension,
                                                  APAJ::DimensionTable* thirdDimension, APAJ::DimensionTable* fourthDimension) {
		for (size_t factTableCnt = startOffset; factTableCnt < stopOffset; ++factTableCnt) {
			if (thirdDimension[factTable[factTableCnt].getDimensionKey(3)].getValue() < selectivityValues[2]) {
				if (fourthDimension[factTable[factTableCnt].getDimensionKey(4)].getValue() < selectivityValues[3]) {
					if (secondDimension[factTable[factTableCnt].getDimensionKey(2)].getValue() < selectivityValues[1]) {
						if (firstDimension[factTable[factTableCnt].getDimensionKey(1)].getValue() < selectivityValues[0]) {
							dummyVar += firstDimension[factTable[factTableCnt].getDimensionKey(1)].getValue();
						}
					}
				}
			}
		}
		return dummyVar;
	}



	uint64_t permutationJoin18(uint64_t dummyVar, size_t startOffset, size_t stopOffset, std::vector<size_t> selectivityValues,
                                                  APAJ::FactTable* factTable, APAJ::DimensionTable* firstDimension, APAJ::DimensionTable* secondDimension,
                                                  APAJ::DimensionTable* thirdDimension, APAJ::DimensionTable* fourthDimension) {
		for (size_t factTableCnt = startOffset; factTableCnt < stopOffset; ++factTableCnt) {
			if (fourthDimension[factTable[factTableCnt].getDimensionKey(4)].getValue() < selectivityValues[3]) {
				if (firstDimension[factTable[factTableCnt].getDimensionKey(1)].getValue() < selectivityValues[0]) {
					if (secondDimension[factTable[factTableCnt].getDimensionKey(2)].getValue() < selectivityValues[1]) {
						if (thirdDimension[factTable[factTableCnt].getDimensionKey(3)].getValue() < selectivityValues[2]) {
							dummyVar += thirdDimension[factTable[factTableCnt].getDimensionKey(3)].getValue();
						}
					}
				}
			}
		}
		return dummyVar;
	}



	uint64_t permutationJoin19(uint64_t dummyVar, size_t startOffset, size_t stopOffset, std::vector<size_t> selectivityValues,
                                                  APAJ::FactTable* factTable, APAJ::DimensionTable* firstDimension, APAJ::DimensionTable* secondDimension,
                                                  APAJ::DimensionTable* thirdDimension, APAJ::DimensionTable* fourthDimension) {
		for (size_t factTableCnt = startOffset; factTableCnt < stopOffset; ++factTableCnt) {
			if (fourthDimension[factTable[factTableCnt].getDimensionKey(4)].getValue() < selectivityValues[3]) {
				if (firstDimension[factTable[factTableCnt].getDimensionKey(1)].getValue() < selectivityValues[0]) {
					if (thirdDimension[factTable[factTableCnt].getDimensionKey(3)].getValue() < selectivityValues[2]) {
						if (secondDimension[factTable[factTableCnt].getDimensionKey(2)].getValue() < selectivityValues[1]) {
							dummyVar += secondDimension[factTable[factTableCnt].getDimensionKey(2)].getValue();
						}
					}
				}
			}
		}
		return dummyVar;
	}



	uint64_t permutationJoin20(uint64_t dummyVar, size_t startOffset, size_t stopOffset, std::vector<size_t> selectivityValues,
                                                  APAJ::FactTable* factTable, APAJ::DimensionTable* firstDimension, APAJ::DimensionTable* secondDimension,
                                                  APAJ::DimensionTable* thirdDimension, APAJ::DimensionTable* fourthDimension) {
		for (size_t factTableCnt = startOffset; factTableCnt < stopOffset; ++factTableCnt) {
			if (fourthDimension[factTable[factTableCnt].getDimensionKey(4)].getValue() < selectivityValues[3]) {
				if (secondDimension[factTable[factTableCnt].getDimensionKey(2)].getValue() < selectivityValues[1]) {
					if (firstDimension[factTable[factTableCnt].getDimensionKey(1)].getValue() < selectivityValues[0]) {
						if (thirdDimension[factTable[factTableCnt].getDimensionKey(3)].getValue() < selectivityValues[2]) {
							dummyVar += thirdDimension[factTable[factTableCnt].getDimensionKey(3)].getValue();
						}
					}
				}
			}
		}
		return dummyVar;
	}



	uint64_t permutationJoin21(uint64_t dummyVar, size_t startOffset, size_t stopOffset, std::vector<size_t> selectivityValues,
                                                  APAJ::FactTable* factTable, APAJ::DimensionTable* firstDimension, APAJ::DimensionTable* secondDimension,
                                                  APAJ::DimensionTable* thirdDimension, APAJ::DimensionTable* fourthDimension) {
		for (size_t factTableCnt = startOffset; factTableCnt < stopOffset; ++factTableCnt) {
			if (fourthDimension[factTable[factTableCnt].getDimensionKey(4)].getValue() < selectivityValues[3]) {
				if (secondDimension[factTable[factTableCnt].getDimensionKey(2)].getValue() < selectivityValues[1]) {
					if (thirdDimension[factTable[factTableCnt].getDimensionKey(3)].getValue() < selectivityValues[2]) {
						if (firstDimension[factTable[factTableCnt].getDimensionKey(1)].getValue() < selectivityValues[0]) {
							dummyVar += firstDimension[factTable[factTableCnt].getDimensionKey(1)].getValue();
						}
					}
				}
			}
		}
		return dummyVar;
	}



	uint64_t permutationJoin22(uint64_t dummyVar, size_t startOffset, size_t stopOffset, std::vector<size_t> selectivityValues,
                                                  APAJ::FactTable* factTable, APAJ::DimensionTable* firstDimension, APAJ::DimensionTable* secondDimension,
                                                  APAJ::DimensionTable* thirdDimension, APAJ::DimensionTable* fourthDimension) {
		for (size_t factTableCnt = startOffset; factTableCnt < stopOffset; ++factTableCnt) {
			if (fourthDimension[factTable[factTableCnt].getDimensionKey(4)].getValue() < selectivityValues[3]) {
				if (thirdDimension[factTable[factTableCnt].getDimensionKey(3)].getValue() < selectivityValues[2]) {
					if (firstDimension[factTable[factTableCnt].getDimensionKey(1)].getValue() < selectivityValues[0]) {
						if (secondDimension[factTable[factTableCnt].getDimensionKey(2)].getValue() < selectivityValues[1]) {
							dummyVar += secondDimension[factTable[factTableCnt].getDimensionKey(2)].getValue();
						}
					}
				}
			}
		}
		return dummyVar;
	}



	uint64_t permutationJoin23(uint64_t dummyVar, size_t startOffset, size_t stopOffset, std::vector<size_t> selectivityValues,
                                                  APAJ::FactTable* factTable, APAJ::DimensionTable* firstDimension, APAJ::DimensionTable* secondDimension,
                                                  APAJ::DimensionTable* thirdDimension, APAJ::DimensionTable* fourthDimension) {
		for (size_t factTableCnt = startOffset; factTableCnt < stopOffset; ++factTableCnt) {
			if (fourthDimension[factTable[factTableCnt].getDimensionKey(4)].getValue() < selectivityValues[3]) {
				if (thirdDimension[factTable[factTableCnt].getDimensionKey(3)].getValue() < selectivityValues[2]) {
					if (secondDimension[factTable[factTableCnt].getDimensionKey(2)].getValue() < selectivityValues[1]) {
						if (firstDimension[factTable[factTableCnt].getDimensionKey(1)].getValue() < selectivityValues[0]) {
							dummyVar += firstDimension[factTable[factTableCnt].getDimensionKey(1)].getValue();
						}
					}
				}
			}
		}
		return dummyVar;
	}


}