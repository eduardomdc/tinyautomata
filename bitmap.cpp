#include "bitmap.hpp"
#include <vector>
#include <iostream>
#include <random>

void makeTable(std::vector<std::vector<int>>* table, int size){
	for (int i = 0; i < 32*size; i++) {
		table->push_back(std::vector<int>(size));
	}		
}

void printTable(std::vector<std::vector<int>>* table){
	std::vector<std::vector<int>>::iterator line;
	std::vector<int>::iterator n;
	for (line = table->begin(); line != table->end(); line++){
		for (n = line->begin(); n != line->end(); n++){
			std::cout << *n << " | ";
		}
		std::cout << std::endl;
	}
}

void randomizeTable(std::vector<std::vector<int>>* table){
	std::vector<std::vector<int>>::iterator line;
        std::vector<int>::iterator n;
        for (line = table->begin(); line != table->end(); line++){
                for (n = line->begin(); n != line->end(); n++){
                        *n = rand();
						*n = *n << 31; //rand_max doesn't go up to lastbit
						*n = *n | rand();
                }
        }
}
