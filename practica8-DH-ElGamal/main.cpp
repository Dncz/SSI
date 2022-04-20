// AUTOR: Dana B. Choque Zárate
// FECHA: 26/04/2022
// EMAIL: alu0101328348@ull.edu.es
// ASIGNATURA: Seguridad en Sistemas Informáticos
// PRÁCTICA: 8º Intercambio de claves de Diffie-Hellman y el Cifrado de ElGamal
// UNIVERSIDAD: Universidad de La Laguna- Escuela Superior de Ingeniería y Tecnología
// TITULACIÓN: Grado en Ingenieria informática
// ACTUALIZAR EL CMAKE: cmake .
// COMPILAR: make
// EJECUTAR: ./mainDH_ElGamal
// ELIMINAR EL EJECUTABLE: make clean
// copyright Copyright (c) 2022

#include <iostream>
#include <vector>
#define RESET   "\x1b[0m"
#define MAGENTA "\x1b[35m"

/**
 * @brief Algoritmo de exponenciacion rápida
 * 
 * @param base 
 * @param exponente 
 * @param modulo 
 * @return int 
 */
int exponenciacionRapida(int base, int exponente, int modulo) {
  int resultado = 1;
  int y = base % modulo;
  while ((exponente > 0) && (y > 1)) {
    if ((exponente % 2) == 1) {
      resultado = (resultado * y) % modulo;
      exponente--;
    } else {
      y = (y * y) % modulo;
      exponente /= 2;
    }
  }
  return resultado;
}

/**
 * @brief Imprime la traza realizada del algoritmo Euclides Extendido
 * 
 * @param x
 * @param z
 */
void imprimirTraza(std::vector<int> &x, std::vector<int> &z) {
  std::cout << std::endl;
  for(int i = 0; i < x.size() && i < z.size(); i++) {
    std::cout << x[i] << " " << z[i + 1] << std::endl;
  }
}

/**
 * @brief Algoritmo euclides extendido: cálculo de la inversa de k
 * 
 * @param dividendo 
 * @param divisor 
 * @return int 
 */
int euclidesExtendido(int &dividendo, int &divisor) {
  int resto = 1;
  
  std::vector<int> z, x;
  x = {dividendo, divisor};
  z = {0, 1};
  int index = 0;
  // iteramos desde el final del vector hasta que el resto sea 1,
  // lo que indica que los números son primos
  for(int i = x.size(); x[i -1] > 1; i++) {
    if (x[i - 1] > 0) {
    // en el vector x se van guarda el resto
    x.push_back(x[i -2] % x[i - 1]);
    // en zi los valores que se calculan con la formula
    int zi = -(x[i-2] / x[i -1]) * z[z.size() - 1] + z[z.size() - 2 % dividendo];
    // en caso de que zi de negativo, se suma el módulo hasta que sea positivo
    while (zi < 0 && zi < dividendo)
      zi += dividendo;
    z.push_back(zi);
    }
  }
  // imprimirTraza(x, z);

  // último valor del vector z
  return z[z.size() - 1];
}

/**
 * @brief Algoritmo diffie Hellman y el Gamal
 * 
 * @param numeroPrimo P
 * @param numeroA Alfa
 * @param xA número secreto de Alice
 * @param xB número secreto de Bob
 * @param mensaje Mensaje a cifrar y descifrar
 */
void diffieHellmanElgamal(int numeroPrimo, int numeroA, int xA, int xB, int mensaje) {
  std::cout << "p = " << numeroPrimo  << ", a(alfa) = " << numeroA << ", k = " << xA << ", x = " << xB << ", m =" << mensaje << std::endl;

  int yA = 0, yB = 0, kA = 0, kB = 0;
  yA = exponenciacionRapida(numeroA, xA, numeroPrimo);
  yB = exponenciacionRapida(numeroA, xB, numeroPrimo);

  kA = exponenciacionRapida(yB, xA, numeroPrimo);
  kB = exponenciacionRapida(yA, xB, numeroPrimo);
  if(kA != kB)
    throw std::invalid_argument("Los valores k son distintos");
  
  int numeroCifrado = 0;
  numeroCifrado = (kA * mensaje) % numeroPrimo;
  int kInversa = 0;
  int mensajeDesencriptado = 0;
  kInversa = euclidesExtendido(numeroPrimo, kA);
  mensajeDesencriptado = (kInversa * kA * mensaje) % numeroPrimo;
  std::cout << "yA: " << yA << ", yB= " << yB << ", kA= " << kA << ", kB= " << kB
            << ", Numero cifrado: " << numeroCifrado << ", k-1: " << kInversa
            << ", M: " << mensajeDesencriptado << std::endl << std::endl;
}


int main(void) {
  std::cout << MAGENTA << "Práctica 7: Modos de cifrado en bloque " << RESET << "\U0001F47E" << std::endl;
  
  int eleccion = 0;
  int numeroPrimo = 0, numeroA = 0, xA = 0, xB = 0, mensaje = 0;
  bool salir = false;
  while (!salir) {
    std::cout << "1. Valores por defecto" << std::endl
              << "2. Introducir valores nuevos" << std::endl
              << "3. salir" << std::endl;
    std::cin >> eleccion;
    switch (eleccion) {
      case 3:
        salir = true;
      break;
      case 1:
        diffieHellmanElgamal(13, 4, 5, 2, 8);
        diffieHellmanElgamal(43, 23, 25, 33, 18);
        diffieHellmanElgamal(113, 43, 54, 71, 28);
      break;
      case 2:
        std::cout << "Introduce el número primo P: ";
        std::cin >> numeroPrimo;
        std::cout << "Introduce el número entero a (alfa): ";
        std::cin >> numeroA;
        std::cout << "Introduce el número Xa de Alice: ";
        std::cin >> xA;
        std::cout << "Introduce el número Xb de Bob: ";
        std::cin >> xB;
        std::cout << "Introduce el mensaje: ";
        std::cin >> mensaje;
        diffieHellmanElgamal(numeroPrimo, numeroA, xA, xB, mensaje);
      break;
    }
  }
  return 0;
}