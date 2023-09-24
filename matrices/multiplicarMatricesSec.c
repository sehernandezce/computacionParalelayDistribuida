#include <stdlib.h>
#include <stdio.h>
#include <time.h>

double generadorNumeroAleatorio()
{   
    // Genera un número aleatorio en el rango [0, 1)
    double numeroAleatorio = (double)rand() / RAND_MAX;
    
    // Multiplica el número aleatorio por el rango deseado y ajústalo según sea necesario
    double mn = 1.0;  // Valor mínimo deseado
    double mx = 100.0;  // Valor máximo deseado
    numeroAleatorio*= (mx - mn);
    numeroAleatorio += mn;

    return numeroAleatorio;
}

void llenarMatriz(double *M, int n)
{
    int i;
    for (i = 0; i < n*n; i++)
    {
        M[i] = generadorNumeroAleatorio();
    }
}

void imprimirMatriz(double *M, int n)
{   
    int i, j;
    for (i = 0; i < n*n; i+=n)
    {
        for (j = 0; j < n; j++)
        {   
            printf("%.7lf ", M[i+j]);
        }
        printf("\n");
    }
}

void multiplicarMatrices(double *A, double *B, double *C, int n)
{   
    int i, j, k, pos, acumN = 0;
    double sum = 0.0;
    for (i = 0; i < n*n; i+=n)
    {
        for (j = 0; j < n; j++)
        {   
            acumN = 0;
            sum = 0.0;
            for (k = 0; k < n; k++)
            {
                sum += A[k+i] * B[acumN + j];
                acumN+=n;
            }
            C[i+j] = sum;
        }
    }
}

void main()
{   
    // Inicializa el generador de números aleatorios con la hora actual como semilla
    srand(time(NULL));

    int n;
    printf("Por favor, ingresa un numero entero, el tamanio de la matriz: ");
    scanf("%d", &n);

    double *matrizA, *matrizB, *matrizC;
    matrizA = (double *) malloc((n*n) * sizeof(double));
    matrizB = (double *) malloc((n*n) * sizeof(double));
    matrizC = (double *) malloc((n*n) * sizeof(double));
    
    // Generar los números aleatorios y llenar matrices
    llenarMatriz(matrizA, n);
    llenarMatriz(matrizB, n);
    
    // Registra el tiempo de inicio
    clock_t inicio = clock();

    multiplicarMatrices(matrizA, matrizB, matrizC, n);

    // Registra el tiempo de finalización
    clock_t fin = clock();

    // Calcula la diferencia de tiempo en segundos
    double tiempo_transcurrido = (double)(fin - inicio) / CLOCKS_PER_SEC;
    printf("Tiempo de ejecucion: %f segundos\n", tiempo_transcurrido);

    // Generar archivo de salida con las matrices generadas y multiplicadas
    freopen("matrices.txt", "w", stdout);

    printf("Matriz A: \n");
    imprimirMatriz(matrizA, n);

    printf("Matriz B: \n");
    imprimirMatriz(matrizB, n);

    printf("Matriz C: \n");
    imprimirMatriz(matrizC, n);
}

//gcc multiplicarMatricesSec.c -o multiplicarMatricesSec && .\multiplicarMatricesSec