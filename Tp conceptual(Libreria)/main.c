#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>



//Nombres de archivos
#define BIBLIOTECA "libros.bin"
#define COPIAS "copias.bin"
#define USUARIOS "usuarios.bin"
#define PRESTAMOS "prestamos.bin"

//Estados
#define DISPONIBLE 2
#define PRESTADO 1

#define TAPADURA 1
#define TAPABLANDA 2
#define BOLSILLO 3




typedef struct
{
    char nombre[30];
    char apellido[30];
    int anioNacimiento;
    char paisNac[30];

} Autor;


typedef struct
{
    Autor autor;
    char titulo[30];
    int cantidadPaginas;
    char genero[30];
    char editorial[30];
    int anioLanzamiento;
    int tipoLibro; //donde 1 indicara tapa dura, 2 tapa blanda y 3 ed.bolsillo
    char isbn[30];
    int diasMaxPrestamo;
    int historialPrestamos;
    int puntuacion;

} Libro;


typedef struct
{
    char nombre [30];
    char apellido [30];
    char dni [10];
    char direccion [30];
    char telefono [30];
    char email [50];
    int deuda;
    int cantidadPrestamos;

} Persona;


typedef struct
{

    int idInterno;
    char isbn[30];
    int estado; //(1-prestada, 2-disponible)

} Copia;


typedef struct
{

    int dia;
    int mes;
    int anio;

} Fecha;


typedef struct
{
    int iDInterno;
    char dni[30];
    Fecha fechaPrestamo;
    Fecha fechaDevolucion;
    int diasMaxPrestamo;
    int estado;

} Prestamo;




/// Prototipados

//Libros:
Autor cargarAutor();
Libro cargarLibro();
void cargarVariosLibros();
void mostrarLibro(Libro unLibro);
void leerListaLibros();
//Copias:
Copia crearCopia(char isbnOriginal[30]);
void crearVariasCopias(char isbnOriginal[30], int cantCopias);
int chequearDisponibles(char archivo); //devuelve un 1 si hay una copia disponible
//Personas/Usuarios:
Persona cargarUsuario();
void mostrarUsuario (Persona unaPersona);
void leerListaUsuarios ();
void chequearEstadoUsuario();
void calcularDeuda(Persona *a);
int chequearPrestamoPrevio(Persona a); //si la persona tiene un libro previo me retorna 1
int encontrarUsuario(char dniIngreso[9],Persona *resultado); //si se pudo encontrar o crear una persona, devuelve 1. En caso de error, 0.
void buscarLibroPorTitulo();
void buscarLibroPorAutor();
void buscarLibroPorAnioLanz();
void buscarLibroPorCantPag();
void buscarLibroPorEditorial();
void buscarLibroPorGenero();
//Prestamos:
void prestarLibro();
void devolverLibro();
//Fechas:
Fecha ingresarFecha();
//Menus
void buscarLibro();
void mensajeOpciones();
void iniciarSesionUsuario();
void opcionesUsuario(Persona ingresado);
void opcionesAdministrador();
void mensajeContinuar();
void opcionesUsuario(Persona ingresado);



/// MAIN

int main()
{
    char flag = 's';
    while(flag != '0')
    {
        mensajeOpciones();
        printf("\t\t[1] Iniciar sesion como administrador.\n");
        printf("\t\t[2] Iniciar sesion como usuario.\n");
        printf("\t\t[3] Registrar un nuevo usuario.\n");
        printf("\t\t[0] Salir.\n");
        fflush(stdin);
        flag = getch();
        system("cls");

        if(flag == '1')
        {
            opcionesAdministrador();
        }
        else if(flag == '2')
        {
            iniciarSesionUsuario();
        }
        else if(flag == '3')
        {
            Persona aux;
            aux = cargarUsuario();
            guardarUsuario(aux);
        }
        system("cls");
    }

    return 0;
}


///----------------- LIBROS -------------------

Autor cargarAutor()
{
    Autor temp;

    printf("Ingrese nombre de el autor\n");
    fflush(stdin);
    gets(temp.nombre);

    printf("Ingrese apellido de el autor\n");
    fflush(stdin);
    gets(temp.apellido);

    printf("Anio de nacimiento\n");
    fflush(stdin);
    scanf("%d", &temp.anioNacimiento);

    printf("Pais de nacimiento\n");
    fflush(stdin);
    gets(temp.paisNac);

    return temp;
}

Libro cargarLibro()
{
    Libro ejemplar;

    printf("\t\tSe procedera a consultar la informacion del libro\n");

    ejemplar.autor=cargarAutor();

    printf("\t\tTitulo del libro\n");
    fflush(stdin);
    gets(ejemplar.titulo);

    printf("\t\tCantidad de paginas\n");
    fflush(stdin);
    scanf("%d",&ejemplar.cantidadPaginas);

    printf("\t\tGenero literario\n");
    fflush(stdin);
    gets(ejemplar.genero);

    printf("\t\tEditorial\n");
    fflush(stdin);
    gets(ejemplar.editorial);

    printf("\t\tAnio de lanzamiento\n");
    fflush(stdin);
    scanf("%d",&ejemplar.anioLanzamiento);

    printf("\t\tTipo de libro donde 1 indicara tapa dura, 2 tapa blanda y 3 ed.bolsillo\n");
    fflush(stdin);
    scanf("%d",&ejemplar.tipoLibro);

    printf("\t\t ISBN\n");
    fflush(stdin);
    gets(ejemplar.isbn);

    printf("\t\tCantidad de dias habilitados para el prestamo\n");
    fflush(stdin);
    scanf("%d",&ejemplar.diasMaxPrestamo);

    ejemplar.historialPrestamos = 0; //como se crea por 1ra vez, no tiene prestamos aún

    printf("\t\tPuntuaje del 1 al 10\n");
    fflush(stdin);
    scanf("%d",&ejemplar.puntuacion);

    return ejemplar;
}

void cargarVariosLibros()
{
    char continuar = 's';

    FILE * archi = fopen(BIBLIOTECA, "ab"); //usamos el modo ab ya que funciona tanto si el archivo ya existe o si no fue creado todavía
    while(continuar == 's')
    {

        Libro temp = cargarLibro();
        fwrite(&temp, sizeof(Libro),1,archi);

        printf("Desea cargar otro libro? s/n\n");
        fflush(stdin);
        continuar = getche();
    }
    fclose(archi);
}

void mostrarLibro(Libro unLibro)
{
    printf("Titulo: %s\n", unLibro.titulo);
    printf("Autor: %s %s\n", unLibro.autor.nombre, unLibro.autor.apellido);
    printf("Anio: %d\n", unLibro.anioLanzamiento);
    printf("Genero: %s\n", unLibro.genero);
    printf("Editorial%s\n", unLibro.editorial);
    printf("#Paginas: %d\n", unLibro.cantidadPaginas);
    if(unLibro.tipoLibro == TAPADURA)
    {
        printf("Tipo: Tapa dura\n");
    }
    else if(unLibro.tipoLibro == TAPABLANDA)
    {
        printf("Tipo: Tapa blanda\n");
    }
    else if(unLibro.tipoLibro == BOLSILLO)
    {
        printf("Tipo: Edicion de bolsillo\n");
    }
    printf("ISBN: %s\n", unLibro.isbn);
    printf("Puntaje del 1 al 10: %d\n", unLibro.puntuacion);
    printf("Maximo de dias de prestamo: %dd\n", unLibro.diasMaxPrestamo);
    printf("Cant. de prestamos hasta la fecha: %d\n", unLibro.historialPrestamos);
}

void leerListaLibros()
{
    FILE * archi = fopen(BIBLIOTECA, "rb");
    if(archi==NULL) //si el archivo esta vacío
    {
        char continuar;
        printf("No existe el archivo.\n Desea cargar libros al archivo? s/n\n"); //se le pregunta al usuario si desea empezar a cargar el archivo
        fflush(stdin);
        continuar = getche();
        if(continuar == 's')
        {
            cargarVariosLibros();
        }

    }
    else //si no esta vacio, comienza a leer los registros
    {
        int i = 1;
        Libro temp;
        printf("\nLibros cargados: \n");
        while(fread(&temp,sizeof(Libro),1,archi)>0) //mientras no estemos al final del archivo, seguimos leyendo registros
        {
            i++;
            printf("--- #%d ---\n", i);
            mostrarLibro(temp);
        }
    }
    fclose(archi);
}

///----------------- LIBROS -------------------


///----------------- COPIAS -------------------


Copia crearCopia(char isbnOriginal[30])
{

    Copia copiaTemp;
    int idAux;
    FILE * archiCopias = fopen(COPIAS, "a+b"); //abro en modo append+lectura

    if(archiCopias==NULL) // en caso de que sea la primera copia creada, el id inicia en 1
    {
        idAux = 1;
    }
    else
    {
        fseek(archiCopias,sizeof(Copia)*(-1),SEEK_END); //me posiciono antes del ultimo registro
        fread(&copiaTemp,sizeof(Copia),1,archiCopias); //leo ese ultimo registro y desplazo el cursor al final del archivo
        idAux = copiaTemp.idInterno++; //copio la ID del ultimo registro y le sumo +1
    }


    //asigno todos los datos de mi nueva copia
    strcpy(copiaTemp.isbn,isbnOriginal); //funcion magica que copia el string, de la manera normal no funciona
    copiaTemp.idInterno = idAux;
    copiaTemp.estado = DISPONIBLE; //como es una copia nueva, esta disponible por defecto
    fwrite(&copiaTemp,sizeof(Copia),1,archiCopias); //escribo la copia al final del archivo
    fclose(archiCopias);
    return copiaTemp;
}

void crearVariasCopias(char isbnOriginal[30], int cantCopias)
{
    for(int i = 0; i < cantCopias; i++)
    {
        crearCopia(isbnOriginal);
    }
}

int chequearDisponibles(char archivo) //devuelve un 1 si hay una copia disponible
{
    FILE * buffer = fopen(COPIAS, "a+b"); //ya entro con el archivo de copias y lo voy a querer modificar y pisar, si usara solo ab no me pisa la info
    int flag = 0; //inicio con la bandera en cero
    Copia aux; //la que voy a usar para comparar
    char isbn[20]; //donde voy a guardar el dato

    printf("\t\tIngrese ISBN de la copia que quiere saber disponibilidad\n");
    fflush(stdin);
    gets(isbn);

    if (buffer!=NULL)
    {

        while((flag == 0)&&(fread(&aux,sizeof(Copia),1,buffer)>0))
        {
            if(strcmpi(aux.isbn,isbn)==0)
            {
                if(aux.estado==DISPONIBLE) //aca estoy comparando dos int (DISPONIBLE=2)
                {
                    flag = 1; //cambio la bandera si el libro esta disponible
                }
            }
        }
    }
    fclose(buffer);
    return flag;
}

///----------------- COPIAS -------------------


///----------- PERSONAS/USUARIOS -------------


void guardarUsuario(Persona temp)
{
    FILE * archi = fopen(USUARIOS, "ab");

    if (archi!=NULL)
    {
        fwrite(&temp,sizeof(Persona),1,archi);
        fclose(archi);
    }

    else
    {
        printf("\t\tError con el archivo\n");
    }

    printf("Usuario creado exitosamente\n");
    printf("Presione cualquier tecla para continuar...");
    getch();

}



Persona cargarUsuario()
{
    Persona unaPersona;

    printf ("Ingrese su nombre:\n");
    fflush (stdin);
    gets (unaPersona.nombre);

    printf ("Ingrese su apellido:\n");
    fflush (stdin);
    gets (unaPersona.apellido);

    printf ("Ingrese su DNI:\n");
    fflush (stdin);
    gets (unaPersona.dni); ///podemos hacer que no se pase los digitos

    printf ("Ingrese la direccion de su casa:\n");
    fflush (stdin);
    gets (unaPersona.direccion);

    printf ("Ingrese su telefono:\n");
    fflush (stdin);
    gets (unaPersona.telefono);

    printf ("Ingrese su email:\n");
    fflush (stdin);
    gets (unaPersona.email);

    unaPersona.deuda = 0; // como se crea por primera vez, no tiene deuda

    unaPersona.cantidadPrestamos=0; // como se crea por primera vez, no tiene deuda

    return unaPersona;
}

void mostrarUsuario (Persona unaPersona)
{
    printf ("Nombre: %s\n", unaPersona.nombre);
    printf ("Apellido: %s\n", unaPersona.apellido);
    printf ("DNI: %s\n", unaPersona.dni);
    printf ("Direcccion: %s\n", unaPersona.direccion);
    printf ("Telefono: %s\n", unaPersona.telefono);
    printf ("Email: %s\n", unaPersona.email);
    printf ("Deuda: %d\n", unaPersona.deuda);

}

void leerListaUsuarios ()
{
    FILE * archi = fopen(USUARIOS,"rb");
    if (archi==NULL)
    {
        char continuar;
        printf ("Todavia no se ha cargado ningun usuario.\n Desea cargar uno?s/n\n"); // si no hay le pregunto si quiere crear un usuario
        fflush (stdin);
        continuar = getche();
        if (continuar == 's')
        {
            Persona aux = cargarUsuario();
            fwrite(&aux, sizeof(Persona),1,archi);
        }
    }

    Persona temp;
    printf ("\nUsuarios cargados: \n");

    while (fread(&temp,sizeof(Persona),1,archi)>0)
    {
        printf ("%s %s\n", temp.nombre, temp.apellido);
    }

    fclose(archi);
}

int chequearPrestamoPrevio(Persona a) //si la persona tiene un libro previo me retorna 1
{
    int flag=0;
    Persona aux;
    FILE *archi;
    fopen("usuarios","rb"); //xq solo quiero chequear si tiene un prestamo previo o no

    if (archi!=NULL)
    {
        while (fread(&aux,sizeof(Persona),1,archi)>0)
        {
            if (strcmpi(a.dni,aux.dni)==0) //me encontro la persona
            {
                if (aux.cantidadPrestamos>0)
                {
                    printf("\t\tEl usuario cuenta con prestamo previo\n");
                    flag=1;
                }

                else
                {
                    printf("\t\tEl usuario no cuenta con deuda previa\n");
                }
            }

        }
        fclose(archi);
    }

    return flag;

}


void chequearEstadoUsuario() //sirve para veriricar si la persona tiene deuda o no
{
    Persona dato;
    Persona aux;
    int flag=0;

    printf("Se procedera a evaluar el estado de deuda del usuario\n");

    printf("Ingrese DNI del interesado\n");
    fflush(stdin);
    gets(dato.dni);

    FILE *archi;
    archi=fopen(USUARIOS,"rb");

    if (archi!=NULL)
    {
        while (fread(&aux,sizeof(Persona),1,archi)>0 && flag==0)

        {

            if (strcmpi(aux.dni,dato.dni)==0)//para comparar dos strings, y uso el que tiene i xq no distingue entre mayusuculas y minusculas
            {
                //si entra en este ciclo, es xq encontro un usuario con ese dni, ahora voy a querer saber si tiene deuda o no
                flag=1; //asi me aseguro que el ciclo se rompe cuando ya encuentra a la persona que quiere

                if (aux.deuda==0)
                {
                    printf("El usuario no cuenta con deuda, puede proceder a pedir un libro\n");
                    //y aca es donde se arranca el proceso para prestar la copia
                }

                else
                {
                    calcularDeuda(&dato); //entro con el archivo que tiene cargado los prestamos
                    printf("El usuario SI cuenta con deuda, y es de %d\n",aux.deuda); //digo que tiene una deuda y le muestro de cuanto es

                }

            }

        }
        fclose(archi);
    }
}

void calcularDeuda(Persona *a) //es el archivo de prestamos donde voy a buscar
{
    Persona aux = *a;
    Prestamo b;
    int dia;
    int mes;
    int anio;

    FILE *archi;
    fopen(PRESTAMOS,"rb");

    if (archi!=NULL)
    {
        while (fread(&b,sizeof(Prestamo),1,archi)>0)
        {
            if (strcmpi(aux.dni,b.dni)==0) //ai paso este if, busco dentro del archivo por medio del dni
            {
                anio = b.fechaDevolucion.anio - b.fechaPrestamo.anio;
                mes = b.fechaDevolucion.mes - b.fechaPrestamo.mes;
                dia = b.fechaDevolucion.dia - b.fechaPrestamo.dia;

                if (anio==0 && mes==0 && dia<=15) //digo que una persona puede tener como maximo 15 dias el libro
                {
                    aux.deuda=0;
                    printf("\t\ta deuda del usuario es %d",aux.deuda);
                }

                else if (dia>15)
                {
                    aux.deuda=dia*500; //por cada dia que se pase, se sumaran 500 a la deuda
                    printf("\t\ta deuda del usuario es %d",aux.deuda);
                }
            }
        }
    }
    *a = aux;

    fclose(archi);
}

int encontrarUsuario(char dniIngreso[9], Persona *resultado) //si se pudo encontrar o crear una persona, devuelve 1. En caso de error, 0.
{
    int flag = 0;
    Persona temp;
    FILE * archi = fopen(USUARIOS, "rb");

    if(archi!=NULL)
    {

        while((fread(&temp,sizeof(Persona),1,archi)>0) && (flag == 0))
        {


            if(strcmpi(temp.dni,dniIngreso)==0)

            {

                flag = 1;
            }
        }
    }
    fclose(archi);

    if(flag == 0)
    {
        char cont;
        printf("No se encontro ningun usuario correspondiente al dni.\n");
        printf("Desea crear un nuevo usuario? [S]/[N]\n");
        fflush(stdin);
        cont = getche();
        system("cls");

        if(cont == 's')
        {
            temp = cargarUsuario();
            guardarUsuario(temp);
            flag = 1;
        }
    }

    *resultado=temp;
    return flag;
}

///----------- PERSONAS/USUARIOS -------------



///--------------- PRESTAMOS -----------------

void prestarLibro()
{
    Persona aux;
    Prestamo dato; //aux que cargamos al archivo

    int flag=0;
    int contenedor; //idInterno si lo encuentra
    char dniTemp[20];

    int i=chequearDisponibles(COPIAS);

    if (i==1)
    {

        printf("Ingrese el DNI de la persona\n");
        fflush(stdin);
        gets(dniTemp);

        FILE *archi2=fopen("usuarios","a+b"); //abro el archivo de los usuarios

        if (archi2!=NULL)
        {
            while (fread(&aux, sizeof(Persona),1,archi2)>0)
            {
                if (strcmpi(dniTemp,dato.dni)==0)
                {
                    int bandera=chequearPrestamoPrevio(aux);

                    if (bandera==1)
                    {
                        printf("\t\tEl usuario cuenta con un prestamo previo\n");

                    }

                    else if (bandera==0)
                    {
                        printf("\t\tComo la persona no cuenta con un prestamo previo, se puede proceder a darle uno\n");

                        FILE*archi=fopen("archivoPrestamos","a+b"); //estoy creando el archivo prestamos y/o se modifica

                        if (archi!=NULL)
                        {
                            while (fread(&dato,sizeof(Prestamo),1,archi)>0 && flag==0)
                            {
                                if (strcmpi(dniTemp,dato.dni)==0 )
                                {

                                    flag=1;

                                    dato.iDInterno = contenedor;
                                    dato.fechaPrestamo = ingresarFecha();
                                    dato.estado = PRESTADO;

                                    fseek(archi2,-1,SEEK_CUR); //xq esta parado despues del ultimo sino y yo quiero cambiar el estado de la ultima persona cargada

                                    aux.cantidadPrestamos=1; //cambio el estado del prestamo
                                    fwrite(&aux,sizeof(Persona),1,archi2); //asi me pisa entera la estructura con los datos nuevos de la persona
                                    fwrite(&dato,sizeof(Prestamo),1,archi); //para que me cargue todos los datos en mi variable aux


                                }

                            }

                            if (flag==0) //esto es por si es la primera vez que saca un libro y el dni no estaba previamente registrado
                            {
                                printf("Como es la primera vez que saca un libro, se procedera a introducirlo\n");

                                printf("Ingrese el ID interno de la copia a prestar\n");
                                fflush(stdin);
                                gets(dato.iDInterno);

                                dato.fechaPrestamo = ingresarFecha();
                                dato.estado = PRESTADO;

                                fwrite(&dato,sizeof(Prestamo),1,archi); //para que me cargue todos los datos en mi variable aux

                            }
                            fclose(archi);
                        }
                    }
                }
            }
            fclose(archi2);
        }


    }
    else
    {
        printf("No hay volumenes disponibles, por lo que no sera posible efectuar el prestamo\n");
    }
}

void devolverLibro()
{
    Prestamo dato; //aux que cargamos al archivo
    char isbn[30]; //isbn del libro que se busca
    char dniTemp[8]; //DNI de la persona

    printf("Ingrese el isbn del libro que va a prestar\n");
    fflush(stdin);
    gets(isbn);

    printf("Ingrese el DNI de la persona\n");
    fflush(stdin);
    gets(dniTemp);

    FILE*archi=fopen("archivoPrestamos","a+b"); //voy a modificar el archivo

    if (archi!=NULL)
    {
        while (fread(&dato,sizeof(Prestamo),1,archi)>0)
        {
            ///quizas chequear si ya hizo un prestamo en algun lado
            if (strcmpi(dniTemp,dato.dni)==0)
            {
                dato.fechaDevolucion = ingresarFecha(); //para guardar la fecha de devolucion
                dato.estado = DISPONIBLE; //para que me cambie el estado de la copia

                fwrite(&dato,sizeof(Prestamo),1,archi); //para que me cargue todos los datos en mi variable aux

            }

        }

        fclose(archi);
    }

}

///--------------- PRESTAMOS -----------------





///--------------- FECHAS -----------------

Fecha ingresarFecha()
{
    Fecha aux;
    printf("Ingrese dia de la fecha\n");
    fflush(stdin);
    scanf("%d",&aux.dia);

    printf("Ingrese mes de la fecha\n");
    fflush(stdin);
    scanf("%d",&aux.mes);

    printf("Ingrese anio de la fecha\n");
    fflush(stdin);
    scanf("%d",&aux.anio);

    return aux;
}

///--------------- FECHAS -----------------




///----------------- MENUS -------------------

void mensajeOpciones()
{
    printf("Presiona la tecla correspondiente a la opcion que desea elegir:\n\n");
}

void iniciarSesionUsuario()
{
    char dniIngresado[9];
    Persona aux;
    printf("Ingrese su DNI por favor:");
    fflush(stdin);
    gets(dniIngresado);



    int flag = encontrarUsuario(dniIngresado,&aux);
    if (flag==1)
    {
        printf("encontro persona\n");

        opcionesUsuario(aux);
    }

}

void opcionesUsuario(Persona ingresado)
{
    char flag = 's';
    while(flag != '0')
    {
        mensajeOpciones();
        printf("\t\t[1] Ver lista completa de libros.\n");
        printf("\t\t[2] Busqueda de libros.\n");
        printf("\t\t[3] Realizar prestamo\n");
        printf("\t\t[4] Realizar devolucion de prestamo.\n");
        printf("\t\t[5] Ver estado de deuda.\n");
        printf("\t\t[0] Salir del menu\n");
        fflush(stdin);
        flag = getch();
        system("cls");

        if(flag=='1')
        {
            leerListaLibros();
        }
        else if(flag == '2')
        {
            buscarLibro();
        }
        else if(flag == '3')
        {
            prestarLibro();
        }
        else if(flag == '4')
        {
            devolverLibro();
        }
        else if(flag == '5')
        {
            calcularDeuda(&ingresado);
        }
    }
}


void prestarRecibirLibro()
{
    int operacion;
    char mander='s';


    while (mander=='s')
    {
        printf("Si desea realizar un prestamo un libro presione 1, si desea hacer una devolucion presione 2\n");
        fflush(stdin);
        scanf("%d",&operacion);

        if(operacion ==1)
        {
            prestarLibro();
            mander='n';
        }

        else if (operacion==2) //faltaria hacer este en caso de devolver un libro
        {
            devolverLibro();
            mander='n';
        }

        else //en caso de que accidentalmente presione otra tecla, asi vuelve a empezar el ciclo
        {
            mander='s';
        }
    }
}

///----------------- MENUS -------------------



void buscarLibro()
{
    int opcion;
    printf("\t\tIndique bajo que condicion quiere buscar el libro donde 1 es por Titulo, 2 por Autor, 3 por Cant de paginas, 4 por Genero, 5 por Editorial y 6 por Anio de lanzaminto\n");
    fflush(stdin);
    scanf("%d",&opcion);

    if (opcion==1)
    {
        buscarLibroPorTitulo;
    }

    else if (opcion==2)
    {
        buscarLibroPorAutor;
    }

    else if (opcion==3)
    {
        buscarLibroPorCantPag;
    }

    else if (opcion==4)
    {
        buscarLibroPorGenero;
    }

    else if (opcion==5)
    {
        buscarLibroPorEditorial;
    }

    else
    {
        buscarLibroPorAnioLanz;
    }

}

void buscarLibroPorTitulo()
{
    Libro buscado;
    char nombre[30];

    printf("\t\tIngrese el titulo del libro que quiere buscar\n");
    fflush(stdin);
    gets(nombre);

    FILE * archi = fopen(BIBLIOTECA, "rb"); //abro este archivo xq es donde tengo los libros guardados

    if (archi!=NULL)
    {
        while(fread(&buscado,sizeof(Libro),1,archi)>0) //mientras no estemos al final del archivo
        {
            if (strcmpi(nombre,buscado.titulo)==0) //si encuentra el libro con ese titulo
            {
                mostrarLibro(buscado);
            }
        }
    }

    else
    {
        printf("\t\tNo fue posible encontrar el libro indicado\n");
    }

    fclose(archi);
}

void buscarLibroPorAutor()
{
    Libro buscado;
    Autor autor;

    printf("\t\tIngrese el nombre del autor del libro que quiere buscar\n");
    fflush(stdin);
    gets(autor.nombre);

    FILE * archi = fopen(BIBLIOTECA, "rb"); //abro este archivo xq es donde tengo los libros guardados

    if (archi!=NULL)
    {
        while(fread(&buscado,sizeof(Libro),1,archi)>0) //mientras no estemos al final del archivo
        {
            if (strcmpi(autor.nombre,buscado.autor.nombre)==0) //si encuentra el libro con ese titulo
            {
                mostrarLibro(buscado);
            }
        }
    }

    else
    {
        printf("\t\tNo fue posible encontrar el libro indicado\n");
    }

    fclose(archi);
}

void buscarLibroPorCantPag()
{
    Libro buscado;
    int paginas;

    printf("\t\tIngrese la cantidad de paginas del libro que quiere buscar\n");
    fflush(stdin);
    scanf("%d",&paginas);

    FILE * archi = fopen(BIBLIOTECA, "rb"); //abro este archivo xq es donde tengo los libros guardados

    if (archi!=NULL)
    {
        while(fread(&buscado,sizeof(Libro),1,archi)>0) //mientras no estemos al final del archivo
        {
            if (paginas==buscado.cantidadPaginas) //si encuentra el libro con ese titulo
            {
                mostrarLibro(buscado);
            }
        }
    }

    else
    {
        printf("\t\tNo fue posible encontrar el libro indicado\n");
    }

    fclose(archi);
}

void buscarLibroPorGenero()
{
    Libro buscado;
    char genero[30];

    printf("\t\tIngrese el genero del libro que quiere buscar\n");
    fflush(stdin);
    gets(genero);

    FILE * archi = fopen(BIBLIOTECA, "rb"); //abro este archivo xq es donde tengo los libros guardados

    if (archi!=NULL)
    {
        while(fread(&buscado,sizeof(Libro),1,archi)>0) //mientras no estemos al final del archivo
        {
            if (strcmpi(genero,buscado.genero)==0) //si encuentra el libro con ese titulo
            {
                mostrarLibro(buscado);
            }
        }
    }

    else
    {
        printf("\t\tNo fue posible encontrar el libro indicado\n");
    }

    fclose(archi);
}
void buscarLibroPorEditorial()
{
    Libro buscado;
    char editorial[30];

    printf("\t\tIngrese la editorial del libro que quiere buscar\n");
    fflush(stdin);
    gets(editorial);

    FILE * archi = fopen(BIBLIOTECA, "rb"); //abro este archivo xq es donde tengo los libros guardados

    if (archi!=NULL)
    {
        while(fread(&buscado,sizeof(Libro),1,archi)>0) //mientras no estemos al final del archivo
        {
            if (strcmpi(editorial,buscado.editorial)==0) //si encuentra el libro con ese titulo
            {
                mostrarLibro(buscado);
            }
        }
    }

    else
    {
        printf("\t\tNo fue posible encontrar el libro indicado\n");
    }

    fclose(archi);
}

void buscarLibroPorAnioLanz()
{
    Libro buscado;
    int anioLanz;

    printf("\t\tIngrese el titulo del libro que quiere buscar\n");
    fflush(stdin);
    scanf("%d",&anioLanz);

    FILE * archi = fopen(BIBLIOTECA, "rb"); //abro este archivo xq es donde tengo los libros guardados

    if (archi!=NULL)
    {
        while(fread(&buscado,sizeof(Libro),1,archi)>0) //mientras no estemos al final del archivo
        {
            if (strcmpi(anioLanz,buscado.anioLanzamiento)==0) //si encuentra el libro con ese titulo
            {
                mostrarLibro(buscado);
            }
        }
    }

    else
    {
        printf("\t\tNo fue posible encontrar el libro indicado\n");
    }

    fclose(archi);
}

void opcionesAdministrador()
{
    char flag = 's';
    while(flag != '0')
    {
        mensajeOpciones();
        printf("\t\t[1] Ver lista completa de libros.\n");
        printf("\t\t[2] Busqueda de libros.\n");
        printf("\t\t[3] Ingresar nuevos libros al sistema.\n");
        printf("\t\t[4] Ingresar copias de un libro existente\n");
        printf("\t\t[5] Leer lista de usuarios\n");
        printf("\t\t[6] Ver informacion de un usuario existente\n");
        printf("\t\t[7] Registrar pago de deuda de usuario\n");
        printf("\t\t[0] Salir del menu\n");

        fflush(stdin);
        flag = getch();
        system("cls");

        if(flag=='1')
        {
            leerListaLibros();
            mensajeContinuar();
        }
        else if(flag == '2')
        {
            buscarLibro();
            mensajeContinuar();
        }
        else if(flag == '3')
        {
            cargarVariosLibros();
        }
        else if(flag == '4')
        {
            char dato[20];
            int cant = 0;
            printf("Ingrese el ISBN del libro que desea cargar copias.");
            fflush(stdin);
            gets(dato);
            printf("Ingrese la cantidad de copias a cargar");
            fflush(stdin);
            scanf("%d",&cant);
            crearVariasCopias(dato,cant);
            mensajeContinuar();
        }
        else if(flag == '5')
        {
            leerListaUsuarios();
            mensajeContinuar();
        }
        else if(flag == '6')
        {
            char dniIngresado[9];
            printf("Ingrese el DNI del usuario.");
            fflush(stdin);
            scanf("%s", &dniIngresado);

            Persona dato;
            flag = encontrarUsuario(dniIngresado,&dato);
            if(flag == 1)
            {
                mostrarUsuario(dato);
            }
            else
            {
                printf("El usuario no existe.");
            }

        }
        else if(flag == '7')
        {
            char dniIngresado[9];
            printf("Ingrese el DNI del usuario.");
            fflush(stdin);
            scanf("%s", &dniIngresado);

            Persona dato;
            flag = encontrarUsuario(dniIngresado,&dato);
            if(flag == 1)
            {
                ///falta funcion cancelar deuda
            }
            else
            {
                printf("El usuario no existe.");
            }
        }
        system("cls");

    }
}

void mensajeContinuar()
{
    printf("Presiona cualquier tecla para continuar...\n");
    getch();
}
