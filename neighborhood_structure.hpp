#ifndef NEIGHBORHOOD_STRUCTURE
#define NEIGHBORHOOD_STRUCTURE

#include <utility>
#include <vector>
#include "data_types.hpp"

std::pair<std::vector<Serv>, Local> exploreNeighborhood(int current, std::vector<std::vector<int>> &m_cost, std::vector<std::vector<int>> &m_time, std::pair<std::vector<Serv>, Local> preSolution);
std::pair<std::vector<Serv>, Local> makeSwap(std::vector<std::vector<int>> &m_cost, std::vector<std::vector<int>> &m_time, std::pair<std::vector<Serv>, Local> preSolution);
std::pair<std::vector< Serv>, Local> makeReinsertion(std::vector<std::vector<int>> &m_cost, std::vector<std::vector<int>> &m_time, std::pair<std::vector< Serv>, Local> preSolution);
std::pair<std::vector< Serv>, Local> makeBlockReinsertion(std::vector<std::vector<int>> &m_cost, std::vector<std::vector<int>> &m_time, std::pair<std::vector< Serv>, Local> preSolution);

#endif