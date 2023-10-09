#include <stdio.h>
#include <stdlib.h>

void main(int argc, char *argv[])
{   
    // Lectura de parametros
    char *archivoLectura = argv[1];
    char *archivoEscritura = argv[2];

    printf("Archivo de lectura: %s\n", archivoLectura);
    printf("Archivo de escritura: %s\n", archivoEscritura);
}

// Windows
//Compila y ejecuta: gcc -o video_reduccion_secuencial video_reduccion_secuencial.c && video_reduccion_secuencial inputVideo.mp4 outputVideo.mp4
//Ejecuta: video_reduccion_secuencial inputVideo.mp4 outputVideo.mp4

// Linux
//Compila y ejecuta: gcc -o "video_reduccion_secuencial" "video_reduccion_secuencial.c" && time ./video_reduccion_secuencial inputVideo.mp4 outputVideo.mp4
//Ejecuta: time ./video_reduccion_secuencial inputVideo.mp4 outputVideo.mp4