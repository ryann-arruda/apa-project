#include "neighborhood_structure.hpp"
#include <iostream>
#include <random>
#include <utility>
#include <chrono>
#include <limits>

std::pair<std::vector<Serv>, Local> exploreNeighborhood(int current, std::vector<std::vector<int>> &m_cost, std::vector<std::vector<int>> &m_time, std::pair<std::vector<Serv>, Local> preSolution){
    
    switch(current){
        case 0:
            return makeSwap(m_cost, m_time, preSolution);
            break;
        case 1:
            return makeReinsertion(m_cost, m_time, preSolution);
            break;
        case 2:
            return makeModifiedEjectionChain(m_cost, m_time, preSolution);
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

            for(int serv2 = serv1 + 1; serv2 < numb_servs; serv2++){
                for(int job2 = 0; job2 < preSolution.first[serv2].job_indexes.size(); job2++){
                    int exec_time_serv1 = m_time[serv1][job1] + preSolution.first[serv1].capacity;
                    int exec_time_serv2 = m_time[serv2][job2] + preSolution.first[serv2].capacity;

                    if(exec_time_serv1 >= m_time[serv1][job2] && exec_time_serv2 >= m_time[serv2][job1]){
                        int exchange_cost = m_cost[serv1][job2] + m_cost[serv2][job1] - m_cost[serv1][job1] - m_cost[serv2][job2];

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
            for(int job2 = 0; job2 < preSolution.second.job_indexes.size(); job2++){
                int exec_time_serv1 = m_time[serv1][job1] + preSolution.first[serv1].capacity;

                int job_local = preSolution.second.job_indexes[job2];
                int local_cost = preSolution.second.local_cost;
                if(exec_time_serv1 >= m_time[serv1][job_local]){
                    int exchange_cost = m_cost[serv1][job_local] - m_cost[serv1][job1];

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
        if(dest_serv != -1){
            preSolution.first[origin_serv].capacity += m_time[origin_serv][origin_job];
            preSolution.first[dest_serv].capacity += m_time[dest_serv][dest_job];

            preSolution.first[origin_serv].capacity -= m_time[origin_serv][dest_job];
            preSolution.first[dest_serv].capacity -= m_time[dest_serv][origin_job];

            std::swap(preSolution.first[origin_serv].job_indexes[origin_job], preSolution.first[dest_serv].job_indexes[dest_job]);
        }
        else{
            preSolution.first[origin_serv].capacity += m_time[origin_serv][origin_job];
            preSolution.first[origin_serv].capacity -= m_time[origin_serv][dest_job];
            
            std::swap(preSolution.first[origin_serv].job_indexes[origin_job], preSolution.second.job_indexes[dest_job]);
        }
    }

    return preSolution;
}

std::pair<std::vector<Serv>, Local> makeReinsertion(std::vector<std::vector<int>> &m_cost, std::vector<std::vector<int>> &m_time, std::pair<std::vector<Serv>, Local> preSolution){
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator(seed);
    std::uniform_int_distribution<int> options(0,1);

    int result = options(generator);
    int serv1;
    int serv2;
    int job1;

    if(result == 0 ){
        std::uniform_int_distribution<int> servs(0,preSolution.first.size() - 1);

        serv1 = servs(generator);

        std::uniform_int_distribution<int> jobs_serv1(0,preSolution.first[serv1].job_indexes.size() - 1);

        job1 = jobs_serv1(generator);

        result = options(generator);

        if(result == 0){

            serv2 = servs(generator);

        }

    }

    else{

        std::uniform_int_distribution<int> jobs_local(0,preSolution.second.job_indexes.size() - 1);

        job1 = jobs_local(generator);

        std::uniform_int_distribution<int> servs(0,preSolution.first.size() - 1);

        serv1 = servs(generator);
    }


    if(preSolution.first[serv2].capacity >= m_time[serv2][job1]){
        if(m_cost[serv2][job1] < m_cost[serv1][job1]){
            preSolution.first[serv2].job_indexes.push_back(preSolution.first[serv1].job_indexes[job1]);
            preSolution.first[serv1].job_indexes.erase(preSolution.first[serv1].job_indexes.begin()+job1);
            preSolution.first[serv1].capacity += m_time[serv1][job1];
            preSolution.first[serv2].capacity -= m_time[serv2][job1];
        }
    }

    return preSolution;
}

std::pair<std::vector<Serv>, Local> makeModifiedEjectionChain(std::vector<std::vector<int>> &m_cost, std::vector<std::vector<int>> &m_time, std::pair<std::vector<Serv>, Local> preSolution){
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator(seed);
    int job;

    for(int serv = 0; serv < preSolution.first.size() - 1; serv++){
        std::uniform_int_distribution<int> jobs_serv(0,preSolution.first[serv].job_indexes.size() - 1);

        job = jobs_serv(generator);

        if(preSolution.first[serv + 1].capacity >= m_time[serv + 1][job]){
            if(m_cost[serv + 1][job] < m_cost[serv][job]){
                preSolution.first[serv].job_indexes.erase(preSolution.first[serv].job_indexes.begin()+job);
                preSolution.first[serv].capacity += m_time[serv][job];
                preSolution.first[serv + 1].job_indexes.push_back(job);
                preSolution.first[serv + 1].capacity -= m_time[serv][job];
            }
        }
    }

    int last_serv = preSolution.first.size() - 1;
    std::uniform_int_distribution<int> jobs_serv(0,preSolution.first[last_serv].job_indexes.size() - 1);
    job = jobs_serv(generator);

    if(preSolution.second.local_cost < m_cost[last_serv][job]){
        preSolution.second.job_indexes.push_back(preSolution.first[last_serv].job_indexes[job]);
        preSolution.first[last_serv].job_indexes.erase(preSolution.first[last_serv].job_indexes.begin()+job);
        preSolution.first[last_serv].capacity += m_time[last_serv][job];
    }

    return preSolution;
}