#! /usr/bin/python3

import cgi
import sys
import os

form = cgi.FieldStorage()

priceLocation = 0
continente = form.getvalue('continente')
n_dias = form.getvalue('n_dias')

if continente == 'America'
	priceLocation = 4
elif continente == 'Europa'
	priceLocation = 1
elif continente == 'Asia'
	priceLocation = 3
elif continente == 'Africa'
	priceLocation = 2
elif continente == 'Oceania'
	priceLocation = 5

total = n_dias * priceLocation

print("total = %s\n" str(total))
