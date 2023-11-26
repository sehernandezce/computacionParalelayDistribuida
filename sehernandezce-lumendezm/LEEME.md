# Librerias necesarias para las practicas

        sudo apt-get install g++

        sudo apt-get install -y libopencv-dev

        sudo apt-get install openmpi-bin openmpi-common libopenmpi3 libopenmpi-dev

        sudo apt-get install nfs-common

        sudo apt-get install nfs-kernel-server

        Instalar CUDA

# Script ejecutar todo

Nota: Para ejecutar las pruebas ejecute el script con bash. El video debe contener el nombre inputVideo.mp4 y debe estar en la carpeta media.

        Ejecutar el comando para asignar permisos:

        chmod +x ./script_ejecutar_todo_linux.sh

        Ejecutar el script:
        
        ./script_ejecutar_todo_linux.sh

El video de salida se encuentra en la carpeta media con el nombre outputVideo<Sufijo>.mp4. El sufijo es el tipo de tecnologia usada y el numero de hilos o procesos que se utilizo para la ejecución.

# Practica 1 - Video Reduccion de Resolucion (Secuecial y Paralelo con OpenMP)

## Linux
1 - Compilar el programa secuencial:
        
        g++ -o video_reduccion_secuencial video_reduccion_secuencial.cpp `pkg-config --cflags --libs opencv4`

2 - Compilar el programa paralelo:
        
        g++ -fopenmp -o video_reduccion_paralelo video_reduccion_paralelo.cpp `pkg-config --cflags --libs opencv4` 

3 - Para ejecutar el programa secuencial: 
        
        time ./video_reduccion_secuencial ../media/inputVideo.mp4 ../media/outputVideo.mp4

4 - Para ejecutar el programa paralelo: 
       
        time ./video_reduccion_paralelo ../media/inputVideo.mp4 ../media/outputVideo.mp4 4

# Practica 2 - Video Reduccion de Resolucion (Paralelo con CUDA C/C++)

Esta se realizo en google Colab, por lo que se puede ejecutar en cualquier navegador web. Se aclara que en el comando ejecutar todo no se ejecuta el programa de CUDA.

https://colab.research.google.com/drive/1-EBPeTqQV-aYDNtXynA2te4x9OTna5W4?usp=sharing

# Practica 3 - Video Reduccion de Resolucion (Paralelo con OpenMPI)

Se realizo la configuración en maquinas virtuales en google cloud.

Espiciaciones de las maquinas virtuales:
- 2 vCPU
- 2 GB de memoria
- 10 GB de almacenamiento
- Debian GNU/Linux 11 (bullseye)

## Configuración de maquinas virtuales

Comandos en servidor (nodo 0):
        
        apt-get update

        adduser mpiuser --uid 1001
        
        apt-get install openmpi-bin openmpi-common libopenmpi3 libopenmpi-dev
        
        apt-get install nfs-kernel-server
        
        apt-get install nfs-common
        
        su - mpiuser
        
        cd /home/mpiuser
        
        mkdir shared
        
        echo "/home/mpiuser/shared *(rw,sync,no_subtree_check)" | sudo tee /etc/exports
        
        sudo service nfs-kernel-server restart
        
        exportfs -a

Comandos en cliente:

        sudo adduser mpiuser --uid 1001
        
        sudo usermod -aG sudo mpiuser

        su - mpiuser
        
        mkdir /home/mpiuser/shared
        
        sudo nano /etc/hosts  =>> Configurar dns local
        
        sudo mount -t nfs node-0:/home/mpiuser/shared /home/mpiuser/shared
        
        mkdir ~/.ssh
        
        cd shared
        
        cp id_rsa.pub ~/.ssh/authorized_keys
        
        cd ..
        
        cd .ssh
        
        ls -al 
        
        sudo chmod 700 ~/.ssh
        
        sudo chmod 600 ~/.ssh/authorized_keys

Ejecutar programa:

        mpic++ -o video_reduccion_paralelo_openmpi video_reduccion_paralelo_openmpi.cpp -lm -fopenmp `pkg-config --cflags --libs opencv4`
        
        time mpirun -np 2 --hostfile mpi-hosts ./video_reduccion_paralelo_openmpi ../media/inputVideo.mp4 ../media/outputVideo 1


# Windows (Secuencial y openMP) - REVISAR
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
