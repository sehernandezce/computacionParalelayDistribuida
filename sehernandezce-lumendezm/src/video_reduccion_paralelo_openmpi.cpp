#include <mpi.h>
#include <omp.h>
#include <stdio.h>
#include <iostream>
#include <opencv2/opencv.hpp>

#define REDUX 3
#define WIDTH 1920
#define HEIGHT 1080

const int frame_width = WIDTH/3, frame_height = HEIGHT/3;

int proceFrame(cv::Mat &frame, cv::Mat &new_frame)
{   
    int row = 0, col = 0, i = 0, j = 0, red = 0, green = 0, blue = 0;
    // Procesar el frame
    #pragma omp parallel shared(frame, new_frame) private(row, col, i, j, red, green, blue)
    {
        #pragma omp for schedule (dynamic) nowait
        for(row = 0; row < new_frame.rows; row++)
        {   
            for(col = 0; col < new_frame.cols; col++)
            {   
                red = 0, green = 0, blue = 0;
                for(i = 0; i < REDUX; i++)
                {
                    for(j = 0; j < REDUX; j++)
                    {
                        blue += static_cast<int>(frame.at<cv::Vec3b>(row*REDUX + i, col*REDUX + j)[0]);
                        green += static_cast<int>(frame.at<cv::Vec3b>(row*REDUX + i, col*REDUX + j)[1]);
                        red += static_cast<int>(frame.at<cv::Vec3b>(row*REDUX + i, col*REDUX + j)[2]);
                    }
                }
                new_frame.at<cv::Vec3b>(row, col)[0] = blue/(REDUX*REDUX);
                new_frame.at<cv::Vec3b>(row, col)[1] = green/(REDUX*REDUX);
                new_frame.at<cv::Vec3b>(row, col)[2] = red/(REDUX*REDUX);
            }
        }
    }
    return 0;
}

int main(int argc, char *argv[]) {

    char *archive_read = argv[1];
    std::string archive_write = argv[2];
    int numThreads = atoi(argv[3]);

    int processId, numProc, nameLen;
    char processorName[MPI_MAX_PROCESSOR_NAME];

    MPI_Init(&argc, &argv);

	MPI_Comm_size(MPI_COMM_WORLD, &numProc);
	MPI_Comm_rank(MPI_COMM_WORLD, &processId);
    MPI_Get_processor_name(processorName, &nameLen);

    // Verificar la cantidad de argumentos de la línea de comandos
    if (argc != 4) {
        // Nodo 0 imprime un mensaje de error si no se proporciona la ruta del video
        if (processId == 0) {
            std::cerr << "Uso: " << argv[0] << " <ruta_al_video>" << std::endl;
        }
        // Finalizar el entorno MPI y salir con código de error
        MPI_Finalize();
        return 1;
    }

    cv::VideoCapture inputVideo;
    // cv::VideoWriter outputVideo;
    int numFrames;
    inputVideo.open(archive_read);
    if (!inputVideo.isOpened()) {
        std::cerr << "Error al abrir el archivo de video original." << std::endl;
        MPI_Finalize();
        return 1;
    }
    // Obtiene las propiedades del video original (ancho, alto, FPS)
    double fps = inputVideo.get(cv::CAP_PROP_FPS);
    numFrames = static_cast<int>(inputVideo.get(cv::CAP_PROP_FRAME_COUNT));

    std::string archive_writecp = archive_write + std::to_string(processId) + ".mp4";

    cv::VideoWriter outputVideo(archive_writecp, cv::VideoWriter::fourcc('X', '2', '6', '4'), fps, cv::Size(frame_width, frame_height));

    cv::Mat frame; // Variable para almacenar el frame capturado
    cv::Mat new_frame(frame_height, frame_width, CV_8UC3);
    // Array para almacenar los frames locales de cada proceso

    int framesForNode = numFrames / (numProc);
    int startFrame = (processId) * framesForNode;
    int endFrame = (processId == numProc - 1) ? (numFrames - 1) : (startFrame + framesForNode - 1);

    // int framesLocal = (framesForNode  / numThreads);
    // std::cout << numFrames << " " << framesForNode << " " << startFrame << " " << endFrame 
    //     << " OF process " << processId << " of " << numProc
    //     << " on " << processorName << std::endl;
    
    inputVideo.set(cv::CAP_PROP_POS_FRAMES, startFrame);
    omp_set_num_threads(numThreads);
    for (int itr = startFrame; itr <= endFrame; itr++)
    {
        inputVideo.read(frame);

        // std::cout  <<  frame.rows <<" OF process " << processId << " of " << numProc
        //       << " on " << processorName << std::endl;
        if(frame.empty()) break;

        proceFrame(frame, new_frame);

        // Escribe el frame procesado en el nuevo video
        outputVideo.write(new_frame);
    }    
    MPI_Barrier(MPI_COMM_WORLD);
    
    // Libera los recursos
    inputVideo.release();
    outputVideo.release();

    // Nodo 0 recoge todos los frames
    if (processId == 0)
    {   
        cv::VideoCapture inputVideo2;
        cv::VideoWriter outputVideo2;
        std::string video_write = archive_write + ".mp4";
        outputVideo2.open(video_write, cv::VideoWriter::fourcc('X', '2', '6', '4'), fps, cv::Size(frame_width, frame_height));
        for(int procId = 0; procId < numProc; procId++)
        {   
            std::string video_read = archive_write + std::to_string(procId) + ".mp4";
            inputVideo2.open(video_read);
            if (!inputVideo2.isOpened()) {
                std::cerr << "Error al abrir el archivo de video original de salida." << video_read << std::endl;
                MPI_Finalize();
                return 1;
            }
            while(inputVideo2.read(frame))
            {
                // Escribe el frame procesado en el nuevo video
                outputVideo2.write(frame);
            }
        }
        // Libera los recursos
        inputVideo2.release();
        outputVideo2.release();
    }

    MPI_Finalize();

    return 0;
}

/*
mpic++ -o video_reduccion_paralelo_openmpi video_reduccion_paralelo_openmpi.cpp -lm -fopenmp `pkg-config --cflags --libs opencv4`
&& mpirun -np 4 --hostfile mpi-hosts ./video_reduccion_paralelo_openmpi ../media/inputVideo.mp4 ../media/outputVideo 4
--hostfile mpi-hosts
*/

/*
/home/mpiuser/shared/computacionParalelayDistribuida/sehernandezce-lumendezm/media/outputVideo.mp4
*/