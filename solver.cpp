#include "instance_reader.hpp"
#include "data_types.hpp"
#include "greedy_algorithm.hpp"
#include "local_search.hpp"
#include "execution_report.hpp"
#include "metaheuristica.hpp"

#include <iostream>
#include <chrono>

int main(int argc, char* argv[]) {
    std::vector<Serv> servs;
    std::vector<std::vector<int>> m_time, m_cost;
    Local local;

    if(argc < 2){
        std::cerr << "Ausência de parâmetros! Informado apenas: [" << argv[0] << "] - Falta o nome da instância e o valor ILSMax." << std::endl;
        return 1;
    }
    else if(argc < 3){
        std::cerr << "Ausência de parâmetros! Informado apenas: [" << argv[0] << "], [" << argv[1] << "] - Falta o valor ILSMax." << std::endl;
        return 1;
    }

    std::string filename = argv[1];
    std::string path = "test instances/" + filename;

    read_instance(path, servs, m_time, m_cost, local);

    std::pair<std::vector<Serv>, Local> solution;

    auto start = std::chrono::high_resolution_clock::now();
    greedy_algorithm(m_time, servs, local, solution);
    auto end = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << "Valor (guloso): " << objective_function(m_cost, solution) << std::endl;
    std::cout << "Tempo para o guloso: " << duration.count() << "ms" << std::endl;

    start = std::chrono::high_resolution_clock::now();
    solution = vnd(3, m_cost, m_time, solution);
    end = std::chrono::high_resolution_clock::now();

    duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Valor (vnd): " << objective_function(m_cost, solution) << std::endl;
    std::cout << "Tempo para o VND: " << duration.count() << "ms" << std::endl;

    std::cout << std::endl;

    int ILSMax = std::stoi(argv[2]);

    start = std::chrono::high_resolution_clock::now();
    std::pair<std::vector<Serv>, Local> solution_ils = ILS(m_cost, m_time, servs, local, ILSMax);
    end = std::chrono::high_resolution_clock::now();

    duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << "Valor ILS: " << objective_function(m_cost, solution_ils) << std::endl;
    std::cout << "Tempo para o ILS: " << duration.count() << "ms" << std::endl;

    std::string output_file_name = "result_" + filename; 

    make_file(output_file_name, m_cost, solution, objective_function(m_cost, solution));

    return 0;
}