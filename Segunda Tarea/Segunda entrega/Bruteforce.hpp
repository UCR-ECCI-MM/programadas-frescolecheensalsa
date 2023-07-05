#ifndef BRUTEFORCE_HPP
#define BRUTEFORCE_HPP

#include "common.hpp"

// ############################# Procedures #################################

namespace FrescolecheEnSalsa {
  /**
   * @brief Recursive procedure to find the best solution to Joseph´s problem
   * using brute force (Pure exhaustive search)
   * @param cookieIndex size_t with the index of the current cookie on the
   * cookies vector to be packed
   * @param cookies vector of @a cookieT with the cookies to pack, each index
   * on the vector should represent a single cookie with its type. The order of
   * the vector is irrelevant
   * @param currentSolution reference to a list of vectors of @a cookieT, where
   * each vector represents a packet. This param is used to temporarily store
   * each solution, build them and move through them
   * @param bestSolution reference to a list of vectors of @a cookieT, where
   * each vector represents a packet. This param is used to store the best
   * solution found and when calling this procedure for the first time, it MUST
   * be EMPTY
   * @param bestNetGain reference to an int where the net gain of the best
   * solution found will be stored, if no solution was found, it wont be modified
   * @param bestVar reference to a double where the variance of the best
   * solution found will be stored, if no solution was found, it wont be modified
   * @param packetPrice int with the sale price of each cookie packet
   * @param packetCapacity size_t with the max number of cookies that fit on
   * each packet
   * @param cookieCosts vector representing the production cost of each type of
   * cookie, where index = cookie type
   * @param cookieAmounts vector representing the number of cookies of each type
   * that were baked, where index = cookie type
   * @remark This procedure should not be used directly, the user should rather
   * call @a void bruteForce() which will make use of this procedure to work
   */
  void bruteForceR(
      const size_t cookieIndex,
      const std::vector<cookieT>& cookies,
      solutionT& currentSolution,
      solutionT& bestSolution,
      int& bestNetGain,
      double& bestVar,
      FrescolecheEnSalsa::cookieRestrictions& restrictions
      ) {

    // ------------------- Try with the current packets ------------------------
    // For loop to go through all the packets in the current solution
    for (solutionT::iterator packet = currentSolution.begin();
      packet != currentSolution.end(); ++packet) {
      // If the packet is not full AND adding the cookie does not generate a
      // negative net gain (is a feasible solution)

      if (canAdd(
          *packet,
          restrictions,
          cookies[cookieIndex]
          )) {
        // ----------------------- Build the solution --------------------------
        // Add the cookie to the packet
        packet->push_back(cookies[cookieIndex]);

        // ------------------------- Recursive call ----------------------------
        // If there is still cookies to pack
        if (cookieIndex < cookies.size() - 1) {
          // Make a recursive call with the current parameters and the next
          // cookie
          bruteForceR(cookieIndex + 1, cookies, currentSolution, bestSolution,
            bestNetGain, bestVar, restrictions);

        // ------------------------- Solution found ----------------------------
        // If the last cookie has been packed (a full solution has been build)
        } else {
          // Calculate the net gain and the var of the current solution
          const int currentNetGain = calculateNetGain(
              currentSolution,
              restrictions.packetPrice,
              restrictions.cookieCosts);
          const double currentVar = calculateVariance(currentSolution);

          // If it is the first solution found OR
          // the net gain of this solution is lower than the one from the
          // current best solution OR
          // the net gain of this solution is equal to the one from the best
          // solution but, its variance is higher
          if (bestSolution.empty() || currentNetGain < bestNetGain ||
            (currentNetGain == bestNetGain && currentVar > bestVar)) {
            // ------------------ Update the best solution ---------------------
            // Update the best solution with the current solution
            bestSolution = currentSolution;
            // Update the best net gain with the current net gain
            bestNetGain = currentNetGain;
            // Update the best variance with the current variance
            bestVar = currentVar;
          }
        }

        // ----------------------------- Regret --------------------------------
        // Take the cookie from the packet
        (*packet).pop_back();
      }
    }

    // ----------------- Add a new packet to the solution ----------------------
    // Create a new empty packet and add it to the current solution
    currentSolution.push_back(packetT());

    // ------------------------- Build the solution ----------------------------
    // Add the cookie to the new packet in the current solution
    currentSolution.back().push_back(cookies[cookieIndex]);

    // --------------------------- Recursive call ------------------------------
    // If there is still cookies to pack
    if (cookieIndex < cookies.size()-1) {
      // Make a recursive call with the current parameters and the next
      // cookie
      bruteForceR(cookieIndex + 1, cookies, currentSolution, bestSolution,
        bestNetGain, bestVar, restrictions);

    // --------------------------- Solution found ------------------------------
    // If the last cookie has been packed (a full solution has been build)
    } else {
      // Calculate the net gain and the var of the current solution
      const int currentNetGain = calculateNetGain(currentSolution, restrictions.packetPrice,
            restrictions.cookieCosts);
      const int currentVar = calculateVariance(currentSolution);

      // If it is the first solution found OR
      // the net gain of this solution is lower than the one from the
      // current best solution OR
      // the net gain of this solution is equal to the one from the best
      // solution but, its variance is higher
      if (bestSolution.empty() || currentNetGain < bestNetGain ||
        (currentNetGain == bestNetGain && currentVar > bestVar)) {
        // Update the best solution with the current solution
        bestSolution = currentSolution;
        // Update the best net gain with the current net gain
        bestNetGain = currentNetGain;
        // Update the best variance with the current variance
        bestVar = currentVar;
      }
    }

    // ------------------------------- Regret ----------------------------------
    // Take the cookie from the new packet
    currentSolution.back().pop_back();
    // Get rid of the new packet
    currentSolution.pop_back();
  }
} // namespace FrescolecheEnSalsa

#endif