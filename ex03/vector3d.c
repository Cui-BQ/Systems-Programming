#include <stdio.h>    // for printf
#include <stdlib.h>   // for EXIT_SUCCESS, malloc, free
#include <assert.h>   // for assert()

typedef struct Vector_3d {
    int x;
    int y;
    int z;
} *Vector3d;

Vector3d Vector3d_new(int x, int y, int z){
    Vector3d ret = (Vector3d) malloc(sizeof(*ret));
    ret->x = x;
    ret->y = y;
    ret->z = z;
    if (ret->x == x && ret->y == y && ret->z == z){
        return ret;
    } else {
        return NULL;
    }
}

void Vector3d_destroy(Vector3d v){
    free(v);
}

int Vector3d_getX(Vector3d v){
    int x = v->x;
    if (v->x == x){
        return x;
    } else {
        return 0;
    }
}

int Vector3d_getY(Vector3d v){
    int y = v->y;
    if (v->y == y){
        return y;
    } else {
        return 0;
    }
}

int Vector3d_getZ(Vector3d v){
    int z = v->z;
    if (v->z == z){
        return z;
    } else {
        return 0;
    }
}

Vector3d Vector3d_setX(Vector3d v, int x){
    v->x = x;
    if (v->x == x){
        return v;
    } else {
        return NULL;
    }
}

Vector3d Vector3d_setY(Vector3d v, int y){
    v->y = y;
    if (v->y == y){
        return v;
    } else {
        return NULL;
    }
}

Vector3d Vector3d_setZ(Vector3d v, int z){
    v->z = z;
    if (v->z == z){
        return v;
    } else {
        return NULL;
    }
}

Vector3d Vector3d_add(Vector3d vec_one, Vector3d vec_two){
    if (vec_one != NULL && vec_two != NULL){
        return Vector3d_new(vec_one->x + vec_two->x, vec_one->y + vec_two->y, vec_one->z + vec_two->z);
    } else {
        return NULL;
    }
    
}

void Vector3d_print(Vector3d v, FILE* f_out){
    fprintf(f_out, "[%d, %d, %d]", v->x, v->y, v->z);
}