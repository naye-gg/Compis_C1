# Analizador léxico y sintáctico de expresiones

Compiladores — Continua 1. Lee una expresión aritmética desde un archivo,
genera la lista de tokens y construye el árbol de sintaxis abstracta (AST).

**Integrantes:** Nayeli Guerrero y David Quispe

## Gramática

```
P    -> CE
CE   -> E (('+' | '-') E)*
E    -> T (('*' | '/') T)*
T    -> F ('**' T)?                        // '**' asocia a la derecha
F    -> NUM | ID | '(' CE ')' | FUNC '(' ARGS ')'
ARGS -> CE (',' CE)*                       // lista de expresiones
```

Los argumentos de una función son una **lista de expresiones**, por eso un
único nodo `FuncExp` sirve tanto para `sqrt`/`abs` (1 argumento) como para
`max`/`min` (2 o más).

- Números enteros y decimales: `2`, `50.5`
- Identificadores: `x1`, `x2`
- Funciones: `sqrt`, `abs`, `max`, `min`

## Uso

Compilar y correr los 10 casos de prueba:

```bash
python3 run_all_inputs.py
```

Un archivo suelto:

```bash
g++ -std=c++17 -o a.out main.cpp scanner.cpp token.cpp parser.cpp ast.cpp
./a.out inputs/input5.txt
```

Cada corrida escribe dos archivos junto al input:

| Archivo | Contenido |
|---|---|
| `inputs/inputN_tokens.txt` | tokens producidos por el scanner |
| `inputs/inputN_ast.dot` | AST en formato DOT |

El `.dot` se convierte a imagen con [graphviz](https://graphviz.org)
(`sudo dnf install graphviz`), que `run_all_inputs.py` invoca solo si está
instalado:

```bash
dot -Tpng inputs/input5_ast.dot -o inputs/input5_ast.png
```

## Pruebas

```bash
python3 test_parser.py
```

Verifica los 10 inputs, 5 expresiones válidas adicionales y 6 inválidas
(errores léxicos, sintácticos y de aridad como `sqrt(2, 3)` o `max(2)`).

## Documentación

El informe de desarrollo está en
[`documentacion/informe.pdf`](documentacion/informe.pdf). Explica en qué orden se
modificó cada archivo y por qué, los errores encontrados durante el proceso y las
pruebas realizadas.

## Archivos

| Archivo | Rol |
|---|---|
| `token.h/.cpp` | tipos de token y su impresión |
| `scanner.h/.cpp` | análisis léxico |
| `parser.h/.cpp` | análisis sintáctico y construcción del AST |
| `ast.h/.cpp` | nodos del AST y exportación a DOT |
| `main.cpp` | lee el archivo y coordina scanner y parser |
