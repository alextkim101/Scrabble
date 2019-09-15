#include <string>
#include <set>
#include <vector>
#include <Player.h>
#include <ctype.h> //[ADDED BY ME]
#include <iostream> //[TESING ONLY]
#include "Exceptions.h"
#include "Tile.h"

Player::Player (std::string const & name, size_t maxTiles) { 
	this->name = name; 
	max_tiles = maxTiles; 
	points = 0; 
}

Player::~Player () { 

}

std::set<Tile*> Player::getHandTiles() const {
	return hand; 
}

bool Player::hasTiles(std::string const & move, bool resolveBlanks) const { //[MAKE MORE EFFICIENT IF TIME PERMITS]
	std::string copy = move; 
	//first erase the filler word for blank to compare with tiles in the hand 
	if(resolveBlanks) { 
		for(unsigned int i = 0; i < copy.length(); ++i) {
			if(copy.at(i) == '?') { 
				copy.erase(i+1,1); //[NOTE] WILL CRASH IF ? IS AT THE END AND THERE'S NO LETTER TO USE IT AS
			}
		}
	}

	if(move.length()-1 > hand.size()) { 
			return false; 
	} else if(!resolveBlanks && move.length() > hand.size()) { 
		return false; 
	} 
	bool hasIt; 
	std::set<Tile*> buf = hand;
	//iterates and deletes the match tile from copy of hand.  
	for(unsigned int i = 0; i < copy.length(); i++) { 
		hasIt = false;
		for(std::set<Tile*>::iterator sit = buf.begin(); sit != buf.end(); ++sit) {
			if((*sit)->getLetter() == (char)tolower(copy.at(i))) { 
			 	hasIt = true; 
			 	buf.erase(sit); 
			 	break; 
			} 
		}
		if(!hasIt) {
			return false; 
		}
	}
	if(!hasIt) { 
		return false;  
	} else { 
		return true;  
	}

}

std::vector<Tile*> Player::takeTiles (std::string const & move, bool resolveBlanks) { 

	std::vector<Tile*> tilesTaken; 
	if(!resolveBlanks && !hasTiles(move, resolveBlanks)) { 
		return tilesTaken; 
	}
	for(unsigned int i = 0; i < move.length(); i++) { 
		for(std::set<Tile*>::iterator sit = hand.begin(); sit != hand.end(); ++sit) {

			if((*sit)->getLetter() == (char)tolower(move.at(i))) { 
				if(resolveBlanks && move.at(i) == '?') { 
					(*sit)->useAs((char)tolower(move.at(i+1)));
				} 
				
				Tile* temp = *sit; 
			 	hand.erase(sit); 
			 	tilesTaken.push_back(temp);

				if(resolveBlanks && move.at(i) == '?') {
					i++; //skips to char after the one used for blank
				}
			 	break; 

			} 
			
		}
	}	

	
	return tilesTaken; 

}

void Player::addTiles (std::vector<Tile*> const & tilesToAdd) { 
	for(size_t i = 0; i < tilesToAdd.size(); i++) { 
		hand.insert(tilesToAdd[i]); 
	}
}





