#include "Arbol.h"
#include "lista.h"
#include <stdio.h>
#include <stdlib.h>



void eliminaraux(tArbol a, tNodo n);
void liberar_memoria(tElemento e);
void anular (tElemento e);


/**
    Libera la memoria ocupada por el elemento e.
*/

void liberar_memoria(tElemento e){
        free(e);
}

/**
    No hace nada.
*/

void anular (tElemento e){

}


void crear_arbol(tArbol * a){
    *a= (tArbol)malloc(sizeof( struct arbol));
    if ((*a) == NULL) exit(ARB_ERROR_MEMORIA);
    (*a)->raiz=NULL;
}

void crear_raiz(tArbol a, tElemento e){
    tNodo aux=a->raiz;


    if(aux != NULL)
        exit(ARB_OPERACION_INVALIDA);

    aux=(tNodo)malloc(sizeof( struct nodo));
    if (aux == NULL) exit(ARB_ERROR_MEMORIA);

    aux->elemento=e;
    aux->padre=NULL;

    crear_lista(&(aux->hijos));

    a->raiz=aux;
}



tNodo a_insertar(tArbol a, tNodo np, tNodo nh, tElemento e){
    tNodo nuevo;
    tPosicion cursor;
    int pertenece;
    tLista hijos;

    pertenece=0;//falso
    nuevo=(tNodo)malloc(sizeof( struct nodo));
    if (nuevo == NULL) exit(ARB_ERROR_MEMORIA);

    nuevo->elemento=e;
    nuevo->padre=np;
    crear_lista(&(nuevo->hijos));
    hijos=np->hijos;


    if(nh== NULL){

        //inserto el nodo nuevo al final de la lista de hijos de np
        l_insertar(hijos, l_fin(hijos) , nuevo);

    }
    else {


        //buscar nh en la lista de hijos de np
        cursor=l_primera(hijos);
        if( (l_primera(hijos) == l_ultima(hijos)) && (l_ultima(hijos)== (l_fin(hijos))) )
            cursor=NULL; //np no tiene hijos (su lista de hijos esta vacia)


        //recorro los hijos de np buscando a nh
        while(cursor!= NULL && pertenece==0){

            if(l_recuperar(hijos, cursor)== nh) //comparo direcciones en memoria
                pertenece=1;
            else{

                if(cursor==l_ultima(hijos))
                    cursor=NULL;
                else
                    cursor=l_siguiente(hijos,cursor);

            }

        }
        if(pertenece==1)
            l_insertar(hijos, cursor , nuevo);
        else
            exit(ARB_POSICION_INVALIDA);
    }

    return nuevo;
}






void a_eliminar(tArbol a, tNodo n, void (*fEliminar)(tElemento)){
    tNodo raiza;
    tNodo aux;
    tLista hijosn;
    tLista hijos;
    tPosicion cursor;
    tPosicion cursoraux;


    raiza=a->raiz;


    if (n==raiza){

        hijosn=raiza->hijos;

        if( (l_primera(hijosn) == l_ultima(hijosn)) && (l_ultima(hijosn) ==l_fin(hijosn)) ){

            fEliminar(raiza->elemento);
            free(raiza);
            l_destruir(&(raiza->hijos), anular);
            a->raiz=NULL;

        }else{//la raiz tiene al menos un hijo

            if(l_primera(hijosn) == l_ultima(hijosn) ){ //la raiz tiene exactamente un hijo (ya habiamos descartado la opcion de que size sea 0)
                a->raiz=(tNodo)l_recuperar(hijosn,l_primera(hijosn));
                fEliminar(raiza->elemento);
                free(raiza);
                l_destruir(&(raiza->hijos), anular);
            }else{
                exit(ARB_OPERACION_INVALIDA);
            }

        }

    }else {

        hijos=n->padre->hijos;
        cursor=l_primera(hijos);
        while( (l_recuperar(hijos, cursor) != n) && (cursor!=NULL) ){
                if(cursor!=l_ultima(hijos))
                    cursor=l_siguiente(hijos, cursor);
                else
                    cursor=NULL;
        }


        if(cursor==NULL)//alguna referencia estaba mal hecha
            exit(ARB_OPERACION_INVALIDA);


        hijosn=n->hijos;
        l_eliminar(hijos, cursor, anular);
        if(   !( (l_primera(hijosn) == l_ultima(hijosn)) && (l_ultima(hijosn) == l_fin(hijosn) ) ) ) {

            cursoraux=l_primera(hijosn);//cursor aux recorre los hijos de n



            //paso todos los hijos de n a la lista de hijos del padre de n
            while(cursoraux!=NULL){

                aux=(tNodo)l_recuperar(hijosn, cursoraux);
                aux->padre=n->padre;
                l_insertar(hijos, cursor, aux  ); //inserto a cursoraux en la lista de hijos del padre de n

                if(cursoraux==l_ultima(hijosn))
                    cursoraux=NULL;
                else
                    cursoraux=l_siguiente(hijosn,cursoraux);

            }
        }

        //libero la memoria de n
        l_destruir(&hijosn, anular);
        fEliminar(n->elemento);
        free(hijosn);//solo quiero liberar la memoria del puntero, no de cada nodo
        free(n);
    }
}
/**
Recorre un árbol en posorden a partir del nodo n, liberando la memoria de este nodo y de sus hijos.

*/

void destruir_recursivo(tLista hijos, void (*fEliminar)(tElemento) ,tNodo n){ //metodo auxiliar

        tPosicion cursor;
        tNodo elem;

        if( !( (l_primera(hijos) == l_ultima(hijos)) && (l_ultima(hijos)== (l_fin(hijos))) ) ){//si la lista de hijos no esta vacia
            cursor=l_primera(hijos);

            while(cursor!=NULL){//recorro la lsita de hijos

                /* primero recorro los hijos de cada nodo borrando la memoria que ocupank
                    luego visito al nodo borrando la memoria que ocupa
                */

                elem=(tNodo)l_recuperar(hijos, cursor);
                destruir_recursivo(elem->hijos, fEliminar, elem);

                if(cursor!=l_ultima(hijos))
                    cursor=l_siguiente(hijos, cursor);
                else
                    cursor=NULL;


            }
        }
        //libero la memoria ocupada por el elemento
        fEliminar(n->elemento);

        //libero la memoria ocupada por la referencia a la lista
        free(n->hijos);

        //libero la memoria ocupada por la referencia al nodo
        free(n);

}

void a_destruir(tArbol * a, void (*fEliminar)(tElemento)){
        tLista hijos;
        tNodo n;

        n=(*a)->raiz;
        if(n!=NULL){
            hijos= n->hijos;
            //libera la memoria de todo el arbol
            destruir_recursivo(hijos, fEliminar, a_raiz(*a));

            free(*a);
        }
}

tElemento a_recuperar(tArbol a, tNodo n){
        if(n==NULL)
            exit(ARB_POSICION_INVALIDA);
        return n->elemento;
}

tNodo a_raiz(tArbol a){
        if(a->raiz==NULL)
            exit(ARB_OPERACION_INVALIDA);
        return a->raiz;
}

tLista a_hijos(tArbol a, tNodo n){
        tPosicion cursor;
        tLista hijos;
        tLista toReturn;

        if(n==NULL)
            exit(ARB_POSICION_INVALIDA);
        crear_lista(&toReturn);


        hijos=n->hijos;

        if(   !( (l_primera(hijos) == l_ultima(hijos)) && (l_ultima(hijos) == l_fin(hijos) ) ) ) { //si el nodo tiene hijos (si su lista de hijos no esta vacia)
            cursor=l_primera(hijos);


            while(cursor!=NULL){

                l_insertar(toReturn,l_ultima(toReturn), l_recuperar(hijos, cursor));

                if(cursor!=l_ultima(hijos))
                    cursor=l_siguiente(hijos, cursor);
                else
                    cursor=NULL;
            }
        }


        return toReturn;
}

void a_sub_arbol(tArbol a, tNodo n, tArbol * sa){
        tNodo padre;
        tLista hijospadre;
        tNodo nuevaRaiz;
        tNodo aux;
        tPosicion cursor;

        crear_arbol(sa);

        if(n!=a->raiz){
            padre=n->padre;
            hijospadre=padre->hijos;
            cursor=l_primera(hijospadre);

            aux=(tNodo) l_recuperar(hijospadre, cursor);
            while( ( aux != n ) && cursor!=NULL ){
                cursor=l_siguiente(hijospadre, cursor);
                aux=(tNodo) l_recuperar(hijospadre, cursor);
                if(cursor==l_ultima(hijospadre)){
                    l_eliminar(hijospadre, cursor, anular);
                    eliminaraux(a,n);
                    cursor=NULL;
                }
                else
                    cursor=l_siguiente(hijospadre, cursor);
            }

            nuevaRaiz=n;
            nuevaRaiz->padre=NULL;



            (*sa)->raiz=nuevaRaiz;

        }
        else{
            (*sa)->raiz=a->raiz;
            a->raiz=NULL;
        }

}

/**
    Elimina el nodo n del arbol a
*/
void eliminaraux(tArbol a, tNodo n){
        tLista hijos;
        tPosicion cursor;

        hijos=n->padre->hijos;
        cursor=l_primera(hijos);
        while( (l_recuperar(hijos, cursor) != n) && (cursor!=NULL) ){
                if(cursor!=l_ultima(hijos))
                    cursor=l_siguiente(hijos, cursor);
                else
                    cursor=NULL;
        }


        if(cursor==NULL)//alguna referencia estaba mal hecha
            exit(ARB_OPERACION_INVALIDA);


        l_eliminar(hijos, cursor, anular);

}


