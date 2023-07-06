#ifndef GENETICALGORITHM_HPP
#define GENETICALGORITHM_HPP

#include <iostream>
#include <list>
#include <map>
#include <vector>
#include <set>
#include <random>
#include <algorithm>

#include "common.hpp"

namespace FrescolecheEnSalsa {
  class GeneticAlgorithm {
    // Attibutes
  private:
    cookieRestrictions restrictions;
    std::vector<Solution> population;
    Cookies totalCookies;
    std::vector<size_t> cookieAmounts;

    // Constructor
  public:
    GeneticAlgorithm(const cookieRestrictions& restrictions,
      std::vector<size_t>& cookieAmounts)
      : restrictions(restrictions),
      population(),
      totalCookies(cookieAmounts),
      cookieAmounts(cookieAmounts) {
      std::shuffle(this->totalCookies.begin(),
        this->totalCookies.end(),
        std::random_device());
    }

    ~GeneticAlgorithm() {}

    std::shared_ptr<Solution> run(
        const size_t populationSize = 10,
        const size_t iterations = 10000
        ) {
      // Resize the population
      this->population.resize(populationSize);
      // Generate a random initial population
      this->generateInitialPopulation();

      // Reset the seed
      srand(time(nullptr));
      
      // Evolve the population iterations number of times
      for (size_t it = 0; it < iterations; ++it) {
        this->evolve();
      }

      // Return the best solution
      return std::make_shared<Solution>(this->population[0]);
    }

    // Initializers
  private:
    void generateInitialPopulation() {
      // Generate all the solutions in the population
      for (size_t solution = 0, populationSize = this->population.size();
      solution < populationSize; ++solution) {
        this->population[solution] = this->generateSolution();
      }

      // Sort the solutions from best to worst
      std::sort(this->population.begin(), this->population.end());
    }
  
    Solution generateSolution() {
      // Shuffle the cookies so the new solution is different
      std::shuffle(this->totalCookies.begin(), this->totalCookies.end(),
        std::random_device());

      // Create a new empty solution
      Solution newSolution;


      // for all cookies
      for (cookieT cookie : this->totalCookies) {
        bool placed  = false;

        for (packetT& packet : newSolution) {
          // If the cookie can be added to the packet
          if (canAdd(packet, this->restrictions, cookie)) {
            // Place the cookie inside the packet
            packet.push_back(cookie);

            // Mark placed as true
            placed = true;

            // Exit loop
            break;
          }
        }

        // if the cookie could not be placed
        if (!placed) {
          // Create a new packet
          newSolution.push_back(packetT());
          // Add the cookie to the new packet
          newSolution.back().push_back(cookie);
        }
      }

      // Set the netGain and variability of the new solution
      newSolution.setnetGain(this->restrictions);
      newSolution.setVariablity();

      // Return the new solution
      return newSolution;
    }

    void evolve() {
      // Add three new solutions to the population by crossing:
      // 1 and 2
      // 2 and 3
      // 3 and 4
      for (size_t solution = 0; solution < 3; ++solution) {
        this->population.push_back(this->cross(
          this->population[solution].front(),
          this->population[solution+1].front()));
      }

      // Sort the population from best to worst with the new solutions
      std::sort(this->population.begin(), this->population.end());

      // Delete the 3 worst solutions
      for(size_t solution = 0; solution < 3; ++solution){
        this->population.pop_back();
      }
    }

    Solution cross(packetT genFirstParent,
      packetT genSecondParent) {
      // Generate a random number to simulate an event
      const int event = rand() % 10;

      // If the event is 2, then mutate
      if (event == 2) {
        // Change the packet of the first parent
        // to a packet from a new solution to simulate
        // a mutation
        genFirstParent = this->generateSolution()[0];
      }

      // Get the remaining cookies
      Cookies remainingCookies = std::move(
        removeLeftovers(genFirstParent, genSecondParent));

      // Create a solution to store the offspring
      Solution offspring;

      // Add the gens of the parents
      offspring.push_back(genFirstParent);
      offspring.push_back(genSecondParent);

      // Shuffle the remaining cookies
      std::shuffle(remainingCookies.begin(), remainingCookies.end(),
        std::random_device());

      for (cookieT cookie : remainingCookies) {
        bool placed  = false;

        for (packetT& packet : offspring) {
          // If the cookie can be added to the packet
          if (canAdd(packet, this->restrictions, cookie)) {
            // Place the cookie inside the packet
            packet.push_back(cookie);

            // Mark placed as true
            placed = true;

            // Exit loop
            break;
          }
        }

        // if the cookie could not be placed
        if (!placed) {
          // Create a new packet
          offspring.push_back(packetT());
          // Add the cookie to the new packet
          offspring.back().push_back(cookie);
        }
      }

      // Set the net gain and the variability of the offspring
      offspring.setnetGain(this->restrictions);
      offspring.setVariablity();

      // Return the offspring
      return offspring;
    }

    Cookies removeLeftovers(packetT& genFirstParent,
      packetT& genSecondParent) {
      // get cookie amount for the first parent
      std::vector<size_t> firstParentCookieCount =
          countCookies(genFirstParent,
          this->cookieAmounts.size());
      // get cookie amount for the second parent
      std::vector<size_t> secondParentCookieCount =
          countCookies(genSecondParent,
          this->cookieAmounts.size());

      size_t availableCookies = 0;
    
      // remove any cookies that were placed but are above amounts
      // for each cookie type
      for (cookieT cookieType = 0; cookieType < this->cookieAmounts.size();
        cookieType++) {
        // get the amount of cookies availble for second parent
        availableCookies =
          this->cookieAmounts[cookieType] - firstParentCookieCount[cookieType];

        // while there are more cookies than available in the second parent packet
        while (secondParentCookieCount[cookieType] > availableCookies) {
          // deduct the cookie from the packet
          genSecondParent.erase(std::find(genSecondParent.begin(),
              genSecondParent.end(), cookieType));
          // reduce our amount counter for the given deducted cookie
          --secondParentCookieCount[cookieType];
        }
      }

      Cookies remainingCookies(this->totalCookies);

      // for loop to remove the cookies from the first packet of the remaining
      // cookies
      for (cookieT cookie : genFirstParent) {
        // find the cookie in the actual cookie count
        packetT::iterator it = std::find(remainingCookies.begin(),
          remainingCookies.end(), cookie);
        // if in the actual cookie count
        if (it != remainingCookies.end()) {
          // remove the cookies from the actual cookie count
          remainingCookies.erase(it);
        }
      }

      // for loop to remove the cookies from the second packet of the remaining
      // cookies
      for (cookieT cookie : genSecondParent) {
        // find the cookie in the actual cookie count
        packetT::iterator it = std::find(remainingCookies.begin(),
          remainingCookies.end(), cookie);
        // if in the actual cookie count
        if (it != remainingCookies.end()) {
          // remove the cookies from the actual cookie count 
          remainingCookies.erase(it);   
        }
      }

      return remainingCookies;
    }
  };
  
};

#endif