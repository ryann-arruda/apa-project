#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include "file_operations.h"


void initialize_matrix_row(std::vector<std::vector<int>> &matrix, std::string line, int n_servs){
    static int current_row = 0;
    std::istringstream iss(line);
    std::string value;
    
    std::vector<int> row;
    matrix.push_back(row);
    while(iss >> value){
        matrix[current_row].push_back(std::stoi(value));
    }

    if(current_row == n_servs - 1){
        current_row = 0;
    }
    else{
        current_row++;
    }
}

void read_instance(std::string path, std::vector<Serv*> &servs, std::vector<std::vector<int>> &m_time, std::vector<std::vector<int>> & m_cost, std::vector<Local*> &local){
    std::ifstream file(path);

    int n_jobs = 0;
    int n_servs = 0;

    if(file.is_open()){
        int i = 0;
        std::string line;

        while(std::getline(file, line)){

            if(i == 1){
                n_servs = std::stoi(line);
            }

            else if(i == 2){
                std::string local_cost = line;
                Local *aux = new Local;

                aux -> local_cost = std::stoi(local_cost);
            }
            else if(i == 4){
                std::string server_capacity = line;

                std::istringstream iss(line);
                std::string capacity;
                while(iss >> capacity){
                    Serv *aux = new Serv;
                    aux -> capacity = std::stoi(capacity);

                    servs.push_back(aux);
                }
            }
            else if(i >= 6 && i < 6 + n_servs){
                initialize_matrix_row(m_time, line, n_servs);
            }
            else if(i > 6 + n_servs){
                initialize_matrix_row(m_cost, line, n_servs);
            }

            i++;
        }

        file.close();
    }
    else{
        std::cerr << "Error opening the file" << std::endl;
    }
}