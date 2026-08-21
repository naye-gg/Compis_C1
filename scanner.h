#ifndef SCANNER_H
#define SCANNER_H

#include <cstddef>
#include <string>
#include "token.h"
using namespace std;

class Scanner {
private:
    string input;
    size_t first;
    size_t current;

public:
    // Constructor
    Scanner(const char* in_s);

    // Retorna el siguiente token
    Token* nextToken();

    // Destructor
    ~Scanner();

};

// Ejecutar scanner
void ejecutar_scanner(Scanner* scanner,const string& InputFile);

#endif // SCANNER_H