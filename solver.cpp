#include "instance_reader.hpp"
#include "data_types.hpp"
#include "neighborhood_structure.hpp"
#include <iostream>
#include <limits>
#include <fstream>
#include <chrono>
#include <thread>

void greedy_algorithm(std::vector<std::vector<int>> &m_cost, std::vector<std::vector<int>> &m_time, std::vector<Serv> &servs, Local& local, std::pair<std::vector<Serv>, Local> &solution);
int objective_function(std::vector<std::vector<int>> &m_cost, std::pair<std::vector<Serv>, Local> &solution);
std::pair<std::vector<Serv>, Local> vnd(int n_neighborhood_structure, std::vector<std::vector<int>> &m_cost, std::vector<std::vector<int>> &m_time, std::pair<std::vector<Serv>, Local> solution);
void make_file(std::string filename, std::vector<std::vector<int>> &m_cost, std::pair<std::vector<Serv>, Local> solution);

int main(int argc, char* argv[]) {
    std::vector<Serv> servs;
    std::vector<std::vector<int>> m_time, m_cost;
    Local local;

    std::string path = "test instances/n60m10A.txt";

    read_instance(path, servs, m_time, m_cost, local);

    std::pair<std::vector<Serv>, Local> solution;

    auto start = std::chrono::high_resolution_clock::now();
    greedy_algorithm(m_cost, m_time, servs, local, solution);
    auto end = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << "Tempo para a Heurística Construtiva: " << duration.count() << "ms" << std::endl;

    start = std::chrono::high_resolution_clock::now();
    solution = vnd(3, m_cost, m_time, solution);
    end = std::chrono::high_resolution_clock::now();

    duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Tempo para o VND: " << duration.count() << "ms" << std::endl;

    make_file("result.txt", m_cost, solution);

    return 0;
}

void greedy_algorithm(std::vector<std::vector<int>> &m_cost, std::vector<std::vector<int>> &m_time, std::vector<Serv> &servs, Local& local, std::pair<std::vector<Serv>, Local> &solution){
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
            if(cost_per_minute[serv][job] < best && m_time[serv][job] <= servs[serv].capacity){
                best = cost_per_minute[serv][job];
                index = serv;
            }
        }

        if(index != -1){
            (servs[index].job_indexes).push_back(job);
            servs[index].capacity -= m_time[index][job];
        }
        else{
            local.job_indexes.push_back(job);
        }
    }

    solution.first = servs;
    solution.second = local;
}

int objective_function(std::vector<std::vector<int>> &m_cost, std::pair<std::vector<Serv>, Local> &solution) {
    int cost_total = 0;

    std::vector<Serv> servs = solution.first;
    for (int i = 0; i < servs.size(); i++) {
        Serv servidor = servs[i];

        for (int j = 0; j < servidor.job_indexes.size(); j++) {
            int job_index = servidor.job_indexes[j];
            cost_total += m_cost[i][job_index];
        }
    }

    Local local = solution.second;
    cost_total += local.local_cost * local.job_indexes.size();

    return cost_total;
}

std::pair<std::vector<Serv>, Local> vnd(int n_neighborhood_structure, std::vector<std::vector<int>> &m_cost, std::vector<std::vector<int>> &m_time, std::pair<std::vector<Serv>, Local> solution){
    int k = 0; // first neighborhood structure

    std::pair<std::vector<Serv>, Local> bestSolution = solution;

    while(k < n_neighborhood_structure){
        std::pair<std::vector<Serv>, Local> aux = exploreNeighborhood(k, m_cost, m_time, bestSolution);

        int firstCost = objective_function(m_cost, bestSolution);
        int secondCost = objective_function(m_cost, aux);

        if(secondCost < firstCost){
            bestSolution = aux;
            k = 0;
        }
        else{
            k += 1;
        }
    }

    return bestSolution;
}

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

void make_file(std::string filename, std::vector<std::vector<int>> &m_cost, std::pair<std::vector<Serv>, Local> solution){
    std::vector<Serv> &servs = solution.first;
    Local &local = solution.second;
    std::ofstream file(filename);

    if(file.is_open()){
        file << objective_function(m_cost, solution) << "\n";
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