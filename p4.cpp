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
   void addWord(int key, string word, int index, int line); // given a word, index to add to, and line number, add a word to the table
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
   os << "]" << endl;
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
   os << "Printing hash table contents below: " << endl << endl;
   for (int i = 0; i < this->size; i ++) {
      os << "Index " << i << ": ";
      if ((this->currWord + i)->isEmpty()) {
         os << "is empty" << endl;
      } else {
         (this->currWord + i)->print(os);
      }
   }
}

void HashTable::addWord(int key, string word, int index, int line) {
   if (index > this->size - 1) {
      cout << "Failed to add word to table: index out of range" << endl;
      return;
   }

   Word* targetWord = (this->currWord + index);

   // change the text if the word pointer is null
   if (targetWord->isEmpty()) {
      (this->numUniqueWords) ++;
      targetWord->key = key;
      targetWord->text = word;
   }
   
   (this->totalWordsScanned) ++;
   targetWord->addInstance(line);
}

void HashTable::hashWord(string word, int currLineNum) {

   int h1 = djb2(word) % this->size;
   int h2;
   
   if (this->dhParameter > 0) {
      h2 = this->dhParameter - (djb2(word) % this->dhParameter);
   }

   if (this->collisionStrat == "lp") {
      for (int i = 0; i < this->size; i ++) {
         int index = (h1 + i) % size;
         
      }
   } else if (this->collisionStrat == "qp") {

   } else if (this->collisionStrat == "dh") {

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
            cout << currWord << " ";
         }
      } else {
         if (currWord.length() > 0) {
            // add current word to table
            cout << currWord << " ";
         }
         currWord = "";
      }   
   }
   cout << endl;
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

   bool doubleHash; //true if the strategy is double hash

   inputFileName = string(argv[1]);
   queryFileName = string(argv[2]);
   hashTableSize = atoi(argv[3]);
   collisionResStrategy = string(argv[4]);

   if (argc == 6) {
      doubleHash = true;
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

   inputFile.close();
   queryFile.close();

   return 0;
}
