import random
import sys
min = 1

print(sys.argv)

if (len(sys.argv) == 2 and sys.argv[1].isdigit()):
	dices = int(sys.argv[1])
	max = 6
elif (len(sys.argv) == 3 and sys.argv[1].isdigit() and sys.argv[2].isdigit()):
	dices = int(sys.argv[1])
	max = int(sys.argv[2])
else:
	dices = 1
	max = 6

print ("Rolling the dices...\n")
for i in range(dices):
	print (random.randint(min, max))