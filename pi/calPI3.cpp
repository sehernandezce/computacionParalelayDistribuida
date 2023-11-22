#include <iostream>
#include <stdio.h>
#include <math.h>
#include <omp.h>
#include <mpi.h>

#define PI acos(-1)
#define PAD 8
#define MAXTHREADS 2

typedef double lf;
typedef long long ll;
typedef unsigned int ui;

const ui numThreads = MAXTHREADS*2, iterations = 2000000000;

int calculatePi(ui threadId, ui factorLocal, ui startProc, lf *piThread)
{   
    ui start = (threadId * factorLocal) + startProc;
    ui end = ((threadId + 1) * factorLocal) + startProc;
    // printf("rocessid %i, start = %u, end = %u\n", processId,start, end);
    ui imp = start*2+1;
    *piThread = 0.0;
    for(ui a = start; a < end; a++)
    {   
        if(~a&1)
        {
            *piThread += 1.0 / (imp);
        }
        else
        {
            *piThread -= 1.0 / (imp);
        }
        imp+=2;
    }
    *piThread *=4.0;
    return 0; 
}

int main(int argc, char *argv[])
{

    lf calPI = 0.0;
    int processId, numProc, nameLen;
    char processorName[MPI_MAX_PROCESSOR_NAME];

    MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numProc);
	MPI_Comm_rank(MPI_COMM_WORLD, &processId);
    MPI_Get_processor_name(processorName, &nameLen);

    ui factorProc = iterations/numProc;
    ui startProc = processId*factorProc;

    lf pi[numThreads][PAD];
    lf sumLocal = 0.0;
    ui itr, factorLocal = (factorProc  / numThreads);
    
    omp_set_num_threads(numThreads);
    #pragma omp parallel
    {
        ui threadId = omp_get_thread_num();
        // calculatePi(threadId, factorLocal, startProc, processId, &pi[threadId][0]);
        calculatePi(threadId, factorLocal, startProc, &pi[threadId][0]);
    }
    for(itr = 0; itr < numThreads; itr++)
    {   
        // printf("Processid %i pi[%i]: %.12lf\n", processId,itr, pi[itr][0]);
        sumLocal += pi[itr][0];
    }
    // std::cout << sumLocal  << " OF process " << processId << " of " << numProc
    //           << " on " << processorName << std::endl;

    MPI_Reduce(&sumLocal, &calPI, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
	if (processId == 0)
    {
        printf("PI = %.9lf\n", calPI);
        printf("PI acos(-1) = %.9lf\n", PI);
        printf("Error = %.9lf\n", PI - calPI);
    }
	MPI_Finalize();
}

/*
mpic++ -o calPI3 calPI3.cpp -lm -fopenmp && mpirun -np 2 ./calPI3
*/