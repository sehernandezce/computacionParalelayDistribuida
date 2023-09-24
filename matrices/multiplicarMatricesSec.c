#include <time.h>
#include <stdio.h>
#include <stdlib.h>

const int N = 1048580;

double matrizA[N], matrizB[N], matrizC[N];

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

void llenarMatriz(double *puntero, int n)
{
    int i;
    for (i = 0; i < n*n; i++)
    {
        *(puntero + i) = generadorNumeroAleatorio();
    }
}

void imprimirMatriz(double *puntero, int n)
{   
    int i, j;
    for (i = 0; i < n*n; i+=n)
    {
        for (j = 0; j < n; j++)
        {   
            printf("%.7lf ", *(puntero + i + j));
        }
        printf("\n");
    }
}

void multiplicarMatrices(double *punteroA, double *punteroB, double *punteroC, int n)
{   
    int i, j, k;
    for (i = 0; i < n*n; i+=n)
    {
        for (j = 0; j < n; j++)
        {   
            *(punteroC + i + j) = 0.0;
            for (k = 0; k < n; k++)
            {
                *(punteroC + i + j) += *(punteroA + k + i) * *(punteroB + k*n + j);
            }
        }
    }
}

void main()
{   
    // Inicializa el generador de números aleatorios con la hora actual como semilla
    srand(time(NULL));

    int n;
    printf("Por favor, ingresa un número entero, el tamaño de la matriz: ");
    scanf("%d", &n);

    // double matrizA[n*n], matrizB[n*n], matrizC[n*n];
    
    // Generar los números aleatorios y llenar matrices
    double *punteroA = matrizA, *punteroB = matrizB, *punteroC = matrizC;
    llenarMatriz(punteroA, n);
    llenarMatriz(punteroB, n);
    
    multiplicarMatrices(punteroA, punteroB, punteroC, n);
    
    // Generar archivo de salida con las matrices generadas y multiplicadas
    freopen("matrices.txt", "w", stdout);

    printf("Matriz A: \n");
    imprimirMatriz(punteroA, n);

    printf("Matriz B: \n");
    imprimirMatriz(punteroB, n);

    printf("Matriz C: \n");
    imprimirMatriz(punteroC, n);
}

//gcc multiplicarMatricesSec.c -o multiplicarMatricesSec && .\multiplicarMatricesSec