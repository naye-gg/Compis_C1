#include <iostream>
#include <fstream>
#include <string>
#include "scanner.h"
#include "parser.h"
#include "ast.h"

using namespace std;

// Reemplaza la extensión del archivo de entrada por el sufijo dado.
static string nombreSalida(const string& entrada, const string& sufijo) {
    size_t pos = entrada.find_last_of('.');
    return (pos == string::npos ? entrada : entrada.substr(0, pos)) + sufijo;
}

int main(int argc, const char* argv[]) {
    // Verificar número de argumentos
    if (argc != 2) {
        cout << "Número incorrecto de argumentos.\n";
        cout << "Uso: " << argv[0] << " <archivo_de_entrada>" << endl;
        return 1;
    }

    // Abrir archivo de entrada
    ifstream infile(argv[1]);
    if (!infile.is_open()) {
        cout << "No se pudo abrir el archivo: " << argv[1] << endl;
        return 1;
    }

    // Leer contenido completo del archivo en un string
    string input, line;
    while (getline(infile, line)) {
        input += line + '\n';
    }
    infile.close();

    // Un escáner para el volcado de tokens y otro para el parser
    Scanner scanner1(input.c_str());
    Scanner scanner2(input.c_str());

    // Tokens
    ejecutar_scanner(&scanner1, argv[1]);

    // Parsear y generar AST
    string dotFile = nombreSalida(argv[1], "_ast.dot");
    ofstream out(dotFile);
    out << "digraph AST {\n";
    int estado = 0;
    try {
        Parser parser(&scanner2);
        Exp* ast = parser.parseProgram();
        int id = 0;
        ast->toDot(out, id);
        delete ast;
    } catch (const exception& e) {
        cout << e.what() << endl;
        out << "    empty [label=\"AST vacio\"];\n";
        estado = 1;
    }
    out << "}\n";
    out.close();
    return estado;
}
