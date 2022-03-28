
// AUTOR: Dana B. Choque Zárate
// FECHA: 29/03/2022
// EMAIL: alu0101328348@ull.edu.es
// ASIGNATURA: Seguridad en Sistemas Informáticos
// PRÁCTICA Nº: 5º MULTIPLICACIÓN EN SNOW 3G y AES
// UNIVERSIDAD: Universidad de La Laguna- Escuela Superior de Ingeniería y Tecnología
// TITULACIÓN: Grado en Ingenieria informática
// ACTUALIZAR EL CMAKE: cmake .
// COMPILAR: make
// EJECUTAR: ./snow3G-AES
// ELIMINAR EL EJECUTABLE: make clean
// Librería de colores: https://gist.github.com/Alfonzzoj/db207b89d56f24d9d0b17ff93e091be8

#include <iostream>
#include <bitset>

#define RESET   "\x1b[0m"
#define BLUE    "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define ROSE    "\x1B[38;2;255;151;203m"
#define CYAN    "\x1b[36m"
#define CLEAR "\x1B[2J\x1B[H"

#define CteSnow3g "A9"
#define CteAES "1B"


std::bitset<8> 
multiplicacion(std::string hex1, std::string hex2, std::string contanteAlg) {
  std::bitset<8> byte1Binario(std::stoi(hex1, nullptr, 16));
  std::bitset<8> byte2Binario(std::stoi(hex2, nullptr, 16));
  std::bitset<8> byteAlg(std::stoi(contanteAlg, nullptr, 16));
  std::cout << std::endl;

  std::cout << "Primer byte: " << byte1Binario << std::endl;
  std::cout << "Segundo byte: " << byte2Binario << std::endl;
  std::cout << "Byte del Algoritmo: " << byteAlg << " (" << contanteAlg << ")" << std::endl;
  std::cout << std::endl;
  
  std::bitset<8> byteXor = byte1Binario;
  std::bitset<8> resultado;
  
  for (int j = 0; j < byteXor.size(); j++) {
    if (byte2Binario[j] == 1) {
      std::cout << j << "-> " << byteXor << std::endl;
      resultado ^= byteXor;
    }
    if (byteXor[7] == 1) {
      byteXor = byteXor << 1;
      byteXor = byteXor ^ byteAlg;
    } else {
      byteXor = byteXor << 1;
    }
  }
  std::cout << std::endl;
  return resultado;
}


int main(void) {

  std::cout << MAGENTA << "Práctica 5: Multiplicación en SNOW 3G y AES " << RESET << "\U0001F47E" << std::endl;

  int opcion;
  std::cout << CYAN << "¿Qué algoritmo quieres realizar para la multiplicación?" << std::endl;
  std::cout << "1. AES" << std::endl;
  std::cout << "2. SNOW 3G" << RESET << std::endl;
  std::cin >> opcion;

  // std::cout << CLEAR;

  std::bitset<8> resultadoAlgoritmo;
  if(opcion == 1) {
    std::cout << ROSE << "\tAlgoritmo AES" << RESET << std::endl;
    std::string hex1, hex2;
    std::cout << "Introduce el primer byte: ";
    std::cin >> hex1;
    std::cout << "Introduce el segundo byte: ";
    std::cin >> hex2;

    resultadoAlgoritmo = multiplicacion(hex1, hex2, CteAES);

  } else if(opcion == 2) {
    std::cout << ROSE << "\t\tAlgoritmo SNOW 3G" << RESET << std::endl;
    std::string hex1, hex2;
    std::cout << "Introduce el primer byte: ";
    std::cin >> hex1;
    std::cout << "Introduce el segundo byte: ";
    std::cin >> hex2;

    resultadoAlgoritmo = multiplicacion(hex1, hex2, CteSnow3g);

  } else {
    std::cout << "Error" << std::endl;
  }

  std::cout << BLUE << "Resultado final: " << RESET << resultadoAlgoritmo << std::endl;
}
