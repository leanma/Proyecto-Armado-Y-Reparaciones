#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Armados Y Reparaciones.h"



int main(int argc, char* argv[])
{

    generarYCargarArchBin_ALU(argv[ARG_PATH_COMP]);

    puts("Componentes antes de actualizar:\n");

    mostrarArchivoComponentes_ALU(argv[ARG_PATH_COMP]);

    puts("");

    puts("Armados/Reparaciones:");
    mostrarArchivoArmadoYRep_ALU(argv[ARG_PATH_ARM_REP]);
    puts("");

    int resp = actualizarComponentes_ALU(argv[ARG_PATH_COMP], argv[ARG_PATH_ARM_REP]);

    if(resp != TODO_OK)
    {
        puts("Error actualizando los componentes.");
        return resp;
    }

    puts("\nComponentes despues de actualizar:\n");
    mostrarArchivoComponentes_ALU(argv[ARG_PATH_COMP]);

    return 0;
}





