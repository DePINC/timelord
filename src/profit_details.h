#ifndef PROFIT_DETAILS_H
#define PROFIT_DETAILS_H

#include <cstdint>

#include <string>
#include <vector>

/*
{
  "height": 1010000,
  "subsidy": 6750000000,
  "subsidyHuman": "67.50",
  "netspaceTB": "523,733",
  "profitPerTB": 43304508,
  "profitPerTBHuman": "0.43304508",
  "profitFullmortgage": [
    {
      "address": "3JLVceFYAgWsq8jk97w7FA1itvGvg7WKBn",
      "netspaceTB": "28,836",
      "latestHeight": "1,009,983"
    },
    {
      "address": "3PnyxgPGi58qk6jr8H5NGpsJRSEfndA1uL",
      "netspaceTB": "519",
      "latestHeight": "1,009,235"
    }
  ],
  "profitFullmortgagePerTB": 738306688,
  "profitFullmortgagePerTBHuman": "7.38306688",
  "profitFullmortgagePerPB": 738306688000,
  "profitFullmortgagePerPBHuman": "7383.06688",
  "days": 7
}
*/

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