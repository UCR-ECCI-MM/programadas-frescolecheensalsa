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
   * @brief Procedure to calculate the total net gain of a given solution to
   * Joseph´s problem
   * @param solution @a solutionT from which the total net gain will be
   * calculated from
   * @param packetPrice int with the sale price of the packets
   * @param cookieCosts vector representing the production cost of each type of
   * cookie, where index = cookie type
   * @details The total net gain is going to be equal to the sum of the net gain
   * on each packet
   * @details The net gain of a packet is going to be equal to the difference
   * between the packetPrice and the total production cost of the packet
   * @details The total production cost of a packet is going to be equal to the
   * sum of the production cost of each cookie on the packet
   * @return int with the total net gain of solution
   */
  int calculateNetGain(const solutionT& solution,
    const int packetPrice, const std::vector<int>& cookieCosts) {
    // Create a variable to store the total net gain of the given solution
    // and a variable to store the production cost of each packet respectively
    int totalNetGain = 0, productionCost = 0;

    // For loop to go through all the packets in the given solution
    for (solutionT::const_iterator packet = solution.cbegin();
      packet != solution.cend(); ++packet) {
      // Reset the production cost
      productionCost = 0;

      // For loop to add the cost of each cookie on the current packet
      for (size_t cookieIndex = 0; cookieIndex < (*packet).size();
        ++cookieIndex) {
        productionCost += cookieCosts[(*packet)[cookieIndex]];
      }

      // Add the difference between the price per packet and the production
      // cost of the current packet to the total net gain
      totalNetGain += (packetPrice - productionCost);
    }

    // Return the calculated total net gain
    return totalNetGain;
  }

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

  inline std::shared_ptr<std::string> getCookie(const cookieT cookie) {
    std::shared_ptr<std::string> cookieString = std::make_shared<std::string>("");

    if (cookie >= 6) {
      *cookieString += "invalid cookie!!!";
    }

    *cookieString += cookieNames[cookie];
    
    return cookieString;
  }

  void printPacket(
      const packetT& packet) {
    // Map to store the number of cookies on each packet
    std::map<cookieT, size_t> cookies;
    // Variability of the packet
    size_t variability = 0;
    
    // For loop to calculate the amount of each cookie on the packet
    // and the variability
    for (cookieT cookie : packet) {
      if (!cookies[cookie]) {
        ++variability;
      }
      ++cookies[cookie];
    }

    // Print the variability of the packet
    std::cout << "    Variability: " << variability << std::endl;
    // Print the total number of cookies on the packet
    std::cout << "    Cookie amount: " << packet.size() << std::endl; 

    // Print the total of each cookie on the package
    for(auto cookieType : cookies) {
      std::cout << "    Cookie: " << *getCookie(cookieType.first)
          << "\n\tAmount: " << cookieType.second << std::endl;
    }
  }
};
#endif