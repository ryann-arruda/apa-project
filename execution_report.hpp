#ifndef EXECUTION_REPORT
#define EXECUTION_REPORT

#include <vector>
#include <string>

#include "data_types.hpp"

int servers_cost(std::vector<std::vector<int>> &m_cost, std::pair<std::vector<Serv>, Local> solution);
void make_file(std::string filename, std::vector<std::vector<int>> &m_cost, std::pair<std::vector<Serv>, Local> solution, int best_value);

#endif