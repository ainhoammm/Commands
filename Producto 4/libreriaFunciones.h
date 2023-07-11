#define _CRT_SECURE_NO_WARNINGS
#define LIBRERIAFUNCIONES_H



void guardarIpconfigAll();
void listarNombresDeAdaptadores();
void inicializarArchivoXML(const char* archivo);
void anadirTag(const char* archivo, char* tag, char* valor, int numTab);
void abrirTag(const char* archivo, char* tag, int numTab);
void cerrarTag(const char* archivo, char* tag);
void finalizarArchivoXML(const char* archivo);
void anadirSaltosXML(const char* archivoXML);
void obtenerInfoadaptador(const char* archivo, char* nombreAdaptador);
char* obtenerNombreAdaptador(int nadaptador);



