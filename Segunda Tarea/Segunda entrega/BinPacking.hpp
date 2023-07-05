#ifndef BINPACKING_HPP
#define BINPACKING_HPP

#include "common.hpp"

namespace FrescolecheEnSalsa {
  std::shared_ptr<Solution> binPacking(
      const FrescolecheEnSalsa::cookieRestrictions& restrictions,
      const std::vector<size_t>& cookieAmounts // amount of each cookie backed
    ) {

    // check if sizes are consistent, if otherwise
    if (restrictions.cookieCosts.size() != cookieAmounts.size()) {
      // exit and report on error
      std::cerr <<  "Error: Inconsistent number of cookie types" << std::endl;
      return nullptr;
    }

    // get an vector of all cokies with the cookie amounts
    Cookies cookies(cookieAmounts);

    // get a list to place the solution
    std::shared_ptr<Solution> solution = std::make_shared<Solution>();

    // for all cookies
    for (cookieT cookie : cookies) {
      bool placed = false;

      // find a bin that can fit the item
      for (packetT& packet : *solution) {
        // if it fits
        if (canAdd(packet, restrictions, cookie)) {
          // place inside
          packet.push_back(cookie);

          // marked placed as true
          placed = true;

          // exit loop
          break;
        }
      }

      // if none found
      if (!placed) {
        // get a new bin
        solution->push_back(packetT());

        // place inside just added bin
        solution->back().push_back(cookie);
      }
    }

    solution->setnetGain(restrictions);
    solution->setVariablity();

    return solution;
  }
};
#endif