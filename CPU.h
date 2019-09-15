#include "Player.h"
#include "Square.h"
#include "Board.h"
#include "Bag.h"
#include "Dictionary.h"
#include "Trie.h" 
#include <iostream> 

class CPU: public Player {
	public:
		void initialize(std::string file); 
		void anchor_square(Board& board, Bag& bag, Dictionary& dic); 
		void cross_checking(); 
		void find_valid_move(); 
		void left_part(std::string partial, TrieNode* node, int limit); 
		void right_part(std::string partial, TrieNode* node, Square* sqr); 
	protected:
		TrieSet prefix;
		TrieSet suffix; 
		std::vector<Square*> anchors;
		std::vector<std::string> list; 
};

