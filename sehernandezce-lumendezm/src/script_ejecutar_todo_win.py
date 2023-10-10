import time
import subprocess

## Nota: Primero se debe haber instalado CMake y OpenCV. Tambien construir proyecto. Ver LEEME.md

## Compilar Secuencial y paralelo
subprocess.run(["cmake", "--build", "../build", "--config", "Release"]) # cmake --build ./build --config Release

with open("resultados.txt", "w") as archivo:
    ## Ejecutar Secuencial
    inicio = time.time()
    subprocess.run(["../build/Release/video_reduccion_secuencial.exe", "../media/inputVideo.mp4", "../media/outputVideo.mp4"])
    fin = time.time()
    tiempoSecuencial = fin-inicio
    archivo.write(f"Tiempo de ejecucion secuencial (s): {tiempoSecuencial:.6f}\n")

    ## Hilos
    numbers = [2, 4, 6, 16]
    ## Ejecutar Paralelo
    for numThreads in numbers:
        inicio = time.time()
        subprocess.run(["../build/Release/video_reduccion_paralelo", "../media/inputVideo.mp4", "../media/outputVideo.mp4", str(numThreads)])
        fin = time.time()
        tiempoParalelo = fin-inicio
        archivo.write(f"Tiempo de ejecucion paralelo, numero de hilos {numThreads} (s): {tiempoParalelo:.6f}\n")