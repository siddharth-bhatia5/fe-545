#include "SimpleMC.h"
#include "Random.h"
#include <vector>

double SimpleMonteCarlo3(const AsianOption& TheOption,
                         double Spot,
                         double Vol,
                         double r,
                         unsigned long NumberOfPaths,
                         unsigned long NumberOfSteps) 
{
    double expiry = 1.0;

    double variance = Vol * Vol * expiry;
    double rootVariance = sqrt(variance);
    double itoCorrection = -0.5 * variance;

    double movedSpot = Spot * exp(r * expiry + itoCorrection);
    double thisSpot;
    double runningSum = 0;

    for (unsigned long i = 0; i < NumberOfPaths; i++)
    {
        std::vector<double> spotPrices(NumberOfSteps);
        spotPrices[0] = movedSpot;
        GetGBMSpotPricePath(spotPrices, r, Vol, expiry, NumberOfSteps);
        double thisPayoff = TheOption.OptionPayOff(spotPrices);
        runningSum += thisPayoff;
    }

    double mean = runningSum / NumberOfPaths;
    mean *= exp(-r * expiry);

    return mean;
}