#include "metaheuristica.hpp"
#include "greedy_algorithm.hpp"
#include "local_search.hpp"

#include <chrono>
#include <random>

std::pair<std::vector<Serv>, Local> pertubation(std::pair<std::vector<Serv>, Local> solution, std::vector<std::vector<int>> &m_time){
    auto tempo_atual = std::chrono::system_clock::now().time_since_epoch().count();
    std::vector<Serv> &servs = solution.first;
    
    std::default_random_engine generator(static_cast<unsigned int>(tempo_atual));
    std::uniform_int_distribution<int> random_serv(0,servs.size() - 1);

    int serv1 = random_serv(generator);
    int serv2 = random_serv(generator);

    while(serv1 == serv2){
        serv2 = random_serv(generator);
    }

    int origin_job = -1;
    int dest_job = -1;
    for(int job1 = 0; job1 < servs[serv1].job_indexes.size(); job1++){
        for(int job2 = 0; job2 < servs[serv2].job_indexes.size(); job2++){
            int job_index1 = servs[serv1].job_indexes[job1];
            int job_index2 = servs[serv2].job_indexes[job2];

            /* A capacidade do servidor para realizar a troca de um job com outro em outro servidor
            * consiste da soma da capacidade residual do servidor com o tempo que o job que irá sair
            * liberará.
            */
            int time_to_execute1 = m_time[serv1][job_index1] + servs[serv1].capacity;
            int time_to_execute2 = m_time[serv2][job_index2] + servs[serv2].capacity;

            // O primeiro job possível de ser troca é escolhido 
            if(time_to_execute1 >= m_time[serv1][job_index2] && time_to_execute2 >= m_time[serv2][job_index1]){
                origin_job = job1;
                dest_job = job2;
                break;
            }
        }

        if(origin_job != -1){
            break;
        }
    }

    // Verifica se encontrou jobs possíveis de serem trocados entre os servidores
    if(origin_job != -1){
        int job1_index = servs[serv1].job_indexes[origin_job];
        int job2_index = servs[serv2].job_indexes[dest_job];

        servs[serv1].capacity += m_time[serv1][job1_index];
        servs[serv2].capacity += m_time[serv2][job2_index];

        servs[serv1].capacity -= m_time[serv1][job2_index];
        servs[serv2].capacity -= m_time[serv2][job1_index];

        std::swap(servs[serv1].job_indexes[origin_job], servs[serv2].job_indexes[dest_job]);
    }

    return solution;
}

std::pair<std::vector<Serv>, Local> ILS(std::vector<std::vector<int>> &m_cost, std::vector<std::vector<int>> &m_time, std::vector<Serv> &servs, Local& local, int ILSMax){
    std::pair<std::vector<Serv>, Local> solution;

    greedy_algorithm(m_time, servs, local, solution);
    solution = vnd(3, m_cost, m_time, solution);

    int currentILS = 0;

    while(currentILS < ILSMax){
        std::pair<std::vector<Serv>, Local> solution_modified = pertubation(solution, m_time);
        solution_modified = vnd(3, m_cost, m_time, solution_modified);

        if(objective_function(m_cost, solution_modified) < objective_function(m_cost, solution)){
            solution = solution_modified;
        }

        currentILS++;
    }

    return solution;
}