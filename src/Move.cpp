#include "Move.h"

#include <sstream> //[ADDED]
#include <iostream> 
#include <vector> 
#include <string>
#include "Tile.h"
#include "Player.h"
#include "Bag.h"
#include "Dictionary.h"
#include "Exceptions.h"
#include "Board.h"
#include <cctype>
using namespace std; 

Move::Move(Player* player) { 
	_player = player; 
}
Move::~Move() {

}
/* Parses the given move m, and constructs the corresponding move subclass.
	   m is assumed to be in the format described on the assignment sheet for a move command.
	   The player passed in is used to verify that the proposed tiles are
	   in fact in the player's hand.
	   It can handle all three types of move (PASS, EXCHANGE, PLACE).
	   Coordinates start with 1.
	*/
Move* Move::parseMove(std::string moveString, Player &p) {
	std::stringstream ss; 
	ss << moveString; 
	std::string mstring;
	ss >> mstring; 
	Move * move; 

	//make mstring to upper case 
	for(unsigned int i = 0; i < mstring.length(); ++i) {
		mstring.at(i) = toupper(mstring.at(i)); 
	}

	if(mstring == "PASS") {
		move = new PassMove(&p); 
	} else if(mstring == "PLACE") {
		char horizontal; 
		ss >> horizontal; 
		size_t x; 
		ss >> x; 
		size_t y;
		ss >> y;  
		std::string tileString; 
		ss >> tileString; 
		if(horizontal == '-') { 
			move = new PlaceMove(y,x,true, tileString, &p);
		} else {
			move = new PlaceMove(y,x,false, tileString, &p);
		} 

	} else if(mstring == "EXCHANGE") {
		std::string tileString; 
		ss >> tileString; 
		move = new ExchangeMove(tileString, &p); 
	} else {
		throw FileException("that is not a valid move"); 
	}

	return move; 
}
PassMove::PassMove(Player* player) : Move(player) { 

}
void PassMove::execute(Board & board, Bag & bag, Dictionary & dictionary) { 
	//do nothing move on to the next player
	//move onto next player. 
}
ExchangeMove::ExchangeMove(std::string tileString, Player * p) : Move(p) { 
	tile_str = tileString; 
}
void ExchangeMove::execute(Board & board, Bag & bag, Dictionary & dictionary) { 
	std::vector<Tile*> exchangedTiles; 
	if(_player->hasTiles(tile_str, false)) { 
		exchangedTiles = _player->takeTiles(tile_str, false); 
		bag.addTiles(exchangedTiles); 
		std::vector<Tile*> newTiles;
		newTiles = bag.drawTiles(exchangedTiles.size()); 
		_player->addTiles(newTiles); 
	} else {//checks that the user has all the tiles
		throw FileException("error: You do not have those tiles!"); 
	}
		
}
//[INCOMPLETE]
PlaceMove::PlaceMove(size_t x, size_t y, bool horizontal, std::string tileString, Player * p) : Move(p) {
	_x = x; 
	_y = y; 
	horiz = horizontal; 
	tile_str = tileString; 
}
void PlaceMove::execute(Board & board, Bag & bag, Dictionary & dictionary) { 
	//check if player has tiles 
	if(_player->hasTiles(tile_str, true)) { 
		placingTiles = _player->takeTiles(tile_str,true);	
	} else { 
		throw FileException(" error: You do not have those tiles!");	
	}
	//check that the square at the coordinate is not occupied. 
	if(board.getSquare(_x,_y)->isOccupied()) {
		string error_msg = "error: The square at following coordinates are already occupied: "; 
		error_msg+=to_string((int)_x); 
		error_msg+=" "; 
		error_msg+=to_string((int)_y); 
		_player->addTiles(placingTiles); 
		placingTiles.clear();
		throw FileException(error_msg); 

	} 
	vector<pair<string,unsigned int>> placeResult = board.getPlaceMoveResults(*this);
	bool allValid = true; 
	int puntos = 0; 
	string invalid_words = ""; 
	//checks that it has at least one neighbor tile previously placed 
	bool start = true;
	if(horiz) {
		if(_y != board.get_sx()) {
			start = false; 
		}
		if(_x < board.get_sy()) {
			size_t end = _x+tile_str.length()-1; 
			if(end < board.get_sy()) {
				start = false; 
			}
		}
	} else {
		if(_x != board.get_sy()) {
			start = false; 
		}
		if(_y < board.get_sx()) {
			size_t end = _y+tile_str.length()-1; 
			if(end < board.get_sx()) {
				start = false; 
			}
		}
	}
	if(!start && placeResult.size() == 1 && placeResult[placeResult.size()-1].first.length() <= placingTiles.size()) {
		_player->addTiles(placingTiles); 
		placingTiles.clear();
		throw FileException("error: you must place your tile adjacent to at least one previously place tile!"); 
	}

	for(size_t i = 0; i < placeResult.size(); i++) {
		if(!dictionary.isLegalWord(placeResult[i].first)) {
			allValid = false; 
			invalid_words+=placeResult[i].first;
			invalid_words+=" ";  
		} 
		puntos+=placeResult[i].second; 
	}
	if(allValid) {
		puntos+=_player->getPoints(); 
		_player->setPoints(puntos);	 
		board.executePlaceMove(*this);
		_player->addTiles(bag.drawTiles(placingTiles.size())); 
		placingTiles.clear(); 

	} else { //throws an exception when the any one of the words from the vector was invalid 
		_player->addTiles(placingTiles); 
		placingTiles.clear(); 
		string error_msg = "error: the following words are not in the dictionary: ";
		error_msg+=invalid_words; 
		throw FileException(error_msg);  

	}
} 

/* Returns the vector of tiles associated with a PLACE/EXCHANGE move.
Return value could be arbitrary for PASS moves. */

std::vector<Tile*> const & PlaceMove::tileVector () const {
	return placingTiles; 
}


//will execute the correct move at run time 
void apply(Move* move, Board & board, Bag & bag, Dictionary & dictionary) { 
	move->execute(board, bag, dictionary); 
}

