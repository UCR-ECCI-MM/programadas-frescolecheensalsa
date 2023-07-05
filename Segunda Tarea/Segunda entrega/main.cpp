// Copyright 2023 Alejandro Jimenez, Joseph Valverde, Kenneth Villalobos
// FrescolecheEnSalsa

#include "BruteForce.hpp"
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
  cookieCosts[simple] = 300;   // Simple
  cookieCosts[chips] = 500;   // Chispas
  cookieCosts[doubleChocolate] = 700;   // Doble chocolate
  cookieCosts[oatmeal] = 400;   // Avena
  cookieCosts[weed] = 2000;  // Weed
  cookieCosts[vegan] = 1000;  // Vegana

  //std::vector<size_t> cookieAmounts(6, 3);

  std::vector<size_t> cookieAmounts(6);
  cookieAmounts[simple] = 3;   // Simple
  cookieAmounts[chips] = 3;   // Chispas
  cookieAmounts[doubleChocolate] = 3;   // Doble chocolate
  cookieAmounts[oatmeal] = 3;   // Avena
  cookieAmounts[weed] = 2;  // Weed
  cookieAmounts[vegan] = 2;  // Vegana

  FrescolecheEnSalsa::cookieRestrictions restrictions = {packetPrice, packetCapacity, cookieCosts};

  std::shared_ptr<FrescolecheEnSalsa::solutionT> solution;

  switch(choice) {
    case algorithms::bruteForce:
      std::cout << "Brute Force solution" << std::endl;
      solution = FrescolecheEnSalsa::bruteForce(restrictions, cookieAmounts);
      break;
    case algorithms::firstFit: 
      std::cout << "First Fit Bin Packing solution" << std::endl;
      solution = FrescolecheEnSalsa::binPacking(restrictions, cookieAmounts);
      break;
    case algorithms::genetic: {
      FrescolecheEnSalsa::GeneticAlgorithm solver(restrictions, cookieAmounts, 10);
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