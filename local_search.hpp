#ifndef LOCAL_SEARCH
#define LOCAL_SEARCH

#include <vector>

#include "data_types.hpp"

int objective_function(std::vector<std::vector<int>> &m_cost, std::pair<std::vector<Serv>, Local> &solution);
std::pair<std::vector<Serv>, Local> vnd(int n_neighborhood_structure, std::vector<std::vector<int>> &m_cost, std::vector<std::vector<int>> &m_time, std::pair<std::vector<Serv>, Local> solution);

#endif