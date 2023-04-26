#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <mysql/mysql.h>
#include <pthread.h>

///-----------------------------------------------------------------------------
int contador;
//Estructura para acceso excluyente
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

///-----------------------------------------------------------------------------
//Variables globales:
//	SQL:
MYSQL *conn;
int err;
MYSQL_RES *resultado;
MYSQL_ROW row;

//Variables para la lista 
int i=0;
int sockets[100];

// Contador de id para cada registrado
int reg;

///-----------------------------------------------------------------------------
//Estructuras:
//	Conectdo
typedef struct {
	//Declaramos una estructura que almacena un nombre y su socket
	char nombre[20];
	int socket; //valor entero que representa una conexion de red, se utiliza para identificar la conexion y permitir el envio y recepcion de datos a traves de ella
}Conectado;

//	ListaConectados
typedef struct {
	Conectado conectados[50];
	int num;
}ListaConectados;

// Creem la llista generica i mes tard la inicialitzarem
ListaConectados miLista;


//	Funcion que nos permite añadir un usuario a la lista cuando este se conecte
int Add (ListaConectados *lista, char nombre[20],int socket){
	//En caso de lista llena retorna -1 y si ha añadido bien al jugador retorna 0	
	printf("Inicio Add\n");
	printf("Numero: %d",lista->num);
	int a = lista->num;
		
	if (lista->num==50)
		return -1;
	else{
		printf("Añadiendo \n");
		strcpy(lista->conectados[lista->num].nombre,nombre);
		printf("Nombre: %s\n",lista->conectados[lista->num].nombre);
		lista->conectados[lista->num].socket = socket;
		printf("Socket: %d\n",socket);
		lista->num=a+1;
		printf("Numero: %d",lista->num);
		printf("Nombre:%s Socket: %d\n",lista->conectados[a].nombre,lista->conectados[a].socket);
		return 0;
		}
	
	printf("Final Add \n");

	}
	
	//Devuelve el Socket en el caso de lo que lo encuentre
	int ReturnSocket (ListaConectados *lista, char nombre[20]){
		int i = 0;
		int encontrado = 0;
		printf("Numero: %d\n",lista->num);
		while((i < 50) && encontrado==0)
		{
			if (strcmp(lista->conectados[i].nombre,nombre) == 0)
			{
				encontrado = 1;
			}
			if(encontrado==0)
			{
				i = i+1;
			}
		}
		if (encontrado==1)
		{
			return lista->conectados[i].socket;
		}
		else
		{
			return -1;
		}
	}
	
	//	Buscamos la posicion de un usuario en nuestra lista
	int SearchPosition (ListaConectados *lista, char nombre[20]){
		//Devuelve la posicion o -1 si no lo encuentra.
		
		printf("Inicio SearchPosition\n");
		int i =0;
		int encontrado=0;
		while ((i<50) && (!encontrado)){
			if (strcmp(lista->conectados[i].nombre,nombre)==0)
				encontrado=1;
			else{
				i=i+1;
			}
		}
		if (encontrado == 1)
			return i;
		else{
			return -1;
		}
		printf("Final SearchPosition\n");
	}

	// S'utilitza per treure de la llista la persona que s'ha desconectat
	int Disconect (ListaConectados *lista, char nombre[20]){
		// Ens retorna 0 en cas que l'usuari descconectat s'hagi eliminat 
		//correctament de la llista i retorna -1 si l'usuari que es vol 
		// desconectar no es troba en la llista de conectats i per tant hi ha un error.
		printf("Inicio Disconect\n");
		int pos = SearchPosition(lista, nombre);
		if (pos==-1){
			return -1;
		}
		else{
			for (int i=pos; i<lista->num-1;i++){
				lista ->conectados[i] = lista->conectados[i+1];
				strcpy(lista->conectados[i].nombre, lista->conectados[i+1].nombre);
				lista->conectados[i].socket==lista->conectados[i+1].socket;
			}
			lista->num--;
			return 0;
		}
		printf("Final Disconect\n");
	}

//	Creamos un string con todos los conectados 
	void DameConectados (ListaConectados *lista, char respuesta [1024]){
		//Devuelve llena la lista conectados. Esta se llenara con el nombre de 
		//todos los conectados separados por /. Ademas primero nos dara el numero 
		//de conectados.
		
		char resp[1024];
		
		printf("Inicio DameConectados\n");
		sprintf(respuesta,"%d",lista->num);
		printf("%s\n",respuesta);
		
		int i;
		for (i=0; i<lista->num; i++){
			sprintf(resp, "%s/%s \n",resp, lista->conectados[i].nombre);
			printf("%s\n",resp);
		}
		sprintf(respuesta,"%d/%s",6,resp);
		printf("Final DameConectados\n");
	}
	//	Comprueba si un usuario existe y lo aï¿±ade a la lista conectados
	int ConectarUsuario (char *p,char respuesta [1024], int socket, ListaConectados *lista){
		//Despues de comprobar si elusuario se encuentra en la base de datos lo aï¿±ado a 
		//la ListaConectados y genera el mensaje pertinente. Devuelve 1 si ha 
		//realizado la funcion con exito y 0 alternativamente.
		
		printf("Inicio ConectarUsuario\n");
		//Variables locales
		char nombre [20];
		char contrasena [20];
		char consulta [1024];
		char resp[1024];
		
		// Conectarse con un usuario registrado
		p = strtok( NULL, "/");
		
		//Obtenemos el nombre y la contraseña
		strcpy (nombre, p);
		p=strtok(NULL,"/");
		strcpy(contrasena,p);
		printf ("Nombre: %s Contraseña: %s \n", nombre, contrasena);
		
		//Establecemos la busqueda
		sprintf(consulta,"SELECT ID FROM JUGADOR Where USERNAME = '%s' AND PASSWORD ='%s';",nombre,contrasena);
		//escribimos en el buffer respuestra la longitud del nombre
		err=mysql_query (conn, consulta);
		if (err!=0)
		{
			printf ("Error al consultar datos de la base %u %s \n",
					mysql_errno(conn), mysql_error(conn));
			sprintf(resp,"Vaya un error se ha producido");
		}
		
		else{
			resultado = mysql_store_result(conn);
			row = mysql_fetch_row(resultado);
			printf("%s\n",row[0]);
			sprintf(resp,"Bien venido");
			int r = Add(lista,nombre, socket);
		}
		sprintf(respuesta,"1/%s",resp);
		printf("%s\n",respuesta);
		printf("Final ConectarUsuario\n");
	}
	
	//	Aï¿±ade un usuario y lo guarda en la lista conectados
	int RegistrarUsuario(char *p,char respuesta [1024], int socket){
		//Aï¿±adimos un nuevo usuario a nuestra base de datos y a nuestra Listadeconectados
		//devolvemos 1 si hemos realizado la accion correctamente y 0 alternativamente.
		
		printf("Inicio RegistrarUsuario\n");
		printf("Socket: %d\n",socket);
		//Variables locales
		char nombre [20];
		char contrasena [20];
		char consulta [1024];
		char resp[1024];
		
		//Crear nuevo ususario
		p = strtok( NULL, "/");
		
		//Obentenmos su nombre y contrasenya
		strcpy (nombre, p);
		p=strtok(NULL,"/");
		strcpy(contrasena,p);
		printf ("Nombre: %s Contraseña: %s \n", nombre, contrasena);

		//Establecemos la busqueda para encontrar un id no repetido para el jugador
		sprintf(consulta,"SELECT MAX(ID) FROM JUGADOR;");
		//Miramos si hay errores en la consulta, si no hay consegu
		err=mysql_query (conn, consulta);
		if (err!=0)
		{
			printf ("Error al consultar datos de la base %u %s \n",
					mysql_errno(conn), mysql_error(conn));
			sprintf(resp,"Vaya un error se ha producido");
		}
		
		else{
			resultado = mysql_store_result(conn);
			row = mysql_fetch_row(resultado);
			printf("Último id registrado:%s\n",row[0]);
			reg = atoi(row[0])+1;
			
			//Registrem el nou jugador a la base de dades
			sprintf(consulta, "INSERT INTO JUGADOR (ID, USERNAME, PASSWORD) VALUES ('%d', '%s', '%s')",reg, nombre, contrasena);
			
			//escribimos en el buffer respuestra la longitud del nombre
			err=mysql_query (conn, consulta);
			if (err!=0)
			{
				printf ("Error al consultar datos de la base %u %s \n",
						mysql_errno(conn), mysql_error(conn));
				sprintf(resp,"Vaya un error se ha producido un error \n");
			}
			else{
				sprintf(resp,"Bien venido al club");
			}
		}
		sprintf(respuesta,"%d/%s",2,resp);
		printf("Final RegistrarUsuario\n");
	}
	
	
	//------------------------------------------------------------------------------
	//Funcion atender cliente:
	void *AtenderCliente (void *socket){
		printf("Inicio \n");
		//Socket para la ListaConectados
		
		int sock_conn;
		int *s;
		s=(int *) socket;
		sock_conn = *s;
		
		printf("Socket: %d \n",sock_conn);
		
		//Aqui recogermos la peticion del usuario y la respuesta del servidor
		char peticion[1024];
		char respuesta[1024];
		char conectado[200];
		int contadorSocket=0;
		int ret;
		
		int socketO;//Origen
		int socketD;//Destino
		char nomO[20];
		char nomD[20];
		
		//Creamos e inicializamos la lista
		
		//Establecemos la conexion con nuestro servidor
		conn = mysql_init(NULL);
		if (conn==NULL){
			
			printf ("Error al crear la conexion: %u %s\n",
					mysql_errno(conn), mysql_error(conn));
			exit (1);
		}
		
		conn = mysql_real_connect (conn, "localhost","root", "mysql","juego",0, NULL, 0);
		
		printf("Conexion iniciada\n");
		if (conn==NULL){
			
			printf ("Error al inicializar la conexion: %u %s\n",
					mysql_errno(conn), mysql_error(conn));
			exit (1);
		}
		
		int terminar =0;
		//Abrimos un bucle que atendera a todas las peticiones que entren de un 
		//determinado cliente hasta que este se desconecte
		
		while(terminar==0){
			//Variables que usaremos en nuestro while
			int codigo;
			char nombre[20];
			char contrasena[20];
			int partidas_ganadas;
			char consulta [80];
			
			// Ahora recibimos su peticion
			ret=read(sock_conn,peticion, sizeof(peticion));
			printf ("Recibido \n");
			
			// Tenemos que añadirle la marca de fin de string 
			// para que no escriba lo que hay despues en el buffer
			peticion[ret]='\0';
			
			//Escribimos el nombre en la consola para comprobar el mensaje recibido
			printf ("Se ha conectado: %s \n",peticion);

			//Recogemos el codigo obtenido
			char *p = strtok( peticion, "/");
			
			// Troceamos el mensaje adquirido dividiendo entre funcion y datos
			codigo =  atoi (p); 
			
			printf ("Se ha conectado: %s \n",peticion);
			//Comprobacion del codigo obtenido
			printf ("Codigo: %d \n",codigo);
			
			//Deconectamos a un usuario
			if (codigo==0){
				//Funcion de desconecion
				printf("Cierre \n");
				sprintf(respuesta,"Adios");
				sprintf(respuesta,"%d/%s",0,respuesta);
				terminar=1;
			}
			else {
				//Conectamos a un usuario
				if (codigo == 1) {
					ConectarUsuario(p,respuesta,sock_conn,&miLista);
					pthread_mutex_unlock(&mutex);
					printf("1\n");
				}
				
				
				
				
				//Registramos a un usuario
				if(codigo == 2){
					RegistrarUsuario(p, respuesta, sock_conn);
					printf("2\n");
				}
				
				
				
				
				//Devuelve la lista de conectados manualmente
				if (codigo == 6){
					DameConectados(&miLista,respuesta);
					printf("6\n");
				}
				
				
				
				
				//Devuelve el numero de consultas hechas
				if (codigo == 8){
					sprintf (respuesta,"%d/%d",8,contador);
					printf("8\n");
				}
				
				
				
				//Enviamos la respuesta de nuestras consultas
				if (codigo != 0){
					printf("Respuesta: %s \n", respuesta);
					printf("Socket receptor: %d\n",sock_conn);
					// Enviamos la respuesta
					write (sock_conn,respuesta, strlen(respuesta)); //envía la respuesta generada por la función 
					//al cliente que ha realizado la solicitud en el socket de conexión establecido.
					printf("Respuesta enviada al cliente \n");
				}

				
				
				//Contamos en numero de consultas
				if ((codigo==1)||(codigo==2)||(codigo==6)||(codigo==8)){
					pthread_mutex_lock(&mutex);//no interrumpas
					contador = contador +1;
					pthread_mutex_unlock(&mutex);//puedes interrumpirme
					printf("Peticion contada\n");
					
					//Notificamos a todos los clientes conectados
					char notificacion[1024];
					char n [1024];
					DameConectados(&miLista,n);
					sprintf (notificacion,"%s",n);
					
					
					//Enviamos a todos los clientes conectados
					int j;
					for (j=0; j<i; j++){
						write (sockets[j],notificacion, strlen(notificacion));
					}
					break;
					//memset(notificacion,"",1024);
					//sprintf("%s",notificacion);
				}

			}
			printf("Numero: %d \n",miLista.num);
			printf("Usuario: %s \n",miLista.conectados[miLista.num].nombre);
			printf("Final\n");
			printf("Respuesta: %s \n", respuesta);
			// Se acabo el servicio para este cliente
		}
		close(sock_conn); 
	}
	
	

int main(int argc, char *argv[]) {
	//Variables
	int sock_conn, sock_listen, ret;
	struct sockaddr_in serv_adr;
	
	// INICIALITZACIONS
	// Obrim el socket
	if ((sock_listen = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		//socket en el que el servidor espera un pedido
		printf("Error creant socket \n");
		// Hacemos el bind al puerto
	}
	
	memset(&serv_adr, 0, sizeof(serv_adr));
	// inicialitza a zero serv_addr
	
	serv_adr.sin_family = AF_INET;
	//asocio el socket con la ip de la maquina que lo ha generado
	
	//htonl formatea el numero que recibe al formato necesario
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY); //INADDR_ANY configura siempre 
	//la ip asignada
	
	
	// escucharemos en el port 9050 este port puede variar en funcion del pc que
	//haga de srvidor
	serv_adr.sin_port = htons(9070); // indicamos el puerto
	
	if (bind(sock_listen, (struct sockaddr *) &serv_adr, sizeof(serv_adr)) < 0)
		printf ("Error al bin \n");
	//La cola de peticiones pendientes no podr? ser superior a 4
	if (listen(sock_listen, 2) < 0) 
		// indicamos que es sock pasivo, el dos marca el numero de peticiones 
		//maximas en cola
		printf("Error en el Listen \n");
	
	pthread_t thread;
	
	ListaConectados miLista;
	miLista.num=0;
	
	for(;;){
		
		printf ("Escuchando\n"); //No ayuda a saber si hemos empezado a escuchar
		
		sock_conn = accept(sock_listen, NULL, NULL);
		//este sock se comunica con el programa con el que hemos conectado
		
		printf ("He recibido conexion\n"); //comprovamos si todo en orden
		//sock_conn es el socket que usaremos para este cliente
		//Bucle de atencion al cliente
		
		//Llenamos el vector de sockets
		sockets[i]=sock_conn;
		
		pthread_create (&thread,NULL,AtenderCliente,&sockets[i]);
		i++;
	}
	
	return 0;
}

