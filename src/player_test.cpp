#include <iostream> 
#include "Player.h"
#include "Tile.h"
#include <vector>
using namespace std; 

int main() { 
	Tile t1('b',1);
	Tile t2('b',2);
	Tile t3('?',3);
	Tile t4('c',4);	

	std::vector<Tile*> test; 
	test.push_back(&t1); 
	test.push_back(&t2); 
	test.push_back(&t3); 
	test.push_back(&t4); 

	string name = "chad"; 
	size_t max = 4; 

	Player player1(name, max); 

	player1.addTiles(test);

	string tiles = "?abb";
	bool blank = true; 

	bool result = player1.hasTiles(tiles, blank); 
	cout << result << endl; 

	vector<Tile*> take_test = player1.takeTiles(tiles, blank);

	cout << "vector size: " << take_test.size() << endl;  

	for(unsigned int i = 0; i < take_test.size(); i++) {
		cout << take_test[i]->getLetter() << endl; 
	} 
}