#include "Tile.h"
#include "Dictionary.h"
#include "Square.h"
#include "ConsolePrinter.h"
#include "Bag.h"
#include "Player.h"
#include "Board.h"
#include "Move.h"
#include <iostream>
#include <fstream> 
#include <sstream> 
#include "Exceptions.h"
using namespace std; 

int main(int argc, char* argv[]) {

	ifstream config(argv[1]); 
	if(config.fail()) {
		cout << "please provide a valid config file." << endl; 
		return 1; 
	}
	string buff; 

	int maxHand; 
	int seed; 
	string bag_txt; 
	string board_txt; 
	string dic_txt;
	string init_txt; 
//parses config file
	while(getline(config,buff)) {
		stringstream ss; 
		ss << buff; 
		string temp; 
		ss >> temp; 
		if(temp == "HANDSIZE:") {
			ss >> maxHand; 
		} else if(temp == "BOARD:") {
			ss >> board_txt; 
			Board board(board_txt); 
		} else if (temp == "TILES:") {
			ss >> bag_txt; 
		} else if(temp == "DICTIONARY:") {
			ss >> dic_txt;
		} else if(temp == "SEED:") {
			ss >> seed; 
		} else if(temp == "INIT:") {
			ss >> init_txt; 
		}
	} 
	
	Bag bag(bag_txt, seed);
	Board board(board_txt);
	ifstream init(init_txt); 
	if(!init.fail())
		board.initialize(init_txt); 
	Dictionary diction(dic_txt); 

	//reading in number of players 
	vector<Player*> players; 
	cout << "Enter number of players (enter a number between 1 and 8)" << endl; 
	string num_str; 
	int numPlayer; 
	getline(cin,num_str); 
	stringstream ss2; 
	ss2 << num_str; 
	ss2 >> numPlayer;  
	//creating players and reading in names 
	for(int i = 0; i < numPlayer; i++) {
		string p_name; 
		cout << "Enter the name of Player" << i+1 << ":" << endl;
		getline(cin,p_name);  
		if(p_name.substr(0,2) == "CPU") {
			//create CPU player 
		}
		Player* gamer = new Player(p_name, maxHand); 
		players.push_back(gamer); 
		players[i]->addTiles(bag.drawTiles(maxHand)); 
	}
	ConsolePrinter printer; 


	bool first_move = true; //used to check if it's first move or not 
	int pass_count = 0; //counts of number of pass move per turn 
	bool game_over = false; //will be set to true with end conditions are met  

	while(bag.tilesRemaining() > 0) { //will go on as long as there are tiles in the bag
		for(int i = 0; i < numPlayer; i++) {
			//prints board at the start of player's turn 
			printer.printBoard(board); 
			printer.printHand(*players[i]);
			string movestr;  
			cout << "Player " << i+1 << " " << players[i]->getName() << "'s Move: " << endl; 
			// cout << "You have " << players[i]->getPoints() << " pts" << endl;
			int prev_pts = players[i]->getPoints(); 
			bool bad_move = true; 
			bool valid = true; 
			Move * pmove; 
			while(bad_move){ //until a valid move is input keeps asking for valid input 
				getline(cin,movestr);
				try {
					pmove = pmove->parseMove(movestr, *players[i]);
					valid = true;
				} catch (FileException &d) {
					cout << d.getMessage() << endl; 
					cout << "Still Player " << i+1 << "'s Move:" << endl; 
					valid = false; 
				}
				//THIS checks only is the scope of the first move 

				if(valid && pmove->isWord() && first_move) {
					bool contains_s = true; 
					PlaceMove* cast = dynamic_cast<PlaceMove*>(pmove); 
					if(cast->getHorizantal()) {
						if (cast->get_y() != board.get_sx()) {
							contains_s = false; 
						} 
						if (cast->get_x() < board.get_sy()) {
							size_t end = cast->get_x()+cast->get_string().length()-1; 
							cout << end << endl; 
							if(end < board.get_sy()) {
								contains_s = false; 
							}
						}
					} else {
						if (cast->get_x() != board.get_sy()) {
							contains_s = false; 
						} 
						if (cast->get_y() < board.get_sx()) {
							size_t end = cast->get_y()+cast->get_string().length()-1; 
							cout << end << endl; 
							if(end < board.get_sx()) {
								contains_s = false; 
							}
						}
					}
					//(movestr.at(8) - '0') != (int)board.get_sx() && (movestr.at(10) - '0') !=(int)board.get_sy()
					if(!contains_s) {
						cout << "error: your 1st move must start at: " << board.get_sx() << " " << board.get_sy() << endl; 
						cout << "Still Player " << i+1 << "'s Move:" << endl; 					
						bad_move = true; 
					} else { 
						try { 
							pmove->execute(board, bag, diction); 
							if(bag.tilesRemaining() <= 0 || players[i]->getHandTiles().empty()) {//only time bag will empty is after a place move
								game_over = true; 
								break;
							}
							bad_move = false; 
							first_move = false; 
						} catch(FileException &d) {
							bad_move = true;  
							cout << d.getMessage() << endl; 
							cout << "Still Player " << i+1 << "'s Move:" << endl; 
							//catch from most specific to most general 
						} 
					}
				} else if(valid){//check validity of the move for everymove after first move 
					try { 
						pmove->execute(board, bag, diction); 
						if(bag.tilesRemaining() <= 0 || players[i]->getHandTiles().empty()) {//only time bag will empty is after a place move
							game_over = true; 
							break;
						}
						bad_move = false; 
					} catch(FileException &d) {
						bad_move = true;  
						cout << d.getMessage() << endl; 
						cout << "Still Player " << i+1 << "'s Move:" << endl; 
						//catch from most specific to most general 
					} 
				}
			}
			if(game_over) {//intermediate break to break of for loop 
				break; 
			}
			//DISPLAYS RESULT OF THE MOVE 
			if (valid && pmove->isPass()) {
				pass_count++; 
				if(pass_count >= numPlayer) {
					game_over = true; 
					break; 
				}
			} else if( valid && pmove->isExchange()) {
				printer.printHand(*players[i]); 

			} else if(valid && pmove->isWord()) {
				printer.printBoard(board); 
			}
			delete pmove; 
			cout << "Your move was " << players[i]->getPoints()-prev_pts << " pts!" << endl; 
			cout << "You now have " << players[i]->getPoints() << " total pts!" << endl; 
			prev_pts = players[i]->getPoints(); 
			//ONLY MOVES TO NEXT TURN AFTER ENTER IS PRESSED 
			cout << "press ENTER to move on" << endl; 
			cin.ignore(256,'\n'); 

		}

		if(game_over) {//break out of while loop 
			break; 
		}

	}
	Player * potential_winner = nullptr; 
	vector<int> remaining_hand_sum; 
	//tallys up points for tiles remaining in each players hand 
	for(int i = 0; i < numPlayer; i++) {
		set<Tile*> remain = players[i]->getHandTiles(); 
		int sum_of_remaining_tiles = 0; 
		for(std::set<Tile*>::iterator sit = remain.begin(); sit != remain.end(); ++sit) {
			int tile_pt = (*sit)->getPoints(); 
			sum_of_remaining_tiles+=tile_pt; 
		}
		if(sum_of_remaining_tiles == 0) {//whoever has the emptyhand is prob the winner 
			potential_winner = players[i]; 
		} else {
			remaining_hand_sum.push_back(sum_of_remaining_tiles); 
		}
		players[i]->setPoints(players[i]->getPoints()-sum_of_remaining_tiles); 
	}
	//only runs when the game ended by a player running out of tiles
	if(potential_winner != nullptr) {
		for(size_t i = 0; i < remaining_hand_sum.size(); i++) {
			int pile = potential_winner->getPoints() + remaining_hand_sum[i];
			potential_winner->setPoints(pile); 
		}
	}
	//prints points after deduction/addition from leftover tiles in each player's hand 
	Player * winner = players[0]; 
	for(size_t i = 0; i < players.size(); i++) {
		cout << players[i]->getName() << "'s pts: " << players[i]->getPoints() << endl;
		if(players[i]->getPoints() > winner->getPoints()) {
			winner = players[i]; 
		} 
	}

	vector<Player*> winner_list; 
	winner_list.push_back(winner);

	for(size_t i = 0; i < players.size(); i++) {
		if(players[i]->getPoints() == winner->getPoints() && winner->getName() != players[i]->getName()) {
			winner_list.push_back(players[i]); 
		}
	}
	cout << "Winners: "; 
	for(size_t i = 0; i < winner_list.size(); i++) {
		cout << winner_list[i]->getName() << " "; 
	}
	cout << endl; 
	//deallocate memory 
	for(int i = 0; i < numPlayer; i++) {
			delete players[i];
		}
}