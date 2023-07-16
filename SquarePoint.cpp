// SquarePoint.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <boost/multiprecision/cpp_int.hpp>
//#include <boost/circular_buffer.hpp>
#include <chrono>

using namespace boost::multiprecision;

struct MeanVariance
{
    double mean;
    double variance;
};

struct MeanVarianceSumSquare
{
    int128_t sum;
    int256_t sumSquare;
    uint64_t numElems;
};

struct MeanVarianceSumSquareTime: public MeanVarianceSumSquare
{
    int128_t sum;
    int256_t sumSquare;
    uint64_t numElems;
    uint64_t time;
};

MeanVariance computeMeanAndVariance(const MeanVarianceSumSquare& sums)
{
    MeanVariance retVal;
    
    retVal.mean = (double)sums.sum / (double)sums.numElems;
    int256_t subWithoutCanc = (int256_t)(sums.numElems)* sums.sumSquare - (int256_t)sums.sum * (int256_t)sums.sum;
    uint128_t numElemsSquare = ((uint128_t)sums.numElems * (uint128_t)sums.numElems);
    // Potential optimization, manual division?
    retVal.variance = (double)(subWithoutCanc) / (double)(numElemsSquare);
    
    return retVal;
}

MeanVariance computeIntMean(int64_t elem)
{
    static int128_t sum = 0;
    static int256_t sumSq = 0;
    static uint64_t numElems = 0;

    numElems++;
    sum += elem;
    int128_t sq = (int128_t)(elem) * (int128_t)(elem);
    sumSq += sq;

    auto retVal = computeMeanAndVariance({ sum, sumSq, numElems });

    return retVal;
}

/**
* 
*/
MeanVariance computeFloatMeanWest(double elem)
{
    static double meanPrev = 0;
    static double varPrev = 0;
    static double tPrev = 0;
    double mean = 0;
    double variance = 0;
    static double numElems = 0;

    numElems++;
    if (numElems == 1)
    {
        mean = elem;
        tPrev = 0;
    }
    else
    {
        mean = meanPrev + (elem - meanPrev) / numElems;
        tPrev = tPrev + (numElems - 1) * (elem - meanPrev) * ((elem - meanPrev) / numElems);
    }
    variance = tPrev / numElems;
    
    meanPrev = mean;
    varPrev = variance;
    
    return { mean, variance };
}


constexpr uint32_t MAX_ELEMS_IN_DAY = 10;
// 1ms, 10ms, 100ms, 500ms, 1s, 15s, 30s, 1m, 5m, 10m, 15m, 30m.
constexpr uint32_t NUM_SLIDING_WINDS = 12;

MeanVariance computeSlide(int64_t elem)
{
    static std::vector<MeanVarianceSumSquareTime> vSumSquareElems;
    
    static std::array<uint32_t, NUM_SLIDING_WINDS> aIdxStart;
    constexpr std::array<uint64_t, NUM_SLIDING_WINDS> aDuration{1, 10, 100, 500, 1'000, 15'000, 30'000, 60'000, 300'000, 600'000, 900'000, 1'800'000};
    static uint32_t idxMsEnd;

    static bool isOneMsInit = false; 
    MeanVariance meanVar1Ms;
    auto curTime = std::chrono::high_resolution_clock::now();
    auto duration = curTime.time_since_epoch();
    auto curTimeMilis = std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count();

    if (vSumSquareElems.size() == 0)
    {
        vSumSquareElems.resize(MAX_ELEMS_IN_DAY);    
        for (auto& idxStart: aIdxStart)
        {
            idxStart = 0;
        }
        // Initialize all elements.
        vSumSquareElems.push_back({ elem, elem * elem, 1, curTimeMilis});
        meanVar1Ms = computeMeanAndVariance(vSumSquareElems.back());
    }
    else
    {
        int128_t sum;
        int256_t sumSquare;
        uint64_t numElems;

        vSumSquareElems.push_back({ vSumSquareElems.back().sum + elem, vSumSquareElems.back().sumSquare + (int128_t)elem * (int128_t)elem,
                   vSumSquareElems.back().numElems + 1, curTimeMilis });
        if (isOneMsInit)
        {
            for (int idx = 0; idx < aIdxStart.size(); idx++)
            {
                auto& idxStart = aIdxStart[idx];
                auto& duration = aDuration[idx];

                // auto startTime = curTime - duration;
                // TODO: binary_search in the rotated array for startTime.
                

                sum = vSumSquareElems[idxMsEnd].sum - vSumSquareElems[idxStart].sum;
                sumSquare = vSumSquareElems[idxMsEnd].sumSquare - vSumSquareElems[idxStart].sumSquare;

                numElems = vSumSquareElems[idxMsEnd].numElems - vSumSquareElems[idxStart].numElems;

                meanVar1Ms = computeMeanAndVariance({ sum, sumSquare, numElems });
            }
            // vSumSquareElems.rresize(aIdxStart.end());
        }
        else
        {
            meanVar1Ms = computeMeanAndVariance(vSumSquareElems.back());
        }
    }
    return meanVar1Ms;
}

int main()
{
    std::vector<int64_t> vInt(1000);
    std::generate(vInt.begin(), vInt.end(), [] {
        static int i = 0;
        return i++;
        });
    for (const auto& elem : vInt)
    {
        auto intMeanVar = computeIntMean(elem);
        std::cout << std::setprecision(15) << "IntMeanVar: " << intMeanVar.mean << " " << intMeanVar.variance << std::endl;

        auto floatMeanVar = computeFloatMeanWest((double)(elem));

        std::cout << std::setprecision(15) << "FloatMeanVar: " << floatMeanVar.mean << " " << floatMeanVar.variance << std::endl;
    }
}
