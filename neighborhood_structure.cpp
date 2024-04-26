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

//std::pair<std::vector<Serv>, Local> preSolution
std::pair<std::vector<Serv>, Local> makeSwap(std::vector<std::vector<int>> &m_cost, std::vector<std::vector<int>> &m_time, std::pair<std::vector<Serv>, Local> preSolution){
    int origin_serv = 0, origin_job = 0;
    int dest_serv = 0, dest_job = 0;
    int best_cost = std::numeric_limits<int>::max();

    int numb_servs = preSolution.first.size();
    for(int serv1 = 0; serv1 < numb_servs - 1; serv1++){
        for(int job1 = 0; job1 < preSolution.first[serv1].job_indexes.size(); job1++){
            int job1_index = preSolution.first[serv1].job_indexes[job1];

            for(int serv2 = serv1 + 1; serv2 < numb_servs; serv2++){
                for(int job2 = 0; job2 < preSolution.first[serv2].job_indexes.size(); job2++){
                    int job2_index = preSolution.first[serv2].job_indexes[job2];

                    int exec_time_serv1 = m_time[serv1][job1_index] + preSolution.first[serv1].capacity;
                    int exec_time_serv2 = m_time[serv2][job2_index] + preSolution.first[serv2].capacity;

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

    for(int serv1 = 0; serv1 < numb_servs; serv1++){
        for(int job1 = 0; job1 < preSolution.first[serv1].job_indexes.size(); job1++){
            int job1_index = preSolution.first[serv1].job_indexes[job1];

            for(int job2 = 0; job2 < preSolution.second.job_indexes.size(); job2++){
                int exec_time_serv1 = m_time[serv1][job1_index] + preSolution.first[serv1].capacity;

                int job2_index = preSolution.second.job_indexes[job2];

                int local_cost = preSolution.second.local_cost;
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

    if(best_cost != std::numeric_limits<int>::max()){
        int job1_index = preSolution.first[origin_serv].job_indexes[origin_job];

        if(dest_serv != -1){
            int job2_index = preSolution.first[dest_serv].job_indexes[dest_job];

            preSolution.first[origin_serv].capacity += m_time[origin_serv][job1_index];
            preSolution.first[dest_serv].capacity += m_time[dest_serv][job2_index];

            preSolution.first[origin_serv].capacity -= m_time[origin_serv][job2_index];
            preSolution.first[dest_serv].capacity -= m_time[dest_serv][job1_index];

            std::swap(preSolution.first[origin_serv].job_indexes[origin_job], preSolution.first[dest_serv].job_indexes[dest_job]);
        }
        else{
            int job2_index = preSolution.second.job_indexes[dest_job];

            preSolution.first[origin_serv].capacity += m_time[origin_serv][job1_index];
            preSolution.first[origin_serv].capacity -= m_time[origin_serv][job2_index];
            
            std::swap(preSolution.first[origin_serv].job_indexes[origin_job], preSolution.second.job_indexes[dest_job]);
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
            int job_index = servs[serv1].job_indexes[job];
            
            int cost = local.local_cost - m_cost[serv1][job_index];

            if(cost < best_cost){
                best_cost = cost;
                origin_serv = serv1;
                dest_serv = -1;
                origin_job = job;
            }

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

                int job1_index = servs[serv1].job_indexes[initial_job];
                int job2_index = servs[serv1].job_indexes[initial_job + 1];

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