#include "Trie.h"
#include <iostream>
#include <string>
 
 using namespace std; 

  TrieSet::TrieSet() {
    head = nullptr; 
  } // a constructor for an empty trie

  TrieSet::~TrieSet() {
    helper(head); 
  } // destructor

  void TrieSet::helper (TrieNode* N) {
    if(N == nullptr) return; 
    for(int i = 0; i < 26; i++) {
      helper(N->children[i]); 
    }
    delete N; 
  }
  void TrieSet::insert (std::string input) {
    if(head == nullptr) {
        head = new TrieNode();
        head->init(); 
        head->parent = nullptr; 
    } else {
      TrieNode* cur = head; 
      for(unsigned int i = 0; i < input.length(); i++) {
        int index = input.at(i) - 'a'; 
        if(cur->children[index] == nullptr) {
          cur->children[index] = new TrieNode(); 
          cur->children[index]->init(); 
          cur->children[index]->parent = cur; 
          cur->children[index]->c = index + 97; 
        }
        cur = cur->children[index]; 
      }
      cur->inSet = true; 
    }
   /* Add this string to the set.
      Do nothing if the string is already in the set. */
  }

void TrieSet::remove (std::string input) {
  bool leaf = true; 
  if(prefix(input) == nullptr) {
    //do nothing 
    } else {
      TrieNode* found = prefix(input); 
      for(int i = 0; i < 26; i++) {
        if(found->children[i] != nullptr) {
          leaf = false; 
        }
      }
      if(leaf) {
        delete found; 
      }
    }

   /* Removes this string from the set.
      Do nothing if the string is not already in the set. */
}

  TrieNode* TrieSet::prefix(std::string px) {
    TrieNode* cur = head; 

    for(unsigned int i = 0; i < px.length(); i++) {
      int index = px.at(i) - 'a'; 
      if(cur->children[index] == nullptr) {
        return nullptr;
      }
      cur = cur->children[index]; 
    }

   /* returns the TrieNode matching the provided string, or returns NULL 
      if it is not the prefix of a word in the Trie. */ 
      return cur;   
  }
