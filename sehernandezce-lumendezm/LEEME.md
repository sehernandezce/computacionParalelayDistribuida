// FALTA COLOCAR EL PDF

# Practica 1



## Windows
1 - Descargar opencv https://opencv.org/releases/

2 - Agregar la ruta de opencv a las variables de entorno, ejemplo: C:\opencv\build\x64\vc16\bin

3 - Descargar cmake https://cmake.org/download/ (Binary distributions => Windows x64 ZIP) y agregar la ruta de cmake a las variables de entorno, ejemplo: C:\Program Files\CMake\bin

4 - Crear carpeta build y ejecutar comando:
        cd build
        cmake ..
        cd ..

5 - Ejecutar comando para construir/compilar el programa: 
        cmake --build ./build --config Release

6 - Para ejecutar el programa secuencial: 
        .\build\Release\video_reduccion_secuencial inputVideo.mp4 outputVideo.mp4

7 - Para ejecutar el programa paralelo: 
        .\build\Release\video_reduccion_paralelo inputVideo.mp4 outputVideo.mp4 4
