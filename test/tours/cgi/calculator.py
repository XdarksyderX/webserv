import sys

def adding(a, b):
    return a + b

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: calculator.py <num1> <num2>")
        sys.exit(1)

    num1 = int(sys.argv[1])
    num2 = int(sys.argv[2])
    result = adding(num1, num2)
    print(f"{num1} + {num2} = {result}")
