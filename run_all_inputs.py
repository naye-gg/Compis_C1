import os
import re
import shutil
import subprocess
import sys

FUENTES = ["main.cpp", "scanner.cpp", "token.cpp", "parser.cpp", "ast.cpp"]
INPUT_DIR = "inputs"
BINARIO = "./a.out"

# Compilar
compilar = ["g++", "-std=c++17", "-Wall", "-o", BINARIO] + FUENTES
print("Compilando:", " ".join(compilar))
resultado = subprocess.run(compilar, capture_output=True, text=True)
if resultado.stderr:
    print(resultado.stderr)
if resultado.returncode != 0:
    sys.exit("Error en compilación")
print("Compilación exitosa\n")

# Ejecutar sobre cada input, ordenados por número.
# El patrón excluye los archivos generados (input1_tokens.txt, etc.).
PATRON = re.compile(r"^input(\d+)\.txt$")
entradas = sorted(
    (os.path.join(INPUT_DIR, f) for f in os.listdir(INPUT_DIR) if PATRON.match(f)),
    key=lambda f: int(PATRON.match(os.path.basename(f)).group(1)),
)
if not entradas:
    sys.exit(f"No se encontraron inputs en {INPUT_DIR}/")

fallos = 0
for ruta in entradas:
    print(f"--- {ruta} ---")
    run = subprocess.run([BINARIO, ruta], capture_output=True, text=True)
    print(run.stdout.strip() or run.stderr.strip())
    if run.returncode != 0:
        fallos += 1
        continue

    # Convertir el AST a imagen si graphviz está disponible
    dot = os.path.splitext(ruta)[0] + "_ast.dot"
    if shutil.which("dot") and os.path.isfile(dot):
        png = os.path.splitext(ruta)[0] + "_ast.png"
        subprocess.run(["dot", "-Tpng", dot, "-o", png], capture_output=True, text=True)
        print(f"AST → {png}")

print(f"\n{len(entradas) - fallos}/{len(entradas)} inputs parseados correctamente")
sys.exit(1 if fallos else 0)
