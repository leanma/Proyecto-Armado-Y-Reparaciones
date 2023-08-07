#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Armados Y Reparaciones.h"


void mostrarArchivoComponentes_ALU(const char* nombreArchivo)
{
    FILE* arch=fopen(nombreArchivo,"rb");
    if(!arch)
        printf ("ERROR DE APERTURA\n");
    t_Componente compAct;

    printf("CODIGO\t\t\t      DESCRIPCION\t\t  STOCK\t    PRECIO\n\n");

    fread(&compAct,sizeof(t_Componente),1,arch);
    while(!feof(arch))
    {
        printf("%-30s%-30s%-8d%.2f\n",compAct.codigo,compAct.descripcion,compAct.stock,compAct.precioUnitario);
        fread(&compAct,sizeof(t_Componente),1,arch);
    }
    fclose(arch);
}

void mostrarArchivoArmadoYRep_ALU(const char* nombreArchivo)
{
    FILE* archTXT=fopen(nombreArchivo,"rt");
    if(!archTXT)
    {
        printf("ERROR AL CARGAR ARCHIVO\n");
        exit(ERROR_ARCH);
    }

    char linea[TAM_LINEA];
    ArmadoYRep armAct;
    fgets(linea,TAM_LINEA,archTXT);
    printf("CODIGO\t\t\t    NUMERO OP\t\t CANT\t   PRECIO UNI\n\n");

    while(!feof(archTXT))
    {
        txtABin_ALU(linea,&armAct);
        printf("%-30s%-20d%-12d%.2f\n",armAct.codigo,armAct.nroOp,armAct.cantidad,armAct.precioUnitario);
        fgets(linea,TAM_LINEA,archTXT);
    }
    fclose(archTXT);
}

int actualizarComponentes_ALU(const char* pathComponentes, const char* pathArmadoYRep)
{
    ArmadoYRep* vecArYRe=malloc(TAM_MAX_VEC*sizeof(ArmadoYRep));
    if(!vecArYRe)
        return ERROR_MEM;

    int ce=0;
    cargarArmadosYRepEnVector_ALU((char*)pathArmadoYRep,vecArYRe,&ce);

    normalizarVector_ALU(vecArYRe,&ce,compararCodigo_ALU);

    ordenarVecGen_ALU(vecArYRe,ce,sizeof(ArmadoYRep),compararCodigo_ALU);

    ArmadoYRep* vecArYReF=malloc(ce*sizeof(ArmadoYRep));
    if(!vecArYReF)
        return ERROR_MEM;

    memcpy(vecArYReF,vecArYRe,ce*sizeof(ArmadoYRep));
    free(vecArYRe);

    FILE* archTXT=fopen(pathArmadoYRep,"wt");
    if(!archTXT)
    {
        printf("ERROR AL CARGAR ARCHIVO\n");
        exit(ERROR_ARCH);
    }

    for(ArmadoYRep*i=vecArYReF;i<vecArYReF+ce;i++)
    {
        fprintf(archTXT,"%d|%s|%d|%.2f\n",i->nroOp,i->codigo,i->cantidad,i->precioUnitario);
    }


    FILE* archComp=fopen(pathComponentes,"r+b");
    if(!archComp)
    {
        printf("ERROR AL CARGAR ARCHIVO\n");
        return ERROR_ARCH;
    }

    t_Componente compAct;
    fread(&compAct,sizeof(t_Componente),1,archComp);
    ArmadoYRep* vec_ptr=vecArYReF;

    while(!feof(archComp) && vec_ptr<vecArYReF+ce)
    {
        int comp=strcmp_ALU(compAct.codigo,vec_ptr->codigo);
        if(comp==0)
        {
            compAct.stock-=vec_ptr->cantidad;
            compAct.precioUnitario=vec_ptr->precioUnitario;
            vec_ptr++;
        }
        if(comp<0)
        {
            fseek(archComp,-(long)sizeof(t_Componente),SEEK_CUR);
            fwrite(&compAct,sizeof(t_Componente),1,archComp);
            fseek(archComp,0,SEEK_CUR);
            fread(&compAct,sizeof(t_Componente),1,archComp);
        }
    }



    fclose(archComp);
    return TODO_OK;

}

void cargarArmadosYRepEnVector_ALU(char* pathArmadoYRep,ArmadoYRep*vecArYRe,int *ce)
{
    FILE* archTXT=fopen(pathArmadoYRep,"rt");
    if(!archTXT)
    {
        printf("ERROR AL CARGAR ARCHIVO\n");
        exit(ERROR_ARCH);
    }

    char linea[TAM_LINEA];
    ArmadoYRep*vec_rec=vecArYRe;
    fgets(linea,TAM_LINEA,archTXT);

    while(!feof(archTXT) && vec_rec<vecArYRe+TAM_MAX_VEC)
    {
        txtABin_ALU(linea,vec_rec);
        vec_rec++;
        (*ce)++;
        fgets(linea,TAM_LINEA,archTXT);
    }
    fclose(archTXT);
}

void txtABin_ALU(char* linea,ArmadoYRep* armActual)
{
    char*act=strchr(linea,'\n');
    if(!act)
        exit(ERROR_LINEA);

    *act='\0';
    act=strrchr(linea,'|');
    sscanf(act+1,"%lf",&armActual->precioUnitario);

    *act='\0';
    act=strrchr(linea,'|');
    sscanf(act+1,"%d",&armActual->cantidad);

    *act='\0';
    act=strrchr(linea,'|');
    strncpy(armActual->codigo,act+1,TAM_COD);
    armActual->codigo[TAM_COD]='\0';

    *act='\0';
    sscanf(linea,"%d",&armActual->nroOp);


}


void normalizarVector_ALU(ArmadoYRep* vecArYRe,int* ce,Cmp cmp)
{
    ArmadoYRep* ult=vecArYRe+*ce-1;
    ArmadoYRep* pos;

    for(ArmadoYRep*i=vecArYRe;i<ult;i++)
    {
        pos=buscarDuplicado_ALU(i+1,ult,i,cmp);

        while(pos!=NULL)
        {
            i->cantidad+=pos->cantidad;
            i->precioUnitario=pos->precioUnitario;
            for(ArmadoYRep*j=pos;j<ult;j++)
            {
                *j=*(j+1);
            }
            (*ce)--;
            ult--;
            pos=buscarDuplicado_ALU(i+1,ult,i,cmp);
        }
    }
}



ArmadoYRep* buscarDuplicado_ALU(ArmadoYRep* ini,ArmadoYRep* fin,ArmadoYRep*elem,Cmp cmp)
{
    for(ArmadoYRep*i=ini;i<=fin;i++)
    {
        if(cmp(i,elem)==0)
            return i;
    }
    return NULL;
}

int compararCodigo_ALU(const void*x,const void*y)
{
    ArmadoYRep armX=*(ArmadoYRep*)x;
    ArmadoYRep armY=*(ArmadoYRep*)y;
    return strcmp_ALU(armX.codigo,armY.codigo);
}

int compararCodigo2_ALU(const void*x,const void*y)
{
    t_Componente armX=*(t_Componente*)x;
    t_Componente armY=*(t_Componente*)y;
    return strcmp_ALU(armX.codigo,armY.codigo);
}

int strcmp_ALU(const char* cad1,const char* cad2)
{
    while(*cad1 && *cad2 && *cad1==*cad2)
    {
        cad1++;
        cad2++;
    }
    if(*cad1>*cad2)
        return 1;
    if(*cad1<*cad2)
        return -1;
    return 0;
}


void ordenarVecGen_ALU(void* vec,int ce,size_t tamElem,Cmp cmp)
{
    void*m;
    void*ult=(char*)vec+(ce-1)*tamElem;

    for(char*i=(char*)vec;i<(char*)ult;i+=tamElem)
    {
        m=buscarMenor_ALU(i,ult,tamElem,cmp);
        if(i!=m)
            intercambiar_ALU(i,m,tamElem);
    }
}

void* buscarMenor_ALU(void*ini,void*fin,size_t tamElem,Cmp cmp)
{
    void*m=ini;
    for(char*j=(char*)ini+tamElem;j<=(char*)fin;j+=tamElem)
    {
        if(cmp(j,m)<0)
            m=j;
    }
    return m;
}

void intercambiar_ALU(void*x,void*y,size_t tamElem)
{
    void*xTemp=malloc(tamElem);
    memcpy(xTemp,x,tamElem);
    memcpy(x,y,tamElem);
    memcpy(y,xTemp,tamElem);
    free(xTemp);
}



void generarYCargarArchBin_ALU(char*pathComponentes)
{
    FILE* archBin=fopen(pathComponentes,"wb");
    if(!archBin)
        exit(ERROR_ARCH);

    t_Componente vecComp[]={
                            {"MOTHASUSXP567","Mother Asus XP567",52,500.00},
                            {"CPUINTELI35GEN","CPU Intel i3 5ta Gen",25,428.00},
                            {"MEMCORSAIR8GB","Mem Corsair 8 GB",60,37.00},
                            {"HDDSSD1TB","HDD SSD 1TB",50,50.00},
                            {"MOTHGIGABYTEHP123","Mother GigaByte HP123",45,460.00},
                            {"CPUAMDRYZEN3","CPU AMD Ryzen 3",35,345.00},
                            {"MEMKINGSTON16GB","Mem Kingston 16 GB",40,123.00},
                            {"HDDSTD2TB","HDD Standard 2TB",30,100.00},
                            {"PVNVIDIAGTX1060TI","P Video Nvidia GTX1060TI",20,150.00},
                            {"PVAMDRX3880X","P Video AMD RX3880X",15,140.00}
                            };


    int ce=10;
    ordenarVecGen_ALU(vecComp,ce,sizeof(t_Componente),compararCodigo2_ALU);

    for(t_Componente*i=vecComp;i<vecComp+ce;i++)
    {
        fwrite(i,sizeof(t_Componente),1,archBin);
    }
    fclose(archBin);
}
