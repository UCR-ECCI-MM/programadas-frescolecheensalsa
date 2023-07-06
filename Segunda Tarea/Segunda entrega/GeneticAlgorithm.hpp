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
  };

};