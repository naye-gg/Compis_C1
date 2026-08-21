"""Auto-verificación: compila y comprueba casos válidos e inválidos.
Uso: python3 test_parser.py
"""
import os
import subprocess
import sys
import tempfile

BINARIO = "./a.out"
FUENTES = ["main.cpp", "scanner.cpp", "token.cpp", "parser.cpp", "ast.cpp"]

subprocess.run(["g++", "-std=c++17", "-Wall", "-Werror", "-o", BINARIO] + FUENTES, check=True)


def correr(fuente):
    """Devuelve (returncode, stdout) al analizar la expresión dada."""
    with tempfile.TemporaryDirectory() as d:
        ruta = os.path.join(d, "caso.txt")
        with open(ruta, "w") as f:
            f.write(fuente)
        r = subprocess.run([BINARIO, ruta], capture_output=True, text=True)
        return r.returncode, r.stdout


VALIDOS = [
    "2 ** 5 + sqrt(5)",
    "50.5 + (10.25 * 3)",
    "max(20, 15, 30) + min(5, 8, 2)",
    "max(x1**2, x2**3) + min(x3, x4)",
    "abs(sqrt(abs(x1**2 + x2**2)) - max(x3**2, min(x4**3, abs(x5))))**2",
]

INVALIDOS = [
    ("2 + $", "lexico"),        # caracter invalido
    ("(2 + 3", "sintactico"),   # parentesis sin cerrar
    ("max(2, 3))", "sintactico"),  # token sobrante
    ("2 +", "sintactico"),      # expresion incompleta
    ("sqrt(2, 3)", "semantico"),   # sqrt espera 1 argumento
    ("max(2)", "semantico"),       # max espera 2 o mas
]

for src in VALIDOS:
    code, out = correr(src)
    assert code == 0 and "Parseo exitoso" in out, f"deberia aceptar {src!r}: {out!r}"

for src, tipo in INVALIDOS:
    code, out = correr(src)
    assert code != 0, f"deberia rechazar {src!r}"
    assert tipo in out.lower(), f"{src!r} → esperaba error {tipo}, se obtuvo {out!r}"

# Todos los inputs del enunciado deben parsear
for i in range(1, 11):
    ruta = os.path.join("inputs", f"input{i}.txt")
    r = subprocess.run([BINARIO, ruta], capture_output=True, text=True)
    assert r.returncode == 0, f"{ruta} fallo: {r.stdout!r}"

print(f"OK: {len(VALIDOS)} validos, {len(INVALIDOS)} invalidos, 10 inputs")
sys.exit(0)
