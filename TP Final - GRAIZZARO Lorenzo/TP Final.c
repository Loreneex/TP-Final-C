#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <time.h>
#include "funciones.h"

/*
    Nombre: Lorenzo
    apellido: GRAIZZARO
    Carrera: TPI (Tecnico universitario en Programacion Informatica)
    Fecha de entrega: 20/06/2023
    Archivos adicionales: "funciones.h","archivoB.dat","prestamos.csv","MENU DE USUARIO.txt","INDICE DE LIBRERIA.txt";
*/

//Esta funcion contiene el menu que deriva a las demas funciones
void funcionMenu(){
    char opcion;
    int CLIENTES=17, i;
    int CLIENTESBAJA=0;
    struct credito cred[100];
    struct credito credXYZ[50];
    for(i=0;i<100;i++){
        cred[i].orden=0;
        cred[i].importe=0.00;
        cred[i].dia=0.00;
        cred[i].anio=0;
        cred[i].nroCuotas=0;
        cred[i].importeCuotas=0.00;
        cred[i].iva=0.00;
        cred[i].totalCuota=0.00;
        cred[i].activo=0;
    }
    for(i=0; i<CLIENTES;i++){
        cred[i].activo=1;
    }
    struct creditoCSV credCSV[50];
    printf("Ingrese el caracter de la opci%cn que quiera ejecutar:\n", 162);
    while(opcion!='L' && opcion!= 'l'){
        printf("--------------------------------MENU--------------------------------\n");
        printf(" A: Emit%cr el archivo de texto.\n", 161);
        printf(" B: Crear un nuevo archivo binario.\n");
        printf(" C: Migrar los datos del archivo de texto al nuevo archivo creado.\n");
        printf(" D: Emit%cr el contenido del archivo binario.\n", 161);
        printf(" E: Dar de alta nuevo cliente.\n");
        printf(" F: Buscar datos de un cliente.\n");//puede ser por numero de orden o por apellido
        printf(" G: Modificar el importe y tipo de cr%cdito.\n", 130);
        printf(" H: Dar baja l%cgica por n%cmero de orden.\n", 162, 163);
        printf(" I: Dar baja f%csica de los datos de clientes inactivos.\n", 161);
        printf(" J: Listar el archivo de texto del punto I.\n");
        printf(" K: Limpiar la consola.\n");
        printf(" L: Cerrar programa.\n");
        printf("--------------------------------------------------------------------\n");
        printf("-");
        scanf("%c", &opcion);
        fflush(stdin);
        printf("\n");
        switch(opcion){
            case 'A': //Emitir el archivo de texto
            case 'a':   funcionEmitirPrestamos(cred, credCSV);
                        printf("\n");
                break;
            case 'B': //Crear un nuevo archivo binario
            case 'b':   funcionCrearArchivoBinario();
                        printf("\n");
                break;
            case 'C': //Migrar los datos del archivo de texton al nuevo archivo creado (comprobar)
            case 'c':   funcionMigrar(cred, credCSV, &CLIENTES);
                        printf("\n");
                break;
            case 'D': //Emitir el contenido del archivo binario y listar los datos
            case 'd':   funcionSubMenu(cred, &CLIENTES);
                        printf("\n");
                break;
            case 'E': //Dar de alta nuevo cliente
            case 'e':   funcionAlta(cred, &CLIENTES);
                        printf("\n");
                break;
            case 'F': //Buscar datos de un cliente
            case 'f':   funcionBuscarDatos(cred, &CLIENTES);
                        printf("\n");
                break;
            case 'G': //Modificar el importe y tipo de credito
            case 'g':   funcionModificar(cred, &CLIENTES);
                        printf("\n");
                break;
            case 'H': //Dar baja l%gica por numero de orden
            case 'h':   funcionBajaLogica(cred, &CLIENTES);
                        printf("\n");
                break;
            case 'I': //Dar baja fisica de los datos de clientes inactivos
            case 'i':   funcionBajaFisica(cred, credXYZ, &CLIENTES, &CLIENTESBAJA);
                        printf("\n");
                break;
            case 'J': //Listar el archivo creado en el I
            case 'j':   funcionListarXYZ(credXYZ, &CLIENTESBAJA);
                        printf("\n");
                break;
            case 'K': //Limpia la consola
            case 'k':   system("cls");
                        printf("Sistema de Gesti%cn de Cr%cditos:\n", 162, 130);
                break;
            case 'L': //Cerrar programa
            case 'l':   printf("Cerrando programa...");
                        return;
            default:
                printf("Ingrese el caracter correctamente...\n");
        }
    }

    return;
}

/*Mi trabajo contiene una funcion en el main que abre un menu que deriva
a todas las demas funciones que se encuentran en la libreria funciones.h*/
int main(){
    printf("Sistema de Gesti%cn de Cr%cditos:\n", 162, 130);
    funcionMenu();

    return 0;
}
