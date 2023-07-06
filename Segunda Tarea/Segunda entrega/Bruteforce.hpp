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
   * @param currentSolution reference to a vector of vectors of @a cookieT, where
   * each vector represents a packet. This param is used to temporarily store
   * each solution, build them and move through them
   * @param bestSolution reference to a vector of vectors of @a cookieT, where
   * each vector represents a packet. This param is used to store the best
   * solution found and when calling this procedure for the first time, it MUST
   * be EMPTY solution found will be stored, if no solution was found, it wont be modified
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
      Solution& currentSolution,
      Solution& bestSolution,
      FrescolecheEnSalsa::cookieRestrictions& restrictions
      ) {

    // ------------------- Try with the current packets ------------------------
    // For loop to go through all the packets in the current solution
    for (size_t packetIndex = 0; packetIndex < currentSolution.size();
      ++packetIndex) {
      // If the packet is not full AND adding the cookie does not generate a
      // negative net gain (is a feasible solution)
      if (canAdd(
          currentSolution[packetIndex],
          restrictions,
          cookies[cookieIndex]
          )) {
        // ----------------------- Build the solution --------------------------
        // Add the cookie to the packet
        currentSolution[packetIndex].push_back(cookies[cookieIndex]);

        // ------------------------- Recursive call ----------------------------
        // If there is still cookies to pack
        if (cookieIndex < cookies.size() - 1) {
          // Make a recursive call with the current parameters and the next
          // cookie
          bruteForceR(cookieIndex + 1, cookies, currentSolution, bestSolution,
            restrictions);

        // ------------------------- Solution found ----------------------------
        // If the last cookie has been packed (a full solution has been build)
        } else {
          // Calculate the net gain and the var of the current solution
          currentSolution.setnetGain(restrictions);
          currentSolution.setVariablity();

          // If it is the first solution found OR
          // the net gain of this solution is lower than the one from the
          // current best solution OR
          // the net gain of this solution is equal to the one from the best
          // solution but, its variance is higher
          if (bestSolution.empty() || currentSolution < bestSolution) {
            // ------------------ Update the best solution ---------------------
            // Update the best solution with the current solution
            bestSolution = currentSolution;
          }
        }

        // ----------------------------- Regret --------------------------------
        // Take the cookie from the packet
        currentSolution[packetIndex].pop_back();
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
        restrictions);

    // --------------------------- Solution found ------------------------------
    // If the last cookie has been packed (a full solution has been build)
    } else {
      // Calculate the net gain and the var of the current solution
      currentSolution.setnetGain(restrictions);
      currentSolution.setVariablity();

      // If it is the first solution found OR
      // the net gain of this solution is lower than the one from the
      // current best solution OR
      // the net gain of this solution is equal to the one from the best
      // solution but, its variance is higher
      if (bestSolution.empty() || currentSolution < bestSolution) {
        // Update the best solution with the current solution
        bestSolution = currentSolution;
      }
    }

    // ------------------------------- Regret ----------------------------------
    // Take the cookie from the new packet
    currentSolution.back().pop_back();
    // Get rid of the new packet
    currentSolution.pop_back();
  }

  /**
   * @brief Procedure to find the best solution to Joseph´s problem using
   * brute force (Pure exhaustive search)
   * @param packetPrice int with the sale price of each cookie packet
   * @param packetCapacity size_t with the max number of cookies that fit on
   * each packet
   * @param cookieCosts vector representing the production cost of each type of
   * cookie, where index = cookie type
   * @param cookieAmounts vector representing the number of cookies of each type
   * that were baked, where index = cookie type
   * @remark The size of the vectors cookieCosts and cookieAmounts must be equal
   * since the number of possible cookie types must be consistent
   * @remark This procedure will print it´s results on the standard output
   * @details This procedure calls @a void bruteForceR() to work
   * @see void bruteForceR()
   */
  std::shared_ptr<Solution> bruteForce(
      FrescolecheEnSalsa::cookieRestrictions& restrictions,
      const std::vector<size_t>& cookieAmounts
      ) {

    // If the number of each type of cookie is not consistent
    if (restrictions.cookieCosts.size() != cookieAmounts.size()) {
      // Print the error
      std::cerr << "Error: Inconsistent number of cookie types" << std::endl;
      return nullptr;
    }

    // Create a vector to store all the cookies
    Cookies cookies(cookieAmounts);

    // Create vectors to move through solutions and to store the best
    // solution respectively
    std::shared_ptr<Solution>
        bestSolution = std::make_shared<Solution>();
    Solution currentSolution;

    // Call the recursive procedure with the index of the first cookie
    bruteForceR(0, cookies, currentSolution, *bestSolution, restrictions);

    return bestSolution;
  }
} // namespace FrescolecheEnSalsa

#endif