// SquarePoint.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <boost/multiprecision/cpp_int.hpp>

using namespace boost::multiprecision;

struct MeanVariance
{
    double mean;
    double variance;
};

MeanVariance computeIntMean(int64_t elem)
{
    static int128_t sum = 0;
    static int256_t sumSq = 0;
    static uint64_t numElems = 0;
    MeanVariance retVal;

    numElems++;
    sum += elem;
    int128_t elem_conv = elem;
    int128_t sq = elem_conv * elem_conv;
    sumSq += sq;
    retVal.mean = (double)sum / (double)numElems;
    int256_t subWithoutCanc = numElems * sumSq - sum * sum;
    uint128_t numElemsSquare = ((uint128_t)numElems * (uint128_t)numElems);
    retVal.variance = (double)(subWithoutCanc) / (double)(numElemsSquare);

    return retVal;
}

MeanVariance computeFLoatMean(double elem)
{
    static double sum = 0;
    static double sumSq = 0;
    static double numElems = 0;
    MeanVariance retVal;

    numElems++;
    sum += elem;
    int128_t elem_conv = elem;
    int128_t sq = elem_conv * elem_conv;
    sumSq += sq;
    retVal.mean = (double)sum / (double)numElems;
    double subWithoutCanc = (double)(numElems * sumSq - sum * sum);
    uint128_t numElemsSquare = ((uint128_t)numElems * (uint128_t)numElems);
    retVal.variance = subWithoutCanc / (double)(numElemsSquare);

    return retVal;
}


int main()
{
    std::vector<int64_t> vInt = { 1, 2, 3, 4, 5 };
    for (const auto& elem : vInt)
    {
        auto meanVar = computeIntMean(elem);
        std::cout << meanVar.mean << std::endl;
        std::cout << meanVar.variance << std::endl;
    }
}
