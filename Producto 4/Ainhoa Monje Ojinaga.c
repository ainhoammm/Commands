#define _CRT_SECURE_NO_WARNINGS
#include "libreriaFunciones.h"
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <locale.h>
#include <string.h>
#include <winsock2.h>
#include <windows.h>
#include <ctype.h>
#include <string.h>



int main() {
	//Constantes:
	const char* archivoXML = "Producto4.xml";

	//Declaración de variables
	int resp;

	//Inicializo variable
	resp = -1;

	while (resp != 0)
	{
		printf("\n Selecciona la opcion deseada:");
		printf("\n 1. Elegir adaptador ");
		printf("\n 0. Pulse 0 para salir del menu. \n");

		scanf("\n %d", &resp);

		if (resp == 1) {

			printf("\n Elegir uno de los dispositivos:\n ");
			guardarIpconfigAll();

			listarNombresDeAdaptadores();


			//Seleccionar adaptador:
			int numadaptador;
			scanf("\n %d", &numadaptador);

			//Escribir en XML:
			inicializarArchivoXML(archivoXML);

			char* nombreAdaptador = obtenerNombreAdaptador(numadaptador);


			obtenerInfoadaptador(archivoXML, nombreAdaptador);

			finalizarArchivoXML(archivoXML);
			printf("Archivo XML generado exitosamente.\n");
		}
		else {
			break;
		}
	}

	return 0;
}