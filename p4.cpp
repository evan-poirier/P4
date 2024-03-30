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

class Word {
public:
   string text; // what the word itself actually is
   vector<int> instances; // a vector where each int represents the line that the word appears on

   Word(); // default constructor
   Word(string inputText); // constructor that will be used in program
};

class HashTable {
private:
   int size;
   Word* table;

public:
   
};

int main() {

   return 0;
}
