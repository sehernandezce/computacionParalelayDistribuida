#!/bin/bash

# IMPORTANTE: el archivo .sh debe tener los permisos necesarios. "chmod +x ./nombre_archivo"
# IMPORTANTE: opencv debe estar relacionado (linked) con el pkg-config del sistema.

# Variables importantes para configuración
carpeta="./sehernandezce-lumendezm/src"
carpeta_media="./sehernandezce-lumendezm/media"
parametros_opencv="`pkg-config --cflags --libs opencv4`" 
nombre_secuencial="$carpeta/video_reduccion_secuencial"
nombre_paralelo_openmp="$carpeta/video_reduccion_paralelo"
nombre_paralelo_cuda="$carpeta/video_reduction_cuda"
archivo_salida="$carpeta/resultados.txt"
archivo_video_entrada="$carpeta_media/inputVideo"
archivo_video_salida_sec="$carpeta_media/outputVideoSec"
archivo_video_salida_Openmp="$carpeta_media/outputVideoOpenMp"
archivo_video_salida_cuda="$carpeta_media/outputVideoCuda"

#-----------------------------------------------------------------------

# Función para crear el archivo de salida si no existe
crear_archivo_salida() {
  if [ ! -f "$archivo_salida" ]; then
    touch "$archivo_salida"
  fi
}

# # Compila el programa C++ secuencial
g++ -o "$nombre_secuencial" "$nombre_secuencial".cpp $parametros_opencv

# Verifica si la compilación fue exitosa (secuencial)
echo "Ejecutando Programa secuencial"
echo "Programa secuencial" >> "$archivo_salida"
if [ $? -eq 0 ]; then
  ejecucion_secuencial=$( { time -p "$nombre_secuencial" "$archivo_video_entrada".mp4 "$archivo_video_salida_sec".mp4; } 2>&1 | grep real | awk '{print $2}' )
  crear_archivo_salida
  echo "Video en Secuencial = $ejecucion_secuencial segundos" > "$archivo_salida"
  echo "El programa secuencial se ha ejecutado y el tiempo real se ha almacenado en $archivo_salida."
else
  echo "Error: La compilación del programa secuencial falló."
fi

#-----------------------------------------------------------------------

# # Compila el programa C++ paralelo OPENMP
# g++ -fopenmp -o "$nombre_paralelo_openmp" "$nombre_paralelo_openmp".cpp $parametros_opencv

# # Verifica si la compilación fue exitosa (Paralelo)
# echo "Ejecutando Programa paralelo OPENMP"
# echo "Programa paralelo OPENMP" >> "$archivo_salida"
# if [ $? -eq 0 ]; then
#   crear_archivo_salida
#   for i in 2 4 8 16; do
#     ejecucion_paralelo=$( { time -p "$nombre_paralelo_openmp" "$archivo_video_entrada".mp4 "$archivo_video_salida_Openmp$i".mp4 $i; } 2>&1 | grep real | awk '{print $2}' )
#     echo "Video en Paralelo ($i hilos) = $ejecucion_paralelo segundos" >> "$archivo_salida"
#     echo "El programa paralelo con $i hilos se ha ejecutado y el tiempo real se ha almacenado en $archivo_salida."
#   done
# else
#   echo "Error: La compilación del programa paralelo falló."
# fi

#-----------------------------------------------------------------------

# Compila el programa C++ paralelo CUDA
nvcc -w -o "$nombre_paralelo_cuda" "$nombre_paralelo_cuda".cu $parametros_opencv -std=c++11

# Verifica si la compilación fue exitosa (Paralelo)
echo "Ejecutando Programa paralelo CUDA C/C++"
echo "Programa paralelo CUDA C/C++" >> "$archivo_salida"
if [ $? -eq 0 ]; then
  crear_archivo_salida
  for i in 8 16 21 26 32; do
    ejecucion_paralelo=$( { time -p "$nombre_paralelo_cuda" "$archivo_video_entrada".mp4 "$archivo_video_salida_cuda$multi".mp4 $i; } 2>&1 | grep real | awk '{print $2}' )
    multi=$((i * i))
    echo "Video en Paralelo ("$multi" hilos*Bloque) = $ejecucion_paralelo segundos" >> "$archivo_salida"
    echo "El programa paralelo con "$multi" hilos*Bloque se ha ejecutado y el tiempo real se ha almacenado en $archivo_salida."
  done
else
  echo "Error: La compilación del programa paralelo falló."
fi