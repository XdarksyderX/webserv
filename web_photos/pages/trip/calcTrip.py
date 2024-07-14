#! /usr/bin/python3

import cgi
import sys
import os

form = cgi.FieldStorage()

priceLocation = 0
continente = form.getvalue('continente')
n_dias = form.getvalue('n_dias')

if continente == 'america':
	priceLocation = 4
elif continente == 'europa':
	priceLocation = 1
elif continente == 'asia':
	priceLocation = 3
elif continente == 'africa':
	priceLocation = 2
elif continente == 'oceania':
	priceLocation = 5
else:
	priceLocation = 0

total = int(n_dias) * int(priceLocation)

print(f'total = {str(total)}')
