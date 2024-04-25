#ifndef FILE_OPERATIONS_H
#define FILE_OPERATIONS_H

#include <string>
#include <vector>
#include "data_types.hpp"

void read_instance(std::string path, std::vector<Serv> &servs, std::vector<std::vector<int>> &m_time, std::vector<std::vector<int>> & m_cost, Local &local);

#endif