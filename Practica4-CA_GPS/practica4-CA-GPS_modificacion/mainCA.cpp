// AUTOR: Dana B. Choque Zárate
// FECHA: 15/03/2022
// EMAIL: alu0101328348@ull.edu.es
// ASIGNATURA: Seguridad en Sistemas Informáticos
// PRÁCTICA Nº: 4º Generador C/A de GPS
// UNIVERSIDAD: Universidad de La Laguna- Escuela Superior de Ingeniería y Tecnología
// TITULACIÓN: Grado en Ingenieria informática
// ACTUALIZAR EL CMAKE: cmake .
// COMPILAR: make
// EJECUTAR: ./mainCA
// ELIMINAR EL EJECUTABLE: make clean
// Librería de colores: https://gist.github.com/Alfonzzoj/db207b89d56f24d9d0b17ff93e091be8

#include <iostream>
#include <vector>
#include <array>
#include <utility>

#define RESET   "\x1b[0m"
#define BLUE    "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define ROSE    "\x1B[38;2;255;151;203m"
#define CYAN    "\x1b[36m"


void imprimeRegistros(std::vector<u_int> registro1, std::vector<u_int> registro2, u_int bitRealimentacion1, u_int bitRealimentacion2) {
  for(int i = 0; i < registro1.size(); i++) {
    std::cout << registro1[i] << " ";
  }
  std::cout << BLUE << "| Bit de realimentación: " << RESET << bitRealimentacion1 << "\t || \t";

  for(int i = 0; i < registro2.size(); i++) {
    std::cout << registro2[i] << " ";
  }
  std::cout << BLUE << "| Bit de realimentación: " << RESET << bitRealimentacion2 << std::endl;
   
}

void imprimeResultado(std::vector<u_int> resultado) {
  std::cout << std::endl;
  std::cout << ROSE << "Secuencia C/A PRN1: " << RESET << std::endl;
  for(int i = 0; i < resultado.size(); i++) {
    std::cout << resultado[i] << " ";
  }
  std::cout << std::endl;
}

std::vector<u_int> 
cifradorCA(int longitudSalida) {
  std::vector<u_int> registro1 = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
  std::vector<u_int> registro2 = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
  std::vector<u_int> resultado(longitudSalida);
  std::cout << CYAN << "\t\tRegistro1 \t\t\t\t" << "\t\t Registro2"  << RESET << std::endl;
  for(int i = 0; i < longitudSalida; i++) {
    u_int bitRealimentacion1 = registro1[2] ^ registro1[9];
    u_int bitRealimentacion2 = registro2[1] ^ registro2[2] ^ registro2[5] ^ registro2[7] ^ registro2[8] ^ registro2[9];

    imprimeRegistros(registro1, registro2, bitRealimentacion1, bitRealimentacion2);
    resultado[i] = registro1[9] ^ registro2[9];

    for(int j = registro1.size() - 1; j > 0 ; j--) {
      registro1[j] = registro1[j - 1];
      registro2[j] = registro2[j - 1];
    }

    registro1[0] = bitRealimentacion1;
    registro2[0] = bitRealimentacion2;
  }
  return resultado;
}

int main(void) {
  std::cout << MAGENTA << "Práctica 4: Generador C/A de GPS " << RESET << "\U0001F47E" << std::endl;

  int longitudSalida = 0;
  std::cout << "Introduce la longitud de la salida: ";
  std::cin >> longitudSalida;
  
  std::vector<u_int> salida(longitudSalida);
  salida = cifradorCA(longitudSalida);
  imprimeResultado(salida);

  return 0;
}