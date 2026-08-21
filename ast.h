#ifndef AST_H
#define AST_H

#include <string>
#include <list>
#include <ostream>

using namespace std;


// Operadores binarios soportados
enum BinaryOp {
    PLUS_OP,
    MINUS_OP,
    MUL_OP,
    DIV_OP,
    POW_OP
};

// Clase abstracta Exp
class Exp {
public:
    virtual ~Exp() = 0;  // Destructor puro → clase abstracta
    static string binopToChar(BinaryOp op);  // Conversión operador → string
    virtual void toDot(std::ostream& out, int& id) const = 0;  // Visualización en DOT
};

// Expresión binaria
class BinaryExp : public Exp {
public:
    Exp* left;
    Exp* right;
    BinaryOp op;
    BinaryExp(Exp* l, Exp* r, BinaryOp op);
    ~BinaryExp();
    void toDot(std::ostream& out, int& id) const override;
};

// Expresión numérica (entera o decimal)
class NumberExp : public Exp {
public:
    double value;
    NumberExp(double v);
    ~NumberExp();
    void toDot(std::ostream& out, int& id) const override;
};

// Identificador
class IdExp : public Exp {
public:
    string value;
    IdExp(string v);
    ~IdExp();
    void toDot(std::ostream& out, int& id) const override;
};

// Llamada a función: sqrt, abs, max, min.
// Sus argumentos son una LISTA de expresiones, por eso cubre tanto
// las funciones de un argumento como las de varios.
class FuncExp : public Exp {
public:
    string name;
    list<Exp*> args;
    FuncExp(const string& name, const list<Exp*>& args);
    ~FuncExp();
    void toDot(std::ostream& out, int& id) const override;
};

#endif // AST_H
