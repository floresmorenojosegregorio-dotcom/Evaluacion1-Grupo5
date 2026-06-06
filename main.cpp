#include <iostream>
#include <string>

using namespace std;

int main() {

    cout << "====================================================\n";
    cout << "          Bienvenidos, somos el Grupo 5.\n";
    cout << "====================================================\n"; 

string nombreUsuario;
    cout << "\nPor favor, ingrese su nombre para continuar: ";
    getline(cin, nombreUsuario);

cout << "\n¡Bienvenido/a, " << nombreUsuario << "!\n\n"
         << "Listado de integrantes del Grupo 5 que participaron:\n"
         << "----------------------------------------------------\n";
