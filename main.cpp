#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#define ALPHABET_SIZE 26

using namespace std;

struct Trie {
    // Next character
    Trie* nextC[ALPHABET_SIZE];
    // a flag determine that this is an end of a word (TRUE)
    // default: false
    bool endWord;
};

/// function:       newNode
/// return type:            Trie*
/// create a new empty TRIE node
Trie* newNode() {
    Trie* t = new Trie;
    // Fill nextC to NULL
    for (char i = 0; i < 26; ++i)
        t->nextC[i] = NULL;
    // default endWord = false
    t->endWord = false;
    return t;
}

/// procedure:      put
/// params:         root    Trie*
///                 key     string
/// INSERT a /key/ (STRING) to TRIE 
void put(Trie* root, string key) {
    // Set an iterator (pointer)
    Trie* curr = root;
    // Each character in key (in sequence)
    for (int i = 0; i < key.length(); ++i) {
        int index = key[i] - 'a';
        // There is no word which has prefix characters like it anymore, so create new TRIE node
        if (!curr->nextC[index])
            curr->nextC[index] = newNode();
        // Travels to the created/next character's node
        curr = curr->nextC[index];
    }
    // Set endWord to true
    curr->endWord = true;
}

/// function:       createTrie
/// params:         dict    ifstream
/// return type:    root    Trie*
/// create a TRIE from a given dictionary
/// required format: each line in dictionary contain only 1 word and all characters must be lowercase
Trie* createTrie(ifstream& dict) {
    // create an empty TRIE (has only one empty root node) 
    Trie* root = newNode();
    string line;
    // read each line of the dictionary
    while (getline(dict, line))
        // insert read word (line) to TRIE
        put(root, line);
    // return the root node
    return root;
}

/// procedure:      search
/// params:         v       vector<string>*
///                 root    Trie*
///                 free    bool*
///                 letters string
///                 str     string
/// search in the TRIE (valid words, ofc) the words which generated by characters of /letters/ (at least 3 characters)
/// this procedure is implemented using RECURSIVE (to generate permutations of characters)
void search(vector<string>& ans, Trie* root, bool* free, string letters, string str) {
    // got a valid word has over 3 characters -> push to vector ans
    if (str.length() >= 3 && root->endWord)
        ans.push_back(str);
    // each character in /letters/
    for (int i = 0; i < letters.length(); ++i) {
        // this member (not character) is not used AND it is FOUND in nextC
        if (free[i] && root->nextC[letters[i] - 'a']) {
            // SET free and str (temporary word)
            free[i] = false;
            str += letters[i];
            // search in the next node 
            search(ans, root->nextC[letters[i] - 'a'], free, letters, str);
            // Undo the SET action above (to generate permutations)
            free[i] = true;
            str.erase(str.length() - 1, 1);
        }
    }
}

/// procedure:      wordGenerator
/// params:         fout    ofstream
///                 root    Trie*
///                 letters string
/// this procedure initiate/delete required resources for procedure search (mainly to generate permutaion)
void wordGenerator(ofstream& fout, Trie* root, string letters) {
    vector<string> ans;

    // create a free array (TRUE - this member can be add to permutation)
    bool* free = new bool[letters.length()];
    for (int i = 0; i < letters.length(); ++i)
        free[i] = true;
    
    // call search 
    search(ans, root, free, letters, "");

    // print answer
    fout << ans.size() << endl;
    for (int i = 0; i < ans.size(); ++i)
        fout << ans[i] << endl;

    // delete allocated resources
    delete[] free;
    free = NULL;
}

/// procedure       rmSpaces
/// params:         str     string&
/// this procedure delete all spaces in string! nte
void rmSpaces(string& str) {
    for (int i = 0; i < str.length(); ++i)
        if (isblank(str[i]))
            str.erase(i, 1);
}

int main() {
    ifstream dict("Dic.txt");
    Trie* root = createTrie(dict);
    dict.close();

    ifstream fin("input.txt");
    ofstream fout("output.txt");
    string letters;

    while (getline(fin, letters)) {
        rmSpaces(letters);
        wordGenerator(fout, root, letters);
    }

    fin.close();
    fout.close();
    return 0;
}