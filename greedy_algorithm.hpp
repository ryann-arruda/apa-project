#ifndef GREEDY_ALGORITHM
#define GREEDY_ALGORITHM

#include <vector>

#include "data_types.hpp"

bool check_chosen_servers(std::vector<int> &check_servs);
void greedy_algorithm(std::vector<std::vector<int>> &m_time, std::vector<Serv> servs, Local local, std::pair<std::vector<Serv>, Local> &solution);

#endif