// FALTA COLOCAR EL PDF

# Practica 1

## Linux
1 - Compilar el programa secuencial:
        
        g++ -o video_reduccion_secuencial video_reduccion_secuencial.cpp `pkg-config --cflags --libs opencv4`

2 - Compilar el programa paralelo:
        
        g++ -fopenmp -o video_reduccion_paralelo video_reduccion_paralelo.cpp `pkg-config --cflags --libs opencv4` 

3 - Para ejecutar el programa secuencial: 
        
        time ./video_reduccion_secuencial inputVideo.mp4 outputVideo.mp4

4 - Para ejecutar el programa paralelo: 
       
        time ./video_reduccion_paralelo inputVideo.mp4 outputVideo.mp4 4

Nota: Para ejecutar las pruebas ejecute el script con bash. El video debe contener el nombre inputVideo.mp4 y debe estar en la carpeta media

        ./script_ejecutar_todo_linux.sh

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
        
        .\build\Release\video_reduccion_secuencial .\media\inputVideo.mp4 .\media\outputVideo.mp4

7 - Para ejecutar el programa paralelo: 
        
        .\build\Release\video_reduccion_paralelo .\media\inputVideo.mp4 .\media\outputVideo.mp4 4

Nota: Para ejecutar las pruebas ejecute el script con python. El video debe contener el nombre inputVideo.mp4 y debe estar en la carpeta media

        `script_ejecutar_todo_win.py`