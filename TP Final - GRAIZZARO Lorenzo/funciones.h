#ifndef PEPITO_H_INCLUDED
#define PEPITO_H_INCLUDED

//struct del archivoB
struct credito{
    int orden;
    char apellido[30];
    char nombre[30];
    float importe;
    char tipoCredito[30];
    int dia;
    char mes[3];
    int anio   ;
    int nroCuotas;
    float importeCuotas;
    float iva;
    float totalCuota;
    int activo;
};

//struct para imprimir el punto 1
struct creditoCSV{
    char cliente[30];
    int mesNum;
};

//funcion errores
void funcionError(int numero){
    switch(numero){
        case 1:printf("ERROR EN EL INGRESO DE CARACTERES O CADENAS.\n");
            break;
        case 2:printf("INGRESE EL NUMERO CORRECTAMENTE.\n");
            break;
        case 3:printf("ERROR EN LA APERTURA DEL ARCHIVO.\n");
            break;
    }

    return;
}

//esta funcion sustituye un punto por una coma en el csv para que se vuelva al archivo original
void sustPunto(){
    FILE *pPrestamos;
    char car;
    pPrestamos=fopen("prestamos.csv","r+");
    if (!pPrestamos){
            funcionError(3);
            return;
    }
    while (fscanf(pPrestamos,"%c",&car)!=EOF){
        if (car=='.'){
            fseek(pPrestamos, -1, SEEK_CUR);
            fputc(',', pPrestamos);
            fflush(pPrestamos);
        }
    }
    fclose(pPrestamos);
}

//esta funcion sustituye la coma por un punto en el csv para que se pueda imprimir correctamente los flotantes
void sustComa(){
    FILE *pPrestamos;
    char car;
    pPrestamos=fopen("prestamos.csv","r+");
    if (!pPrestamos){
            funcionError(3);
            return;
    }
    while (fscanf(pPrestamos,"%c",&car)!=EOF){
        if (car==','){
            fseek(pPrestamos, -1, SEEK_CUR);
            fputc('.', pPrestamos);
            fflush(pPrestamos);
        }
    }
    fclose(pPrestamos);
}

//esta funcion crea un archivo Binario
void funcionCrearArchivoBinario(){
    FILE *pArchivoB;
    pArchivoB=fopen("archivoB.dat","wb");
    if(pArchivoB!=NULL){
        printf("Archivo creado.\n");
        fclose(pArchivoB);
    }else{
        funcionError(3);
    }

    return;
}

//extrae los datos del archivo de texto
void extraerArchivos(char *linea, struct credito cred[], struct creditoCSV credCSV[], int *i){
    char *delimit;
    delimit=strtok(linea, ";");/*delimitadores*/
    if(delimit != NULL){
        cred[(*i)].orden=atoi(delimit);
        delimit=strtok(NULL, ";");
        strcpy(credCSV[(*i)].cliente, delimit);
        delimit=strtok(NULL, ";");
        cred[(*i)].importe=atof(delimit);
        delimit=strtok(NULL, ";");
        strcpy(cred[(*i)].tipoCredito, delimit);
        delimit=strtok(NULL, ";");
        cred[(*i)].dia=atoi(delimit);
        delimit=strtok(NULL, ";");
        credCSV[(*i)].mesNum=atoi(delimit);
        delimit=strtok(NULL, ";");
        cred[(*i)].anio=atoi(delimit);
        delimit=strtok(NULL, ";");
        cred[(*i)].nroCuotas=atoi(delimit);
        delimit=strtok(NULL, ";");
        cred[(*i)].importeCuotas=atof(delimit);
        delimit=strtok(NULL, ";");
        cred[(*i)].iva=atof(delimit);
        delimit=strtok(NULL, ";");
        cred[(*i)].totalCuota=atof(delimit);
    }

    return;
}

//Esta funcion Migra los datos del archivo de texto al archivo binario
void funcionMigrar(struct credito cred[], struct creditoCSV credCSV[], int *CLIENTES){
    int i,j, largo;
    char *delimit;
    FILE *pArchivoB;
    pArchivoB = fopen("archivoB.dat", "wb");
    if(pArchivoB==NULL){
        funcionError(3);
        return;
    }else{
        for(i=0; i<*CLIENTES; i++){
            // Aplico las modificaciones necesarias
            // Y asigno los valores a la estructura credito
            //aqui separo por el espacio el nombre y el apellido
            delimit=strtok(credCSV[i].cliente, " ");
            if(delimit != NULL){
                strcpy(cred[i].nombre, delimit);
                delimit = strtok(NULL, " ");
                if (delimit != NULL) {
                    strcpy(cred[i].apellido, delimit);
                }
            }
            //convierto el apellido en mayuscula
            largo=strlen(cred[i].apellido);
            for(j=0;j<largo;j++){
                cred[i].apellido[j]=toupper(cred[i].apellido[j]);
            }
            //copio el tipo de credito y lo paso a mayusculas
            largo=strlen(cred[i].tipoCredito);
            for(j=0;j<largo;j++){
                cred[i].tipoCredito[j]=toupper(cred[i].tipoCredito[j]);
            }
            //transformo el mes en un char de 3 letras minusculas dependiendo cual mes es del año
            switch(credCSV[i].mesNum){
                case 1:     strcpy(cred[i].mes , "ene");
                    break;
                case 2:     strcpy(cred[i].mes , "feb");
                    break;
                case 3:     strcpy(cred[i].mes , "mar");
                    break;
                case 4:     strcpy(cred[i].mes , "abr");
                    break;
                case 5:     strcpy(cred[i].mes , "may");
                    break;
                case 6:     strcpy(cred[i].mes , "jun");
                    break;
                case 7:     strcpy(cred[i].mes , "jul");
                    break;
                case 8:     strcpy(cred[i].mes , "ago");
                    break;
                case 9:     strcpy(cred[i].mes , "sep");
                    break;
                case 10:    strcpy(cred[i].mes , "oct");
                    break;
                case 11:    strcpy(cred[i].mes , "nov");
                    break;
                case 12:    strcpy(cred[i].mes , "dic");
                    break;
            }

            // Escribo la estructura credito en el archivo binario
            fwrite(&cred[i], (sizeof(struct credito)), 1, pArchivoB);
        }
        fclose(pArchivoB);
    }
    printf("Migracion al archivo binario completada.\n");

    return;
}

//esta funcion emite el archivo de texto con el uso del strtok en la funcion extraer
void funcionEmitirPrestamos(struct credito cred[], struct creditoCSV credCSV[]){
    int i=0, bandera=0;
    sustComa();
    FILE *pPrestamos;
    pPrestamos = fopen("prestamos.csv","r");
    if(pPrestamos != NULL){
            printf(" ------------------------------------------------------------------------------------------------------------\n");
            printf(" %-6s%-18s%-9s%-16s|FECHA     %-12s%-14s%-9s%s","|ORDEN","|CLIENTE","|IMPORTE","|TIPO DE CREDITO","|NRO. CUOTAS","|IMPORTE CUOTA","|IVA","|TOTAL CUOTA|\n");
            printf(" ------------------------------------------------------------------------------------------------------------\n");
            char linea[2000];
            fgets(linea,2000,pPrestamos);
            while(!feof(pPrestamos)){
                if(bandera!=0){
                    extraerArchivos(linea,  cred, credCSV, &i);
                    printf(" |%-5i|%-17s|%-8.2f|%-15s|%-2i|%-2i|%-4i|%-11i|%-13.2f|%-8.2f|%-11.2f|\n", cred[i].orden, credCSV[i].cliente, cred[i].importe, cred[i].tipoCredito, cred[i].dia, credCSV[i].mesNum, cred[i].anio, cred[i].nroCuotas, cred[i].importeCuotas, cred[i].iva, cred[i].totalCuota);
                i++;
                }
                fgets(linea,2000,pPrestamos);
                bandera=1;
            }
            printf(" ------------------------------------------------------------------------------------------------------------\n");
            fclose(pPrestamos);
            sustPunto();
		}else{
		    funcionError(3);
        }

    return;
}

//esta es el submenu del punto 7
void funcionSubMenu(struct credito cred[], int *CLIENTES){
    int i;
    char opcion;
    int diaMin, diaMax, anioMin, anioMax, mesMinInt, mesMaxInt, mesNumInt;
    printf("Elija una opcion:\n");
    while(opcion!='e'&&opcion!='E'){
        printf("------------------------SUBMENU------------------------\n");
        printf(" A: Listar todos (activos y inactivos)\n");
        printf(" B: Imprimir solo los activos.\n");
        printf(" C: El ingreso por teclado de un tipo de credito\n");
        printf(" D: El ingreso de un rango de tiempo (minimo y maximo)\n");
        printf(" E: Salir del submenu\n");
        printf("-------------------------------------------------------\n");
        printf("-");
        scanf("%c", &opcion);
        fflush(stdin);
        switch(opcion){
            case 'a'://listar todos (activos y inactivos)
            case 'A':   printf("Lista de todos los clientes:\n");
                        printf(" ------------------------------------------------------------------------------------------------------------------------\n");
                        printf(" %-6s%-10s%-10s%-10s%-16s|FECHA      %-12s%-14s%-9s%-11s%s","|ORDEN","|APELLIDO","|NOMBRE","|IMPORTE","|TIPO DE CREDITO","|NRO. CUOTAS","|IMPORTE CUOTA","|IVA","|TOTAL CUOTA","|ACTIVOS|\n");
                        printf(" ------------------------------------------------------------------------------------------------------------------------\n");
                        for(i=0; i<*CLIENTES;i++){
                            printf(" |%-5i|%-9s|%-9s|%-9.2f|%-15s|%-2i|%-3s|%-4i|%-11i|%-13.2f|%-8.2f|%-11.2f|%-7i|\n", cred[i].orden, cred[i].apellido, cred[i].nombre, cred[i].importe, cred[i].tipoCredito, cred[i].dia, cred[i].mes, cred[i].anio, cred[i].nroCuotas, cred[i].importeCuotas, cred[i].iva, cred[i].totalCuota, cred[i].activo);
                        }
                        printf(" ------------------------------------------------------------------------------------------------------------------------\n");
                        printf("\n");
                break;
            case 'b'://sólo el campo activo = 1
            case 'B':   printf("Lista de clientes activos:\n");
                        printf(" ------------------------------------------------------------------------------------------------------------------------\n");
                        printf(" %-6s%-10s%-10s%-10s%-16s|FECHA      %-12s%-14s%-9s%-11s%s","|ORDEN","|APELLIDO","|NOMBRE","|IMPORTE","|TIPO DE CREDITO","|NEO. CUOTAS","|IMPORTE CUOTA","|IVA","|TOTAL CUOTA","|ACTIVOS|\n");
                        printf(" ------------------------------------------------------------------------------------------------------------------------\n");
                        for(i=0; i<*CLIENTES;i++){
                            if(cred[i].activo==1){
                                printf(" |%-5i|%-9s|%-9s|%-9.2f|%-15s|%-2i|%-3s|%-4i|%-11i|%-13.2f|%-8.2f|%-11.2f|%-7i|\n", cred[i].orden, cred[i].apellido, cred[i].nombre, cred[i].importe, cred[i].tipoCredito, cred[i].dia, cred[i].mes, cred[i].anio, cred[i].nroCuotas, cred[i].importeCuotas, cred[i].iva, cred[i].totalCuota, cred[i].activo);
                            }
                        }
                        printf(" ------------------------------------------------------------------------------------------------------------------------\n");
                        printf("\n");
                break;
            case 'c'://el ingreso por teclado de un tipo de crédito
            case 'C':   int TipoCred;
                        printf("\n");
                        printf("Ingrese el Tipo de credito\n");
                        printf(" 1: CON GARANTIA\n");
                        printf(" 2: A SOLA FIRMA\n");
                        scanf("%i", &TipoCred);
                        fflush(stdin);
                        switch(TipoCred){
                            case 1:
                                    printf("Lista CON GARANTIA:\n");
                                    printf(" ------------------------------------------------------------------------------------------------------------------------\n");
                                    printf(" %-6s%-10s%-10s%-10s%-16s|FECHA      %-12s%-14s%-9s%-11s%s","|ORDEN","|APELLIDO","|NOMBRE","|IMPORTE","|TIPO DE CREDITO","|NRO. CUOTAS","|IMPORTE CUOTA","|IVA","|TOTAL CUOTA","|ACTIVOS|\n");
                                    printf(" ------------------------------------------------------------------------------------------------------------------------\n");
                                    for(i=0; i<*CLIENTES;i++){
                                        if(strcmp(cred[i].tipoCredito,"CON GARANTIA")==0){
                                            printf(" |%-5i|%-9s|%-9s|%-9.2f|%-15s|%-2i|%-3s|%-4i|%-11i|%-13.2f|%-8.2f|%-11.2f|%-7i|\n", cred[i].orden, cred[i].apellido, cred[i].nombre, cred[i].importe, cred[i].tipoCredito, cred[i].dia, cred[i].mes, cred[i].anio, cred[i].nroCuotas, cred[i].importeCuotas, cred[i].iva, cred[i].totalCuota, cred[i].activo);
                                        }
                                    }
                                    printf(" ------------------------------------------------------------------------------------------------------------------------\n");
                                    printf("\n");
                                break;
                            case 2:
                                    printf("Lista A SOLA FIRMA:\n");
                                    printf(" ------------------------------------------------------------------------------------------------------------------------\n");
                                    printf(" %-6s%-10s%-10s%-10s%-16s|FECHA      %-12s%-14s%-9s%-11s%s","|ORDEN","|APELLIDO","|NOMBRE","|IMPORTE","|TIPO DE CREDITO","|NRO. CUOTAS","|IMPORTE CUOTA","|IVA","|TOTAL CUOTA","|ACTIVOS|\n");
                                    printf(" ------------------------------------------------------------------------------------------------------------------------\n");
                                    for(i=0; i<*CLIENTES;i++){
                                        if(strcmp(cred[i].tipoCredito,"A SOLA FIRMA")==0){
                                            printf(" |%-5i|%-9s|%-9s|%-9.2f|%-15s|%-2i|%-3s|%-4i|%-11i|%-13.2f|%-8.2f|%-11.2f|%-7i|\n", cred[i].orden, cred[i].apellido, cred[i].nombre, cred[i].importe, cred[i].tipoCredito, cred[i].dia, cred[i].mes, cred[i].anio, cred[i].nroCuotas, cred[i].importeCuotas, cred[i].iva, cred[i].totalCuota, cred[i].activo);
                                        }
                                    }
                                    printf(" ------------------------------------------------------------------------------------------------------------------------\n");
                                    printf("\n");
                                break;
                            default:
                                funcionError(2);
                        }
                break;
            case 'd'://el ingreso de un rango de tiempo (mínimo y máximo)
            case 'D':   printf("\n");
                        do{
                            printf("Ingrese una fecha minima:\n");
                            printf("Dia:");
                            scanf("%i", &diaMin);
                            fflush(stdin);
                            if(diaMin<0 || diaMin>32){
                                funcionError(2);
                            }
                        }while(diaMin<0 || diaMin>32);
                        do{
                            printf("Mes(como numero entero):");
                            scanf("%i", &mesMinInt);
                            fflush(stdin);
                            if(mesMinInt<0 || mesMinInt>12){
                                funcionError(2);
                            }
                        }while(mesMinInt<0 || mesMinInt>12);
                        do{
                            printf("A%co:", 164);
                            scanf("%i", &anioMin);
                            fflush(stdin);
                            if(anioMin>2023 || anioMin<2000){
                                printf("LA FECHA ECCEDE EL A%cO ACTUAL.\n", 165);
                            }
                        }while(anioMin>2023 || anioMin<2000);
                        printf("\n");
                        do{
                            printf("Ingrese una fecha maxima:\n");
                            printf("Dia:");
                            scanf("%i", &diaMax);
                            fflush(stdin);
                            if(diaMax<0 || diaMax>32){
                                funcionError(2);
                            }
                        }while(diaMax<0 || diaMax>32);
                        do{
                            printf("Mes(como numero entero):");
                            scanf("%i", &mesMaxInt);
                            fflush(stdin);
                            if(mesMaxInt<0 || mesMaxInt>12){
                                funcionError(2);
                            }
                        }while(mesMaxInt<0 || mesMaxInt>12);
                        do{
                            printf("A%co:", 164);
                            scanf("%i", &anioMax);
                            fflush(stdin);
                            if(anioMax>2023|| anioMax<2000){
                                printf("LA FECHA ECCEDE EL AÑO ACTUAL.\n");
                            }
                        }while(anioMax>2023|| anioMax<2000);
                        printf("Lista con rango de tiempo:\n");
                        printf(" ------------------------------------------------------------------------------------------------------------------------\n");
                        printf(" %-6s%-10s%-10s%-10s%-16s|FECHA      %-12s%-14s%-9s%-11s%s","|ORDEN","|APELLIDO","|NOMBRE","|IMPORTE","|TIPO DE CREDITO","|NRO. CUOTAS","|IMPORTE CUOTA","|IVA","|TOTAL CUOTA","|ACTIVOS|\n");
                        printf(" ------------------------------------------------------------------------------------------------------------------------\n");
                        for(i=0; i<*CLIENTES;i++){
                            if(anioMin<=cred[i].anio && anioMax>=cred[i].anio){
                                if (strcmp(cred[i].mes, "ene") == 0){
                                    mesNumInt = 1;
                                }else if (strcmp(cred[i].mes, "feb") == 0){
                                    mesNumInt = 2;
                                }else if (strcmp(cred[i].mes, "mar") == 0){
                                    mesNumInt = 3;
                                }else if (strcmp(cred[i].mes, "abr") == 0){
                                    mesNumInt = 4;
                                }else if (strcmp(cred[i].mes, "may") == 0){
                                    mesNumInt = 5;
                                }else if (strcmp(cred[i].mes, "jun") == 0){
                                    mesNumInt = 6;
                                }else if (strcmp(cred[i].mes, "jul") == 0){
                                    mesNumInt = 7;
                                }else if (strcmp(cred[i].mes, "ago") == 0){
                                    mesNumInt = 8;
                                }else if (strcmp(cred[i].mes, "sep") == 0){
                                    mesNumInt = 9;
                                }else if (strcmp(cred[i].mes, "oct") == 0){
                                    mesNumInt = 10;
                                }else if (strcmp(cred[i].mes, "nov") == 0){
                                    mesNumInt = 11;
                                }else if (strcmp(cred[i].mes, "dic") == 0){
                                    mesNumInt = 12;
                                }

                                if(mesMinInt<=mesNumInt && mesMaxInt>=mesNumInt){
                                    if(diaMin<=cred[i].dia && diaMax>=cred[i].dia){
                                        printf(" |%-5i|%-9s|%-9s|%-9.2f|%-15s|%-2i|%-3s|%-4i|%-11i|%-13.2f|%-8.2f|%-11.2f|%-7i|\n", cred[i].orden, cred[i].apellido, cred[i].nombre, cred[i].importe, cred[i].tipoCredito, cred[i].dia, cred[i].mes, cred[i].anio, cred[i].nroCuotas, cred[i].importeCuotas, cred[i].iva, cred[i].totalCuota, cred[i].activo);
                                    }
                                }
                            }
                        }
                        printf(" ------------------------------------------------------------------------------------------------------------------------\n");
                        printf("\n");

                break;
            case 'e'://sale del submenu
            case 'E': printf("Saliendo del menu...\n");
                break;
            default:
                printf("Ingrese una opcion valida...\n");
        }
    }

    return;
}

//En esta funcion se dan de alta a los nuevos clientes y se imprime el struct modificado en el archivo binario
void funcionAlta(struct credito cred[], int *CLIENTES){
    char opcion;
    int ordenA, i, j, tipo, V=0;
    printf("Desea dar de alta un cliente? (S/N)\n-");
    scanf("%c", &opcion);
    fflush(stdin);
    if(opcion=='S'||opcion=='s'){
        do{
            do{

                printf("Ingrese el numero de orden:\n-");
                scanf("%i", &ordenA);
                fflush(stdin);
                if(ordenA<0){
                    funcionError(2);
                }
            }while(ordenA<0);
            if(cred[ordenA-1].orden!=0){
                //error de ingreso de numero
                printf("YA EXISTE UN NUMERO DE ORDEN %i.\n", ordenA);
            }
        }while(cred[ordenA-1].orden!=0);
        if(ordenA>*CLIENTES){
            *CLIENTES=ordenA;
        }
        cred[ordenA-1].orden=ordenA;
        int largo;
        do{
            printf("Ingrese el nombre del cliente:\n-");
            scanf("%s", cred[ordenA-1].nombre);
            fflush(stdin);
            largo=strlen(cred[ordenA-1].nombre);
            V=0;
            for(i=0; i<largo; i++){
                if(isdigit(cred[ordenA-1].nombre[i])){
                    V++;
                }
            }
            if(V>0){
                funcionError(1);
                printf("No ingrese numeros.\n");
            }
        }while(V>0);
        for(i=1;i<largo;i++){
            cred[ordenA-1].nombre[i]=tolower(cred[ordenA-1].nombre[i]);
        }
        cred[ordenA-1].nombre[0]=toupper(cred[ordenA-1].nombre[0]);
        do{
            printf("Ingrese el apellido del cliente:\n-");
            scanf("%s", cred[ordenA-1].apellido);
            fflush(stdin);
            largo=strlen(cred[ordenA-1].apellido);
            V=0;
            for(i=0; i<largo; i++){
                if(isdigit(cred[ordenA-1].apellido[i])){
                    V++;
                }
            }
            if(V>0){
                funcionError(1);
                printf("No ingrese numeros.\n");
            }
        }while(V>0);
        for(j=0;j<largo;j++){
            cred[ordenA-1].apellido[j]=toupper(cred[ordenA-1].apellido[j]);
        }while(V>0);
        do{
            printf("Ingrese el importe:\n-");
            scanf("%f", &cred[ordenA-1].importe);
            fflush(stdin);
            if(cred[ordenA-1].importe<0){
                funcionError(2);
            }
        }while(cred[ordenA-1].importe<0);
        do{
            printf("Ingrese el tipo de credito: \n");
            printf("1: CON GARANTIA\n");
            printf("2: A SOLA FIRMA\n-");
            scanf("%i", &tipo);
            fflush(stdin);
            switch(tipo){
                case 1: strcpy(cred[ordenA-1].tipoCredito, "CON GARANTIA");
                break;
                case 2: strcpy(cred[ordenA-1].tipoCredito, "A SOLA FIRMA");
                break;
                default:
            }
        }while(tipo<0 && tipo>3);
        //utilizo la libreria time.h para utilizar la fecha de la computadora en el momento que se esta utilizando el programa
        time_t t = time(NULL);
        struct tm *tm = localtime(&t);
        int year = tm->tm_year + 1900;
        int month = tm->tm_mon + 1;
        int day = tm->tm_mday;
        cred[ordenA-1].dia = day;
        switch(month){
            case 1:     strcpy(cred[ordenA-1].mes , "ene");
                break;
            case 2:     strcpy(cred[ordenA-1].mes , "feb");
                break;
            case 3:     strcpy(cred[ordenA-1].mes , "mar");
                break;
            case 4:     strcpy(cred[ordenA-1].mes , "abr");
                break;
            case 5:     strcpy(cred[ordenA-1].mes , "may");
                break;
            case 6:     strcpy(cred[ordenA-1].mes , "jun");
                break;
            case 7:     strcpy(cred[ordenA-1].mes , "jul");
                break;
            case 8:     strcpy(cred[ordenA-1].mes , "ago");
                break;
            case 9:     strcpy(cred[ordenA-1].mes , "sep");
                break;
            case 10:    strcpy(cred[ordenA-1].mes , "oct");
                break;
            case 11:    strcpy(cred[ordenA-1].mes , "nov");
                break;
            case 12:    strcpy(cred[ordenA-1].mes , "dic");
                break;
        }
        cred[ordenA-1].anio=year;
        do{
            printf("Ingrese el numero de cuotas:\n-");
            scanf("%i", &cred[ordenA-1].nroCuotas);
            fflush(stdin);
            if(cred[ordenA-1].nroCuotas<0){
                funcionError(2);
            }
        }while(cred[ordenA-1].nroCuotas<0);
        float division=0.00;
        division=cred[ordenA-1].nroCuotas;
        cred[ordenA-1].importeCuotas= cred[ordenA-1].importe/division;
        cred[ordenA-1].iva=cred[ordenA-1].importeCuotas*0.21;
        cred[ordenA-1].totalCuota=cred[ordenA-1].importeCuotas+cred[ordenA-1].iva;
        cred[ordenA-1].activo=1;
        FILE *pArchivoB;
        pArchivoB = fopen("archivoB.dat", "wb");
        if(pArchivoB==NULL){
            funcionError(3);
            return;
        }else{
            for(i=0; i<*CLIENTES; i++){
                fwrite(&cred[i], (sizeof(struct credito)), 1, pArchivoB);
            }
        }
        fclose(pArchivoB);
    }else{
        printf("Cancelando alta...\n");
    }

    return;
}

//Esta es la funcion donde se buscan los datos de los clientes por orden o apellido
void funcionBuscarDatos(struct credito cred[], int *CLIENTES){
    char buscarApellido[30];
    int buscarOrden, largo, i;
    int busqueda;
    printf("Como desea buscar?\n");
    printf("1: Por numero de orden.\n");
    printf("2: Por apellido.\n");
    printf("3: Salir de busqueda.\n-");
    scanf("%i", &busqueda);
    fflush(stdin);
    switch(busqueda){
        case 1: printf("Ingrese el numero de orden:\n");
                scanf("%i", &buscarOrden);
                fflush(stdin);
                if(cred[buscarOrden-1].orden!=0){
                    for(i=0; i<*CLIENTES; i++){
                        if(buscarOrden==cred[i].orden){
                            printf(" ------------------------------------------------------------------------------------------------------------------------\n");
                            printf(" %-6s%-10s%-10s%-10s%-16s|FECHA      %-12s%-14s%-9s%-11s%s","|ORDEN","|APELLIDO","|NOMBRE","|IMPORTE","|TIPO DE CREDITO","|NRO. CUOTAS","|IMPORTE CUOTA","|IVA","|TOTAL CUOTA","|ACTIVOS|\n");
                            printf(" ------------------------------------------------------------------------------------------------------------------------\n");
                            printf(" |%-5i|%-9s|%-9s|%-9.2f|%-15s|%-2i|%-3s|%-4i|%-11i|%-13.2f|%-8.2f|%-11.2f|%-7i|\n", cred[i].orden, cred[i].apellido, cred[i].nombre, cred[i].importe, cred[i].tipoCredito, cred[i].dia, cred[i].mes, cred[i].anio, cred[i].nroCuotas, cred[i].importeCuotas, cred[i].iva, cred[i].totalCuota, cred[i].activo);
                            printf(" ------------------------------------------------------------------------------------------------------------------------\n");
                        }
                    }
                }else{
                    printf("NO SE A ENCONTRADO EL NUMERO DE ORDEN.\n");
                }
            break;
        case 2: printf("Ingrese el apellido: \n");
                scanf("%s", buscarApellido);
                fflush(stdin);
                largo=strlen(buscarApellido);
                for(i=0; i<largo; i++){
                    buscarApellido[i]=toupper(buscarApellido[i]);
                }
                for(i=0; i<*CLIENTES; i++){
                    if(strcmp(buscarApellido, cred[i].apellido)==0){
                        printf(" ------------------------------------------------------------------------------------------------------------------------\n");
                        printf(" %-6s%-10s%-10s%-10s%-16s|FECHA      %-12s%-14s%-9s%-11s%s","|ORDEN","|APELLIDO","|NOMBRE","|IMPORTE","|TIPO DE CREDITO","|NRO. CUOTAS","|IMPORTE CUOTA","|IVA","|TOTAL CUOTA","|ACTIVOS|\n");
                        printf(" ------------------------------------------------------------------------------------------------------------------------\n");
                        printf(" |%-5i|%-9s|%-9s|%-9.2f|%-15s|%-2i|%-3s|%-4i|%-11i|%-13.2f|%-8.2f|%-11.2f|%-7i|\n", cred[i].orden, cred[i].apellido, cred[i].nombre, cred[i].importe, cred[i].tipoCredito, cred[i].dia, cred[i].mes, cred[i].anio, cred[i].nroCuotas, cred[i].importeCuotas, cred[i].iva, cred[i].totalCuota, cred[i].activo);
                        printf(" ------------------------------------------------------------------------------------------------------------------------\n");
                        break;
                    }
                }
                if(i==*CLIENTES){
                    printf("No se a encontrado el apellido ingresado.\n");
                }
            break;
        case 3:
            printf("Cancelando busqueda...\n");
            break;
    }

    return;
}

//esta funcion modifica el importe y el tipo de credito del archivo Binario
void funcionModificar(struct credito cred[], int *CLIENTES){
    int modificar, ordenModif, i;
    char verificar;
    float importeNew;
    int tipoCreditoNew;
    do{
        printf("Ingrese el numero de orden del cliente a modificar:\n-");
        scanf("%i", &ordenModif);
        fflush(stdin);
        if(ordenModif<1 || ordenModif>*CLIENTES){
            funcionError(2);
        }
    }while(ordenModif<1 || ordenModif>*CLIENTES);
    do{
        printf("Que desea modificar?\n");
        printf("1: El importe.\n");
        printf("2: El tipo de credito.\n");
        printf("3: Salir.\n-");
        scanf("%i", &modificar);
        fflush(stdin);
        switch(modificar){
            case 1:
                    do{
                        printf("Ingrese el nuevo importe:\n-");
                        scanf("%f", &importeNew);
                        fflush(stdin);
                        if(importeNew<0){
                            funcionError(2);
                        }
                    }while(importeNew<0);
                    printf("Esta seguro que desea modificar los datos del importe? (S/N):\n");
                    scanf("%c", &verificar);
                    fflush(stdin);
                    if(verificar=='s'||verificar=='S'){
                        cred[ordenModif-1].importe = importeNew;
                    }else{
                        printf("Cancelando modificacion...\n");
                    }
                break;
            case 2:
                    do{
                        printf("Ingrese el nuevo tipo de credito:\n");
                        printf("1: CON GARANTIA\n");
                        printf("2: A SOLA FIRMA\n");
                        scanf("%i", &tipoCreditoNew);
                        fflush(stdin);
                        if(tipoCreditoNew>2 || tipoCreditoNew<1){
                            funcionError(2);
                        }
                    }while(tipoCreditoNew>2 || tipoCreditoNew<1);
                    printf("Esta seguro que desea modificar los datos del tipo de cr%cdito? (S/N):\n", 130);
                    scanf("%c", &verificar);
                    fflush(stdin);
                    if(verificar=='s'||verificar=='S'){
                        switch(tipoCreditoNew){
                            case 1: strcpy(cred[ordenModif-1].tipoCredito, "CON GARANTIA");
                                break;
                            case 2: strcpy(cred[ordenModif-1].tipoCredito, "A SOLA FIRMA");
                            break;
                            default: funcionError(2);
                        }
                    }else{
                        printf("Cancelando la modificaci%cn...\n", 162);
                    }
                break;
            case 3:
                    printf("Saliendo...");
                break;
            default: funcionError(2);
        }
    }while(modificar!=3);
    FILE *pArchivoB;
    pArchivoB = fopen("archivoB.dat", "wb");
    if(pArchivoB==NULL){
        funcionError(3);
        return;
    }else{
        for(i=0; i<*CLIENTES; i++){
            fwrite(&cred[i], (sizeof(struct credito)), 1, pArchivoB);
        }
    }
    fclose(pArchivoB);

    return;
}

//Esta funcion da baja logica a un cliente intercambiando el valor de activos de 1 a 0 en ese cliente
void funcionBajaLogica(struct credito cred[], int *CLIENTES){
    int ordenB, i;
    char opcionB;
    do{
        printf("Ingrese el numero de orden del cliente a dar de baja: \n-");
        scanf("%i", &ordenB);
        fflush(stdin);
        if(cred[ordenB-1].activo==0){
            printf("El cliente ya esta dado de baja.\n");
        }
    }while(cred[ordenB-1].activo==0);
    printf("Esta seguro que quiere darle de baja a %s? (S/N)\n", cred[ordenB-1].nombre);
    scanf("%c", &opcionB);
    fflush(stdin);
    if(opcionB=='s' || opcionB=='S'){
        cred[ordenB-1].activo = 0;
        printf("Listado con los clientes activos: \n");
        printf(" ------------------------------------------------------------------------------------------------------------------------\n");
        printf(" %-6s%-10s%-10s%-10s%-16s|FECHA      %-12s%-14s%-9s%-11s%s","|ORDEN","|APELLIDO","|NOMBRE","|IMPORTE","|TIPO DE CREDITO","|NEO. CUOTAS","|IMPORTE CUOTA","|IVA","|TOTAL CUOTA","|ACTIVOS|\n");
        printf(" ------------------------------------------------------------------------------------------------------------------------\n");
        for(i=0; i<*CLIENTES;i++){
            if(cred[i].activo==1){
                printf(" |%-5i|%-9s|%-9s|%-9.2f|%-15s|%-2i|%-3s|%-4i|%-11i|%-13.2f|%-8.2f|%-11.2f|%-7i|\n", cred[i].orden, cred[i].apellido, cred[i].nombre, cred[i].importe, cred[i].tipoCredito, cred[i].dia, cred[i].mes, cred[i].anio, cred[i].nroCuotas, cred[i].importeCuotas, cred[i].iva, cred[i].totalCuota, cred[i].activo);
            }
        }
        printf(" ------------------------------------------------------------------------------------------------------------------------\n");
        printf("\n");
        FILE *pArchivoB;
        pArchivoB = fopen("archivoB.dat", "wb");
        if(pArchivoB==NULL){
            funcionError(3);
            return;
        }else{
            for(i=0; i<*CLIENTES; i++){
                fwrite(&cred[i], (sizeof(struct credito)), 1, pArchivoB);
            }
        }
        fclose(pArchivoB);
    }else{
        printf("Cancelando baja...\n");
    }

    return;
}

/*En esta funcion se le da de baja fisica a los clientes dados de baja logica,
se convierte todos los datos del cliente en 0 y se agrega en el archivo xyz*/
void funcionBajaFisica(struct credito cred[], struct credito credXYZ[], int *CLIENTES, int *CLIENTESBAJA){
    int i;
    time_t t = time(NULL);
    struct tm *fecha = localtime(&t);

    int dia = fecha->tm_mday;
    int mes = fecha->tm_mon + 1; // Los meses en struct tm van de 0 a 11, se suma 1 para obtener el mes correcto
    int anio = fecha->tm_year + 1900; // Se suma 1900 para obtener el año correcto

    char fechaConcatenada[50]; // Espacio para la cadena
    strcpy(fechaConcatenada, "clientes_bajas__");

    char diaStr[3];
    sprintf(diaStr, "%02d", dia); // Convertir el día a cadena

    char mesStr[3];
    sprintf(mesStr, "%02d", mes); // Convertir el mes a cadena

    char anioStr[5];
    sprintf(anioStr, "%04d", anio); // Convertir el año a cadena
    strcat(fechaConcatenada, diaStr);
    strcat(fechaConcatenada, "_");
    strcat(fechaConcatenada, mesStr);
    strcat(fechaConcatenada, "_");
    strcat(fechaConcatenada, anioStr);
    strcat(fechaConcatenada, ".xyz");
    int largo = strlen(fechaConcatenada);
    fechaConcatenada[largo]='\0';
    FILE *pArchivoXYZ;
    pArchivoXYZ = fopen(fechaConcatenada, "wb");
    if(pArchivoXYZ!=NULL){
        for(i=0; i<*CLIENTES; i++){
            if(cred[i].orden!=0 && cred[i].activo==0){
                credXYZ[*CLIENTESBAJA].orden= cred[i].orden;
                strcpy(credXYZ[*CLIENTESBAJA].nombre, cred[i].nombre);
                strcpy(credXYZ[*CLIENTESBAJA].apellido, cred[i].apellido);
                credXYZ[*CLIENTESBAJA].importe=cred[i].importe;
                strcpy(credXYZ[*CLIENTESBAJA].tipoCredito, cred[i].tipoCredito);
                credXYZ[*CLIENTESBAJA].dia=cred[i].dia;
                strcpy(credXYZ[*CLIENTESBAJA].mes, cred[i].mes);
                credXYZ[*CLIENTESBAJA].anio=cred[i].anio;
                credXYZ[*CLIENTESBAJA].nroCuotas=cred[i].nroCuotas;
                credXYZ[*CLIENTESBAJA].importeCuotas=cred[i].importeCuotas;
                credXYZ[*CLIENTESBAJA].iva=cred[i].iva;
                credXYZ[*CLIENTESBAJA].totalCuota=cred[i].totalCuota;
                credXYZ[*CLIENTESBAJA].activo=cred[i].activo;
                (*CLIENTESBAJA)++;
                cred[i].orden=0;
                strcpy(cred[i].nombre, "");
                strcpy(cred[i].apellido, "");
                cred[i].importe=0.00;
                strcpy(cred[i].tipoCredito, "");
                cred[i].dia=0.00;
                strcpy(cred[i].mes, "");
                cred[i].anio=0;
                cred[i].nroCuotas=0;
                cred[i].importeCuotas=0.00;
                cred[i].iva=0.00;
                cred[i].totalCuota=0.00;
                cred[i].activo=0;
            }
        }
        fprintf(pArchivoXYZ, " ------------------------------------------------------------------------------------------------------------------------\n");
        fprintf(pArchivoXYZ, " %-6s%-10s%-10s%-10s%-16s|FECHA      %-12s%-14s%-9s%-11s%s","|ORDEN","|APELLIDO","|NOMBRE","|IMPORTE","|TIPO DE CREDITO","|NEO. CUOTAS","|IMPORTE CUOTA","|IVA","|TOTAL CUOTA","|ACTIVOS|\n");
        fprintf(pArchivoXYZ, " ------------------------------------------------------------------------------------------------------------------------\n");
        for(i=0; i<*CLIENTESBAJA; i++){
            fprintf(pArchivoXYZ," |%-5i|%-9s|%-9s|%-9.2f|%-15s|%-2i|%-3s|%-4i|%-11i|%-13.2f|%-8.2f|%-11.2f|%-7i|\n", credXYZ[i].orden, credXYZ[i].apellido, credXYZ[i].nombre, credXYZ[i].importe, credXYZ[i].tipoCredito, credXYZ[i].dia, credXYZ[i].mes, credXYZ[i].anio, credXYZ[i].nroCuotas, credXYZ[i].importeCuotas, credXYZ[i].iva, credXYZ[i].totalCuota, credXYZ[i].activo);
        }
        fprintf(pArchivoXYZ, " ------------------------------------------------------------------------------------------------------------------------\n");
        fclose(pArchivoXYZ);
    }else{
        funcionError(3);
        return;
    }
    FILE *pArchivoB;
    pArchivoB = fopen("archivoB.dat", "wb");
    if(pArchivoB==NULL){
        funcionError(3);
        return;
    }else{
        for(i=0; i<*CLIENTES; i++){
            fwrite(&cred[i], (sizeof(struct credito)), 1, pArchivoB);
        }
    }
    fclose(pArchivoB);
    printf("Baja f%csica realizada.\n", 161);

    return;
}

//Esta funcion lista el archivo de texto "clientes_bajas__<fecha>.xyz"
void funcionListarXYZ(struct credito credXYZ[], int *CLIENTESBAJA){
    int i;
    printf("Archivo XYZ listado:\n");
    printf(" ------------------------------------------------------------------------------------------------------------------------\n");
    printf(" %-6s%-10s%-10s%-10s%-16s|FECHA      %-12s%-14s%-9s%-11s%s","|ORDEN","|APELLIDO","|NOMBRE","|IMPORTE","|TIPO DE CREDITO","|NRO. CUOTAS","|IMPORTE CUOTA","|IVA","|TOTAL CUOTA","|ACTIVOS|\n");
    printf(" ------------------------------------------------------------------------------------------------------------------------\n");
    for(i=0; i<*CLIENTESBAJA;i++){
        printf(" |%-5i|%-9s|%-9s|%-9.2f|%-15s|%-2i|%-3s|%-4i|%-11i|%-13.2f|%-8.2f|%-11.2f|%-7i|\n", credXYZ[i].orden, credXYZ[i].apellido, credXYZ[i].nombre, credXYZ[i].importe, credXYZ[i].tipoCredito, credXYZ[i].dia, credXYZ[i].mes, credXYZ[i].anio, credXYZ[i].nroCuotas, credXYZ[i].importeCuotas, credXYZ[i].iva, credXYZ[i].totalCuota, credXYZ[i].activo);
    }
    printf(" ------------------------------------------------------------------------------------------------------------------------\n");
    printf("\n");

    return;
}

#endif // PEPITO_H_INCLUDED
