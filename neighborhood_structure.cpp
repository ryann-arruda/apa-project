#include "neighborhood_structure.hpp"
#include <iostream>
#include <random>
#include <utility>
#include <chrono>
#include <thread>
#include <limits>

std::pair<std::vector<Serv>, Local> exploreNeighborhood(int current, std::vector<std::vector<int>> &m_cost, std::vector<std::vector<int>> &m_time, std::pair<std::vector<Serv>, Local> preSolution){
    
    switch(current){
        case 0:
            return makeReinsertion(m_cost, m_time, preSolution);            
            break;

        case 1:
            return makeSwap(m_cost, m_time, preSolution);
            break;

        case 2:
            return makeBlockReinsertion(m_cost, m_time, preSolution);
            break;
        
        default:
            std::pair<std::vector<Serv>, Local> aux;
            return aux;
    }
}

std::pair<std::vector<Serv>, Local> makeSwap(std::vector<std::vector<int>> &m_cost, std::vector<std::vector<int>> &m_time, std::pair<std::vector<Serv>, Local> preSolution){
    std::vector<Serv> &servs = preSolution.first;
    Local &local = preSolution.second;
    
    int origin_serv = 0, origin_job = 0;
    int dest_serv = 0, dest_job = 0;
    int best_cost = std::numeric_limits<int>::max();

    // As linhas 40 até 69 verifica qual o melhor movimento de swap entre os servidores
    int numb_servs = servs.size();
    for(int serv1 = 0; serv1 < numb_servs - 1; serv1++){
        for(int job1 = 0; job1 < servs[serv1].job_indexes.size(); job1++){
            // Recupera o índice do primerio job armazenado na lista de jobs do primeiro servidor
            int job1_index = servs[serv1].job_indexes[job1];

            for(int serv2 = serv1 + 1; serv2 < numb_servs; serv2++){
                for(int job2 = 0; job2 < servs[serv2].job_indexes.size(); job2++){
                    // Recupera o índice do segundo job armazenado na lista de jobs do segundo servidor
                    int job2_index = servs[serv2].job_indexes[job2];

                    // O espaço disponível será a capacidade residual do servidor somado ao tempo do job que irá sair desse servidor
                    int exec_time_serv1 = m_time[serv1][job1_index] + servs[serv1].capacity;
                    int exec_time_serv2 = m_time[serv2][job2_index] + servs[serv2].capacity;

                    if(exec_time_serv1 >= m_time[serv1][job2_index] && exec_time_serv2 >= m_time[serv2][job1_index]){
                        int exchange_cost = m_cost[serv1][job2_index] + m_cost[serv2][job1_index] - m_cost[serv1][job1_index] - m_cost[serv2][job2_index];

                        if(exchange_cost < best_cost){
                            best_cost = exchange_cost;
                            origin_serv = serv1;
                            dest_serv = serv2;
                            origin_job = job1;
                            dest_job = job2;
                        }
                    }

                }
            }
        }
    }

    // As linhas de 72 até 95 verifica qual o melhor movimento de swap entre os servidores e o local
    for(int serv1 = 0; serv1 < numb_servs; serv1++){
        for(int job1 = 0; job1 < servs[serv1].job_indexes.size(); job1++){
            // Recupera o índice do primerio job armazenado na lista de jobs do servidor
            int job1_index = servs[serv1].job_indexes[job1];

            for(int job2 = 0; job2 < local.job_indexes.size(); job2++){
                /* O espaço disponível no servidor será a capacidade residual desse servidor somado ao tempo do job que irá sair do servidor
                * para o local
                */
                int exec_time_serv1 = m_time[serv1][job1_index] + servs[serv1].capacity;

                // Recupera o índice do segundo job armazenado na lista de jobs do local
                int job2_index = local.job_indexes[job2];

                int local_cost = local.local_cost;
                if(exec_time_serv1 >= m_time[serv1][job2_index]){
                    int exchange_cost = m_cost[serv1][job2_index] - m_cost[serv1][job1_index];

                    if(exchange_cost < best_cost){
                        best_cost = exchange_cost;
                        origin_serv = serv1;
                        dest_serv = -1;
                        origin_job = job1;
                        dest_job = job2;
                    }
                }
            }
        }
    }

    // Verifica se houve melhora para que a troca seja realizada
    if(best_cost != std::numeric_limits<int>::max()){
        int job1_index = servs[origin_serv].job_indexes[origin_job];

        if(dest_serv != -1){
            int job2_index = servs[dest_serv].job_indexes[dest_job];

            // Restaura a capacidade dos servidores resultante da saída dos jobs
            servs[origin_serv].capacity += m_time[origin_serv][job1_index];
            servs[dest_serv].capacity += m_time[dest_serv][job2_index];

            // Reduz a capacidade dos servidores resultante da chegada de novos jobs
            servs[origin_serv].capacity -= m_time[origin_serv][job2_index];
            servs[dest_serv].capacity -= m_time[dest_serv][job1_index];

            std::swap(servs[origin_serv].job_indexes[origin_job], servs[dest_serv].job_indexes[dest_job]);
        }
        else{
            int job2_index = local.job_indexes[dest_job];

            // Restabelece a capacidade do servidor resultante da saída do job dele para o local
            servs[origin_serv].capacity += m_time[origin_serv][job1_index];
            // Reduz a capacidade do servidor resultante da chegada do job do local
            servs[origin_serv].capacity -= m_time[origin_serv][job2_index];
            
            std::swap(servs[origin_serv].job_indexes[origin_job], local.job_indexes[dest_job]);
        }
    }

    return preSolution;
}

std::pair<std::vector<Serv>, Local> makeReinsertion(std::vector<std::vector<int>> &m_cost, std::vector<std::vector<int>> &m_time, std::pair<std::vector<Serv>, Local> preSolution){
    std::vector<Serv> &servs = preSolution.first;
    Local &local = preSolution.second;

    int origin_serv = 0, origin_job = 0;
    int dest_serv = 0;
    int best_cost = std::numeric_limits<int>::max();

    for(int serv1 = 0; serv1 < servs.size(); serv1++){
        for(int job = 0; job < servs[serv1].job_indexes.size(); job++){
            // Recupera o índice do job armazenado na lista de jobs do servidor
            int job_index = servs[serv1].job_indexes[job];
            
            // Verifica a priori o custo de realizar o reinsertion no local
            int cost = local.local_cost - m_cost[serv1][job_index];

            if(cost < best_cost){
                best_cost = cost;
                origin_serv = serv1;
                dest_serv = -1;
                origin_job = job;
            }

            /* Verifica a possibilidade de realizar o reinsertion entre o job do servidor atual
            com os demais servidores*/
            for(int serv2 = 0; serv2 < servs.size(); serv2++){
                if(serv1 == serv2){
                    continue;
                }

                if(servs[serv2].capacity >= m_time[serv2][job_index]){
                    cost = m_cost[serv2][job_index] - m_cost[serv1][job_index];

                    if(cost < best_cost){
                        best_cost = cost;
                        origin_serv = serv1;
                        dest_serv = serv2;
                        origin_job = job;
                    }
                }

            }
        }
    }

    /* Verifica a possibilidade de realizar o reinsertion entre os jobs do local
    com os servidores*/
    for(int job = 0; job < local.job_indexes.size(); job++){
        for(int serv1 = 0; serv1 < servs.size(); serv1++){
            int local_job_index = local.job_indexes[job];

            if(servs[serv1].capacity >= m_time[serv1][local_job_index]){
                int cost = m_cost[serv1][local_job_index] - local.local_cost;
                
                if(cost < best_cost){
                    best_cost = cost;
                    origin_serv = -1;
                    dest_serv = serv1;
                    origin_job = job;
                }
            }
        }
    }

    // Se houve melhora, realiza o movimento de reinsertion
    if(origin_serv != -1){
        int job_index = servs[origin_serv].job_indexes[origin_job];
        servs[origin_serv].capacity += m_time[origin_serv][job_index];

        if(dest_serv != -1){
            servs[dest_serv].capacity -= m_time[dest_serv][job_index];

            servs[origin_serv].job_indexes.erase(servs[origin_serv].job_indexes.begin()+origin_job);
            servs[dest_serv].job_indexes.push_back(job_index);
        }
        else{
            servs[origin_serv].job_indexes.erase(servs[origin_serv].job_indexes.begin()+origin_job);
            local.job_indexes.push_back(job_index);
        }
    }
    else{
        int job_index = local.job_indexes[origin_job];

        servs[dest_serv].capacity -= m_time[dest_serv][job_index];

        local.job_indexes.erase(local.job_indexes.begin()+origin_job);
        servs[dest_serv].job_indexes.push_back(job_index);
    }

    return preSolution;
}

std::pair<std::vector<Serv>, Local> makeBlockReinsertion(std::vector<std::vector<int>> &m_cost, std::vector<std::vector<int>> &m_time, std::pair<std::vector<Serv>, Local> preSolution){
    std::vector<Serv> &servs = preSolution.first;
    Local &local = preSolution.second;

    int origin_serv = -1, dest_serv = -1;
    int origin_job = -1;
    int best_cost = std::numeric_limits<int>::max();
    for(int serv1 = 0; serv1 < servs.size(); serv1++){
        int numb_jobs = servs[serv1].job_indexes.size();

        if(numb_jobs < 2){
            continue;
        }

        for(int initial_job = 0; initial_job < numb_jobs - 1; initial_job++){
            
            for(int serv2 = 0; serv2 < servs.size(); serv2++){

                if(serv2 == serv1){
                    continue;
                }

                // Recupera o índice dos jobs do bloco a ser reinserido
                int job1_index = servs[serv1].job_indexes[initial_job];
                int job2_index = servs[serv1].job_indexes[initial_job + 1];

                /* O espaço necessário para reinserir esse bloco em outro servidor
                * consista na soma dos tempos que cada job leva para executar naquele servidor
                */
                int space_required = m_time[serv2][job1_index] + m_time[serv2][job2_index];

                if(servs[serv2].capacity >= space_required){
                    int cost = m_cost[serv2][job1_index] + m_cost[serv2][job2_index] - m_cost[serv1][job1_index] - m_cost[serv1][job2_index];

                    if(cost < best_cost){
                        best_cost = cost;
                        origin_serv = serv1;
                        dest_serv = serv2;
                        origin_job = initial_job;
                    }
                }
            }
        }
    }

    // Se houve melhora, realiza o movimento de reinserir o bloco de um servidor para outro
    if(origin_serv != -1){
        int job1_index = servs[origin_serv].job_indexes[origin_job];
        int job2_index = servs[origin_serv].job_indexes[origin_job + 1];

        servs[origin_serv].capacity += m_time[origin_serv][job1_index] + m_time[origin_job][job2_index];
        servs[origin_serv].job_indexes.erase(servs[origin_serv].job_indexes.begin() + origin_job);
        servs[origin_serv].job_indexes.erase(servs[origin_serv].job_indexes.begin() + origin_job);

        servs[dest_serv].capacity -= m_time[dest_serv][job1_index] + m_time[dest_serv][job2_index];
        servs[dest_serv].job_indexes.push_back(job1_index);
        servs[dest_serv].job_indexes.push_back(job2_index);
    }

    return preSolution;
}