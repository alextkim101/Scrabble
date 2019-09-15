#include "Dictionary.h"
#include <iostream>
#include <fstream>

int main() { 
	std::string filename = "test.txt";
	//std::ifstream testt(filename);	
	Dictionary dic(filename); 
	std::string test = "yay";
	bool legal_test = dic.isLegalWord(test); 
	std::cout << legal_test << std::endl; 
}