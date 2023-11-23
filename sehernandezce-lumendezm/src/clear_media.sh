#!/bin/bash

# Ruta de la carpeta que contiene los videos
carpeta_media="../media"

# Bucle para eliminar los videos con nombres como "outputVideoi.mp4"
for i in {0..15}; do
    archivo_a_eliminar="${carpeta_media}/outputVideo${i}.mp4"

    # Verificar si el archivo existe antes de eliminarlo
    if [ -e "$archivo_a_eliminar" ]; then
        rm "$archivo_a_eliminar"
        echo "Archivo eliminado: $archivo_a_eliminar"
    else
        echo "El archivo no existe: $archivo_a_eliminar"
    fi
done