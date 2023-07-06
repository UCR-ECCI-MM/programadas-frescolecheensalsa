// Copyright 2023 Alejandro Jimenez, Joseph Valverde, Kenneth Villalobos
// FrescolecheEnSalsa

#include "Bruteforce.hpp"
#include "BinPacking.hpp"
#include "GeneticAlgorithm.hpp"

int main(int argc, char* argv[]) {
  const int packetPrice = 5000;
  const size_t packetCapacity = 5;

  int choice = 0;

  if (argc > 1) {
    choice = std::stoi(argv[1]);
  }

  std::vector<int> cookieCosts(6);
  cookieCosts[FrescolecheEnSalsa::SIMPLE] = 300;
  cookieCosts[FrescolecheEnSalsa::CHIPS] = 500;
  cookieCosts[FrescolecheEnSalsa::DOUBLE_CHOCOLATE] = 700;
  cookieCosts[FrescolecheEnSalsa::OATMEAL] = 400;
  cookieCosts[FrescolecheEnSalsa::WEED] = 2000;
  cookieCosts[FrescolecheEnSalsa::VEGAN] = 1000;

  std::vector<size_t> cookieAmounts(6);
  cookieAmounts[FrescolecheEnSalsa::SIMPLE] = 2;
  cookieAmounts[FrescolecheEnSalsa::CHIPS] = 2;
  cookieAmounts[FrescolecheEnSalsa::DOUBLE_CHOCOLATE] = 2;
  cookieAmounts[FrescolecheEnSalsa::OATMEAL] = 2;
  cookieAmounts[FrescolecheEnSalsa::WEED] = 2;
  cookieAmounts[FrescolecheEnSalsa::VEGAN] = 2;

  FrescolecheEnSalsa::cookieRestrictions restrictions = {packetPrice,
    packetCapacity, cookieCosts};

  std::shared_ptr<FrescolecheEnSalsa::Solution> solution;

  switch(choice) {
    case FrescolecheEnSalsa::algorithms::BRUTEFORCE:
      std::cout << "Brute Force solution" << std::endl;
      solution = FrescolecheEnSalsa::bruteForce(restrictions, cookieAmounts);
      break;
    case FrescolecheEnSalsa::algorithms::FIRSTFIT: 
      std::cout << "First Fit Bin Packing solution" << std::endl;
      solution = FrescolecheEnSalsa::binPacking(restrictions, cookieAmounts);
      break;
    case FrescolecheEnSalsa::algorithms::GENETIC: {
      FrescolecheEnSalsa::GeneticAlgorithm solver(restrictions,
        cookieAmounts);
      solution = solver.run();
      std::cout << "Genetic Algorithm solution" << std::endl;
      break;
    }
    default:
      std::cout << "Invalid choice" << std::endl;
      break;
  }

  FrescolecheEnSalsa::reportResults(solution, restrictions);

  return 0;
}