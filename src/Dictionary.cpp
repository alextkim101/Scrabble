#include "Dictionary.h"
#include <iostream>
#include <fstream>

//[UPDATE] BOTH FUNCTIONS SEEM TO BE WORKING 
Dictionary::Dictionary (std::string &dictionary_file_name) { 
	std::ifstream dic_file(dictionary_file_name); 
	if(dic_file.fail()) {
		std::cout << "fail open" << std::endl;
	}
	while(!dic_file.eof()) {
		std::string temp; 
		dic_file >> temp; 
		dic.insert(temp); 

	}
}

Dictionary::~Dictionary() { 
	dic.clear(); 
}

bool Dictionary::isLegalWord (std::string const & word) const { 
	if(dic.find(word) != dic.end()) { 
		return true; 
	} else { 
		return false; 
	}
}
