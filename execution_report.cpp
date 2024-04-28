#include "execution_report.hpp"

#include <iostream>
#include <fstream>

int servers_cost(std::vector<std::vector<int>> &m_cost, std::pair<std::vector<Serv>, Local> solution){
    std::vector<Serv> &servs = solution.first;
    int cost = 0;

    for(int serv = 0; serv < servs.size(); serv++){
        for(int job = 0; job < servs[serv].job_indexes.size(); job++){
            cost += m_cost[serv][servs[serv].job_indexes[job]];
        }
    }

    return cost;
}

void make_file(std::string filename, std::vector<std::vector<int>> &m_cost, std::pair<std::vector<Serv>, Local> solution, int best_value){
    std::vector<Serv> &servs = solution.first;
    Local &local = solution.second;
    std::ofstream file(filename);

    if(file.is_open()){
        file << best_value << "\n";
        file << servers_cost(m_cost, solution) << "\n";
        file << local.job_indexes.size() * local.local_cost;

        file << "\n\n";

        for(int serv = 0; serv < servs.size(); serv++){
            std::string line = "";

            for(int job = 0; job < servs[serv].job_indexes.size(); job++){
                line += std::to_string(servs[serv].job_indexes[job]);
                line += " ";
            }

            file << line + "\n";
        }

        file.close();
    }
    else{
        std::cerr << "Erro ao criar o arquivo de saída\n\n";
    }
}