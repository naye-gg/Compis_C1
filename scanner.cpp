#include <iostream>
#include <cctype>
#include <cstring>
#include <fstream>
#include <set>
#include "token.h"
#include "scanner.h"

using namespace std;

// -----------------------------
// Constructor
// -----------------------------
Scanner::Scanner(const char* s): input(s), first(0), current(0) { }

// -----------------------------
// Funciones auxiliares
// -----------------------------

bool is_white_space(char c) {
    return c == ' ' || c == '\n' || c == '\r' || c == '\t';
}

// Funciones reconocidas por el lenguaje
static bool es_funcion(const string& lexema) {
    static const set<string> funciones = {"sqrt", "max", "min", "abs"};
    return funciones.count(lexema) > 0;
}

// isdigit/isalnum son UB con char negativo: se castea a unsigned char
static bool es_digito(char c) { return isdigit(static_cast<unsigned char>(c)); }
static bool es_letra(char c)  { return isalpha(static_cast<unsigned char>(c)); }
static bool es_alfanum(char c){ return isalnum(static_cast<unsigned char>(c)); }

// -----------------------------
// nextToken: obtiene el siguiente token
// -----------------------------

Token* Scanner::nextToken() {
    Token* token;

    // Saltar espacios en blanco
    while (current < input.length() && is_white_space(input[current]))
        current++;

    // Fin de la entrada
    if (current >= input.length())
        return new Token(Token::END);

    char c = input[current];

    first = current;

    // Números: entero o decimal (1, 4.5, 10.25)
    if (es_digito(c)) {
        current++;
        while (current < input.length() && es_digito(input[current]))
            current++;
        // Parte decimal: sólo si hay al menos un dígito tras el punto
        if (current + 1 < input.length() && input[current] == '.' && es_digito(input[current + 1])) {
            current++;
            while (current < input.length() && es_digito(input[current]))
                current++;
        }
        token = new Token(Token::NUM, input, first, current - first);
    }
    // Identificadores y funciones
    else if (es_letra(c)) {
        current++;
        while (current < input.length() && es_alfanum(input[current]))
            current++;
        string lexema = input.substr(first, current - first);
        token = new Token(es_funcion(lexema) ? Token::FUNC : Token::ID, input, first, current - first);
    }
    // Operadores y separadores
    else if (strchr("+-*/(),", c)) {
        switch (c) {
            case '+': token = new Token(Token::PLUS,  c); break;
            case '-': token = new Token(Token::MINUS, c); break;
            case '*':
                // '**' es potencia, '*' es multiplicación
                if (input[current + 1] == '*') {
                    current++;
                    token = new Token(Token::POW, input, first, current + 1 - first);
                } else {
                    token = new Token(Token::MUL, c);
                }
                break;
            case '/': token = new Token(Token::DIV,    c); break;
            case '(': token = new Token(Token::LPAREN, c); break;
            case ')': token = new Token(Token::RPAREN, c); break;
            case ',': token = new Token(Token::COMMA,  c); break;
        }
        current++;
    }
    // Carácter inválido
    else {
        token = new Token(Token::ERR, c);
        current++;
    }

    return token;
}

// -----------------------------
// Destructor
// -----------------------------
Scanner::~Scanner() { }

// -----------------------------
// Función de prueba
// -----------------------------

void ejecutar_scanner(Scanner* scanner, const string& InputFile) {
    Token* tok;

    // Crear nombre para archivo de salida
    string OutputFileName = InputFile;
    size_t pos = OutputFileName.find_last_of(".");
    if (pos != string::npos) {
        OutputFileName = OutputFileName.substr(0, pos);
    }
    OutputFileName += "_tokens.txt";

    ofstream outFile(OutputFileName);
    if (!outFile.is_open()) {
        cerr << "Error: no se pudo abrir el archivo " << OutputFileName << endl;
        return;
    }

    outFile << "Scanner\n" << endl;

    while (true) {
        tok = scanner->nextToken();

        if (tok->type == Token::END) {
            outFile << *tok << endl;
            delete tok;
            outFile << "\nScanner exitoso" << endl << endl;
            outFile.close();
            return;
        }

        if (tok->type == Token::ERR) {
            outFile << *tok << endl;
            delete tok;
            outFile << "Caracter invalido" << endl << endl;
            outFile << "Scanner no exitoso" << endl << endl;
            outFile.close();
            return;
        }

        outFile << *tok << endl;
        delete tok;
    }
}
