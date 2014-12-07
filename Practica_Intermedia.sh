#!/bin/bash


# funcion que muestra las diez primeras lineas del programa
function mostrar_10lineas()
{
  cat abuelo.c | head -n 10 
}

while true # Bucle infinito
do
  # Visualizamos el menú
  echo -e "Por favor, seleccione una opción de la lista:"
  echo -e "\t1.  Ejecutar programa"
  echo -e "\t2.  Mostrar 10 primeras lineas del programa"
  echo -e "\t3.  Salir"
  
  
  read option
  case $option in
    1) echo -e "\n"
       cc abuelo.c -o abuelo && ./abuelo
       echo -e "\n";;
    2) Mostrar=`mostrar_10lineas`
       echo -e "\nLas diez primeras lineas del programa son: \n$Mostrar\n"
       ;;
    3) break
       ;;
    *) echo -e "\nOpción no válida\n"
       ;;
  esac
done
# Fin del script
 
