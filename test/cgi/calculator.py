import sys

def sumar_numeros(a, b):
    return a + b

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Uso: calculator.py <numero1> <numero2>")
        sys.exit(1)

    numero1 = int(sys.argv[1])
    numero2 = int(sys.argv[2])
    resultado = sumar_numeros(numero1, numero2)
    print(f"{numero1} + {numero2} = {resultado}")
