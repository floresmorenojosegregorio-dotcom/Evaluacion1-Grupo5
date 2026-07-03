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

‎// Función para pausar la ejecución hasta que se presione una tecla
‎void esperarTecla() {
‎    std::cout << "\nPresione ENTER para continuar y volver al menu...";
‎    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
‎    std::cin.get(); // Espera la pulsación de la tecla
‎}

‎// Funciones de validación y entrada de datos
‎double solicitarDouble(const std::string& mensaje) {
‎    double valor = 0.0;
‎    while (true) {
‎        std::cout << mensaje;
‎        if (std::cin >> valor && valor > 0) {
‎            return valor;
‎        }
‎        else {
‎            std::cout << "[!] ERROR!! Por favor, introzca un valor numerico mayor a cero.\n";
‎            std::cin.clear();
‎            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
‎        }
‎    }
‎}

‎// Función para realizar las conversiones lógicas
‎void procesarConversion(int opcion, const TasasCambio& tasas, const std::string& MonedaLocal, const std::string& Crypto) {
‎    double monto = 0.0;
‎    double resultado = 0.0;
‎
‎    switch (opcion) {
‎    case 1: // Bolivares a USD
‎        monto = solicitarDouble("Introduce el monto en " + MonedaLocal + ": ");
‎        resultado = monto / tasas.usdToLocal;
‎        std::cout << std::fixed << std::setprecision(2) << "\n-> " << monto << " " << MonedaLocal << " equivalen a " << resultado << " USD.\n";
‎        break;
‎
‎    case 2: // USD a Bolivares
‎        monto = solicitarDouble("Introduce el monto en USD: ");
‎        resultado = monto * tasas.usdToLocal;
‎        std::cout << std::fixed << std::setprecision(2) << "\n-> " << monto << " USD equivalen a " << resultado << " " << MonedaLocal << ".\n";
‎        break;
‎
‎    case 3: // Bolivares a ETH
‎        monto = solicitarDouble("Introduce el monto en " + MonedaLocal + ": ");
‎        resultado = (monto / tasas.usdToLocal) * tasas.usdToCrypto;
‎        std::cout << std::fixed << std::setprecision(8) << "\n-> " << monto << " " << MonedaLocal << " equivalen a " << resultado << " " << Crypto << ".\n";
‎        break;
‎
‎    case 4: // ETH a Bolivares
‎        monto = solicitarDouble("Introduce el monto en " + Crypto + ": ");
‎        resultado = (monto / tasas.usdToCrypto) * tasas.usdToLocal;
‎        std::cout << std::fixed << std::setprecision(2) << "\n-> " << monto << " " << Crypto << " equivalen a " << resultado << " " << MonedaLocal << ".\n";
‎        break;
‎
‎    case 5: // USD a ETH
‎        monto = solicitarDouble("Introduce el monto en USD: ");
‎        resultado = monto * tasas.usdToCrypto;
‎        std::cout << std::fixed << std::setprecision(8) << "\n-> " << monto << " USD equivalen a " << resultado << " " << Crypto << ".\n";
‎        break;
‎
‎    case 6: // ETH a USD
‎        monto = solicitarDouble("Introduce el monto en " + Crypto + ": ");
‎        resultado = monto / tasas.usdToCrypto;
‎        std::cout << std::fixed << std::setprecision(2) << "\n-> " << monto << " " << Crypto << " equivalen a " << resultado << " USD.\n";
‎        break;
‎
‎    default:
‎        std::cout << "[!] ERROR!! Opcion Invalida.\n";
‎        break;
‎    }
‎}
‎
‎
‎//Funcion para Imprimir el menu
‎void imprimirMenu() {
‎    std::cout << "==================================================\n";
‎    std::cout << "              OPCIONES DISPONIBLES                \n";
‎    std::cout << "==================================================\n";
‎    std::cout << "1. Convertir Bolivares (VES) --> USD\n";
‎    std::cout << "2. Convertir Dolares (USD)   --> VES\n";
‎    std::cout << "3. Convertir Bolivares (VES) --> ETH\n";
‎    std::cout << "4. Convertir ETHEREUM (ETH)  --> VES\n";
‎    std::cout << "5. Convertir Dolares (USD)   --> ETH\n";
‎    std::cout << "6. Convertir ETHEREUM (ETH)  --> USD\n";
‎    std::cout << "7. Salir del programa\n";
‎    std::cout << "--------------------------------------------------\n";
‎    std::cout << "Seleccione una opcion (1-7): ";
‎}
‎
int main() {
‎    TasasCambio tasas = { 0.0, 0.0 };

‎    std::cout << "==================================================\n";
‎    std::cout << "             BIENVENIDO USUARIO AL                \n";
‎    std::cout << "      CONVERSOR DE DIVISAS Y CRIPTOACTIVOS        \n";
‎    std::cout << "                     C.D.C                        \n";
‎    std::cout << "==================================================\n";

  //Fase 1: Inicializacion de Tasas de Cambio 
‎    int config=0;
‎    double precioCryptoEnUsd;
‎    //Bucle de configuracion inicial
‎    while(true){
‎
‎    //Limpia la pantalla al inicio del bucle
‎    limpiarPantalla();
‎
‎    std::cout << "\n------ CONFIGURACION INICIAL ------\n";
‎
‎    std::cout << "\n------Seleccione el metodo de recopilacion de los activos------\n";
‎    std::cout << "1) Obtener valores por internet\n";
‎    std::cout << "2) Ingresar valores manualmente\n";
‎    std::cout << "3) Salir del programa\n";
‎    std::cout << "Seleccione una opcion: ";
‎
‎    if (!(std::cin >> config)) {
‎        std::cout << "[!] ERROR!! Por favor, introduzca un numero valido.\n";
‎        std::cin.clear();
‎        esperarTecla();
‎        continue;
‎    }
