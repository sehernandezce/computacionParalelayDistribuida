#!/bin/bash

# IMPORTANTE: el archivo .sh debe tener los permisos necesarios. "chmod +x ./nombre_archivo"
# IMPORTANTE: opencv debe estar relacionado (linked) con el pkg-config del sistema.

# Variables importantes para configuración
parametros_opencv="`pkg-config --cflags --libs opencv4`" 
nombre_secuencial="video_reduccion_secuencial"
nombre_paralelo="video_reduccion_paralelo"
archivo_salida="resultados.txt"

#-----------------------------------------------------------------------

# Función para crear el archivo de salida si no existe
crear_archivo_salida() {
  if [ ! -f "$archivo_salida" ]; then
    touch "$archivo_salida"
  fi
}

# Compila el programa C++ secuencial
g++ -o "$nombre_secuencial" "$nombre_secuencial".cpp $parametros_opencv

# Verifica si la compilación fue exitosa (secuencial)
if [ $? -eq 0 ]; then
  ejecucion_secuencial=$( { time -p ./video_reduccion_secuencial inputVideo.mp4 outputVideo.mp4; } 2>&1 | grep real | awk '{print $2}' )
  crear_archivo_salida
  echo "Video Secuencial = $ejecucion_secuencial segundos" > "$archivo_salida"
  echo "El programa secuencial se ha ejecutado y el tiempo real se ha almacenado en $archivo_salida."
else
  echo "Error: La compilación del programa secuencial falló."
fi

#-----------------------------------------------------------------------

# Compila el programa C++ paralelo
g++ -fopenmp -o "$nombre_paralelo" "$nombre_paralelo".cpp $parametros_opencv

# Verifica si la compilación fue exitosa (Paralelo)
if [ $? -eq 0 ]; then
  crear_archivo_salida
  for i in 2 4 8 16; do
    ejecucion_paralelo=$( { time -p ./video_reduccion_paralelo inputVideo.mp4 outputVideo.mp4 $i; } 2>&1 | grep real | awk '{print $2}' )
    echo "Video Paralelo ($i hilos) = $ejecucion_paralelo segundos" >> "$archivo_salida"
    echo "El programa paralelo con $i hilos se ha ejecutado y el tiempo real se ha almacenado en $archivo_salida."
  done
else
  echo "Error: La compilación del programa paralelo falló."
fi
