#pragma once

#include <random>
#include <chrono>
#include <ctime>

int RandInt(int exclusiveMax);
int RandInt(int min, int max);
double RandDouble(double exclusiveMax);
double RandDouble(double min, double max);
bool RandBool(double probability = 0.5);