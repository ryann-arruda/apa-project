#include "file_operations.h"
#include <iostream>
#include <limits>

void greedy_algorithm(std::vector<std::vector<int>> &m_cost, std::vector<std::vector<int>> &m_time, std::vector<Serv*> &servs, Local& local, std::pair<std::vector<Serv*>*, Local*> &solution);

int main(int argc, char* argv[]) {
    std::vector<Serv*> servs;
    std::vector<std::vector<int>> m_time, m_cost;
    Local local;

    std::string path = "test instances/n5m15A.txt";

    read_instance(path, servs, m_time, m_cost, local);

    std::pair<std::vector<Serv*>*, Local*> solution;

    greedy_algorithm(m_cost, m_time, servs, local, solution);

    return 0;
}

void greedy_algorithm(std::vector<std::vector<int>> &m_cost, std::vector<std::vector<int>> &m_time, std::vector<Serv*> &servs, Local& local, std::pair<std::vector<Serv*>*, Local*> &solution){
    std::vector<std::vector<double>> cost_per_minute;

    for(int i = 0; i < m_cost.size(); i++){
        std::vector<double> line; 
        
        for(int j = 0; j < m_cost[i].size(); j++){
            line.push_back((double)m_cost[i][j]/m_time[i][j]);
        }

        cost_per_minute.push_back(line);
    }
    
    for(int job = 0; job < cost_per_minute[0].size(); job++){
        double best = std::numeric_limits<double>::max();
        int index = -1;

        for(int serv = 0; serv < cost_per_minute.size(); serv++){
            if(cost_per_minute[serv][job] < best && m_time[serv][job] <= servs[serv] -> capacity){
                best = cost_per_minute[serv][job];
                index = serv;
            }
        }

        if(index != -1){
            (servs[index]->job_indexes).push_back(job);
            servs[index]->capacity -= m_time[index][job];
        }
        else{
            local.job_indexes.push_back(job);
        }
    }

    solution.first = &servs;
    solution.second = &local;
}