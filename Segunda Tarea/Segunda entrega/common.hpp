// Copyright 2023 Alejandro Jimenez, Joseph Valverde, Kenneth Villalobos
// FrescolecheEnSalsa

#ifndef COMMON_HPP
#define COMMON_HPP

#include <algorithm>
#include <cstddef>
#include <iostream>
#include <map>
#include <memory>
#include <random>
#include <vector>

/**
 * @brief Namespace to store code from the group
 */
namespace FrescolecheEnSalsa {
//  ------------------- ENUMERATIONS -------------------------
  /**
 * @brief Enum for the types of algorithms
 */
enum algorithms {
  NOTHING,
  BRUTEFORCE,
  FIRSTFIT,
  GENETIC
};

/**
 * @brief Enum for the types of cookies
 */
enum cookieType {
  SIMPLE,
  CHIPS,
  DOUBLE_CHOCOLATE,
  OATMEAL,
  WEED,
  VEGAN
};

//  ------------------- Constants -------------------------

static const char* cookieNames[6] = {
  "Simple",
  "Chips",
  "Double chocolate",
  "Oatmeal",
  "Happy (Weed)",
  "Vegan"
};

//  ------------------- Typedefs -------------------------

/**
 * @brief Typedef for the type used to represent the type of cookie
 */
typedef std::size_t cookieT;

/**
 * @brief Typedef for the type used to represent each packet
 */
typedef std::vector<cookieT> packetT;

//  ------------------- Classes -------------------------
/**
 * @brief Class for a group of cookies
 */
class Cookies : public std::vector<cookieT> {
  // Constructors
  public:
  // default constructor
  Cookies() :
    std::vector<cookieT> () {
  }

  // Cookie amounts constructor
  Cookies(const std::vector<cookieT>& cookieAmounts) : 
  std::vector<cookieT> () { 
    size_t size = cookieAmounts.size();

    // for every type
    for (size_t cookieType = 0; cookieType < size; ++cookieType) {
      // place in all cookies
      for (size_t cookie = 0; cookie < cookieAmounts[cookieType]; ++cookie) {
        // with their type of cookie as value
        this->push_back(cookieType);
      }
    }
  }
};

/**
 * @brief restrictions for Joseph's cookie problem 
 */
struct cookieRestrictions {
  int packetPrice;
  size_t packetCapacity;
  std::vector<int>& cookieCosts;
};

/**
 * @brief Class that represents a solution (group of packetT)
 */
class Solution : public std::vector<packetT> {
  // Attributes
 private:
  int netGain;
  double variability;

  // Constructor
 public:
  Solution()
   : std::vector<packetT>(),
   netGain(0),
   variability(0) {
  }

  // General methods
 public:
  void setnetGain(const cookieRestrictions& restrictions) {
    int productionCost = 0;
    // Calculate the production cost of fall the cookies
    for (const packetT& packet : (*this)) {
      for (const cookieT cookie : packet) {
        productionCost += restrictions.cookieCosts[cookie];
      }
    }

    // Calculate the net gain of the solution as
    // the packet price (selling price) times the number of
    // packets minus all the production cost
    this->netGain =
      (restrictions.packetPrice * this->size()) - productionCost;
  }

  void setVariablity() {
    // Create a map to avoid a cookie twice
    std::map<cookieT, bool> foundCookies;
    // Create a variable for the total number of different cookies
    // per packet
    double differentCookies = 0;

    // Calculate the different cookies on each packet and add them
    // to the total
    for (const packetT& packet : (*this)) {
      foundCookies.clear();
      for (const cookieT cookie : packet) {
        if (!foundCookies[cookie]) {
          foundCookies[cookie] = true;
          ++differentCookies;
        }
      }
    }

    // Calculate the variability as the total different cookies
    // divided by the number of packets
    this->variability = differentCookies / this->size();
  }

  inline int getNetGain() const {
    return this->netGain;
  }

  inline double getVariability() const {
    return this->variability;
  }

  // Overloads (used for sorting)
 public:
  bool operator <(const Solution& other) const {
    if (this->netGain != other.netGain) {
      return (this->netGain < other.netGain);
    }
    return (this->variability > other.variability);
  }

  bool operator >(const Solution& other) const {
    if (this->netGain != other.netGain) {
      return (this->netGain > other.netGain);
    }
    return (this->variability < other.variability);
  }
};


/**
 * @brief Gets the price of the packet based on the cookie contents
 * of each packet
 * 
 * @param packet that has the cookies
 * @param restrictions where the prices of each cookie type are
 * @return the price of the packet
 */
int getPacketPrice (const packetT& packet, const cookieRestrictions& restrictions) {
  int totalPrice = 0;
  // add the values of all cookies already inside the packet
  for (cookieT cookieInBin : packet) {
    totalPrice += restrictions.cookieCosts[cookieInBin];
  }

  return totalPrice;
}

/**
 * @brief checks if a cookie can be added to a solution
 * 
 * @param packet where the cookie will be added
 * @param restrictions where the prices of each cookie type are
 * @param cookie the cookie to be added
 * @return true if the cookie can be added
 * @return false if the cookie can not be added
 */
bool canAdd(const packetT& packet,
    const cookieRestrictions& restrictions,
    const cookieT cookie) {
  // add the price of the current cookie
  int totalPrice = restrictions.cookieCosts[cookie];

  totalPrice += getPacketPrice(packet, restrictions);

  // return if
  return packet.size() < restrictions.packetCapacity // within capacity
    && totalPrice < restrictions.packetPrice; // and within price range
}

/**
 * @brief Gets the string value for each cookie type
 * 
 * @param cookie 
 * @return std::shared_ptr<std::string> 
 */
inline std::shared_ptr<std::string> getCookie(const cookieT cookie) {
  std::shared_ptr<std::string> cookieString = std::make_shared<std::string>("");

  if (cookie >= 6) {
    *cookieString += "invalid cookie!!!";
  }

  *cookieString += cookieNames[cookie];
  
  return cookieString;
}

/**
 * @brief 
 * 
 * @param packet 
 * @param cookieTypeCount 
 * @return std::vector<size_t> 
 */
std::vector<size_t> countCookies(const packetT packet,
  const size_t cookieTypeCount) {
  // Create a vector to know the number of each type of cookie in
  // a packet
  std::vector<size_t> cookieAmounts(cookieTypeCount);
  // For loop to count each cookie type inside the packet
  for(cookieT cookieType = 0; cookieType < cookieTypeCount; ++cookieType){
    cookieAmounts[cookieType] =
      std::count(packet.begin(), packet.end(), cookieType);
  }

  // Return the vector
  return cookieAmounts;
}

/**
 * @brief 
 * 
 * @param packet 
 */
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

/**
 * @brief 
 * 
 * @param solution 
 * @param restrictions 
 */
void reportResults(
    const std::shared_ptr<Solution> solution,
    const cookieRestrictions& restrictions) {
  // if solution is empty
  if (solution == nullptr || solution->empty()) {
    // report no solutions were found
    std::cout << "No solutions were found" << std::endl;
    return;
  }

  // Report the best solution
  std::cout << "The best solution found was:" << std::endl;

  size_t packetsCount = 0;
  int netGain = solution->getNetGain();
  double variability = solution->getVariability();

  for (packetT& packet : *solution) {
    int packetPrice = getPacketPrice(packet, restrictions);

    std::cout << "\nPacket #" << ++packetsCount << std::endl;
    std::cout << "    Price: " << packetPrice << std::endl;

    // amount of packets
    printPacket(packet);
  }

  // Print the total number of packets
  std::cout << "\nThe total number of packets is: " << solution->size()
    << std::endl;
  // Print the total net gain
  std::cout << "The total net gain is: " << netGain << std::endl;
  // Print the variability
  std::cout << "The mean variability is: " << variability << std::endl;
}
};
#endif