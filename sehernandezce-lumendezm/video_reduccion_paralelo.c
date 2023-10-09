#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

void main(int argc, char *argv[])
{   
    // Lectura de parametros
    char *archivoLectura = argv[1];
    char *archivoEscritura = argv[2];
    int numeroHilos = atoi(argv[3]);

    printf("Archivo de lectura: %s\n", archivoLectura);
    printf("Archivo de escritura: %s\n", archivoEscritura);
    printf("Numero de hilos: %d\n", numeroHilos);
    
    // Crear hilos
    omp_set_num_threads(numeroHilos);
    
    #pragma omp parallel
    {
        printf("Hola mundo\n");
    }

}

// Windows
//Compila y ejecuta: gcc -fopenmp -o video_reduccion_paralelo video_reduccion_paralelo.c && video_reduccion_paralelo inputVideo.mp4 outputVideo.mp4 4
//Ejecuta: video_reduccion_paralelo inputVideo.mp4 outputVideo.mp4 4

// Linux
//Compila y ejecuta: gcc -fopenmp -o "video_reduccion_paralelo" "video_reduccion_paralelo.c" && time ./video_reduccion_paralelo inputVideo.mp4 outputVideo.mp4 4
//Ejecuta: time ./video_reduccion_paralelo inputVideo.mp4 outputVideo.mp4 4