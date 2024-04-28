#include "local_search.hpp"
#include "neighborhood_structure.hpp"

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