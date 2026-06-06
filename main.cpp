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
cout << " 1. Edixon Castro        -  V-31210293\n"
         << " 2. Jorge Andrade        -  V-31174882\n"
         << " 3. Bryan Nieves         -  V-32589282\n"
         << " 4. Felix Perez          -  V-31583083\n";
cout << " 5. Jose Flores          -  V-31381418\n"
         << " 6. Xander Losada        -  V-31842240\n"
         << " 7. Williamsnis Acosta   -  V-31797435\n"
         << " 8. Cesar Castrillo      -  V-31970219\n";
cout << " 9. Santiago Noguera     -  V-31541024\n"
         << "10. Johanfer Mendoza     -  V-32429275\n"
         << "11. Yorman Mendoza       -  V-31540613\n"
         << "----------------------------------------------------\n";

    return 0;
}
