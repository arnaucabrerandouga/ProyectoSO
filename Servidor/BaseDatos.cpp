#include <mysql.h>
#include <mysql.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>


int main(int argc, char **argv)
{
	MYSQL *conn;
	int err;
	MYSQL_RES *resultado;
	MYSQL_ROW row;
	char consulta[80];
	
	//Creamos una conexion al servidor MYSQL 
	conn = mysql_init(NULL);
	if (conn == NULL) {
		printf("Error al crear la conexión: %u %s\n", mysql_errno(conn), mysql_error(conn));
		exit(1);
	}
	
	// Inicializar la conexión
	MYSQL *conexion = mysql_real_connect(conn, "localhost", "root", "mysql", "juego", 0, NULL, 0);
	if (conexion == NULL) {
		printf("Error al inicializar la conexión: %u %s\n", mysql_errno(conn), mysql_error(conn));
		exit(1);
	}
	
	// Definir la consulta SQL como una cadena de caracteres
	strcpy(consulta, "SELECT COUNT(*) as 'Total partidas Jugadas' FROM REGISTRO WHERE REGISTRO.ID_J = '1';");
	
	// Ejecutar la consulta SQL
	if (mysql_query(conexion, consulta)) {
		printf("Error en la consulta: %s\n", mysql_error(conexion));
		exit(1);
	}
	
	// Obtener el resultado de la consulta y mostrarlo en la pantalla
	resultado = mysql_store_result(conexion);
	row = mysql_fetch_row(resultado);
	int total_partidas_jugadas = atoi(row[0]);
	printf("Total de partidas jugadas por el jugador: %d\n", total_partidas_jugadas);
	
	// cerrar la conexion con el servidor MYSQL 
	mysql_close(conexion);
	exit(0);
}
