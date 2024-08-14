#ifndef PROFIT_DETAILS_H
#define PROFIT_DETAILS_H

#include <cstdint>

#include <string>
#include <vector>

struct ProfitMiner {
    std::string address;
    std::string netspaceTB;
    std::string latestHeight;
};

struct ProfitDetails {
    int height;
    std::uint64_t subsidy;
    std::string subsidyHuman;
    std::string netspaceTB;
    std::uint64_t profitPerTB;
    std::string profitPerTBHuman;

    std::vector<ProfitMiner> profitFullmortgage;
    std::uint64_t profitFullmortgagePerTB;
    std::string profitFullmortgagePerTBHuman;
    std::uint64_t profitFullmortgagePerPB;
    std::string profitFullmortgagePerPBHuman;

    int days;
};

#endif