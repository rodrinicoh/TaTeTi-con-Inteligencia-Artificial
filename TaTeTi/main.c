




#include <stdio.h>
#include <stdlib.h>
#include "lista.h"
#include "arbol.h"
#include "ia.h"
#include "partida.h"

static void quien_comienza(int * comienza, char nom1[41], char nom2[41]);
static void mostrar_tablero(tTablero T);
static void ingresar_movimiento( int* x, int* y);

/**
    Retorna las coordenadas de un movimiento ingresado por el usuario
*/
static void ingresar_movimiento( int* x, int* y){
    printf("\n");
    printf("\n");
    printf("\n");

    printf("ingrese la coordenada horizontal del siguiente movimiento\n");
    scanf(" %i", x);
    printf("ingrese la coordenada vertical del siguiente movimiento\n");
    scanf(" %i", y);

    printf("\n");
    printf("\n");
    printf("\n");

}

/**
    Imprime el tablero de juego
*/

static void mostrar_tablero(tTablero T){
    int i;
    int j;
    i=0;
    j=0;
    printf("\n");
    printf("\n");
    printf("\n");
    printf("               0   1   2\n");
    for(; i<3; i++){
            printf("          %i   ", i);
        for(;j<3; j++){
            if(T->grilla[i][j]== 2)
                printf("|  |");
            if(T->grilla[i][j]== 0)
                printf("|o |");
            if(T->grilla[i][j]== 1)
                printf("|x |");

        }
        printf("          \n");
        printf("              ------------\n");
        j=0;
    }
    printf("\n");
    printf("\n");
    printf("\n");
}

/**
    Retorna el jugador que debe comenzar la partida
*/

static void quien_comienza(int * comienza, char nom1[41], char nom2[41]){
    printf("Elija a continuacion el jugador que comenzara la partida   \n " );
    printf("1. %s \n ", nom1);
    printf("2. %s \n ", nom2);
    printf("3. Jugador al azar \n ");
    scanf(" %i", comienza);
    if( *comienza!=1 && *comienza!=2)
        *comienza=(rand()%2)+1;
    *comienza=*comienza+99;
}



int main()
{

    char nom1[41];
    char nom2[41];
    int *x;
    int *y;
    int *opcion;
    int *comienza=NULL;
    int movimiento=0;
    tPartida partida;
    tTablero tablero;
    tBusquedaAdversaria b;

    opcion=(int*)malloc(sizeof(int));
    comienza=(int*)malloc(sizeof(int));
    x=(int*)malloc(sizeof(int));
    y=(int*)malloc(sizeof(int));



    while(*opcion != 4){

        printf("\n");
        printf("\n");
        printf("\n");
        printf("Elija un modo de juego \n " );
        printf("1. Usuario vs Usuario \n ");
        printf("2. Usuario vs Computadora \n " );
        printf("3. Computadora vs Computadora \n ");
        printf("4. Salir \n");


        scanf(" %i", opcion);


        if(*opcion == 1){



            printf("Ingrese el nombre del jugador 1 \n " );
            scanf(" %s", nom1);
            printf("Ingrese el nombre del jugador 2  \n " );
            scanf(" %s", nom2);

            quien_comienza(comienza, nom1, nom2);

            nueva_partida(&partida, 103, *comienza, nom1, nom2);

            tablero=partida->tablero;
            mostrar_tablero(tablero);


            while(partida->estado==111){
                if(partida->turno_de == 100){
                    printf("Es el turno del jugador %s \n", nom1);
                }
                else{
                    printf("Es el turno del jugador %s \n", nom2);
                }

                ingresar_movimiento(x,y);
                movimiento=nuevo_movimiento(partida,*x, *y );
                mostrar_tablero(tablero);

                if(movimiento==107)
                    printf("la posicion elegida para el movimiento no es valida, vuelva a ingresar el movimiento \n");
                else{
                    if(partida->turno_de == 100){
                        partida->turno_de=101;
                    }
                    else{
                        partida->turno_de=100;
                    }
                }


            }


            printf("el resultado de la partida es: \n");
            if(partida->estado == 109)
                printf(" Gana %s \n", nom1);
            if(partida-> estado == 110)
                printf(" Gana %s \n", nom2);
            if(partida-> estado == 112)
                printf(" EMPATE \n");


        }

        if(*opcion == 2){

            printf("Ingrese el nombre del jugador 1 \n " );
            scanf(" %s", nom1);
            printf("Ingrese el nombre de la computadora  \n " );
            scanf(" %s", nom2);

            quien_comienza(comienza, nom1, nom2);

            nueva_partida(&partida, 104, *comienza, nom1, nom2);

            tablero=partida->tablero;
            mostrar_tablero(tablero);




            while(partida->estado==111){

                if(partida->turno_de == 100){
                    printf("Es el turno del jugador %s \n", nom1);
                }
                else{
                    printf("Es el turno del jugador %s \n", nom2);
                }

                if(partida->turno_de == 100){
                    ingresar_movimiento(x,y);
                }
                else{

                    //actualiza el tablero despues de la jugada del otro jugador (Si o si debo crear un arbol nuevo para esto pues en el caso de que el algoritmo minmax haya decidido podar  el subarbol que contenia el movimiento que hizo el otro jugador en el turno anterior, nunca podre recorrer el arbol de busqueda y encontrar ese subarbol)
                    crear_busqueda_adversaria(&b, partida);
                    proximo_movimiento(b, x,y);
                    destruir_busqueda_adversaria(&b);
                }

                movimiento=nuevo_movimiento(partida,*x, *y );
                mostrar_tablero(tablero);

                if(movimiento==107)
                    printf("la posicion elegida para el movimiento no es valida, vuelva a ingresar el movimiento \n");
                else{
                    if(partida->turno_de == 100){
                        partida->turno_de=101;
                    }
                    else{
                        partida->turno_de=100;
                    }
                }
            }


            printf("el resultado de la partida es: \n");
            if(partida->estado == 109)
                printf(" Gana %s \n", nom1);
            if(partida-> estado == 110)
                printf(" Gana %s \n", nom2);
            if(partida-> estado == 112)
                printf(" EMPATE \n");
        }

         if(*opcion == 3){
            printf("Ingrese el nombre de la computadora 1 \n " );
            scanf(" %s", nom1);
            printf("Ingrese el nombre de la computadora 2 \n " );
            scanf(" %s", nom2);

            quien_comienza(comienza, nom1, nom2);

            nueva_partida(&partida, 105, *comienza, nom1, nom2);

            tablero=partida->tablero;
            mostrar_tablero(tablero);


            while(partida->estado==111){

                if(partida->turno_de == 100){
                    printf("Es el turno del jugador %s \n", nom1);
                }
                else{
                    printf("Es el turno del jugador %s \n", nom2);
                }

                if(partida->turno_de == 100){

                    crear_busqueda_adversaria(&b, partida);
                    proximo_movimiento(b, x,y);
                    destruir_busqueda_adversaria(&b);
                }
                else{

                    //actualiza el tablero despues de la jugada del otro jugador (Si o si debo crear un arbol nuevo para esto pues en el caso de que el algoritmo minmax haya decidido podar  el subarbol que contenia el movimiento que hizo el otro jugador en el turno anterior, nunca podre recorrer el arbol de busqueda y encontrar ese subarbol)
                    crear_busqueda_adversaria(&b, partida);
                    proximo_movimiento(b, x,y);
                    destruir_busqueda_adversaria(&b);
                }

                movimiento=nuevo_movimiento(partida,*x, *y );
                mostrar_tablero(tablero);

                if(movimiento==107)
                    printf("la posicion elegida para el movimiento no es valida, vuelva a ingresar el movimiento \n");
                else{
                    if(partida->turno_de == 100){
                        partida->turno_de=101;
                    }
                    else{
                        partida->turno_de=100;
                    }
                }
            }

            printf("el resultado de la partida es: \n");
            if(partida->estado == 109)
                printf(" Gana el %s \n", nom1);
            if(partida-> estado == 110)
                printf(" Gana el %s \n", nom2);
            if(partida-> estado == 112)
                printf(" EMPATE \n");

         }





    }






    return 0;
}


