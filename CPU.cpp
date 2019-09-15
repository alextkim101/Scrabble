#include <algorithm>
#include <fstream>
#include "CPU.h"
#include "Exceptions.h"
using namespace std; 

void CPU::initialize(string file) {
	ifstream dic_file(file); 
	if(dic_file.fail()) {
		throw FileException("provide a valid dictionary file"); 
	}
	while(!dic_file.eof()) {
		std::string temp; 
		dic_file >> temp; 
		prefix.insert(temp); 
		std::reverse(temp.begin(), temp.end());
		suffix.insert(temp); 
	}
}

void CPU::anchor_square( Board& board, Bag& bag, Dictionary& dic) {
	int** visit = new int*[board.getRows()]; 
	for(unsigned int i = 0; i < board.getRows(); i++) {
		visit[i] = new int[board.getColumns()]; 
	}
	for(unsigned int i = 0; i < board.getRows(); i++) {
		for(int j = 0; i < board.getColumns(); j++) {
			visit[i][j] = 0; 
		}
	} 
	for(unsigned int i = 0; i < board.getRows(); i++) {
		for(unsigned int j = 0; j < board.getColumns(); j++) {
			if(board.getSquare(i,j)->isOccupied()) {

				if(i+1 < board.getRows()) {
					if(!board.getSquare(i+1,j)->isOccupied() && visit[i+1][j] == 0) {
						visit[i+1][j] = 1; 
						anchors.push_back(board.getSquare(i+1,j)); 
					} 
				}
				if(i-1 < 0) {
					if(!board.getSquare(i+1,j)->isOccupied() && visit[i-1][j] == 0) {
						visit[i-1][j] = 1; 
						anchors.push_back(board.getSquare(i-1,j)); 
					} 
				}
				if(j+1 < board.getColumns()) {
					if(!board.getSquare(i,j+1)->isOccupied() && visit[i][j+1] == 0) {
						visit[i][j+1] = 1; 
						anchors.push_back(board.getSquare(i,j+1)); 
					} 
				}
				if(j-1 < 0) {
					if(!board.getSquare(i,j-1)->isOccupied() && visit[i][j-1] == 0) {
						visit[i][j-1] = 1; 
						anchors.push_back(board.getSquare(i,j-1)); 
					} 
				}
			}
		}
	}	
}
void CPU::cross_checking() {
	/*For each anchor square (squares adjacent to tiles already placed on the board) check all 
	check all possible letters that can be placed by checking with the TrieSet. 
	Case 1: 
		if (the anchor square is left of an occupied square)
			while(right of anchor tile is occupied) 
				build the string of the tile already there;  	
			reverse the string; 
			check against the suffix tree
			if(valid) 
				add to the the set of vaid_char for that anchor square. 
	Case 2: 
		if(the anchor square is right of occupied square) 
			while(left of anchor is occupied)
				build the string; 
			reverse the string; 
			check against the prefix tree
			if(valid)
				add to the set of valid_char of that anchor square;
	Case 3/4: 
		similar process for up and down. check against suffix tree or up and prefix for down. */
}

void find_valid_move() {
	//list of best from each anchor square 
	/* for (all anchors squares)
		left_part(); 
		above_part()*/
	//AT THIS POINT, IT WOULD;VE RETURNED THE LIST OF VALID MOVES 
	//call getplacemoveresults to get score and valid strings 
		//all moves will be valid since we already checked for non-trivial valid moves by cross 
		//checking 
	/*at this points, depending which AI we want, we decide what the appriate move is 
	(break ties arbirarily*/ 

	//add to the list of best moves from each anchor square
	//find the best from among those moves 

	/* construct move string based on best move and corresponding square and direction 

	if(list is empty meaning to valid move)
		pass 
	else 
		execute placemove 

	*/

}
void CPU::left_part(string partial, TrieNode* node, int handsize) {

	/*
	right_part(string partial, TrieNode*, sqr); 
	if(handsize > 0 && within bounds)
		for(each edge out of node)
			if(letter from hand exist as the edge)
				remove from hand 
				new_node = new node after following said edge
				left_part(partial+=letter, new_node, limit -1); 
				put the tile back to hand 
					*/
}	

void CPU::right_part(string partial, TrieNode* node, Square* sqr) {
	/*if(sqr is vacant)
		if(node is inSet)
			list.add(partil)
		if(each edge out of node)
			if(letter from hand exist as the edge and that letter is in the valid_char set for
			that anchor square from the cross checking)
				remove from hand 
				new_node = new node after following said edge
				new_sqr = move to next square to the right 
				right_part(partial, new_node, new_sqr) 
				put tile back to hand 
	else 
		partial+=letter occupying the sqr 
		if(that letter exists as an edge from node)
			new_node = new node after following said edge
			new_sqr = move to next square to the right 
			right_part(partial, new_node, new_sqr) 

}

/*same logic for vertical placing */
