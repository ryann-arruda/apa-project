#include "file_operations.h"
#include <iostream>

void greedy_algorithm(std::vector<std::vector<int>> &m_cost, std::vector<std::vector<int>> &m_time, std::pair<std::vector<std::vector<Serv*>>, int> solution);

int main(int argc, char* argv[]) {
    std::vector<Serv*> servs;
    std::vector<std::vector<int>> m_time, m_cost;
    int local_cost = 0;

    std::string path = "test instances/n5m15A.txt";

    read_instance(path, servs, m_time, m_cost, local_cost);

    std::pair<std::vector<std::vector<Serv*>>, int> solution;

    greedy_algorithm(m_cost, m_time, solution);

    return 0;
}

void greedy_algorithm(std::vector<std::vector<int>> &m_cost, std::vector<std::vector<int>> &m_time, std::pair<std::vector<std::vector<Serv*>>, int> solution){
    std::vector<std::vector<double>> cost_per_minute;

    for(int i = 0; i < m_cost.size(); i++){
        std::vector<double> line; 
        
        for(int j = 0; j < m_cost[i].size(); j++){
            line.push_back((double)m_cost[i][j]/m_time[i][j]);
        }

        cost_per_minute.push_back(line);
    }

    for(int i = 0; i < m_cost.size(); i++){        
        for(int j = 0; j < m_cost[i].size(); j++){
            std::cout << cost_per_minute[i][j] << " ";
        }

        std::cout << std::endl;
    }
}