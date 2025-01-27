#include <stdlib.h>
#include <stdio.h>
#include "lista.h"
#include "arbol.h"
#include "ia.h"

// Prototipos de funciones auxiliares.
static void ejecutar_min_max(tBusquedaAdversaria b);
static void crear_sucesores_min_max(tArbol a, tNodo n, int es_max, int alpha, int beta, int jugador_max, int jugador_min);
static int valor_utilidad(tEstado e, int jugador_max);
static tLista estados_sucesores(tEstado e, int ficha_jugador);
static void diferencia_estados(tEstado anterior, tEstado nuevo, int * x, int * y);
static tEstado clonar_estado(tEstado e);

int max2(int A, int B);
int min2(int A, int B);
void liberar_estado(tElemento e);
void recorrido_random(int componentes[2][3]);
void dummy(tElemento e);
int esta_ganando(tEstado T, int ficha, int*x, int*y);

void crear_busqueda_adversaria(tBusquedaAdversaria * b, tPartida p){
    int i, j;
    tEstado estado;

    (*b) = (tBusquedaAdversaria) malloc(sizeof(struct busqueda_adversaria));
    if ((*b) == NULL) exit(IA_ERROR_MEMORIA);

    estado = (tEstado) malloc(sizeof(struct estado));
    if (estado == NULL) exit(IA_ERROR_MEMORIA);

    // Se clona el estado del tablero de la partida, al estado inicial de la b�squeda adversaria.
    for(i=0; i<3; i++){
        for(j=0; j<3; j++){
            estado->grilla[i][j] = p->tablero->grilla[i][j];
        }
    }

	// Se asume que el estado de la partida es PART_EN_JUEGO por lo que, la utilidad del estado
	// inicialmente es IA_NO_TERMINO
    estado->utilidad = IA_NO_TERMINO;

    // Inicializa los valores que representar�n a los jugadores MAX y MIN respectivamente.
    (*b)->jugador_max = p->turno_de;
    (*b)->jugador_min = (p->turno_de == PART_JUGADOR_1) ? PART_JUGADOR_2 : PART_JUGADOR_1;

    // Inicializa un �rbol para la b�squeda adversaria inicialmente vac�o.
    crear_arbol(&((*b)->arbol_busqueda));

    // Inicializa la ra�z del �rbol de b�squeda con el estado del tablero T.
    crear_raiz((*b)->arbol_busqueda, estado);

    // Ejecuta algoritmo Min-Max con podas Alpha-Beta.
    ejecutar_min_max((*b));
}

/**
>>>>>  A IMPLEMENTAR   <<<<<
*/

/**
    Libera la memoria ocupada por e
*/

void liberar_estado(tElemento e){
    tEstado est=(tEstado)e;
    free(&est->utilidad);
    free(est->grilla);
}

void proximo_movimiento(tBusquedaAdversaria b, int * x, int * y){
    tArbol arbol;
    tLista hijos;
    tPosicion hijo;
    tNodo raiz;
    tNodo nodo;
    tEstado e;
    tEstado ganador;
    tEstado ganador_optativo;
    tEstado empate;
    tEstado mejor_movimiento;
    tEstado estado_actual;


    ganador=(tEstado)malloc(sizeof(struct estado));
    if (ganador == NULL) exit(IA_ERROR_MEMORIA);
    empate=(tEstado)malloc(sizeof(struct estado));
    if (empate == NULL) exit(IA_ERROR_MEMORIA);
    mejor_movimiento=(tEstado)malloc(sizeof(struct estado));
    if (mejor_movimiento == NULL) exit(IA_ERROR_MEMORIA);
    estado_actual=(tEstado)malloc(sizeof(struct estado));
    if (estado_actual == NULL) exit(IA_ERROR_MEMORIA);



    arbol=b->arbol_busqueda;
    raiz=a_raiz(arbol);
    estado_actual=(tEstado)a_recuperar(arbol,raiz);
    hijos=a_hijos(arbol, raiz);
    if( !( ( l_primera(hijos)==l_ultima(hijos) ) && (l_ultima(hijos)==l_fin(hijos) ) ) ) {//si el nodo del arbol tiene hijos
        hijo=l_primera(hijos);
        ganador=NULL;
        empate=NULL;
        ganador_optativo=NULL;
        while(hijo!=NULL && ganador==NULL){
            nodo=(tNodo)l_recuperar(hijos, hijo);
            e=(tEstado)a_recuperar(arbol,nodo);

            if(e->utilidad==1003){
                if(e->utilidad==valor_utilidad(e, b->jugador_max)){
                ganador=e;
                }
                ganador_optativo=e;

            }
            if(e->utilidad==1002){
                if(valor_utilidad(e, b->jugador_max)!=1001 ){
                    empate=e;
                }
            }

            if(hijo==l_ultima(hijos))
                hijo=NULL;
            else
                hijo=l_siguiente(hijos, hijo);
        }


        if(ganador!=NULL && empate==NULL)
            mejor_movimiento=ganador;
        else{
            if(empate!=NULL && ganador==NULL)
                mejor_movimiento=empate;
            else{
                if(empate!=NULL && ganador!=NULL){
                    if(e->utilidad!=valor_utilidad(e, b->jugador_max))
                        mejor_movimiento=empate;
                    else
                        mejor_movimiento=ganador;
                }
                else{
                    if(ganador_optativo!=NULL)
                        mejor_movimiento=ganador_optativo;
                    else
                        mejor_movimiento=e;//movimiento perdedor
                }
            }
        }

        diferencia_estados(estado_actual, mejor_movimiento, x, y);

    }


}


/**
>>>>>  A IMPLEMENTAR   <<<<<
**/
void destruir_busqueda_adversaria(tBusquedaAdversaria * b){
    void * liberar;
    liberar=liberar_estado;
    a_destruir( &((*b)->arbol_busqueda) , liberar);
}

// ===============================================================================================================
// FUNCIONES Y PROCEDEMIENTOS AUXILIARES
// ===============================================================================================================

/**
Ordena la ejecuci�n del algoritmo Min-Max para la generaci�n del �rbol de b�squeda adversaria, considerando como
estado inicial el estado de la partida almacenado en el �rbol almacenado en B.
**/
static void ejecutar_min_max(tBusquedaAdversaria b){
    tArbol a = b->arbol_busqueda;
    tNodo r = a_raiz(a);
    int jugador_max = b->jugador_max;
    int jugador_min = b->jugador_min;

    crear_sucesores_min_max(a, r, 1, IA_INFINITO_NEG, IA_INFINITO_POS, jugador_max, jugador_min);
}

/**
    No hace nada.
*/
void dummy(tElemento e){
}

/**
    Calcula el valor minimo entre 2 parametros
**/
int min2(int A, int B){
    int min=0;
    if(A==1000)
        min=A;
    if(B==1000)
        min=B;
    if(min==0){
        if(A<B){
            min=A;
        }
        else{
            min=B;
        }
    }
    return min;
}


/**
    Calcula el valor maximo entre 2 parametros
**/
int max2(int A, int B){
    int max=0;
    if(A==1004)
        max=A;
    if(B==1004)
        max=B;
    if(max==0){
        if(A<=B ){
            max=B;
        }
        else{
            max=A;
        }
    }
    return max;
}


/**
>>>>>  A IMPLEMENTAR   <<<<<
Implementa la estrategia del algoritmo Min-Max con podas Alpha-Beta, a partir del estado almacenado en N.
- A referencia al �rbol de b�squeda adversaria.
- N referencia al nodo a partir del cual se construye el sub�rbol de b�squeda adversaria.
- ES_MAX indica si N representa un nodo MAX en el �rbol de b�squeda adversaria.
- ALPHA y BETA indican sendos valores correspondientes a los nodos ancestros a N en el �rbol de b�squeda A.
- JUGADOR_MAX y JUGADOR_MIN indican las fichas con las que juegan los respectivos jugadores.
**/
static void crear_sucesores_min_max(tArbol a, tNodo n, int es_max, int alpha, int beta, int jugador_max, int jugador_min){
    int mejor_valor_sucesores;
    int valor_sucesor;
    int parar;
    int ficha_jugador;
    tNodo aux;
    tEstado e;
    tEstado estado_sucesor;
    tPosicion posicion_sucesor;
    tLista sucesores;
    void * liberar;


    e=(tEstado)malloc(sizeof(struct nodo));
    if (e == NULL) exit(IA_ERROR_MEMORIA);
    estado_sucesor=(tEstado)malloc(sizeof(struct nodo));
    if (estado_sucesor == NULL) exit(IA_ERROR_MEMORIA);

    liberar=dummy;
    e=(tEstado)n->elemento;
    e->utilidad=valor_utilidad(e, jugador_max);

    if( e->utilidad==1005 ){//no estoy en un estado terminal
        if(es_max==1){
            mejor_valor_sucesores = 1000;//INFINITO NEG

            if(jugador_min==100)
                ficha_jugador=1;
            else
                ficha_jugador=0;

            sucesores=estados_sucesores(e, ficha_jugador);//jugador min
            posicion_sucesor=l_primera(sucesores);

            parar=0;
            //recorro los sucesores (y el subarbol correspondiente a cada uno) buscando cual tiene como ganador a jugador max, cual logra un empate y cual tiene como perdedor a jugador max
            //El mejor_valor_sucesores sera el que tenga como GANADOR a jugador max
            //busco el mayor valor entre los sucesores y se lo asigno a mi estado sucesor
            while( posicion_sucesor!=NULL && parar==0){
                estado_sucesor=(tEstado)l_recuperar(sucesores,posicion_sucesor);
                //agrego el sucesor al arbol a con padre n
                aux=a_insertar(a,n,NULL, estado_sucesor);
                //llamo recursivamente al metodo para actualizar los valores alpha y beta con el sucesor aux
                crear_sucesores_min_max(a,aux, 0, alpha, beta, jugador_max, jugador_min);
                valor_sucesor=estado_sucesor->utilidad;
                mejor_valor_sucesores=max2(mejor_valor_sucesores, valor_sucesor);
                alpha = max2(alpha, mejor_valor_sucesores);
                if(beta<=alpha)
                    parar=1;
                e->utilidad=mejor_valor_sucesores;

                if(posicion_sucesor==l_ultima(sucesores))
                    posicion_sucesor=NULL;
                else
                    posicion_sucesor=l_siguiente(sucesores, posicion_sucesor);
            }

            l_destruir(&sucesores,liberar);

        }else{
            mejor_valor_sucesores = 1004;//INFINITO POS
            if(jugador_max==100)
                ficha_jugador=1;
            else
                ficha_jugador=0;

            sucesores=estados_sucesores(e, ficha_jugador);//jugador_max
            posicion_sucesor=l_primera(sucesores);
            parar=0;
            //recorro los sucesores (y el subarbol correspondiente a cada uno) buscando cual tiene como ganador a jugador max, cual logra un empate y cual tiene como perdedor a jugador max
            //El mejor_valor_sucesores sera el que tenga como PERDEDOR a jugador max
            //busco el menor valor entre los sucesores y se lo asigno a mi estado sucesor
            while( posicion_sucesor!=NULL && parar==0){
                estado_sucesor=(tEstado)l_recuperar(sucesores,posicion_sucesor);
                //agrego el sucesor al arbol a
                aux=a_insertar(a,n,NULL, estado_sucesor);
                //llamo recursivamente al metodo para actualizar los valores alpha y beta con el sucesor aux
                //De esta manera recorro el subarbol con raiz en aux
                crear_sucesores_min_max(a,aux, 1, alpha, beta, jugador_max, jugador_min);
                valor_sucesor=estado_sucesor->utilidad;;
                mejor_valor_sucesores = min2(mejor_valor_sucesores, valor_sucesor);
                beta = min2(beta, mejor_valor_sucesores);
                if(beta<=alpha)
                    parar=1;
                e->utilidad=mejor_valor_sucesores;

                if(posicion_sucesor==l_ultima(sucesores))
                    posicion_sucesor=NULL;
                else
                    posicion_sucesor=l_siguiente(sucesores, posicion_sucesor);
            }
            l_destruir(&sucesores,liberar);
        }
    }
}





/**
>>>>>  A IMPLEMENTAR   <<<<<
Computa el valor de utilidad correspondiente al estado E, y la ficha correspondiente al JUGADOR_MAX, retornado:
- IA_GANA_MAX si el estado E refleja una jugada en el que el JUGADOR_MAX gan� la partida.
- IA_EMPATA_MAX si el estado E refleja una jugada en el que el JUGADOR_MAX empat� la partida.
- IA_PIERDE_MAX si el estado E refleja una jugada en el que el JUGADOR_MAX perdi� la partida.
- IA_NO_TERMINO en caso contrario.
**/
static int valor_utilidad(tEstado e, int jugador_max){//jugador es 100 o 101
    int resultado=0;
    switch (jugador_max){
        case(100):{
            //casos donde gana max
            if( resultado==0 && (e->grilla[0][0]==0 && e->grilla[0][1]==0 && e->grilla[0][2]==0 ) )
                resultado=1003;
            if( resultado==0 && (e->grilla[1][0]==0 && e->grilla[1][1]==0 && e->grilla[1][2]==0 ) )
                resultado=1003;
            if( resultado==0 && (e->grilla[2][0]==0 && e->grilla[2][1]==0 && e->grilla[2][2]==0 ) )
                resultado=1003;
            if( resultado==0 && (e->grilla[0][0]==0 && e->grilla[1][0]==0 && e->grilla[2][0]==0 ) )
                resultado=1003;
            if( resultado==0 && (e->grilla[0][1]==0 && e->grilla[1][1]==0 && e->grilla[2][1]==0 ) )
                resultado=1003;
            if( resultado==0 && (e->grilla[0][2]==0 && e->grilla[1][2]==0 && e->grilla[2][2]==0 ) )
                resultado=1003;
            if( resultado==0 && (e->grilla[0][0]==0 && e->grilla[1][1]==0 && e->grilla[2][2]==0 ) )
                resultado=1003;
            if( resultado==0 && (e->grilla[0][2]==0 && e->grilla[1][1]==0 && e->grilla[2][0]==0 ) )
                resultado=1003;

            //casos donde pierde max
            if( resultado==0 && (e->grilla[0][0]==1 && e->grilla[0][1]==1 && e->grilla[0][2]==1 ) )
                resultado=1001;
            if( resultado==0 && (e->grilla[1][0]==1 && e->grilla[1][1]==1 && e->grilla[1][2]==1 ) )
                resultado=1001;
            if( resultado==0 && (e->grilla[2][0]==1 && e->grilla[2][1]==1 && e->grilla[2][2]==1 ) )
                resultado=1001;
            if( resultado==0 && (e->grilla[0][0]==1 && e->grilla[1][0]==1 && e->grilla[2][0]==1 ) )
                resultado=1001;
            if( resultado==0 && (e->grilla[0][1]==1 && e->grilla[1][1]==1 && e->grilla[2][1]==1 ) )
                resultado=1001;
            if( resultado==0 && (e->grilla[0][2]==1 && e->grilla[1][2]==1 && e->grilla[2][2]==1 ) )
                resultado=1001;
            if( resultado==0 && (e->grilla[0][0]==1 && e->grilla[1][1]==1 && e->grilla[2][2]==1 ) )
                resultado=1001;
            if( resultado==0 && (e->grilla[0][2]==1 && e->grilla[1][1]==1 && e->grilla[2][0]==1 ) )
                resultado=1001;
            break;
        }
        case(101):{
            //casos donde gana max
            if( resultado==0 && (e->grilla[0][0]==1 && e->grilla[0][1]==1 && e->grilla[0][2]==1 ) )
                resultado=1003;
            if( resultado==0 && (e->grilla[1][0]==1 && e->grilla[1][1]==1 && e->grilla[1][2]==1 ) )
                resultado=1003;
            if( resultado==0 && (e->grilla[2][0]==1 && e->grilla[2][1]==1 && e->grilla[2][2]==1 ) )
                resultado=1003;
            if( resultado==0 && (e->grilla[0][0]==1 && e->grilla[1][0]==1 && e->grilla[2][0]==1 ) )
                resultado=1003;
            if( resultado==0 && (e->grilla[0][1]==1 && e->grilla[1][1]==1 && e->grilla[2][1]==1 ) )
                resultado=1003;
            if( resultado==0 && (e->grilla[0][2]==1 && e->grilla[1][2]==1 && e->grilla[2][2]==1 ) )
                resultado=1003;
            if( resultado==0 && (e->grilla[0][0]==1 && e->grilla[1][1]==1 && e->grilla[2][2]==1 ) )
                resultado=1003;
            if( resultado==0 && (e->grilla[0][2]==1 && e->grilla[1][1]==1 && e->grilla[2][0]==1 ) )
                resultado=1003;

            //casos donde pierde max
            if( resultado==0 && (e->grilla[0][0]==0 && e->grilla[0][1]==0 && e->grilla[0][2]==0 ) )
                resultado=1001;
            if( resultado==0 && (e->grilla[1][0]==0 && e->grilla[1][1]==0 && e->grilla[1][2]==0 ) )
                resultado=1001;
            if( resultado==0 && (e->grilla[2][0]==0 && e->grilla[2][1]==0 && e->grilla[2][2]==0 ) )
                resultado=1001;
            if( resultado==0 && (e->grilla[0][0]==0 && e->grilla[1][0]==0 && e->grilla[2][0]==0 ) )
                resultado=1001;
            if( resultado==0 && (e->grilla[0][1]==0 && e->grilla[1][1]==0 && e->grilla[2][1]==0 ) )
                resultado=1001;
            if( resultado==0 && (e->grilla[0][2]==0 && e->grilla[1][2]==0 && e->grilla[2][2]==0 ) )
                resultado=1001;
            if( resultado==0 && (e->grilla[0][0]==0 && e->grilla[1][1]==0 && e->grilla[2][2]==0 ) )
                resultado=1001;
            if( resultado==0 && (e->grilla[0][2]==0 && e->grilla[1][1]==0 && e->grilla[2][0]==0 ) )
                resultado=1001;
        }
    }

    //caso donde empata max
    if(resultado==0 && e->grilla[0][0]!=2 && e->grilla[0][1]!=2 && e->grilla[0][2]!=2 && e->grilla[1][0]!=2 && e->grilla[1][1]!=2 && e->grilla[1][2]!=2 && e->grilla[2][0]!=2 && e->grilla[2][1]!=2 && e->grilla[2][2]!=2 )
        resultado=1002;

    //si no empato, no perdio y no gano, la partida sigue
    if(resultado==0)
        resultado=1005;


    return resultado;

}

/**
    Inicializa una matriz donde:
    La fila 0 representa las filas de la grilla de juego ordenadas de manera aleatoria
    La fila 1 representa las columnas de la grilla de juego ordenadas de manera aleatoria
**/

void recorrido_random(int componentes[2][3]){
    int parar;
    int randi1;
    int randi2;
    int randi3;
    int randj1;
    int randj2;
    int randj3;
    if(componentes[0][0]!= 0 && componentes[0][0]!= 1 && componentes[0][0]!= 2)
        randi1=rand()%3;
    else
        randi1=componentes[0][0];
    parar=0;
    while(parar==0){
        randi2=rand()%3;
        if(randi2!=randi1)
            parar=1;
    }
    parar=0;
    while(parar==0){
        randi3=rand()%3;
        if(randi3!=randi1 && randi3!=randi2)
            parar=1;
    }

    componentes[0][0]=randi1;
    componentes[0][1]=randi2;
    componentes[0][2]=randi3;

    if(componentes[1][0]!= 0 && componentes[1][0]!=1 && componentes[1][0]!= 2)
        randj1=rand()%3;
    else
        randj1=componentes[1][0];

    parar=0;
    while(parar==0){
        randj2=rand()%3;
        if(randj2!=randj1)
            parar=1;
    }
    parar=0;
    while(parar==0){
        randj3=rand()%3;
        if(randj3!=randj1 && randj3!=randj2)
            parar=1;
    }
    componentes[1][0]=randj1;
    componentes[1][1]=randj2;
    componentes[1][2]=randj3;

}



/**
    Verifica en el estado de juego T, si el jugador que usa la ficha pasada por par�metro, puede ganar en el pr�ximo movimiento y retorna 1 si es as�,
    junto con el movimiento m�s aconsejable a realizar en el siguiente turno.
    En el caso de que el jugador que usa la ficha pasada por par�metro no est� por ganar, retorna 0.
*/

int esta_ganando(tEstado T, int ficha, int*x, int*y){
    int resultado;
    int ficha_contraria;
    resultado=0;

    if(ficha==0)
        ficha_contraria=1;
    else
        ficha_contraria=0;


    if(resultado==0 && ((T->grilla[0][0]==ficha && T->grilla[2][2]==ficha && T->grilla[1][1]!=ficha_contraria) || (T->grilla[0][1]==ficha && T->grilla[2][1]==ficha && T->grilla[1][1]!=ficha_contraria) || (T->grilla[0][2]==ficha && T->grilla[2][0]==ficha && T->grilla[1][1]!=ficha_contraria) || (T->grilla[1][0]==ficha && T->grilla[1][2]==ficha)) ){
        resultado=1;
        if(x!=NULL && y!=NULL){
            *x=1;
            *y=1;
        }
    }
    if(resultado==0 && ((T->grilla[0][2]==ficha && T->grilla[1][2]==ficha && T->grilla[2][2]!=ficha_contraria) || (T->grilla[0][0]==ficha && T->grilla[1][1]==ficha && T->grilla[2][2]!=ficha_contraria) || (T->grilla[2][0]==ficha && T->grilla[2][1]==ficha && T->grilla[2][2]!=ficha_contraria)) ){
        resultado=1;
        if(x!=NULL && y!=NULL){
            *x=2;
            *y=2;
        }
    }
    if(resultado==0 && ((T->grilla[2][0]==ficha && T->grilla[2][2]==ficha && T->grilla[2][1]!=ficha_contraria) || (T->grilla[0][1]==ficha && T->grilla[1][1]==ficha && T->grilla[2][1]!=ficha_contraria))){
        resultado=1;
        if(x!=NULL && y!=NULL){
            *x=2;
            *y=1;
        }
    }
    if(resultado==0 && ((T->grilla[2][1]==ficha && T->grilla[2][2]==ficha && T->grilla[2][0]!=ficha_contraria) || (T->grilla[0][2]==ficha && T->grilla[1][1]==ficha && T->grilla[2][0]!=ficha_contraria) || (T->grilla[1][0]==ficha && T->grilla[0][0]==ficha && T->grilla[2][0]!=ficha_contraria) )){
        resultado=1;
        if(x!=NULL && y!=NULL){
            *x=2;
            *y=0;
        }
    }
    if(resultado==0 && ((T->grilla[1][1]==ficha && T->grilla[1][2]==ficha && T->grilla[1][0]!=ficha_contraria) || (T->grilla[0][0]==ficha && T->grilla[0][2]==ficha && T->grilla[1][0]!=ficha_contraria))){
        resultado=1;
        if(x!=NULL && y!=NULL){
            *x=1;
            *y=0;
        }
    }
    if(resultado==0 && ((T->grilla[1][1]==ficha && T->grilla[2][2]==ficha && T->grilla[0][0]!=ficha_contraria) || (T->grilla[0][1]==ficha && T->grilla[0][2]==ficha && T->grilla[0][0]!=ficha_contraria) || (T->grilla[1][0]==ficha && T->grilla[1][2]==ficha && T->grilla[0][0]!=ficha_contraria) )){
        resultado=1;
        if(x!=NULL && y!=NULL){
            *x=0;
            *y=0;
        }
    }
    if(resultado==0 && ((T->grilla[1][1]==ficha && T->grilla[2][1]==ficha && T->grilla[0][1]!=ficha_contraria) || (T->grilla[0][0]==ficha && T->grilla[0][2]==ficha && T->grilla[0][1]!=ficha_contraria))){
        resultado=1;
        if(x!=NULL && y!=NULL){
            *x=0;
            *y=1;
        }
    }
    if(resultado==0 && ((T->grilla[1][1]==ficha && T->grilla[2][0]==ficha && T->grilla[0][2]!=ficha_contraria) || (T->grilla[0][0]==ficha && T->grilla[0][1]==ficha && T->grilla[0][2]!=ficha_contraria) || (T->grilla[1][2]==ficha && T->grilla[2][2]==ficha && T->grilla[0][2]!=ficha_contraria) )){
        resultado=1;
        if(x!=NULL && y!=NULL){
            *x=0;
            *y=2;
        }
    }
    if(resultado==0 && ((T->grilla[1][1]==ficha && T->grilla[1][0]==ficha && T->grilla[1][2]!=ficha_contraria) || (T->grilla[0][2]==ficha && T->grilla[2][2]==ficha && T->grilla[1][2]!=ficha_contraria))){
        resultado=1;
        if(x!=NULL && y!=NULL){
            *x=1;
            *y=2;
        }
    }
    return resultado;
}







/**
>>>>>  A IMPLEMENTAR   <<<<<
Computa y retorna una lista con aquellos estados que representan estados sucesores al estado E.
Un estado sucesor corresponde a la clonaci�n del estado E, junto con la incorporaci�n de un nuevo movimiento
realizado por el jugador cuya ficha es FICHA_JUGADOR por sobre una posici�n que se encuentra libre en el estado E.
La lista de estados sucesores se debe ordenar de forma aleatoria, de forma tal que una doble invocaci�n de la funci�n
estados_sucesores(estado, ficha) retornar�a dos listas L1 y L2 tal que:
- L1 y L2 tienen exactamente los mismos estados sucesores de ESTADO a partir de jugar FICHA.
- El orden de los estado en L1 posiblemente sea diferente al orden de los estados en L2.
**/
static tLista estados_sucesores(tEstado e, int ficha_jugador){
    tLista L;
    int componentes[2][3];
    int x;
    int y;
    int*i;
    int*j;
    int fila;
    int columna;
    int ficha_oponente;
    crear_lista(&L);
    tEstado aux;

    aux=(tEstado)malloc(sizeof(struct nodo));
    if (aux == NULL) exit(IA_ERROR_MEMORIA);
    i=(int*)malloc(sizeof(struct nodo));
    if (i == NULL) exit(IA_ERROR_MEMORIA);
    j=(int*)malloc(sizeof(struct nodo));
    if (j == NULL) exit(IA_ERROR_MEMORIA);

    if(ficha_jugador==0)
        ficha_oponente=1;
    else
        ficha_oponente=0;


    if(esta_ganando(e, ficha_oponente, i, j)==1){
        componentes[0][0]=*i;
        componentes[1][0]=*j;
    }



    //inicializo la matriz componentes
    recorrido_random(componentes);





    for (x=0;x<3;x++){
        for(y=0; y<3; y++){

            fila=componentes[0][x];//elije una fila de la grilla de juego de manera aleatoria
            columna=componentes[1][y];//elije una columna de la grilla de juego de manera aleatoria

            if(e->grilla[fila][columna]==2){//si el espacio esta vacio
                aux=clonar_estado(e);
                aux->grilla[fila][columna]=ficha_jugador;
                l_insertar(L,l_ultima(L),aux);
            }
        }
    }

    return L;

}


/**
>>>>>  A IMPLEMENTAR   <<<<<
Inicializa y retorna un nuevo estado que resulta de la clonaci�n del estado E.
Para esto copia en el estado a retornar los valores actuales de la grilla del estado E, como su valor
de utilidad.
**/
static tEstado clonar_estado(tEstado e){
    tEstado aux;
    int i;
    int j;

    aux=(tEstado) malloc(sizeof(struct estado));
    if (aux == NULL) exit(IA_ERROR_MEMORIA);

    aux->utilidad=e->utilidad;
    i=0;
    j=0;
    for (i=0;i<3;i++){
        for(j=0;j<3 ; j++){
            aux->grilla[i][j]=e->grilla[i][j];
        }

    }

    return aux;
}

/**
Computa la diferencia existente entre dos estados.
Se asume que entre ambos existe s�lo una posici�n en el que la ficha del estado anterior y nuevo difiere.
La posici�n en la que los estados difieren, es retornada en los par�metros *X e *Y.
**/
static void diferencia_estados(tEstado anterior, tEstado nuevo, int * x, int * y){
    int i,j, hallado = 0;
    for(i=0; i<3 && !hallado; i++){
        for(j=0; j<3 && !hallado; j++){
            if (anterior->grilla[i][j] != nuevo->grilla[i][j]){
                *x = i;
                *y = j;
                hallado = 1;
            }
        }
    }
}






