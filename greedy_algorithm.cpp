#include "greedy_algorithm.hpp"

#include <iostream>
#include <limits>

bool check_chosen_servers(std::vector<int> &check_servs){
    bool answer = false;

    for(int i = 0; i < check_servs.size(); i++){

        if(check_servs[i] == 0){
            answer = true;
        }
    }

    return answer;
}

void greedy_algorithm(std::vector<std::vector<int>> &m_time, std::vector<Serv> servs, Local local, std::pair<std::vector<Serv>, Local> &solution){
    std::vector<int> jobs;
    std::vector<int> check_servs;

    for(int i = 0; i < m_time.size(); i++){
        check_servs.push_back(0);
    }

    for(int i = 0; i < m_time[0].size(); i++){
        jobs.push_back(i);
    }

    while(check_chosen_servers(check_servs)){

        for(int serv = 0; serv < m_time.size(); serv++){
            int best = std::numeric_limits<int>::max();
            int job_index = -1;

            for(int job = 0; job < jobs.size(); job++){
                int time = m_time[serv][jobs[job]];

                if(time < best && m_time[serv][jobs[job]] <= servs[serv].capacity){
                    best = time;
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

    solution.first = servs;
    solution.second = local;
}