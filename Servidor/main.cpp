#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <mysql/mysql.h>



int iniciar_sesion(char* nombre, char* contrasena) { //La funcion "iniciar_sesion" toma dos parametros: "nombre" y "contrasena" que son los datos de inicio de sesiÃ³n del usuario que se van a verificar en la base de datos. Devuelve un valor entero que indica si el usuario existe en la base de datos o no.
	MYSQL* conexion;
	MYSQL_RES* resultado;
	MYSQL_ROW fila;
	int existe_usuario = 0;
	
	conexion = mysql_init(NULL); //Se inicializa la conexiÃ³n a la base de datos utilizando la funciÃ³n "mysql_init". En caso de que la conexiÃ³n no se pueda establecer, se imprime un mensaje de error y se termina el programa.
	if (mysql_real_connect(conexion, "localhost", "root", "mysql", "juego", 0, NULL, 0) == NULL) {
		fprintf(stderr, "%s\n", mysql_error(conexion));
		exit(1);
	}
	
	char consulta[1000];
	sprintf(consulta, "SELECT COUNT(*) FROM usuarios WHERE nombre='%s' AND contrasena='%s'", nombre, contrasena); //Esta lÃ­nea construye una consulta SQL que cuenta el nÃºmero de filas en la tabla usuarios que coinciden con el nombre y la contraseÃ±a proporcionados. La consulta utiliza la funciÃ³n sprintf para construir la consulta utilizando los valores de los parÃ¡metros nombre y contrasena.
	
	if (mysql_query(conexion, consulta)) { //Se ejecuta la consulta utilizando la funciÃ³n "mysql_query" y se verifica si hubo algÃºn error en la ejecuciÃ³n de la consulta. Si hay un error, se imprime un mensaje de error y se termina el programa.
		fprintf(stderr, "%s\n", mysql_error(conexion));
		exit(1);
	}
	
	resultado = mysql_store_result(conexion); //Esta linea almacena los resultados de la consulta SQL en el objeto resultado.
	fila = mysql_fetch_row(resultado); //A continuaciÃ³n, se llama a la funciÃ³n mysql_fetch_row() para obtener la siguiente fila de resultados. Esta funciÃ³n devuelve un puntero a una estructura MYSQL_ROW que contiene los datos de una fila de resultados. Como la consulta devuelve una sola fila con un unico valor (el nÃºmero de usuarios con el nombre y contraseÃ±a proporcionados), no es necesario iterar sobre las filas de resultados.
	existe_usuario = atoi(fila[0]); //Entonces, como esta consulta devuelve una Ãºnica fila y una Ãºnica columna con el nÃºmero de usuarios que cumplen la condiciÃ³n, se obtiene el valor de esta columna usando fila[0] y se convierte a un valor entero usando atoi(). Si el valor de existe_usuario es 0, significa que no existe un usuario con el nombre y contraseÃ±a proporcionados. Si el valor de existe_usuario es 1, significa que sÃ­ existe un usuario con el nombre y contraseÃ±a proporcionados. Este valor se devuelve al final de la funciÃ³n iniciar_sesion().
	
	mysql_free_result(resultado);
	mysql_close(conexion);
	
	return existe_usuario;
}

int registrarse(char* usuario, char* contrasena)
{
	MYSQL* conexion = mysql_init(NULL); // inicializar objeto de conexión a MySQL
	int resultado = 0; // variable para almacenar el resultado de la operación
	
	if (conexion == NULL) // comprobar si la inicialización de la conexión fue exitosa
	{
		printf("Error al inicializar la conexión a MySQL: %s\n", mysql_error(conexion));
		return resultado; // devolver valor predeterminado de resultado (0)
	}
	
	// conectar a la base de datos
	if (mysql_real_connect(conexion, "localhost", "root", "mysql", "juego", 0, NULL, 0) == NULL)
	{
		printf("Error al conectar a la base de datos: %s\n", mysql_error(conexion));
		mysql_close(conexion);
		return resultado; // devolver valor predeterminado de resultado (0)
	}
	
	// crear consulta SQL para comprobar si el usuario ya existe en la base de datos
	char consulta1[200];
	sprintf(consulta1, "SELECT COUNT(*) FROM JUGADOR WHERE USERNAME='%s'", usuario);
	
	// ejecutar la consulta1: Luego se ejecuta la consulta utilizando la funciÃ³n mysql_query, y si ocurre algÃºn error en la ejecuciÃ³n, se muestra un mensaje de error, se libera la memoria utilizada y se devuelve el valor predeterminado de la variable resultado, que es 0.
	if (mysql_query(conexion, consulta1))
	{
		printf("Error al ejecutar la consulta1: %s\n", mysql_error(conexion));
		mysql_close(conexion);
		return resultado; // devolver valor predeterminado de resultado (0)
	}
	
	// obtener los resultados de la consulta1: Si la consulta se ejecuta correctamente, se obtienen los resultados utilizando la funciÃ³n mysql_use_result, que devuelve un puntero a una estructura MYSQL_RES que contiene los resultados de la consulta. Si ocurre algÃºn error al obtener los resultados, se muestra un mensaje de error, se libera la memoria utilizada y se devuelve el valor predeterminado de la variable resultado.
	MYSQL_RES* resultados1 = mysql_use_result(conexion);
	if (resultados1 == NULL)
	{
		printf("Error al obtener los resultados de la consulta1: %s\n", mysql_error(conexion));
		mysql_close(conexion);
		return resultado; // devolver valor predeterminado de resultado (0)
	}
	
	// leer el valor de la columna COUNT(*) del primer registro de la tabla de resultados
	MYSQL_ROW fila1 = mysql_fetch_row(resultados1); //La lÃ­nea MYSQL_ROW fila1 = mysql_fetch_row(resultados1); es la que lee la primera fila de resultados de la consulta que se ejecutÃ³ previamente. Esta fila contiene un conjunto de valores de la tabla que se corresponden con la consulta realizada.La funciÃ³n mysql_fetch_row devuelve un puntero a un array de cadenas que representan los valores de la fila actual. Cada elemento del array representa un campo de la tabla y el orden de los elementos corresponde al orden de los campos en la tabla.
	if (fila1 == NULL)
	{
		printf("Error al leer los resultados de la consulta1: %s\n", mysql_error(conexion));
		mysql_free_result(resultados1);
		mysql_close(conexion);
		return resultado; // devolver valor predeterminado de resultado (0)
	}
	
	int num_filas1 = mysql_num_rows(resultados1);
	int num_campos1 = mysql_num_fields(resultados1);
	int valor1 = atoi(fila1[0]); // A continuaciÃ³n, se convierte el valor obtenido de la columna COUNT(*) a un entero utilizando la funciÃ³n atoi. Si el valor obtenido es mayor que cero, entonces el usuario ya existe en la base de datos, se muestra un mensaje indicando que el usuario ya existe, se libera la memoria utilizada y se devuelve el valor predeterminado de la variable resultado.
	
	// comprobar si el usuario ya existe en la base de datos
	if (valor1 > 0) 
	{
		printf("El usuario ya existe en la base de datos\n");
		mysql_free_result(resultados1);
		mysql_close(conexion);
		return resultado; // devolver valor predeterminado de resultado (0)
	}
	
	// crear consulta SQL para insertar el nuevo usuario y la contraseña en la base de datos
	char consulta2[200];
	sprintf(consulta2, "INSERT INTO usuarios (nombre, contrasena) VALUES ('%s', '%s')", usuario, contrasena);
	
	// ejecutar la consulta2
	if (mysql_query(conexion, consulta2))
	{
		printf("Error al ejecutar la consulta2: %s\n", mysql_error(conexion));
		mysql_free_result(resultados1);
		mysql_close(conexion);
		return resultado; // devolver valor predeterminado de resultado (0)
	}
	
	// En el código que mostraste, no hay un else después del if porque no es necesario en este caso. Si la función mysql_query() devuelve un valor diferente de 0, se asume que hubo un error al ejecutar la consulta, por lo que el código dentro del if se ejecuta y devuelve el valor predeterminado de resultado (0).
	// Si la función mysql_query() devuelve un valor igual a 0, se asume que la consulta se ejecutó correctamente, por lo que el código dentro del if no se ejecuta y se continúa con el resto del código. En este caso, no es necesario un else porque no hay ninguna acción adicional que realizar si la consulta se ejecuta correctamente.
	
	resultado = 1; // asignar 1 a la variable resultado para indicar que el registro se realizó correctamente
	
	mysql_free_result(resultados1); // liberar la memoria de los resultados de la consulta1
	mysql_close(conexion); // cerrar la conexión a la base de datos
	
	return resultado; // devolver el valor de la variable resultado (1 si el registro se realizó correctamente, 0 si hubo algún error)
}

	
using namespace std;

int main (int argc, char *argv[]) {
	
	int sock_conn, sock_listen, ret;
	struct sockaddr_in serv_adr;
	char peticion[512];
	char respuesta[512];
	// INICIALITZACIONS
	// Obrim el socket
	if ((sock_listen = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		printf("Error creant socket");
	// Fem el bind al port
	
	
	memset(&serv_adr, 0, sizeof(serv_adr));// inicialitza a zero serv_addr
	serv_adr.sin_family = AF_INET;
	
	// asocia el socket a cualquiera de las IP de la m?quina. 
	//htonl formatea el numero que recibe al formato necesario
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	// escucharemos en el port 9070
	serv_adr.sin_port = htons(9000);
	if (bind(sock_listen, (struct sockaddr *) &serv_adr, sizeof(serv_adr)) < 0)
		printf ("Error al bind");
	//La cola de peticiones pendientes
	if (listen(sock_listen, 3) < 0)
		printf("Error en el Listen");
	
	int finConexion = 0;
	
	while (finConexion==0)
	{
		printf ("Escuchando\n");
		
		sock_conn = accept(sock_listen, NULL, NULL);
		printf ("He recibido conexi?n\n");
		//sock_conn es el socket que usaremos para este cliente
		
		// Ahora recibimos el mensaje, que dejamos en buff
		ret=read(sock_conn,peticion, sizeof(peticion));
		printf ("Recibido\n");
		
		// Tenemos que a?adirle la marca de fin de string 
		// para que no escriba lo que hay despues en el buffer
		peticion[ret]='\0';
		
		printf ("Peticion: %s\n",peticion);
		

		char *p = strtok(peticion, "/");
		int codigo = atoi (p);
			
		if (p == 0)
			finConexion = 1;
				
		else
		{
			p = strtok (NULL, "/");
			char usuario[20];
			strcpy (usuario, p);
				
			p = strtok (NULL, "/");
			char contrasenya[20];
			strcpy (contrasenya, p);
				
			if (codigo == 1)
			{
				int inicioSesion = iniciar_sesion(usuario,contrasenya); // funcion que devuelve 1 se todo va bien 0 si hay error 2 si el usuario y la contraseña no coinciden
				//int inicioSesion = 1;
				if (inicioSesion == 1) // El usuario y la contraaeña coinciden
					sprintf (respuesta, "BIEN");
					
				else if (inicioSesion == 0) // El usuario y la contraseña no coinciden
					sprintf(respuesta, "INCORRECTO");
				else if (inicioSesion == 2) // El usuario ya existe
					sprintf(respuesta, "EXISTE");
			}
			else if (codigo == 2)
			{
				//int registro = registrarse(usuario,contrasenya); // funcion que devuelve 1 se todo va bien 0 si hay error 2 si el usuario ya existe
				int registro = 1;
				if (registro == 1) // Se ha registrado
					sprintf (respuesta, "BIEN");
				
				else if (registro == 0) // ERROR
					sprintf(respuesta, "ERROR");
				else if (registro == 2) // El usuario ya existe
					sprintf(respuesta, "EXISTE");
			}
		}
			
			printf("%s",respuesta);
			write (sock_conn,respuesta, strlen(respuesta));
			
			// Se acabo el servicio para este cliente
			close(sock_conn); 
	}		
}
























