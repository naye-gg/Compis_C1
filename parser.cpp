#include <iostream>
#include <stdexcept>
#include "token.h"
#include "scanner.h"
#include "ast.h"
#include "parser.h"

using namespace std;

// =============================
// Aridad esperada por función
// =============================

// Devuelve true si 'n' argumentos son válidos para la función 'name'.
static bool aridad_valida(const string& name, size_t n) {
    if (name == "sqrt" || name == "abs") return n == 1;
    if (name == "max"  || name == "min") return n >= 2;
    return false;
}

static string aridad_esperada(const string& name) {
    if (name == "sqrt" || name == "abs") return "exactamente 1 argumento";
    return "al menos 2 argumentos";
}

// Describe el token actual en los mensajes de error
static string describir(const Token* tok) {
    return tok->type == Token::END ? "fin de entrada" : "'" + tok->text + "'";
}

// =============================
// Métodos de la clase Parser
// =============================

Parser::Parser(Scanner* sc) : scanner(sc), current(nullptr), previous(nullptr) {
    current = scanner->nextToken();
    if (current->type == Token::ERR) {
        throw runtime_error("Error lexico: caracter invalido '" + current->text + "'");
    }
}

Parser::~Parser() {
    delete current;
    delete previous;
}

bool Parser::match(Token::Type ttype) {
    if (check(ttype)) {
        advance();
        return true;
    }
    return false;
}

bool Parser::check(Token::Type ttype) {
    if (isAtEnd()) return false;
    return current->type == ttype;
}

bool Parser::advance() {
    if (!isAtEnd()) {
        Token* temp = current;
        if (previous) delete previous;
        current = scanner->nextToken();
        previous = temp;

        if (current->type == Token::ERR) {
            throw runtime_error("Error lexico: caracter invalido '" + current->text + "'");
        }
        return true;
    }
    return false;
}

bool Parser::isAtEnd() {
    return (current->type == Token::END);
}

void Parser::expect(Token::Type ttype, const string& esperado) {
    if (!match(ttype)) {
        throw runtime_error("Error sintactico: se esperaba " + esperado +
                            " y se encontro " + describir(current));
    }
}

// =============================
// Reglas gramaticales
// =============================

Exp* Parser::parseProgram() {
    Exp* ast = parseP();
    if (!isAtEnd()) {
        delete ast;
        throw runtime_error("Error sintactico: token inesperado " + describir(current));
    }
    cout << "Parseo exitoso" << endl;
    return ast;
}

// P -> CE
Exp* Parser::parseP() {
    return parseCE();
}

// CE -> E (('+' | '-') E)*
Exp* Parser::parseCE() {
    Exp* l = parseE();
    while (match(Token::PLUS) || match(Token::MINUS)) {
        BinaryOp op = (previous->type == Token::PLUS) ? PLUS_OP : MINUS_OP;
        Exp* r = parseE();
        l = new BinaryExp(l, r, op);
    }
    return l;
}

// E -> T (('*' | '/') T)*
Exp* Parser::parseE() {
    Exp* l = parseT();
    while (match(Token::MUL) || match(Token::DIV)) {
        BinaryOp op = (previous->type == Token::MUL) ? MUL_OP : DIV_OP;
        Exp* r = parseT();
        l = new BinaryExp(l, r, op);
    }
    return l;
}

// T -> F ('**' T)?   → la potencia asocia a la derecha: 2**3**2 == 2**(3**2)
Exp* Parser::parseT() {
    Exp* l = parseF();
    if (match(Token::POW)) {
        Exp* r = parseT();
        l = new BinaryExp(l, r, POW_OP);
    }
    return l;
}

// ARGS -> CE (',' CE)*
list<Exp*> Parser::parseArgs() {
    list<Exp*> args;
    args.push_back(parseCE());
    while (match(Token::COMMA)) {
        args.push_back(parseCE());
    }
    return args;
}

// F -> NUM | ID | '(' CE ')' | FUNC '(' ARGS ')'
Exp* Parser::parseF() {
    if (match(Token::NUM)) {
        return new NumberExp(stod(previous->text));
    }
    else if (match(Token::ID)) {
        return new IdExp(previous->text);
    }
    else if (match(Token::LPAREN)) {
        Exp* e = parseCE();
        expect(Token::RPAREN, "')'");
        return e;
    }
    else if (match(Token::FUNC)) {
        string name = previous->text;   // 'previous' cambia en el siguiente match
        expect(Token::LPAREN, "'(' despues de " + name);
        FuncExp* f = new FuncExp(name, parseArgs());
        expect(Token::RPAREN, "')' para cerrar " + name);
        if (!aridad_valida(name, f->args.size())) {
            size_t n = f->args.size();
            delete f;
            throw runtime_error("Error semantico: " + name + " espera " +
                                aridad_esperada(name) + " y recibio " + to_string(n));
        }
        return f;
    }
    else {
        throw runtime_error("Error sintactico: se esperaba una expresion y se encontro " +
                            describir(current));
    }
}
