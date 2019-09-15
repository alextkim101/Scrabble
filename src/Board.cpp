#include <Board.h>
#include <fstream>
#include <sstream> 
#include <iostream>
#include <algorithm>
#include "Exceptions.h"
using namespace std; 

Board::Board (std::string board_file_name) {
	std::string buf; 
	std::ifstream board_file(board_file_name); 
	std::stringstream ss; 
	getline(board_file, buf); //reads in rows and cols 
	ss << buf; 
	ss >> rows; 
	ss >> cols; 
	ss.clear(); 
	board = new Square**[rows]; 
	for(size_t i = 0; i < rows; i++) {
		board[i] = new Square*[cols]; 
	}
		getline(board_file, buf); //reads in starting x and starting y  
		ss << buf; 
		ss >> sx; 
		ss >> sy; 
		ss.clear(); 
	for(size_t i = 0; i < rows; i++) {
		getline(board_file, buf);
		unsigned int LMult = 1; 
		unsigned int WMult = 1;  
		for(size_t j = 0; j < cols; j++) { 
			bool start = false;
			if(i == sx-1 && j == sy-1) {
				start = true; 
			}
			if(buf.at(j) == 't') { 
				 WMult = 3; 
			} else if(buf.at(j) == 'd') { 
				WMult = 2; 
			} else if(buf.at(j) == '2') {
				LMult = 2; 
			} else if(buf.at(j) == '3') { 
				LMult = 3; 
			} else if(buf.at(j) == '.') {

			}
			board[i][j] = new Square(LMult, WMult, start); 
			//reset variables 
			LMult = 1; 
			WMult = 1; 
			start = false; 
		}
	}		
}

Board::~Board () { 
	for(size_t i = 0; i < rows; i++) {
		for(size_t j = 0; j < cols; j++) { 
			delete board[i][j]; 
		}
		delete[] board[i];
	}
	delete[] board; 
}
void Board::initialize(string file) {
	std::string buf; 
	std::ifstream init_file(file); 
	std::stringstream ss; 
	
	for(size_t i = 0; i < rows ; i++) {
		getline(init_file, buf);
		for(size_t j = 0; j < cols*3; j+=3) { 
			if(buf.at(j) != '.') {
				string temp = ""; 
				stringstream ss2; 
				char letter = buf.at(j); 
				temp+=buf.at(j+1); 
				temp+=buf.at(j+2); 
				int pt; 
				ss << temp; 
				ss >> pt; 
				Tile* init = new Tile(letter, pt); 
				board[i][j/3]->placeTile(init); 
			} else {
				//do nothing 
			}
		}
	}		
}
/* returns a vector of all words that would be formed by executing the
           given move. The first element of the pair is a string containing the word
           formed, and the second element is the score that that word earns
           (with all multipliers, but not the 50-point bonus for all letters).

       Words returned are all in uppercase.

	   The last entry of the vector is always the "main" word formed
	   in the direction chosen by the user; the others could be in arbitrary
	   order. (This is helpful for backtracking search.)
 
	   This function does not check that the words formed are actually in the dictionary.
	   The words returned from this function must be checked against the dictionary to
	   determine if the move is legal.	*/
std::vector<std::pair<std::string, unsigned int>> Board::getPlaceMoveResults(const PlaceMove &m) const {
	std::vector<std::pair<std::string, unsigned int>> list; 
	std::vector<Tile*> tiles = m.tileVector();
	if(m.getHorizantal()) { 
		int score = 0; 
		int wMult = 1; 
		//stores the newly formed array as string
		std::string newWord = ""; 
		std::string newWordL = ""; 
		int j = 0;
		int leftTravel = 1; 
		//checks left 
		while(m.get_x()-leftTravel >= 1 && getSquare(m.get_x()-leftTravel,m.get_y())->isOccupied()) {
			newWordL+=(getSquare(m.get_x()-leftTravel,m.get_y())->getLetter()); 
			score+=(getSquare(m.get_x()-leftTravel,m.get_y())->getScore());
			leftTravel++; 

		} 
		
		int skip = 0;
		for(size_t i = m.get_x()-1; i < m.get_x()-1 + tiles.size(); i++) {	
			int k = i+skip; 		
			//adds up skip spaces since we can't actually place tiles in squares yet 
			while(board[m.get_y()-1][k]->isOccupied()) {
				newWord+=board[(int)m.get_y()-1][k]->getLetter(); 
				score+=board[(int)m.get_y()-1][k]->getScore();
				k++; 
				skip++; 
			}
			//adds up the word mulitpliers 
			wMult*= board[m.get_y()-1][i+skip]->getWMult();  

			if(!board[m.get_y()-1][i+skip]->isOccupied()) {
				int vertScore = 0; 
				std::string vertWord = ""; 
				int up = 1; 
				//check upwards 
				while((int)m.get_y()-1-up >= 0 && board[(int)m.get_y()-1-up][i+skip]->isOccupied()) {
					vertWord+=board[(int)m.get_y()-1-up][i+skip]->getLetter(); 
					vertScore+=board[(int)m.get_y()-1-up][i+skip]->getScore();
					up++; 
				}
				std::reverse(vertWord.begin(), vertWord.end()); //reverses top part of perpendicular word formed
				vertWord+=tiles[j]->getUse(); 
				vertScore+=(tiles[j]->getPoints()*board[m.get_y()-1][i+skip]->getLMult()); 

				int down = 1; 
				//check that we're not on the edge of the board 
				//only check downwwards
				while((int)m.get_y()+down <= (int)rows && board[(int)m.get_y()-1+down][i+skip]->isOccupied()) {
					vertWord+=board[(int)m.get_y()-1+down][i+skip]->getLetter(); 
					vertScore+=board[(int)m.get_y()-1+down][i+skip]->getScore();
					down++; 
				}
				if(vertWord.length() > 1) { 
					vertScore*=board[m.get_y()-1][i+skip]->getWMult();
					std::pair<std::string, unsigned int> pair1(vertWord,vertScore); 
					list.push_back(pair1); 
				}
			}
			newWord+=tiles[j]->getUse();
			score+=(tiles[j]->getPoints()*board[m.get_y()-1][i+skip]->getLMult()); 
			
			j++;  
		}
		std::string newWordR = ""; 
		int rightTravel = tiles.size()+skip; 
		//check right
		while(m.get_x()+rightTravel <= cols && getSquare(m.get_x()+rightTravel,m.get_y())->isOccupied()) {
			newWordR+=(getSquare(m.get_x()+rightTravel,m.get_y())->getLetter());
			score+=(getSquare(m.get_x()+rightTravel,m.get_y())->getScore());
			rightTravel++; 

		} 
		if(m.get_x()+rightTravel > cols) { //this is the expected coordinate of the last letter
			throw FileException("your word will be out of bounds!"); 
		}
		//add Lstring + word + Rstring 
		std::reverse(newWordL.begin(), newWordL.end()); 
		newWordL+=newWord; 
		newWordL+=newWordR; 
		score*=wMult; 
		std::pair<std::string, unsigned int> pair2(newWordL,score); 
		list.push_back(pair2); 
		
	} else { //if placed vertically 
		int score = 0; 
		int wMult = 1; 

		std::string newWord = ""; 
		std::string newWordUp = ""; 
		int j = 0;
		int up = 1; 
		//checks left 
		while(m.get_y()-up >= 1 && getSquare(m.get_x(),m.get_y()-up)->isOccupied()) {
			newWordUp+=(getSquare(m.get_x(),m.get_y()-up)->getLetter()); 
			score+=(getSquare(m.get_x(),m.get_y()-up)->getScore());
			up++; 

		}
		
		int skip = 0;	
		for(size_t i = m.get_y()-1; i < m.get_y()-1 + tiles.size(); i++) {	

			int k = i+skip;
			while(board[k][m.get_x()-1]->isOccupied()) {
				newWord+=board[k][m.get_x()-1]->getLetter(); 
				score+=board[k][m.get_x()-1]->getScore();
				k++; 
				skip++; 
			}

			wMult*= board[i+skip][m.get_x()-1]->getWMult(); 

			if(!board[i+skip][m.get_x()-1]->isOccupied()) {
				int horizScore = 0; 
				std::string horizWord = ""; 
				int left = 1; 
				//check upwards 
				while((int)m.get_x()-1-left >= 0 && board[i+skip][(int)m.get_x()-1-left]->isOccupied()) {
					horizWord+=board[i+skip][(int)m.get_x()-1-left]->getLetter(); 
					horizScore+=board[i+skip][(int)m.get_x()-1-left]->getScore();
					left++; 
				}
				std::reverse(horizWord.begin(), horizWord.end()); //reverses top part of perpendicular word formed
				horizWord+=tiles[j]->getUse(); 
				horizScore+=(tiles[j]->getPoints()*board[i+skip][(int)m.get_x()-1]->getLMult()); 
				
				int right = 1; 
				//check that we're not on the edge of the board 
				//only check downwwards
				while((int)m.get_x()+right <= (int)cols && board[i+skip][(int)m.get_x()-1+right]->isOccupied()) {
					horizWord+=board[i+skip][(int)m.get_x()-1+right]->getLetter(); 
					horizScore+=board[i+skip][(int)m.get_x()-1+right]->getScore();
					right++; 
				}
				if(horizWord.length() > 1) { 
					horizScore*=board[i+skip][m.get_x()-1]->getWMult();
					std::pair<std::string, unsigned int> pair1(horizWord,horizScore); 
					list.push_back(pair1); 
				}
			}
			newWord+=tiles[j]->getUse();
			score+=(tiles[j]->getPoints()*board[i+skip][m.get_x()-1]->getLMult()); 
			
			j++;
		}

		std::string newWordDown = ""; 
		int down = tiles.size()+skip; //this coordinates between start and the end of word
		//check right
		while(m.get_y()+down <= rows && getSquare(m.get_x(),m.get_y()+down)->isOccupied()) {
			newWordDown+=(getSquare(m.get_x(),m.get_y()+down)->getLetter());
			score+=(getSquare(m.get_x(),m.get_y()+down)->getScore());
			down++; 
		} 
		if(m.get_y()+down > rows) {
			throw FileException("your word will be out of bounds!"); 
		}
		std::reverse(newWordUp.begin(), newWordUp.end()); //revereses upper part of newword since we traversed down up 
		newWordUp+=newWord; 
		newWordUp+=newWordDown; 
		score*=wMult; 
		std::pair<std::string, unsigned int> pair2(newWordUp,score); 
		list.push_back(pair2);
	}
	return list; 

}

/* Executes the given move by taking tiles and placing them on the board.
	   This function does not check for correctness of the move, so could
	   segfault or cause other errors if called for an incorrect move.
	   When a blank tile '?' is placed on the board, it is treated as a letter,
	   i.e., the corresponding square has that letter (with score 0) placed on it.
	*/
void Board::executePlaceMove (const PlaceMove & m) {
	std::vector<Tile*> copy = m.tileVector(); 
	if(m.getHorizantal()) {
		int j = 0;
		for(size_t i = m.get_x()-1; i < m.get_x()-1 + copy.size(); i++) {			
			int k = i;
			int skip = 0;  			
			while(board[m.get_y()-1][k]->isOccupied()) { 
				k++; 
				skip++; 
			}
			board[m.get_y()-1][i+skip]->placeTile(copy[j]); 
			j++; 
		}
	} else { 
		int j = 0;
		for(size_t i = m.get_y()-1; i < m.get_y()-1 + copy.size(); i++) {			
			int k = i;
			int skip = 0;  			
			while(board[k][m.get_x()-1]->isOccupied()) { 
				k++; 
				skip++; 
			}
			board[i+skip][m.get_x()-1]->placeTile(copy[j]); 
			j++;		
		}
	}
}

/* Returns a pointer to the Square object representing the
       (y,x) position of the board. Indexing starts at 1 here.
	   This is needed only to display the board. */
Square * Board::getSquare (size_t x, size_t y) const {
	return board[y-1][x-1]; 
}

/* Returns the number of rows of the board.*/
size_t Board::getRows() const {
	return rows; 
}

/* Returns the number of columns of the board.*/
size_t Board::getColumns() const {
	return cols; 
}
