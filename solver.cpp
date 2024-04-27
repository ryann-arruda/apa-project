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

    std::string path = "test instances/n5m15A.txt";

    read_instance(path, servs, m_time, m_cost, local);

    std::pair<std::vector<Serv>, Local> solution;

    auto start = std::chrono::high_resolution_clock::now();
    greedy_algorithm(m_cost, m_time, servs, local, solution);
    std::cout << "Valor (guloso): " << objective_function(m_cost, solution) << std::endl;
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

bool check(std::vector<int> &check_servs){
    bool answer = false;

    for(int i = 0; i < check_servs.size(); i++){

        if(check_servs[i] == 0){
            answer = true;
        }
    }

    return answer;
}

void greedy_algorithm(std::vector<std::vector<int>> &m_cost, std::vector<std::vector<int>> &m_time, std::vector<Serv> &servs, Local& local, std::pair<std::vector<Serv>, Local> &solution){
    std::vector<int> jobs;
    std::vector<int> check_servs;

    for(int i = 0; i < m_cost.size(); i++){
        check_servs.push_back(0);
    }

    for(int i = 0; i < m_cost[0].size(); i++){
        jobs.push_back(i);
    }

    while(check(check_servs)){

        for(int serv = 0; serv < m_cost.size(); serv++){
            double best = std::numeric_limits<double>::max();
            int job_index = -1;

            for(int job = 0; job < jobs.size(); job++){
                double cost = m_time[serv][jobs[job]];

                if(cost < best && m_time[serv][jobs[job]] <= servs[serv].capacity){
                    best = cost;
                    job_index = job;
                }
            }

            if(job_index != -1){
                servs[serv].capacity -= m_time[serv][jobs[job_index]];
                servs[serv].job_indexes.push_back(jobs[job_index]);
                jobs.erase(jobs.begin()+job_index); 
            }
            else{
                check_servs[serv] = 1;
            }
        }
    }

    if(jobs.size() > 0){
        for(int job = 0; job < jobs.size(); job++){
            local.job_indexes.push_back(jobs[job]);
        }
    }

    for(int serv = 0; serv < servs.size(); serv++){
        for(int job = 0; job < servs[serv].job_indexes.size(); job++){
            std::cout << servs[serv].job_indexes[job] << " ";
        }

        std::cout << std::endl;
    }

    std::cout << "--------------------------------------" << std::endl;

    for(int job = 0; job < local.job_indexes.size(); job++){
        std::cout << local.job_indexes[job] << " ";
    }

    std::cout << std::endl;

    std::cout << "COLOCAR PARA RECEBER O NOME DO ARQUIVO PELO TERMINAL" << std::endl;

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