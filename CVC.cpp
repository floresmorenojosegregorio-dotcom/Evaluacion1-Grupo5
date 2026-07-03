‎#include <iostream>
‎#include <string>
‎#include <cstring>
‎#include <limits>
‎#include <iomanip>
‎// Necesario para std::system
‎#include <cstdlib>

‎//Incluye lo necesario para trabajar con archivos JSON
‎#include <json-c/json.h>
‎//Incluye las funciones necesarias para gestionar las respuestas HTTP
‎#include <cpr/cpr.h> 
‎#undef max 
‎//Retiramos la definicion de max que incluye cpr junto a un archivo de cabecera, ya que ocasiona conflictos con la funcion estatica max() perteneciente a numeric_limits
‎‎
// Estructura para almacenar las tasas de cambio referenciadas a 1 USD
‎struct TasasCambio {
‎    double usdToLocal;
‎    double usdToCrypto;
‎};

// Función para limpiar la pantalla según el sistema operativo
‎void limpiarPantalla() {
‎#if defined(_WIN32) || defined(_WIN64)
‎    std::system("cls");
‎#else
‎    std::system("clear");
‎#endif
‎}
