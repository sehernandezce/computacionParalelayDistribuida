#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <opencv2/opencv.hpp>

int main(int argc, char *argv[])
{  
    // Lectura de parametros
    char *archive_read = argv[1];
    char *archive_write = argv[2];

    // Abre el archivo de video original
    cv::VideoCapture inputVideo(archive_read);

    // Verifica si se pudo abrir el archivo de video
    if (!inputVideo.isOpened()) {
        std::cerr << "Error al abrir el archivo de video original." << std::endl;
        return -1;
    }

    // Obtiene las propiedades del video original (ancho, alto, FPS)
    double fps = inputVideo.get(cv::CAP_PROP_FPS);

    // Ancho y alto del video objetivo
    int redux = 3;
    int frame_width = 640;
    int frame_height = 360;

    // Crea el objeto para escribir el nuevo video procesado
    cv::VideoWriter outputVideo(archive_write, cv::VideoWriter::fourcc('X', '2', '6', '4'), fps, cv::Size(frame_width,frame_height));

    cv::Mat frame; // Variable para almacenar el frame capturado
    cv::Mat new_frame(frame_height, frame_width, CV_8UC3, cv::Scalar(255,255,255));

    // Captura un frame del video original
    while (inputVideo.read(frame)) {

        // cv::cvtColor(frame, frame, cv::COLOR_BGR2RGB);
        
        // Procesar el frame
        for(int row = 0; row < new_frame.rows; row++)
        {
            for(int col = 0; col < new_frame.cols; col++)
            {   
                int red = 0, green = 0, blue = 0;

                for(int i = 0; i < redux; i++)
                {
                    for(int j = 0; j < redux; j++)
                    {
                        blue += static_cast<int>(frame.at<cv::Vec3b>(row*redux + i, col*redux + j)[0]);
                        green += static_cast<int>(frame.at<cv::Vec3b>(row*redux + i, col*redux + j)[1]);
                        red += static_cast<int>(frame.at<cv::Vec3b>(row*redux + i, col*redux + j)[2]);
                    }
                }
                new_frame.at<cv::Vec3b>(row, col)[0] = blue/(redux*redux);
                new_frame.at<cv::Vec3b>(row, col)[1] = green/(redux*redux);
                new_frame.at<cv::Vec3b>(row, col)[2] = red/(redux*redux);
            }
        }

        // Escribe el frame procesado en el nuevo video
        outputVideo.write(new_frame);

        // Muestra el frame procesado
        // cv::imshow("Frame Procesado", new_frame);
    }

    // Libera los recursos
    inputVideo.release();
    outputVideo.release();
    cv::destroyAllWindows();

    return 0;
}

// Windows
//Compila y ejecuta: g++ -o video_reduccion_secuencial video_reduccion_secuencial.cpp && video_reduccion_secuencial inputVideo.mp4 outputVideo.mp4
//Ejecuta: video_reduccion_secuencial inputVideo.mp4 outputVideo.mp4

// Linux
//Compila y ejecuta: g++ -o "video_reduccion_secuencial" "video_reduccion_secuencial.cpp" && time ./video_reduccion_secuencial inputVideo.mp4 outputVideo.mp4
//Ejecuta: time ./video_reduccion_secuencial inputVideo.mp4 outputVideo.mp4