#ifndef ARMADOS_Y_REPARACIONES_H_INCLUDED
#define ARMADOS_Y_REPARACIONES_H_INCLUDED


#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct
{
    char codigo[21];
    char descripcion[51];
    int stock;
    double precioUnitario;
}
t_Componente;


typedef struct
{
    int nroOp;
    char codigo[21];
    int cantidad;
    double precioUnitario;
}
ArmadoYRep;


typedef struct
{
    char codigo[21];
    int nroReg;
}
IndComponente;

#define TODO_OK     0
#define VEC_LLENO   1
#define DUPLICADO   2
#define SIN_MEM     3
#define ERROR_MEM     3
#define ERROR_ARCH     4
#define ERROR_LINEA     5


#define TAM_MAX_VEC 1000
#define TAM_LINEA 1000
#define TAM_COD 20


#define ARG_PATH_COMP 1
#define ARG_PATH_ARM_REP 2

typedef int (*Cmp)(const void*, const void*);

int actualizarComponentes_ALU(const char* pathComponentes, const char* pathArmadoYRep);

void cargarArmadosYRepEnVector_ALU(char* pathArmadoYRep,ArmadoYRep*vecArYRe,int *ce);
void txtABin_ALU(char* linea,ArmadoYRep* armActual);
void normalizarVector_ALU(ArmadoYRep*vecArYRe,int* ce,Cmp cmp);
int compararCodigo_ALU(const void*x,const void*y);
int compararCodigo2_ALU(const void*x,const void*y);
ArmadoYRep* buscarDuplicado_ALU(ArmadoYRep* ini,ArmadoYRep* fin,ArmadoYRep*elem,Cmp cmp);
int strcmp_ALU(const char* cad1,const char* cad2);

void mostrarArchivoComponentes_ALU(const char* nombreArchivo);
void mostrarArchivoArmadoYRep_ALU(const char* nombreArchivo);

void generarYCargarArchBin_ALU(char*pathComponentes);

void ordenarVecGen_ALU(void* vec,int ce,size_t tamElem,Cmp cmp);
void* buscarMenor_ALU(void*ini,void*fin,size_t tamElem,Cmp cmp);
void intercambiar_ALU(void*x,void*y,size_t tamElem);

#endif // ARMADOS_Y_REPARACIONES_H_INCLUDED
