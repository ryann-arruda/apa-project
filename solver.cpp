#include "file_operations.h"
#include <iostream>

int main(int argc, char* argv[]) {
    
    std::vector<Serv*> servs;
    std::vector<std::vector<int>> m_time, m_cost;

    std::string path = "test instances\\n5m15A.txt";

    read_instance(path, servs, m_time, m_cost);

    return 0;
}
