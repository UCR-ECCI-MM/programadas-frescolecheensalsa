#ifndef COMMON_HPP
#define COMMON_HPP

#include <list>
#include <vector>
#include <cstddef>
#include <map>
#include <memory>
#include <iostream>
#include <cstring>

enum algorithms {
  nothing,
  bruteForce,
  firstFit,
  genetic
};

// enum with vector positions
enum cookieType {
  simple,
  chips,
  doubleChocolate,
  oatmeal,
  weed,
  vegan
};


namespace FrescolecheEnSalsa {
  /**
   * @brief Typedef for the type used to represent the type of cookie
   */
  typedef std::size_t cookieT;

  /**
   * @brief Typedef for the type used to represent each packet
   */
  typedef std::vector<cookieT> packetT;

  struct paquete {
    std::vector<cookieT> galletas;
    int precio;
    size_t cantidadGalletas;

    paquete(){
      this->precio = 0;
      this->cantidadGalletas = 0;
    }

    bool operator <(const paquete& nuevoPaquete)const{
      return (this->precio < nuevoPaquete.precio);
    }
  };

  struct Solution {
    std::vector<paquete> solution;
    int aptitud;

    Solution(){
      this->aptitud = 0;
    }

    bool operator <(const Solution& Nuevasolution) const {
      return (this->aptitud < Nuevasolution.aptitud);
    }
  };

  /**
   * @brief Procedure to calculate the variance of a given solution to Joseph´s
   * problem
   * @param solution @a solutionT from which the variance will be calculated
   * from
   * @details The variance is going to be equal to the mean number of different
   * cookie types per packet, in other words, the sum of the number of different
   * cookie types on each packet, divided by the number of packets
   * @return double with the variance of the solution
   */
  double calculateVariance(const solutionT& solution) {
    // Create a variable to store the total number of different cookies
    // per packet
    double totalDifferentCookies = 0.0;
    // Create a map to know which cookie types have already being counted
    // per packet
    std::map<cookieT, bool> foundCookies;

    // For loop to go through all the packets in the given solution
    for (solutionT::const_iterator packet = solution.cbegin();
      packet != solution.cend(); ++packet) {
      // Reset the map (No cookies have been counted on the current packet)
      foundCookies.clear();

      // For loop to go through all the cookies in the current packet
      for (size_t cookieIndex = 0; cookieIndex < (*packet).size();
        ++cookieIndex) {
        // If the current cookie has not been found on the current packet
        if (!foundCookies[(*packet)[cookieIndex]]) {
          // Establish that cookie as found to avoid counting it again
          foundCookies[(*packet)[cookieIndex]] = true;
          // Increment the number of different cookies found
          ++totalDifferentCookies;
        }
      }
    }

    // Return the mean number of cookie types per packet (the variance)
    return totalDifferentCookies / solution.size();
  }

  int getPacketPrice (const packetT& packet, const cookieRestrictions& restrictions) {
    int totalPrice = 0;
    // add the values of all cookies already inside the packet
    for (cookieT cookieInBin : packet) {
      totalPrice += restrictions.cookieCosts[cookieInBin];
    }

    return totalPrice;
  }

  bool canAdd(const packetT& packet,const cookieRestrictions& restrictions,const cookieT cookie) {
    // add the price of the current cookie
    int totalPrice = restrictions.cookieCosts[cookie];

    totalPrice += getPacketPrice(packet, restrictions);

    // return if
    return packet.size() <= restrictions.packetCapacity // within capacity
        && totalPrice < restrictions.packetPrice; // and within price range
  }
};
#endif