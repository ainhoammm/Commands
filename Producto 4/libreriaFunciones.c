#include "libreriaFunciones.h" 
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <locale.h>
#include <string.h>
#include <winsock2.h>
#include <windows.h>
#include <ctype.h>


#define MAX_LENGTH 1000
#define MAX_IP_LENGTH 16 
#define MAX_COMMAND_LENGTH 100


void guardarIpconfigAll() {
	system("ipconfig /all > informacionlocal.txt");
}


void listarNombresDeAdaptadores() {
	//Declaro puntero tipo FILE
	FILE* fp;

	fp = fopen("informacionlocal.txt", "r");
	// Verificar que el archivo se abrió correctamente
	if (fp == NULL) {
		printf("No se pudo abrir el archivo.\n");
		return;
	}

	// Declarar un buffer para leer cada línea
	char buffer[100];
	int i = 0;

	while (fgets(buffer, sizeof(buffer), fp) != NULL) {

		//Cuando la línea coincida con el texto Dirección física se mostrará
		if (strstr(buffer, "Adaptador de") != NULL) {
			i++;

			//Declaro un char array de 100 caracteres (Las direcciones MAC tienen ese tamaño siempre)
			char subStringLinea[100] = "";

			//Añadimos uno a la posicion encontrada y la sumamos al buffer para comenzar a leer desde ahi:
			strncpy(subStringLinea, buffer, sizeof(buffer));
			printf("%d - %s \n", i, subStringLinea);
		}
	}


	//Cerramos el archivo para liberar recursos
	fclose(fp);

}

// Inicializar contenido de archivo XML:
void inicializarArchivoXML(const char* archivo) {
	FILE* file = fopen(archivo, "w"); //write
	if (file == NULL) {
		fprintf(stderr, "Error al abrir el archivo XML\n");
		return;
	}

	fprintf(file, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
	fprintf(file, "<Producto4>\n");
	fprintf(file, "\t<Interface>\n");
	fclose(file);
}

// Añadir etiqueta y su contenido al archivo XML:
void anadirTag(const char* archivo, char* tag, char* valor, int numTab) {
	FILE* file = fopen(archivo, "a"); //append
	if (file == NULL) {
		fprintf(stderr, "Error al abrir el archivo XML\n");
		return;
	}

	//Añado tabulaciones:
	while (numTab > 0) {
		numTab--;
		fprintf(file, "\t");
	}

	fprintf(file, "<%s>%s</%s> \n", tag, valor, tag);
	fclose(file);
}


//abrir etiqueta XML
void abrirTag(const char* archivo, char* tag, int numTab) {
	FILE* file = fopen(archivo, "a"); //append
	if (file == NULL) {
		fprintf(stderr, "Error al abrir el archivo XML\n");
		return;
	}

	//Añado tabulaciones:
	while (numTab > 0) {
		numTab--;
		fprintf(file, "\t");
	}

	fprintf(file, "\t<%s> \n", tag);
	fclose(file);
}

//cerrar etiqueta XML
void cerrarTag(const char* archivo, char* tag) {
	FILE* file = fopen(archivo, "a"); //append
	if (file == NULL) {
		fprintf(stderr, "Error al abrir el archivo XML\n");
		return;
	}

	fprintf(file, "\t</%s> \n", tag);
	fclose(file);
}

// Cerrar etiqueta "Producto4" para finalizar el contenido de XML:
void finalizarArchivoXML(const char* archivo) {
	FILE* file = fopen(archivo, "a"); //append
	if (file == NULL) {
		fprintf(stderr, "Error al abrir el archivo XML\n");
		return;
	}
	fprintf(file, "\t\t</saltos>\n");
	fprintf(file, "\t</dns>\n");
	fprintf(file, "</Producto4>");
	fclose(file);
}



void anadirSaltosXML(const char* archivoXML) {
	FILE* archivo = fopen("saltosIPs.txt", "r");
	if (archivo == NULL) {
		printf("No se pudo abrir el archivo.\n");
		return;
	}

	char linea[256];

	int contadorSaltos = 0;

	while (fgets(linea, sizeof(linea), archivo)) {

		if (strstr(linea, "Traza") != NULL) {
			// La línea que contiene la palabra "Traza" se omite para guardar solo los saltos y no la ip destino en primer lugar ni la ultima linea en blanco
			continue;
		}
		if (strstr(linea, "solicitud") != NULL) {
			// La línea que contiene la palabra "solicitud" se omite para evitar guardar en caso de "5     *        *        *     Tiempo de espera agotado para esta solicitud."
			continue;
		}

		char* token;
		char* ip = NULL;

		// Buscar el primer espacio en blanco para omitir los primeros caracteres
		token = strtok(linea, " ");

		// Iterar hasta encontrar la dirección IP
		while (token != NULL) {
			// Comprobar si el token es una dirección IP válida
			if (strstr(token, ".") != NULL) {
				ip = token;
				break;
			}
			token = strtok(NULL, " ");
		}

		// Imprimir la dirección IP encontrada
		if (ip != NULL) {
			printf("IP: %s\n", ip);
			anadirTag(archivoXML, "salto", ip, 3);
		}

	}
	//anadirTag(archivoXML, "num-saltos", contadorSaltos, 1);

	fclose(archivo);
}


void obtenerInfoadaptador(const char* archivo, char* nombreAdaptador) {
	FILE* file;
	char line[MAX_LENGTH];
	int found = 0;

	file = fopen("informacionlocal.txt", "r");
	if (file == NULL) {
		printf("Error al abrir el archivo.\n");
		return;
	}

	while (fgets(line, sizeof(line), file)) {
		// Buscar el adaptador 
		if (strstr(line, nombreAdaptador) != NULL) {
			found = 1;
			continue;
		}
		// Leer la IP
		if (found && strstr(line, "IPv4") != NULL) {
			char ip[MAX_LENGTH];
			sscanf(line, "%*[^:]: %[^\n]", ip);
			anadirTag(archivo, "ip", ip, 2);
		}
		// Leer la máscara de subred
		if (found && strstr(line, "subred") != NULL) {
			char mascara[MAX_LENGTH];
			sscanf(line, "%*[^:]: %[^\n]", mascara);
			anadirTag(archivo, "mascara", mascara, 2);
		}

		// Leer la puerta de enlace predeterminada
		if (found && strstr(line, "predeterminada") != NULL) {
			char gateway[MAX_LENGTH];
			sscanf(line, "%*[^:]: %[^\n]", gateway);
			anadirTag(archivo, "gateway", gateway, 2);
			cerrarTag(archivo, "Interface");

		}

		// Leer el servidor DNS
		if (found && strstr(line, "Servidores DNS") != NULL) {
			char dns[MAX_LENGTH];
			sscanf(line, "%*[^:]: %[^\n]", dns);

			abrirTag(archivo, "dns", 0);
			anadirTag(archivo, "primario", dns, 2);
			abrirTag(archivo, "saltos", 1);


			//Hacer tracert

			char command[100];
			sprintf(command, "tracert -d -4 %s > saltosIPs.txt", dns);
			system(command);

			//Leer saltos y añadirlos a XML:
			anadirSaltosXML(archivo);


			break;
		}

	}

	fclose(file);
}



char* obtenerNombreAdaptador(int nadaptador) {
	char command[200];
	char tempfile[200];
	char line[200];
	char nombre[100];
	FILE* fp;

	// Componemos el comando para ejecutar en el shell y el nombre del archivo tempfile
	strcpy(command, "netsh interface ipv4 show config > tmpfile.txt");
	strcpy(tempfile, "tmpfile.txt");

	// Ejecutamos el comando
	system(command);

	// Abrimos el archivo tempfile en modo lectura
	fp = fopen(tempfile, "r");

	// Leemos cada línea del archivo tempfile y filtramos las que contienen "Configuracion para la interfaz" y de esas lineas filtradas cogemos solo la parte entre comillas que es el nombre del dispositivo
	int i = 1;
	while (fgets(line, sizeof(line), fp) != NULL) {

		if (strstr(line, "para la interfaz") != NULL) {
			//Si la interfaz es el numero de interfaz seleccionado obtengo su nombre:
			if (i == nadaptador) {
				//Obtener solo la parte entrecomillada:
				char* start, * end;
				start = strstr(line, "\"") + 1; // encuentra la primera comilla doble y avanza un caracter
				end = strstr(start, "\""); // encuentra la segunda comilla doble

				if (start && end) {
					*end = '\0'; // establece el caracter de final de cadena para extraer la parte entrecomillada

					char* nombre = start;
					printf("El adaptador seleccionado es: %s\n", nombre);
					return nombre;
				}
			}
			else {
				i = i + 1;
			}



		}
	}

	// Cerramos el archivo y eliminamos el archivo 
	fclose(fp);
	remove(tempfile);
}
