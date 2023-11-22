#include <math.h>
#include <stdio.h>
#include <omp.h>

#define PI acos(-1)
#define PAD 8
#define MAXTHREADS 2

typedef double lf;
typedef long long ll;
typedef unsigned int ui;

const ui num_threads = MAXTHREADS*2, num_itr = 2000000000;

int calculatePi(ui ID, ui factor, lf *piThread)
{   
    ui start = ID * factor;
    ui end = (ID + 1) * factor;
    // printf("start = %u, end = %u\n", start, end);
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
    *piThread *= 4.0;
    return 0; 
}

int main()
{   
    lf pi[num_threads][PAD];
    lf tot = 0.0;
    ui a, factor = (num_itr  / num_threads);
    
    omp_set_num_threads(num_threads);
    #pragma omp parallel
    {
        ui ID = omp_get_thread_num();
        calculatePi(ID, factor, &pi[ID][0]);
    }

    for(a = 0; a < num_threads; a++)
    {   
        // printf("pi[a]: %.12lf\n", pi[a][0]);
        tot += pi[a][0];
    }
    printf("PI = %.9lf\n", tot);
    printf("PI acos(-1) = %.9lf\n", PI);
    printf("Error = %.9lf\n", PI - tot);
}
// g++ -fopenmp -o "calPI" "calPI.cpp" && ./calPI
// time -p ./calPI