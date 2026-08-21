#ifndef PARSER_H
#define PARSER_H

#include <list>
#include <string>
#include "scanner.h"    // Incluye la definición del escáner (provee tokens al parser)
#include "ast.h"        // Incluye las definiciones para construir el AST

// Gramática reconocida:
//   P    -> CE
//   CE   -> E (('+' | '-') E)*
//   E    -> T (('*' | '/') T)*
//   T    -> F ('**' T)?              // '**' asocia a la derecha
//   F    -> NUM | ID | '(' CE ')' | FUNC '(' ARGS ')'
//   ARGS -> CE (',' CE)*             // una función recibe una LISTA de expresiones
class Parser {
private:
    Scanner* scanner;          // Puntero al escáner, de donde se leen los tokens
    Token *current, *previous; // Punteros al token actual y al anterior
    bool match(Token::Type ttype);   // Verifica si el token actual coincide con un tipo esperado y avanza si es así
    bool check(Token::Type ttype);   // Comprueba si el token actual es de cierto tipo, sin avanzar
    bool advance();                  // Avanza al siguiente token
    bool isAtEnd();                  // Comprueba si ya se llegó al final de la entrada
    void expect(Token::Type ttype, const string& esperado); // Consume un token obligatorio o lanza error
public:
    Parser(Scanner* scanner);
    ~Parser();
    Exp* parseProgram();    // Punto de entrada: analiza un programa completo
    Exp* parseP();
    Exp* parseCE();
    Exp* parseE();
    Exp* parseT();
    Exp* parseF();
    list<Exp*> parseArgs(); // Lista de expresiones separadas por comas
};
#endif // PARSER_H
