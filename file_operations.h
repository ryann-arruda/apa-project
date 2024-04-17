#ifndef FILE_OPERATIONS_H
#define FILE_OPERATIONS_H

#include <string>
#include <vector>

typedef struct serv{
    int capacity = 0;
    std::vector<int> job_indexes;
}Serv;


void read_instance(std::string path, std::vector<Serv*> &servs, std::vector<std::vector<int>> &m_time, std::vector<std::vector<int>> & m_cost);

#endif
