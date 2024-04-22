#include "neighborhood_structure.hpp"
#include <iostream>
#include <random>
#include <utility>

std::pair<std::vector<Serv*>*, Local*> exploreNeighborhood(int current, std::vector<std::vector<int>> &m_cost, std::vector<std::vector<int>> &m_time, std::pair<std::vector<Serv*>*, Local*> preSolution){
    
    switch(current){
        case 0:
            return makeSwap(m_cost, m_time, preSolution);
            break;
        case 1:
            break;
        case 2:
            break;
        default:
            break;
    }
}

std::pair<std::vector<Serv*>*, Local*> makeSwap(std::vector<std::vector<int>> &m_cost, std::vector<std::vector<int>> &m_time, std::pair<std::vector<Serv*>*, Local*> preSolution){

    std::default_random_engine generator;
    std::uniform_int_distribution<int> options(0,1);

    int result = options(generator);

    int serv1 = 0;
    int serv2 = 0;
    int job1 = 0;
    int job2 = 0;

    if(result == 0){
        std::uniform_int_distribution<int> servs(0,preSolution.first->size() - 1);

        serv1 = servs(generator);
        serv2 = -1;

        std::uniform_int_distribution<int> jobs_serv1(0,(*preSolution.first)[serv1]->job_indexes.size() - 1);

        job1 = jobs_serv1(generator);
        job2 = -1;

        result = options(generator);

        if(result == 0){
            serv2 = servs(generator);

            std::uniform_int_distribution<int> jobs_serv2(0,(*preSolution.first)[serv2]->job_indexes.size() - 1);

            job2 = jobs_serv2(generator);
        }
        else{
            std::uniform_int_distribution<int> jobs_local(0,preSolution.second->job_indexes.size() - 1);

            job2 = jobs_local(generator);
        }
    }
    else{
        std::uniform_int_distribution<int> jobs_local(0,preSolution.second->job_indexes.size() - 1);

        job1 = jobs_local(generator);

        std::uniform_int_distribution<int> servs(0,preSolution.first->size() - 1);
        serv1 = servs(generator);

        std::uniform_int_distribution<int> jobs_serv(0,(*preSolution.first)[serv1]->job_indexes.size() - 1);
        job2 = jobs_serv(generator);
    }

    if((*preSolution.first)[serv1]->capacity + m_time[serv1][job1] >= m_time[serv1][job2] && (*preSolution.first)[serv2]->capacity + m_time[serv2][job2] >= m_time[serv2][job1]){
        if(m_cost[serv1][job2] + m_cost[serv2][job1] < m_cost[serv1][job1] + m_cost[serv2][job2]){
            std::swap((*preSolution.first)[serv1]->job_indexes[job1], (*preSolution.first)[serv2]->job_indexes[job2]);
        }
    }

    return preSolution;
}

std::pair<std::vector<Serv*>*, Local*> makeReinsertion(std::vector<std::vector<int>> &m_cost, std::vector<std::vector<int>> &m_time, std::pair<std::vector<Serv*>*, Local*> preSolution){
    
    std::default_random_engine generator;
    std::uniform_int_distribution<int> options(0,1);

    int result = options(generator);
    int serv1;
    int serv2;
    int job1;

    if(result == 0 ){
        std::uniform_int_distribution<int> servs(0,preSolution.first->size() - 1);

        serv1 = servs(generator);

        std::uniform_int_distribution<int> jobs_serv1(0,(*preSolution.first)[serv1]->job_indexes.size() - 1);

        job1 = jobs_serv1(generator);

        result = options(generator);

        if(result == 0){

            serv2 = servs(generator);

        }

    }

    else{

        std::uniform_int_distribution<int> jobs_local(0,preSolution.second->job_indexes.size() - 1);

        job1 = jobs_local(generator);

        std::uniform_int_distribution<int> servs(0,preSolution.first->size() - 1);

        serv1 = servs(generator);
    }


    if((*preSolution.first)[serv2]->capacity >= m_time[serv2][job1]){
        if(m_cost[serv2][job1] < m_cost[serv1][job1]){
            (*preSolution.first)[serv1]->job_indexes.erase((*preSolution.first)[serv1]->job_indexes.begin()+job1);
            (*preSolution.first)[serv1]->capacity += m_time[serv1][job1];
            (*preSolution.first)[serv2]->job_indexes.push_back(job1);
            (*preSolution.first)[serv2]->capacity -= m_time[serv2][job1];
        }
    }

    return preSolution;
}