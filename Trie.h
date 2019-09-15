#include <string>
struct TrieNode {
  bool inSet;   // Is this node the end of a string in your dictionary?
  TrieNode* parent = nullptr;
  TrieNode* children[26]; // one child for each letter of the alphabet.
  void init() {
    for (int i = 0; i < 26; ++i)
    {
      children[i] = nullptr;       
    }
  }
  char c = 'a'; 
};

class TrieSet {
 public:

  TrieSet (); // a constructor for an empty trie
  ~TrieSet (); // destructor

  void helper(TrieNode* N); 

  void insert (std::string input);
   /* Add this string to the set.
      Do nothing if the string is already in the set. */

  void remove (std::string input);
   /* Removes this string from the set.
      Do nothing if the string is not already in the set. */

  TrieNode* prefix(std::string px); 
   /* returns the TrieNode matching the provided string, or returns NULL 
      if it is not the prefix of a word in the Trie. */
private:
  TrieNode* head; 
};