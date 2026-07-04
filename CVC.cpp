#include <iostream>
#include <string>
#include <cstring>
#include <limits>
#include <iomanip>
//Incluye lo necesario para trabajar con archivos JSON
#include <json-c/json.h>
//Incluye las funciones necesarias para gestionar las respuestas HTTP
#include <cpr/cpr.h>
// Necesario para std::system
#include <cstdlib> 
#undef max //Retiramos la definicion de max que incluye cpr junto a un archivo de cabecera, ya que ocasiona conflictos con la funcion estatica max() perteneciente a numeric_limits



// Estructura para almacenar las tasas de cambio referenciadas a 1 USD
struct TasasCambio {
    double usdToLocal;
    double usdToCrypto;
};

// Función para limpiar la pantalla según el sistema operativo
void limpiarPantalla() {
#if defined(_WIN32) || defined(_WIN64)
    std::system("cls");
#else
    std::system("clear");
#endif
}

// Función para pausar la ejecución hasta que se presione una tecla
void esperarTecla() {
    std::cout << "\nPresione ENTER para continuar y volver al menu...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get(); // Espera la pulsación de la tecla
}

// Funciones de validación y entrada de datos
double solicitarDouble(const std::string& mensaje) {
    double valor = 0.0;
    while (true) {
        std::cout << mensaje;
        if (std::cin >> valor && valor > 0) {
            return valor;
        }
        else {
            std::cout << "[!] ERROR!! Por favor, introzca un valor numerico mayor a cero.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
}

// Función para realizar las conversiones lógicas
void procesarConversion(int opcion, const TasasCambio& tasas, const std::string& MonedaLocal, const std::string& Crypto) {
    double monto = 0.0;
    double resultado = 0.0;

    switch (opcion) {
    case 1: // Bolivares a USD
        monto = solicitarDouble("Introduce el monto en " + MonedaLocal + ": ");
        resultado = monto / tasas.usdToLocal;
        std::cout << std::fixed << std::setprecision(2) << "\n-> " << monto << " " << MonedaLocal << " equivalen a " << resultado << " USD.\n";
        break;

    case 2: // USD a Bolivares
        monto = solicitarDouble("Introduce el monto en USD: ");
        resultado = monto * tasas.usdToLocal;
        std::cout << std::fixed << std::setprecision(2) << "\n-> " << monto << " USD equivalen a " << resultado << " " << MonedaLocal << ".\n";
        break;

    case 3: // Bolivares a ETH
        monto = solicitarDouble("Introduce el monto en " + MonedaLocal + ": ");
        resultado = (monto / tasas.usdToLocal) * tasas.usdToCrypto;
        std::cout << std::fixed << std::setprecision(8) << "\n-> " << monto << " " << MonedaLocal << " equivalen a " << resultado << " " << Crypto << ".\n";
        break;

    case 4: // ETH a Bolivares
        monto = solicitarDouble("Introduce el monto en " + Crypto + ": ");
        resultado = (monto / tasas.usdToCrypto) * tasas.usdToLocal;
        std::cout << std::fixed << std::setprecision(2) << "\n-> " << monto << " " << Crypto << " equivalen a " << resultado << " " << MonedaLocal << ".\n";
        break;

    case 5: // USD a ETH
        monto = solicitarDouble("Introduce el monto en USD: ");
        resultado = monto * tasas.usdToCrypto;
        std::cout << std::fixed << std::setprecision(8) << "\n-> " << monto << " USD equivalen a " << resultado << " " << Crypto << ".\n";
        break;

    case 6: // ETH a USD
        monto = solicitarDouble("Introduce el monto en " + Crypto + ": ");
        resultado = monto / tasas.usdToCrypto;
        std::cout << std::fixed << std::setprecision(2) << "\n-> " << monto << " " << Crypto << " equivalen a " << resultado << " USD.\n";
        break;

    default:
        std::cout << "[!] ERROR!! Opcion Invalida.\n";
        break;
    }
}

//Funcion para Imprimir el menu
void imprimirMenu() {
    std::cout << "==================================================\n";
    std::cout << "              OPCIONES DISPONIBLES                \n";
    std::cout << "==================================================\n";
    std::cout << "1. Convertir Bolivares (VES) --> USD\n";
    std::cout << "2. Convertir Dolares (USD)   --> VES\n";
    std::cout << "3. Convertir Bolivares (VES) --> ETH\n";
    std::cout << "4. Convertir ETHEREUM (ETH)  --> VES\n";
    std::cout << "5. Convertir Dolares (USD)   --> ETH\n";
    std::cout << "6. Convertir ETHEREUM (ETH)  --> USD\n";
    std::cout << "7. Salir del programa\n";
    std::cout << "--------------------------------------------------\n";
    std::cout << "Seleccione una opcion (1-7): ";
}

int main() {
    TasasCambio tasas = { 0.0, 0.0 };

    std::cout << "==================================================\n";
    std::cout << "             BIENVENIDO USUARIO AL                \n";
    std::cout << "      CONVERSOR DE DIVISAS Y CRIPTOACTIVOS        \n";
    std::cout << "                     C.D.C                        \n";
    std::cout << "==================================================\n";

    //Fase 1: Inicializacion de Tasas de Cambio 
    int config = 0;
    double precioCryptoEnUsd;
    //Bucle de configuracion inicial
    while (true) {

        //Limpia la pantalla al inicio del bucle
        limpiarPantalla();
        std::cout << "\n------ CONFIGURACION INICIAL ------\n";

        std::cout << "\n------Seleccione el metodo de recopilacion de los activos------\n";
        std::cout << "1) Obtener valores por internet\n";
        std::cout << "2) Ingresar valores manualmente\n";
        std::cout << "3) Salir del programa\n";
        std::cout << "Seleccione una opcion: ";

        if (!(std::cin >> config)) {
            std::cout << "[!] ERROR!! Por favor, introduzca un numero valido.\n";
            std::cin.clear();
            esperarTecla();
            continue;
        }

        else if (config == 1) {
            //Creamos los punteros de tipo json_object que se usaran para extraer las tasas
            json_object* json;
            json_object* valor;
            //Ingresamos el URL de la API que tiene acceso al dolar oficial (BCV) 
            std::string url = "https://ve.dolarapi.com/v1/dolares/oficial";
            //Variable que almacenara la respuesta web
            cpr::Response respuesta;
            std::cout << "Extrayendo valores de internet...";
            //Bucle for que extrae los valores
            for (int i = 0; i < 2; i++) {
                //La funcion Get devuelve una instancia de tipo Response de la url que se almacena en la variable respuesta
                respuesta = cpr::Get(cpr::Url{ url });
                std::cout << "\n";
                //Verifica que la respuesta haya sido exitosa
                if (respuesta.status_code != 200) { std::cout << "Error al intentar extraer los valores...\n";config = 0;esperarTecla();break; }
                if (i == 0) {
                    //Convierte el texto en un json object
                    json = json_tokener_parse(respuesta.text.c_str());
                    //Extrae un puntero hacia el valor de la clave "promedio" que contiene el precio de dolar
                    valor = json_object_object_get(json, "promedio");
                    //Uso de la funcion json_object_get_double que convierte ese valor en un double
                    tasas.usdToLocal = json_object_get_double(valor);
                    //Se actualiza la url para que apunte hacia la api para extraer el valor del Ethereum
                    url = "https://api.binance.com/api/v3/ticker/price?symbol=ETHUSDT";
                    //Se libera la memoria de json_object antes de cada finalizar la iteracion, esto porque la funcion usa memoria dinamica, y si no se libera puede generar fugas de memoria
                    json_object_put(json);

                }
                else {
                    //Mismo procedimiento que en el if anterior, pero aplicado para extraer el  Ethereum
                    json = json_tokener_parse(respuesta.text.c_str());
                    //En este caso la clave ya no es "promedio" si no "price"
                    valor = json_object_object_get(json, "price");
                    precioCryptoEnUsd = json_object_get_double(valor);
                    tasas.usdToCrypto = 1.0 / precioCryptoEnUsd;
                    //Se libera la memoria de json_object antes de cada finalizar la iteracion, esto porque la funcion usa memoria dinamica, y si no se libera puede generar fugas de memoria
                    json_object_put(json);
                }


            }
            //Mensajes de notificacion al usuario
            if (config == 0)continue;
            std::cout << "\nValores extraidos con exito:\n\n";
            std::cout << "Dolar (Bs): " << tasas.usdToLocal << std::endl;
            std::cout << "Ethereum (Usd): " << precioCryptoEnUsd << std::endl;
            esperarTecla();
            break;
        }

        else if (config == 2) {
            //Ingreso de datos de manera manual
            tasas.usdToLocal = solicitarDouble("Ingrese el Valor de 1 USD en Bolivares: ");
            precioCryptoEnUsd = solicitarDouble("Ingrese el Valor de 1 ETH en USD: ");
            tasas.usdToCrypto = 1.0 / precioCryptoEnUsd;
            break;
        }

        else if (config == 3) {
            std::cout << "Saliendo del programa\n";
            return 0;
        }

        else {
            std::cout << "[!] ERROR!! Opcion Invalida. Reintente de nuevo.\n";
            esperarTecla();
        }


    }

    int opcionMenu = 0;
    //Bucle principal 
    do {
        limpiarPantalla(); // Limpia el buffer visual 
        // Fase 2: Menu de opciones Principal
        imprimirMenu();

        // Fase 3: Procesamiento de transacciones
        if (!(std::cin >> opcionMenu)) {
            std::cout << "[!] ERROR!! Por favor, introduzca un numero valido.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            esperarTecla();
            continue;
        }

        if (opcionMenu >= 1 && opcionMenu <= 6) {
            std::cout << "\n";
            procesarConversion(opcionMenu, tasas, "VES", "ETH");
            esperarTecla(); // Pausa después de mostrar el resultado
        }
        else if (opcionMenu != 7) {
            std::cout << "[!] ERROR!! Opcion Invalida. Reintente de nuevo.\n";
            esperarTecla();
        }

    } while (opcionMenu != 7);

    //Despedida
    limpiarPantalla();
    std::cout << "\nEstimado Usuario, gracias por usar el C.D.C. ¡Tenga un excelente dia!\n";
    return 0;
}