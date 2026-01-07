/*
Comparación experimental entre BubbleSort y BogoSort.

Se miden:
- Tiempo de ejecución
- Número de comparaciones
- Número de intercambios

BubbleSort se ejecuta múltiples veces para obtener tiempos medibles.
BogoSort se ejecuta varias veces para obtener medias estadísticas.
Se define un límite máximo de intentos para evitar ejecuciones no viables.

Los resultados se guardan en un fichero CSV para su análisis posterior.
*/

#include <iostream>
#include <chrono> //Cronometro
#include <random> //Generador mejor semilla
#include <stdio.h>
//Limite de max de intentos para Bongo por crecimiento !n
const unsigned long long LIMITE = 10000000ULL;
/*Repeticiones max para Bongo por si no se consigue ordenar
al primer intento*/
const int REPETICIONES_BONGO=20;
/*Repeticiones de Bubble que permite medir con precision
su tiempo de ejecucion*/
const int REPETICIONES_BUBBLE=10000;


//FUNCION QUE INDICA SI ARRAY ESTA ORDENADO
bool ordenado (int a[], int n)
{
    for (int i=0; i<n-1; i++)
    {
        if (a[i]<a[i+1])
        {
            return false;
        }
    }
    return true;
}

//FUNCION QUE BARAJA TODOS LOS COMPONENTES DEL ARRAY
int barajar(int a[], int n,std::mt19937 &gen)
{
    int swaps=0;
    std::uniform_int_distribution<int> dist (0,n-1);
    for (int i = 0; i < n; i++)
    {
        int j = dist(gen);
        if (i!=j)
        {
            std::swap(a[i],a[j]);
            swaps++;
        }
    }
    return swaps;
}
//FUNCION QUE MIDE PARÁMETROS DE BONGOSHORT
unsigned long long bongo_time (int n, unsigned long long LIMITE,
                               double &tiempo_segundos,
                               unsigned long long &comparaciones,
                               unsigned long long &swaps,
                               std::mt19937 &gen)
{
    int a[50];
    std::uniform_int_distribution<int> dist_val(0,99);
    for (int i=0; i<n; i++)
    {

        a[i]= dist_val(gen);
    }
    using clock = std::chrono::steady_clock;
    auto inicio = clock::now();
    unsigned long long intentos=0;
    comparaciones=0;
    swaps=0;
    while (!ordenado(a,n)&& intentos < LIMITE)
    {
        comparaciones+= n-1;
        swaps+=barajar(a,n,gen);
        intentos++;
    }
    auto fin = clock::now();
    std::chrono::duration<double> duracion= fin- inicio;
    tiempo_segundos=duracion.count();

    return intentos;
}

//FUNCION QUE MIDE PARÁMETROS DE BUBBLESHORT
void bubble_time (int a[],int n, unsigned long long &comparaciones,
                  unsigned long long &swaps, double &tiempo_segundos)
{
    comparaciones=0;
    swaps=0;
    using clock=std::chrono::steady_clock;
    auto inicio =clock::now();
    for (int r=0; r<REPETICIONES_BUBBLE; r++)
    {
        int b[100]; //Copia para no ordenar ya ordenado
        for (int i=0; i<n; i++)
        {
            b[i]=a[i];
        }

        for (int i=0; i<n-1; i++)
        {
            for (int j=0; j<n-1-i; j++)
            {
                comparaciones ++;
                if (b[j]<b[j+1])
                {
                    std::swap(b[j],b[j+1]);
                    swaps++;
                }
            }
        }

    }
    auto fin= clock::now();
    std::chrono::duration<double> duracion= fin-inicio;
    tiempo_segundos=duracion.count();
}
//FUNCION QUE ORDENARA ARRAY PARA MOSTRARLO
void bubble_short(int a[],int n)
{
    for (int i=0; i<n-1; i++)
    {
        for (int j=0; j<n-1-i; j++)
        {
            if(a[j]<a[j+1])
            {
                std::swap(a[j], a[j+1]);
            }
        }
    }
}

int main()
{
    int n = 2;          // tamaño inicial del array
    int a[100];
    // Inicializar la semilla del generador
    std::mt19937 gen(std::random_device{}());
    /*Abrimos archivo, el nombre es arbitrario y no
    es necesario mantenerlo*/
    FILE *f=fopen("resultados.csv","a");
    if (f== NULL)
    {
        std::cout << "Error al abrir el excel";
        return 1;
    }

    while (n<=20)
    {
        //CREAMOS VARIABLES DE MEDICION PARA BONGO
        double tiempo_medio=0.0;
        unsigned long long intentos_medio=0;
        unsigned long long swaps_medios=0;
        unsigned long long comps_medias=0;
        int fallos=0;
        for (int r=0; r<REPETICIONES_BONGO; r++)
        {
            double tiempo_tmp;
            unsigned long long swaps_tmp, comps_tmp;
            unsigned long long intentos_tmp= bongo_time(n,LIMITE,tiempo_tmp,comps_tmp,swaps_tmp,gen);

            if (intentos_tmp>=LIMITE)
            {
                fallos++;
                continue;

            }
            tiempo_medio+=tiempo_tmp;
            intentos_medio+=intentos_tmp;
            swaps_medios+=swaps_tmp;
            comps_medias+=comps_tmp;
        }
        int exitos= REPETICIONES_BONGO-fallos;
        // Llenar el array con enteros aleatorios entre 0 y 99
        std::uniform_int_distribution<int> dist_val(0,99);
        for (int i = 0; i < n; i++)
        {
            a[i] = dist_val(gen);
        }
        // ENSEÑAMOS ARRAY ORIGINAL Y EL ORDEN
        std::cout <<"ORDEN DE: "<<n<<" COMPONENTES \n";
        std::cout <<"Array random de origen\n\n\n";
        fprintf(f,"Orden n= %d\n",n);
        fprintf(f,"\n\nArray random de origen\n\n\n");
        for (int i = 0; i < n; i++)
        {

            std::cout << a[i] << " ";
            fprintf(f,"%d ",a[i]);
        }

        //Array con Bubbleshort

        int b[100];
        //Copia de array original a b[i] para ordenar
        for (int i=0; i<n; i++)
        {
            b[i]=a[i];
        }
        unsigned long long comps_bubble, swaps_bubble;
        double tiempo_bubble;
        //MEDICIONES DE BUBBLESHORT
        bubble_time(b,n,comps_bubble,swaps_bubble,tiempo_bubble);
        tiempo_bubble /= REPETICIONES_BUBBLE;
        //Mostrar array ordenado
        std::cout <<"\n\n\nBUBBLESHORT\n\n\n";
        fprintf(f,"\n\n\nBUBBLESHORT\n\n\n");
        bubble_short(b,n); //ORDENAMOS Y MOSTRAMOS
        for (int i=0; i<n; i++)
        {
            std::cout << b[i]<<" ";
            fprintf(f,"%d ",b[i]);
        }

        std::cout << "\n\nComprobaciones Bubbleshort: "<< comps_bubble
                  <<"\nIntercambios Bubbleshort: "<<swaps_bubble
                  <<"\nTiempo Bubbleshort (s): "<< tiempo_bubble<<"\n";
        fprintf(f,"\n\nComprobaciones Bubbleshort: %11u\nIntercambios Bubbleshort: %11u\n",comps_bubble,swaps_bubble);
        fprintf(f,"Tiempo Bubbleshort (s): %lf\n\n",tiempo_bubble);


        //Array con Bongoshort
        /*DEBEMOS COMPROBAR QUE EL ORDEN DEL ARRAY MEDIANDO BONGOSHORT
        HA SIDO EXITOSO EN MENOS DE 20 INTENTOS DE 100000INTERCAMBIOS*/
        if (exitos>0)
        {
            //DAREMOS LAS MEDIDAS PROMEDIAS DE TODOS LOS INTENTOS
            tiempo_medio /= exitos;
            intentos_medio /=exitos;
            swaps_medios /=exitos;
            comps_medias /= exitos;
            //Mostrar array ordenado
           /*
            Para mostrar el array ordenado se utiliza BubbleSort, ya que es determinista.
            Esto no afecta al experimento, ya que las mediciones se realizan por separado.
            */

            std::cout <<"\n\n\nBONGOSHORT\n\n\n";
            fprintf(f,"\n\n\nBONGOSHORT\n\n\n");
            bubble_short(b,n);
            for (int i=0; i<n; i++)
            {
                std::cout << b[i]<<" ";
                fprintf(f,"%d ",b[i]);
            }

            std::cout <<"\n";
            std::cout << "Para n ="<<n<<" \nTiempo medio= "<<tiempo_medio
                      <<" \nIntentos medios = "<< intentos_medio
                      <<"\nSwaps medios = "<<swaps_medios
                      <<"\nComparaciones medias = "<< comps_medias
                      <<" \nFallos ="<< fallos <<"\n";

            fprintf(f,"\n\nPara n= %d\nTiempo medio = %lf\nIntentos medios = %11u",n,tiempo_medio,intentos_medio);
            fprintf(f,"\nSwaps medios = %11u\nComparaciones medias = %11u",swaps_medios,comps_medias);
            fprintf(f,"\nFallos = %d\n\n",fallos);

        }
        else
        {
            //EN CASO DE QUE NO SE CONSIGA ORDENAR BONGO SE INDICARÁ Y SE SALDRA DEL BUCLE
            std::cout <<"n = "<<n << " Bongoshort no viable\n";
            fprintf(f,"n= %d\nBongoshort no viable\n",n);
            break;
        }

        n++;
    }
    //CERRAMOS FICHERO DE MEDICIONES
    fclose(f);
    return 0;
}
