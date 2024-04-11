#!/bin/bash

# URL a la que se realizarán las peticiones
URL="http://127.0.0.1:8001"

# Bucle infinito para realizar peticiones continuamente
while true; do
  curl $URL
  # Puedes descomentar la siguiente línea para agregar un pequeño retraso entre las peticiones
  # sleep 1
done

