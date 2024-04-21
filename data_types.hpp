#ifndef DATA_TYPES
#define DATA_TYPES

#include <vector>

typedef struct serv{
    int capacity = 0;
    std::vector<int> job_indexes;
}Serv;

typedef struct local{
    int local_cost = 0;
    std::vector<int> job_indexes;
}Local;

#endif