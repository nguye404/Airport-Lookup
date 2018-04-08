// C++ program to demonstrate auto-complete feature
// using Trie data structure.
#include <bits/stdc++.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
using namespace std;

#define ALPHABET_SIZE (27)

class CityTrie
{
	private:
    /*Node for trie
     holds latitude and longitude for the particular city
     bool to mark whether the word is at the end*/
		struct TrieNode
		{
			struct TrieNode *children[ALPHABET_SIZE];
			double lat;
			double lon;
			bool isWordEnd;
		};

		TrieNode *root;

	public:
		// Returns new trie node (initialized to NULLs)
		TrieNode* getNode() {
            //initialize new children array and initialize all of the values of the characters to null
			TrieNode *pNode = new TrieNode();
			for (int i = 0; i < ALPHABET_SIZE; i++)
				pNode->children[i] = NULL;
			pNode->lat = 0.0;
			pNode->lon = 0.0;
			pNode->isWordEnd = false;
			return pNode;
		};
    /*constructor to ensure that we read the file once and don't have to reinitialize the tree multiple times
     //when reading the file, parses the file for the state, city, latitude and longitude
     makes a key holding the state and city names, inserts into the trie. */
		CityTrie()
		{
	    root = getNode();

			// To check whether city is duplicates or not
			unordered_set<string> citySet;

			// read file
			ifstream file;
			file.open("places2k.txt");
			
			if (file.is_open())
			{
				string line;
				while (getline(file, line)) {
					string state, city, key, cityTemp;
					double latitude, longitude;
					state = line.substr(0, 2);
					cityTemp = line.substr(9, 63);
					latitude = stod(line.substr(143, 10));
					longitude = stod(line.substr(153, 10));

					// remove spaces after end of city
					int cityTempSize = cityTemp.size();
					int end = 0;
					bool found = false;					
					for (int i = 0; i < cityTempSize; i++)
					{
						if(found && isspace(cityTemp[i])) {
							end--;
							break;
						} else if(isspace(cityTemp[i])) {
							found = true;
							end++;
						} else {
							found = false;
							end++;
						}
					}
					city = cityTemp.substr(0, end);
					
					// convert city and state to all lower case
					transform(state.begin(), state.end(), state.begin(), ::tolower);
					transform(city.begin(), city.end(), city.begin(), ::tolower);

					// insert into prefix tree
					key = state + " " + city;
					if(citySet.find(key)== citySet.end()){
						citySet.insert(key);
						insert(key, latitude, longitude);
					}
				}
			}
			file.close();
		}
    /*If not present, inserts key into trie. If the
     key is prefix of trie node, just marks leaf node
     after the end of the word, we set the latitude and longitude of the values */		void insert(const string key, double latitude, double longitude)
		{
			TrieNode *pCrawl = this->root;
			for (int level = 0; level < key.length(); level++)
			{
				int index = key[level] - 'a';
				if(index < 0) {
					index = 26;
				}
				if(!pCrawl->children[index]) {
					pCrawl->children[index] = getNode();
				}
			 	pCrawl = pCrawl->children[index];
			}
			pCrawl->lat = latitude;
			pCrawl->lon = longitude;
			pCrawl->isWordEnd = true;
		}

    /* Returns true if key presents in trie, else false
     searches through the tree but only works for the whole word */
		bool search(const string key)
		{
			TrieNode *pCrawl = root;
			for (int level = 0; level < key.length(); level++)
			{
				int index = key[level] - 'a';
				if(index < 0){
					index = 26;
				}
				if (!pCrawl->children[index])
					return false;
				pCrawl = pCrawl->children[index];
			}
			return (pCrawl != NULL && pCrawl->isWordEnd);
		}

    /*Returns 0 if current node has a child
     If all children are NULL, return 1.
     checks if there is another child for that particular word*/
		bool isLastNode(TrieNode *node)
		{
			for (int i = 0; i < ALPHABET_SIZE; i++)
				if (root->children[i])
					return 0;
			return 1;
		}
    /*getter function to get the latitude of the valid city
     will not work with an invalid city*/
		double getLatitude(string key) {
			transform(key.begin(), key.end(), key.begin(), ::tolower);
		  TrieNode *pCrawl = this->root;
			key =  autoSuggestions(key);
			int length = key.length();
			for (int level = 0; level < length; level++)
			{
				int index = key[level] - 'a';
				if(index<0 || index>ALPHABET_SIZE){
					index =26;
				}
				pCrawl = pCrawl->children[index];
			}
			return pCrawl->lat;
		}
    /*getter function to get the longitude of the valid city
     will not work with an invalid city */
		double getLongitude(string key) {
			transform(key.begin(), key.end(), key.begin(), ::tolower);
			TrieNode *pCrawl = this->root;
			key =  autoSuggestions(key);
			int length = key.length();
			for (int level = 0; level < length; level++)
			{
				int index = key[level] - 'a';
				if(index <0 || index> ALPHABET_SIZE){
					index = 26;
				}
				pCrawl = pCrawl->children[index];
			}
			return pCrawl->lon;
		}
    /*checks if the city name is invalid or not utilizing the auto complete function
     if the return value is "$" then there is an error with the city (it does not exist
     if there is "@" then the value is too ambiguous*/
		bool isValidCity(string city)
		{
			string test = autoSuggestions(city);
			if (test == "$" || test == "@") {
				return false;
			}
			else {
				return true;
			}
		}
    /*
     auto suggestion to run through and see if the city name provided
     is part of or is the whole word. returns the string of the full city
     name if it is a valid city
     */
		string autoSuggestions(const string query)
		{
			struct TrieNode* pCrawl = root;
			string error = "$";
			string ambig = "@";

			// Check if prefix is present and find the
			// the node (of last level) with last character
			// of given string.
			int level;
			int n = query.length();
			for (level = 0; level < n; level++)
			{
				int index = query[level] - 'a';
				if(index <0 || index> ALPHABET_SIZE ){
					index = 26;
				}
				// no string in the Trie has this prefix
				if (!pCrawl->children[index]) {
					return error;
				}
				pCrawl = pCrawl->children[index];
			}

			// If prefix is present as a word.
			bool isWord = (pCrawl->isWordEnd == true);

			// If prefix is last node of tree (has no
			// children)
			bool isLast = isLastNode(pCrawl);

			// If prefix is present as a word, but
			// there is no subtree below the last
			// matching node.
			if (isWord && isLast)
			{
				return query;
			}

			// If there are are nodes below last
			// matching character.
			if (!isLast)
			{
				string prefix = query;
				vector<string> words;
				suggestionsRec(pCrawl, words, prefix);
				if (words.size() > 1) {
					return ambig;
				}
				else if (words.size() == 1) {
					for (int x = 0; x != words.size(); ++x) {
						return words[x];
					}
				}
			}
		}
    /*
     recursive method to check all the possiblities from the current prefix
     goes through the whole word adds it to vector if it is a valid city
     */
		void suggestionsRec(TrieNode* current, vector<string> &words, string currPrefix)
		{
			// found a string in Trie with the given prefix
			if (current->isWordEnd)
			{
				words.push_back(currPrefix);
			}
			// All children struct node pointers are NULL
			if (isLastNode(current))
				return;
			for (int i = 0; i < ALPHABET_SIZE; i++)
			{
				if (current->children[i])
				{
					// append current character to currPrefix string
					if(i==26) {
						currPrefix.push_back(32);
					} else {
						currPrefix.push_back(97 + i);
					}
					// recur over the rest
					suggestionsRec(current->children[i], words, currPrefix);
				}
			}
		}
};
