#include <stdio.h>
#include <stdlib.h>
#include <opencv2/opencv.hpp>

int main(int argc, char *argv[])
{   
    // Lectura de parametros
    char *archivoLectura = argv[1];
    char *archivoEscritura = argv[2];

    printf("Archivo de lectura: %s\n", archivoLectura);
    printf("Archivo de escritura: %s\n", archivoEscritura);

    printf("version: %d.%d.%d\n", CV_MAJOR_VERSION, CV_MINOR_VERSION, CV_SUBMINOR_VERSION);
    return 0;
}

// Windows
//Compila y ejecuta: g++ -o video_reduccion_secuencial video_reduccion_secuencial.cpp && video_reduccion_secuencial inputVideo.mp4 outputVideo.mp4
//Ejecuta: video_reduccion_secuencial inputVideo.mp4 outputVideo.mp4

// Linux
//Compila y ejecuta: g++ -o "video_reduccion_secuencial" "video_reduccion_secuencial.cpp" && time ./video_reduccion_secuencial inputVideo.mp4 outputVideo.mp4
//Ejecuta: time ./video_reduccion_secuencial inputVideo.mp4 outputVideo.mp4