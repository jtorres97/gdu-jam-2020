#include "Random.h"

auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
std::mt19937 mt(seed);

int RandInt(int exclusiveMax)
{
    std::uniform_int_distribution<int> dist(0, exclusiveMax - 1);
    return dist(mt);
}

int RandInt(int min, int max)
{
    std::uniform_int_distribution<int> dist(0, max - min);
    return dist(mt) + min;
}

double RandDouble(double exclusiveMax)
{
    std::uniform_real_distribution<double> dist(0, exclusiveMax - 1.0);
    return dist(mt);
}

double RandDouble(double min, double max)
{
    std::uniform_real_distribution<double> dist(0, max - min);
    return dist(mt);
}

bool RandBool(double probability)
{
    std::bernoulli_distribution dist(probability);
    return dist(mt);
}