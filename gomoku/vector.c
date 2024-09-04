#include <stdio.h>
#include <stdlib.h>

#define N 18
// point-------------------------------------------------------------------------------------------------------
typedef struct{
    int x; int y;
    } point;

//struct point_vector
typedef struct {
    point* Array;
    int size;
} point_vector;

//pushback point into point_vector
void pushback_point(point_vector *pv, point item){

    //allocate memory
    int i = pv->size;
    pv->Array = (point*) realloc(pv->Array , (i + 1)*sizeof(point));
    if (pv->Array == NULL )
        exit (-1);

    //pushback item
    pv->Array[i] = item;

    //increase size
    pv->size++;
}

//erase point at givven index
void erase_point_atindex(point_vector *pv, int index){
    if(index != -1){
        //copy elemnts with index > than index of erased to index-1
        for(int i = index; i < pv->size-1; i++)
            pv->Array[i] = pv->Array[i+1];
        //reallocate memory
        pv->Array = (point*) realloc(pv->Array , (pv->size)*sizeof(point));
        //decrease size
        pv->size--;
    }
}

//find point in point_vector return index except if not in vector, than retur -1
int find_point(point_vector pv, point item){
    for(int i = 0; i < pv.size; i++){
        if((pv.Array[i].x == item.x) && (pv.Array[i].y == item.y)){
            return i;
        }
    }
    return -1;
}

//copy data from one pointvector to another
void copy_point_vector(point_vector source, point_vector *destination){
    destination->size = source.size;
    destination->Array = (point*) realloc(destination->Array , destination->size*sizeof(point));
    for(int i = 0; i < destination->size; i++){
        destination->Array[i].x = source.Array[i].x;
        destination->Array[i].y = source.Array[i].y;
    }
}

// int_vector------------------------------------------------------------------------------------------------
//struct point_vector
typedef struct {
    int* Array;
    int size;
} int_vector;

//pushback point into point_vector
void pushback_int(int_vector *iv, int item){

    //allocate memory
    int i = iv->size;
    iv->Array = (int*) realloc(iv->Array , (i + 1)*sizeof(int));
    if (iv->Array == NULL )
        exit (-1);
    
    //pushback item
    iv->Array[i] = item;

    //increase size
    iv->size++;
}

//pattern------------------------------------------------------------------------------------------------
//struct for patterns with pattern, kmp tabel and value
typedef struct{
    int_vector int_pattern; int_vector kmp_table; int point_value;
} pattern;


//grid---------------------------------------------------------------------------------------------------
typedef struct{
    int_vector values; int n; int m;
} grid;

//copy contents of grid to another
void copy_grid(grid source, grid *destination){
    destination->n = source.n;
    destination->m = source.m;
    destination->values.size = (destination->n)*(destination->m);
    destination->values.Array = (int*) realloc(destination->values.Array , destination->values.size*sizeof(int));
    for(int i = 0; i < destination->values.size; i++) destination->values.Array[i] = source.values.Array[i];
};