#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>
#include <cctype>
#include <cmath>
#include <vector>
#include <list>

using namespace std;

// HASH FUNCTION
// Store the return of this function as an unsigned long!
unsigned long djb2(string str) {
   const char *ptr = str.c_str();
   unsigned long hash = 5381;
   int c;
   while ((c = *ptr++)) {
      hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
   }
   return hash;
}


// *************
// CLASS HEADERS
// *************

class Word {
public:
   int key; // key value stored
   string text; // what the word itself actually is
   vector<int> instances; // a vector where each int represents the line that the word appears on
   int instancesSize; // keeps track of the size of the instances vector without having to call instances.size()

   Word(); // default constructor
   Word(string inputText, int inputLineNum); // constructor that will be used in program
   void print(ostream &os); // prints word data
   void addInstance(int lineNum); // adds an instance to the instances vector
   bool isEmpty();
};

class HashTable {
private:
   // command line argument values
   int size; // size of the table
   string collisionStrat; // strategy to resolve collision while inserting or searching for a word
   int dhParameter; // if the strategy is double hashing, the parameter for the second hash offset

   // other info
   int totalWordsScanned; // the total number of words
   int numUniqueWords; // the number of unique words in the table
   int numCollisions; // the number of collisions generated while creating the table

   // pointer to the first word in the array of words declared upon construction
   Word* currWord;

public:
   // make private later
   int addWord(int key, string word, int index, int line); // given a word, index to add to, and line number, add a word to the table
   // addWord returns 1 if it added a new word or incremented instances, and 0 if spot was full
   void hashWord(string word, int currLineNum);
   
   void insertLine(string line, int currLineNum);
   void print(ostream &os);

   HashTable();
   HashTable(int size, string collisionStrat);
   HashTable(int size, string collisionStrat, int dhParameter);
};

// ***************************
// CLASS FUNCTION DECLARATIONS
// ***************************

Word::Word() {
   this->text = "Empty object";
   this->instancesSize = 0;
}

Word::Word(string inputText, int inputLineNum) {
   this->text = inputText;
   this->instances.push_back(inputLineNum);
   this->instancesSize = 1;
}

void Word::print(ostream &os) {
   os << this->text << " appears on lines [";
   for (int i = 0; i < this->instancesSize; i ++) {
      if (i < instancesSize - 1) {
         os << this->instances.at(i) << ",";
      } else {
         os << this->instances.at(i);
      }
   }
   os << "]";
}

void Word::addInstance(int lineNum) {
   this->instances.push_back(lineNum);
   (this->instancesSize) ++;
}

bool Word::isEmpty() {
   if (this->instancesSize == 0) {
      return true;
   }
   
   if (this->instancesSize < 0) {
      cout << "Something is very wrong." << endl;
   }

   return false;
}


HashTable::HashTable() {
   this->size = 0;
   this->collisionStrat = "lp";
   this->dhParameter = 0;
   
   this->totalWordsScanned = 0;
   this->numUniqueWords = 0;
   this->numCollisions = 0;

   // need to figure out how to initialize this
   Word *array = new Word[size];
   this->currWord = array;
}

HashTable::HashTable(int size, string collisionStrat) {
   this->size = size;
   this->collisionStrat = collisionStrat;
   this->dhParameter = 0;
   
   this->totalWordsScanned = 0;
   this->numUniqueWords = 0;
   this->numCollisions = 0;

   // need to figure out how to initialize this
   Word *array = new Word[size];
   this->currWord = array;
}

HashTable::HashTable(int size, string collisionStrat, int dhParameter) {
   this->size = size;
   this->collisionStrat = collisionStrat;
   this->dhParameter = dhParameter;
   
   this->totalWordsScanned = 0;
   this->numUniqueWords = 0;
   this->numCollisions = 0;

   // need to figure out how to initialize this
   Word *array = new Word[size];
   this->currWord = array;
}

// works with uninitialized table, test with added contents
void HashTable::print(ostream &os) {
   os << "The number of words found in the file was " << this->totalWordsScanned << endl;
   os << "The number of unique words found in the file was " << this->numUniqueWords << endl;
   os << "The number of collisions was " << this->numCollisions << endl << endl;

   /* print every index in the table (not used for project)
   for (int i = 0; i < this->size; i ++) {
      os << i << ": ";
      if (!(this->currWord + i)->isEmpty()) {
         (this->currWord + i)->print(os);
      }
      os << endl;
   }
   */
}

int HashTable::addWord(int key, string word, int index, int line) {
   if (index > this->size - 1) {
      cout << "Failed to add word to table: index out of range" << endl;
      return 0;
   }

   Word* targetWord = (this->currWord + index);

   // if inserting into empty cell, just insert and return 1
   if (targetWord->isEmpty()) {
      targetWord->key = key;
      targetWord->text = word;
      targetWord->addInstance(line);
      (this->numUniqueWords) ++;
      return 1;
   } else {
      // if the word matches the cell, increment instances, else return 0
      if (targetWord->text == word) {
         targetWord->addInstance(line);
         return 1;
      } else {
         return 0;
      }
   }
}

void HashTable::hashWord(string word, int currLineNum) {
   (this->totalWordsScanned) ++;

   int h1 = djb2(word) % this->size;
   int h2;
   
   if (this->dhParameter > 0) {
      h2 = this->dhParameter - (djb2(word) % this->dhParameter);
   }

   if (this->collisionStrat == "lp") {
      for (int i = 0; i < this->size; i ++) {
         int index = (h1 + i) % size;
         if (this->addWord(h1, word, index, currLineNum)) {
            break;
         }
         (this->numCollisions) ++;
      }
   } else if (this->collisionStrat == "qp") {
      for (int i = 0; i < this->size; i ++) {
         int index = (h1 + i*i) % size;
         if (this->addWord(h1, word, index, currLineNum)) {
            break;
         }
         (this->numCollisions) ++;
      }
   } else if (this->collisionStrat == "dh") {
      for (int i = 0; i < this->size; i ++) {
         int index = (h1 + (i*h2)) % size;
         if (this->addWord(h1, word, index, currLineNum)) {
            break;
         }
         (this->numCollisions) ++;
      }
   } else {
      cout << "Invalid collsion strategy" << endl;
   }
}

void HashTable::insertLine(string line, int currLineNum) {
   int length = line.length();

   char c;
   string currWord;

   for (int i = 0; i < length; i ++) {
      c = line.at(i);
      c = tolower(c);

      if (c <= 'z' && c >= 'a') {
         currWord.append(string(1, c));

         if ((i == length - 1) && (currWord.length() > 0)) {
            // add current word to table
            this->hashWord(currWord, currLineNum);
         }
      } else {
         if (currWord.length() > 0) {
            // add current word to table
            this->hashWord(currWord, currLineNum);
         }
         currWord = "";
      }   
   }
}


// ****
// MAIN
// ****

int main(int argc, char* argv[]) {
   string inputFileName;
   string queryFileName;
   int hashTableSize;
   string collisionResStrategy;
   int doubleHashParameter = 0;

   inputFileName = string(argv[1]);
   queryFileName = string(argv[2]);
   hashTableSize = atoi(argv[3]);
   collisionResStrategy = string(argv[4]);

   if (argc == 6) {
      doubleHashParameter = atoi(argv[5]);
   }

   ifstream inputFile;
   ifstream queryFile;

   inputFile.open(inputFileName);
   queryFile.open(queryFileName);

   // the table that will be used in main for this project
   HashTable *table = new HashTable(hashTableSize, collisionResStrategy, doubleHashParameter);

   int lineNum = 1;
   string line;

   while (getline(inputFile, line)) {

      table->insertLine(line, lineNum);
      
      lineNum ++;
   }

   table->print(cout);

   inputFile.close();
   queryFile.close();

   return 0;
}
