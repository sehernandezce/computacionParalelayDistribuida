cmake_minimum_required(VERSION 3.10)
project(sehernandezce-lumendezm)

# Configuración de compilación personalizada
set(CMAKE_CXX_COMPILER "g++") # Establece el compilador como g++
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /openmp")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /openmp")

set(OpenCV_DIR "C:/opencv/build")
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

# Crear el ejecutable para video_reduccion_secuencial.cpp
add_executable(video_reduccion_secuencial src/video_reduccion_secuencial.cpp)
target_link_libraries(video_reduccion_secuencial ${OpenCV_LIBS})

# Crear el ejecutable para video_reduccion_paralelo.cpp
add_executable(video_reduccion_paralelo src/video_reduccion_paralelo.cpp)
target_link_libraries(video_reduccion_paralelo ${OpenCV_LIBS})