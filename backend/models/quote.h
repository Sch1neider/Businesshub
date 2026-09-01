#pragma once

#include <string>

struct Quote
{
    std::string ticker;
    std::string name;
    std::string currency;

    double price = 0.0;
    double change = 0.0;
    double changePercent = 0.0;

    double open = 0.0;
    double dayHigh = 0.0;
    double dayLow = 0.0;
    double previousClose = 0.0;

    long long volume = 0;
    long long marketCap = 0;

    double fiftyTwoWeekHigh = 0.0;
    double fiftyTwoWeekLow = 0.0;

    std::string marketTime;
};