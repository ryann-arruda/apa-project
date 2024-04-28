#ifndef METAHEURISTICA_HPP
#define METAHEURISTICA_HPP

#include <utility>
#include <vector>

#include "data_types.hpp"

std::pair<std::vector<Serv>, Local> pertubation(std::pair<std::vector<Serv>, Local> solution, std::vector<std::vector<int>> &m_time);
std::pair<std::vector<Serv>, Local> ILS(std::vector<std::vector<int>> &m_cost, std::vector<std::vector<int>> &m_time, std::vector<Serv> &servs, Local& local, int ILSMax);

#endif