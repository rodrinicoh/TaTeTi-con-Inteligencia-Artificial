#include "partida.h"
#include <stdio.h>
#include <stdlib.h>

void nueva_partida(tPartida * p, int modo_partida, int comienza, char * j1_nombre, char * j2_nombre)
{
    int i;
    int j;
    tTablero T;

    T=(tTablero)malloc(sizeof(struct tablero));
    if(T==NULL ) exit(PART_ERROR_MEMORIA);

    (*p)=(tPartida)malloc( sizeof (struct partida));
    if((*p)==NULL ) exit(PART_ERROR_MEMORIA);
    (*p)->modo_partida=modo_partida;
    (*p)->turno_de=comienza;
    ((*p)->tablero)=(tTablero)malloc(sizeof (struct tablero));
    if((*p)->tablero==NULL ) exit(PART_ERROR_MEMORIA);
    (*p)->estado=111;//en curso

    T=(*p)->tablero;
    for(i=0; i<3 ; i++){
        for(j=0; j<3 ; j++){
            T->grilla[i][j]=2;
        }
    }

    (*p)->nombre_jugador_1[0]=*j1_nombre;
    (*p)->nombre_jugador_2[0]=*j2_nombre;
}




int nuevo_movimiento(tPartida p, int mov_x, int mov_y){
    tTablero T;
    int retorno;
    int resultado;



    T=p->tablero;
    resultado=0;
    if( (mov_x!=0 && mov_x!=1 && mov_x!=2)  || (mov_y!=0 && mov_y!=1 && mov_y!=2))
        retorno=107;
    else{
        if(T->grilla[mov_x][mov_y]!=0 && T->grilla[mov_x][mov_y]!=1 ){
            retorno=106;
            if(p->turno_de==100){
                T->grilla[mov_x][mov_y]=0;
            }
            if(p->turno_de==101){
                T->grilla[mov_x][mov_y]=1;
            }




            switch (p->turno_de){
                    case(100):{
                        if( resultado==0 && (T->grilla[0][0]==0 && T->grilla[0][1]==0 && T->grilla[0][2]==0 ) )
                            resultado=109;
                        if( resultado==0 && (T->grilla[1][0]==0 && T->grilla[1][1]==0 && T->grilla[1][2]==0 ) )
                            resultado=109;
                        if( resultado==0 && (T->grilla[2][0]==0 && T->grilla[2][1]==0 && T->grilla[2][2]==0 ) )
                            resultado=109;
                        if( resultado==0 && (T->grilla[0][0]==0 && T->grilla[1][0]==0 && T->grilla[2][0]==0 ) )
                            resultado=109;
                        if( resultado==0 && (T->grilla[0][1]==0 && T->grilla[1][1]==0 && T->grilla[2][1]==0 ) )
                            resultado=109;
                        if( resultado==0 && (T->grilla[0][2]==0 && T->grilla[1][2]==0 && T->grilla[2][2]==0 ) )
                            resultado=109;
                        if( resultado==0 && (T->grilla[0][0]==0 && T->grilla[1][1]==0 && T->grilla[2][2]==0 ) )
                            resultado=109;
                        if( resultado==0 && (T->grilla[0][2]==0 && T->grilla[1][1]==0 && T->grilla[2][0]==0 ) )
                            resultado=109;
                        break;
                    }
                    case(101):{

                        if( resultado==0 && (T->grilla[0][0]==1 && T->grilla[0][1]==1 && T->grilla[0][2]==1 ) )
                            resultado=110;
                        if( resultado==0 && (T->grilla[1][0]==1 && T->grilla[1][1]==1 && T->grilla[1][2]==1 ) )
                            resultado=110;
                        if( resultado==0 && (T->grilla[2][0]==1 && T->grilla[2][1]==1 && T->grilla[2][2]==1 ) )
                            resultado=110;
                        if( resultado==0 && (T->grilla[0][0]==1 && T->grilla[1][0]==1 && T->grilla[2][0]==1 ) )
                            resultado=110;
                        if( resultado==0 && (T->grilla[0][1]==1 && T->grilla[1][1]==1 && T->grilla[2][1]==1 ) )
                            resultado=110;
                        if( resultado==0 && (T->grilla[0][2]==1 && T->grilla[1][2]==1 && T->grilla[2][2]==1 ) )
                            resultado=110;
                        if( resultado==0 && (T->grilla[0][0]==1 && T->grilla[1][1]==1 && T->grilla[2][2]==1 ) )
                            resultado=110;
                        if( resultado==0 && (T->grilla[0][2]==1 && T->grilla[1][1]==1 && T->grilla[2][0]==1 ) )
                            resultado=110;
                        break;
                    }
            }

            if(resultado==0 && T->grilla[0][0]!=2 && T->grilla[0][1]!=2 && T->grilla[0][2]!=2 && T->grilla[1][0]!=2 && T->grilla[1][1]!=2 && T->grilla[1][2]!=2 && T->grilla[2][0]!=2 && T->grilla[2][1]!=2 && T->grilla[2][2]!=2 )
                resultado=112;

        }else
            retorno=107;
    }
    if(resultado!=0)
        p->estado=resultado;
    return retorno;
}

void finalizar_partida(tPartida * p){
    free((*p)->tablero->grilla);
    free((*p)->tablero);
    free(*p);
}
