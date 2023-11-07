#include <iostream>
#include <opencv2/opencv.hpp>
#include <cuda_runtime.h>

const int redux = 3;
const int frame_width = 640;
const int frame_height = 360;

__global__ void cudaProcess(cv::Vec3b* input, cv::Vec3b* output, int width, int redux) {
    int bx = blockIdx.x;
    int by = blockIdx.y;
    int tx = threadIdx.x;
    int ty = threadIdx.y;

    int col = bx * blockDim.x * redux + tx * redux;
    int row = by * blockDim.y * redux + ty * redux;

    int red = 0, green = 0, blue = 0;

    for (int i = 0; i < redux; i++) {
        for (int j = 0; j < redux; j++) {
            blue += static_cast<int>(input[(row + i) * width + col + j].val[0]);
            green += static_cast<int>(input[(row + i) * width + col + j].val[1]);
            red += static_cast<int>(input[(row + i) * width + col + j].val[2]);
        }
    }

    int outputCol = bx * blockDim.x + tx;
    int outputRow = by * blockDim.y + ty;

    output[outputRow * (width / redux) + outputCol].val[0] = blue / (redux * redux);
    output[outputRow * (width / redux) + outputCol].val[1] = green / (redux * redux);
    output[outputRow * (width / redux) + outputCol].val[2] = red / (redux * redux);
}

int main(int argc, char *argv[]) {
    if(argc != 4) {
        std::cerr << "Usage: " << argv[0] << " <input video path> <output video path> <threads per block>" << std::endl;
        return -1;
    }

    char *archive_read = argv[1];
    char *archive_write = argv[2];
    int threadsPerDim = atoi(argv[3]);

    cv::VideoCapture inputVideo(archive_read);
    if (!inputVideo.isOpened()) {
        std::cerr << "Error al abrir el archivo de video original." << std::endl;
        return -1;
    }

    if(threadsPerDim > 32) 
    {
      std::cerr << "Error. El valor debe ser menor o igual a 32. Tenga encuenta que el parametro al cuadrado define el numero de hilos por bloque." << std::endl;
      return -1;
    }

    double fps = inputVideo.get(cv::CAP_PROP_FPS);
    cv::VideoWriter outputVideo(archive_write, cv::VideoWriter::fourcc('X', '2', '6', '4'), fps, cv::Size(frame_width, frame_height));

    int originalWidth = inputVideo.get(cv::CAP_PROP_FRAME_WIDTH);
    int originalHeight = inputVideo.get(cv::CAP_PROP_FRAME_HEIGHT);

    cv::Vec3b *d_input, *d_output;
    cudaMalloc(&d_input, originalWidth * originalHeight * sizeof(cv::Vec3b));
    cudaMalloc(&d_output, frame_width * frame_height * sizeof(cv::Vec3b));

    dim3 threadsPerBlock(threadsPerDim, threadsPerDim);
    dim3 blocksPerGrid((frame_width + threadsPerBlock.x - 1) / threadsPerBlock.x,
                      (frame_height + threadsPerBlock.y - 1) / threadsPerBlock.y);

    cv::Mat frame;
    cv::Mat new_frame(frame_height, frame_width, CV_8UC3, cv::Scalar(255, 255, 255));

    // Declaración de eventos para medir tiempos
    cudaEvent_t start, stop;
    float elapsedTime;

    // Inicializar los eventos para medir tiempos
    cudaEventCreate(&start);
    cudaEventCreate(&stop);

    // Registrar el evento de inicio
    cudaEventRecord(start, 0);

    while (inputVideo.read(frame)) {
      cudaMemcpyAsync(d_input, frame.data, originalWidth * originalHeight * sizeof(cv::Vec3b), cudaMemcpyHostToDevice);

      cudaProcess<<<blocksPerGrid, threadsPerBlock>>>(d_input, d_output, originalWidth, redux); // Añadido el argumento redux

      cudaMemcpyAsync(new_frame.data, d_output, frame_width * frame_height * sizeof(cv::Vec3b), cudaMemcpyDeviceToHost);

      outputVideo.write(new_frame);
  }


    // Registrar el evento de finalización
    cudaEventRecord(stop, 0);
    cudaEventSynchronize(stop);

    cudaEventElapsedTime(&elapsedTime, start, stop);
    std::cout << "Tiempo total de procesamiento: " << elapsedTime / 1000.0 << " segundos" << std::endl;

    // Limpieza
    cudaEventDestroy(start);
    cudaEventDestroy(stop);

    cudaFree(d_input);
    cudaFree(d_output);

    inputVideo.release();
    outputVideo.release();

    return 0;
}

//Compila el programa
//!nvcc -w -o ./sehernandezce-lumendezm/src/video_reduction_cuda ./sehernandezce-lumendezm/src/video_reduction_cuda.cu `pkg-config --cflags --libs opencv4` -std=c++11

// Ejecuta el programa para 32*32 = 1024 hilos por bloque
//!time -p ./sehernandezce-lumendezm/src/video_reduction_cuda ./sehernandezce-lumendezm/media/inputVideo.mp4 ./sehernandezce-lumendezm/media/outputVideo.mp4 32