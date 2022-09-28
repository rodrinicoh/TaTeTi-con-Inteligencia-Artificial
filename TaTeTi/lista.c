#include "lista.h"
#include <stdio.h>
#include <stdlib.h>

void crear_lista(tLista * l){
    *l= (tLista)malloc(sizeof( struct celda));
    if ((*l) == NULL) exit(LST_ERROR_MEMORIA);
    (*l)->siguiente=NULL;
    (*l)->elemento=NULL;
}

void l_insertar(tLista l, tPosicion p, tElemento e){
    tPosicion aux;
    aux=(tPosicion)malloc(sizeof(struct celda));
    if (aux == NULL) exit(LST_ERROR_MEMORIA);
    aux->elemento=e;
    aux->siguiente=p->siguiente;
    p->siguiente=aux;
}

void l_eliminar(tLista l, tPosicion p, void (*fEliminar)(tElemento)){
    tPosicion aux;

    if (p->siguiente == NULL)
        exit(LST_POSICION_INVALIDA);
    else
    {
        aux=p->siguiente;
        p->siguiente=aux->siguiente;
        fEliminar(aux->elemento);
        free(aux);
    }
}

void l_destruir(tLista * l, void (*fEliminar)(tElemento)){
    tPosicion cursor;
    tPosicion aux;
    cursor= (*l)->siguiente;
    while (  cursor != NULL   ){
        fEliminar(cursor->elemento);
        aux=cursor;
        cursor=cursor->siguiente;
        free(aux);
    }
    free(*l);
}

tElemento l_recuperar(tLista l, tPosicion p){
    if(p->siguiente == NULL){
        exit(LST_POSICION_INVALIDA);
    }
    return p->siguiente->elemento;
}

tPosicion l_siguiente(tLista l, tPosicion p){
    tPosicion aux;

    if(p->siguiente == NULL)
        exit(LST_NO_EXISTE_SIGUIENTE);
    if(p->siguiente->siguiente == NULL)
        exit(LST_NO_EXISTE_SIGUIENTE);
    aux=p->siguiente;

    return aux;
}


tPosicion l_anterior(tLista l, tPosicion p){
    tPosicion aux;
    if (p==l)
        exit(LST_NO_EXISTE_ANTERIOR);

    else{
        aux=l;
        while( aux->siguiente != p){
            aux=aux->siguiente;
        }
    }
    return aux;
}

tPosicion l_primera(tLista l){
    return l;
}

tPosicion l_fin(tLista l){
    tPosicion aux;
    aux=l;
    if(aux->siguiente!= NULL)
        while ( ( (aux->siguiente) != NULL ) ){
            aux=aux->siguiente;
        }

    return aux;
}

tPosicion l_ultima(tLista l){
    tPosicion cursor;
    cursor= l;

    if(cursor->siguiente != NULL)
        while ( ( (cursor->siguiente->siguiente) != NULL ) ){
            cursor=cursor->siguiente;
        }

    return cursor;
}
