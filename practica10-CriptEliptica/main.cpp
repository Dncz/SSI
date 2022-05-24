// AUTOR: Dana B. Choque Zárate
// FECHA: 10/05/2022
// EMAIL: alu0101328348@ull.edu.es
// ASIGNATURA: Seguridad en Sistemas Informáticos
// PRÁCTICA: 10º Entrega Diffie-Hellman y ElGamal Elípticos
// UNIVERSIDAD: Universidad de La Laguna- Escuela Superior de Ingeniería y Tecnología
// TITULACIÓN: Grado en Ingenieria informática
// ACTUALIZAR EL CMAKE: cmake .
// COMPILAR: make
// EJECUTAR: ./mainEliptica
// ELIMINAR EL EJECUTABLE: make clean
// copyright Copyright (c) 2022

#include <iostream>
#include <utility>
#include <vector>
#include <math.h>

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

int euclidesExtendido(int dividendo, int divisor) {
  int resto = 1;
  
  std::vector<int> z, x;
  x = {dividendo, divisor};
  z = {0, 1};

  for(int i = x.size(); x[i -1] > 1; i++) {
    if (x[i - 1] > 0) {
    x.push_back(x[i -2] % x[i - 1]);
    int zi = -(x[i-2] / x[i -1]) * z[z.size() - 1] + z[z.size() - 2 % dividendo];
    while (zi < 0 && zi < dividendo)
      zi += dividendo;
    z.push_back(zi);
    }
  }
  std::cout << "euclides: " << z[z.size() - 1] << std::endl;
  return z[z.size() - 1];
}



std::pair<int, int>
calcularPunto(int &primo, std::pair<int, int> &puntoA, std::pair<int, int> &puntoB, int &a) {
  std::cout << puntoA.first << " " << puntoA.second << " " << puntoB.first << " " << puntoB.second << std::endl;
  int lamda = 0;
  int numerador = 0;
  int denominator = 0;
  int x3 = 0, y3 = 0;
  if (puntoA == puntoB) {
    numerador = 3 * (puntoA.first * puntoA.first) + a;
    denominator = 2 * puntoA.second;
    lamda = (numerador * euclidesExtendido(denominator, primo)) % primo;
    // std::cout << "lamda1: " << lamda << std::endl;
    // std::cout << "denominator1: " << denominator << std::endl;
    // std::cout << "numerator1: " << numerador << std::endl;
  } else {
    numerador = puntoB.second - puntoA.second;
    while (numerador < 0) {
      numerador += primo;
    }
    denominator = puntoB.first - puntoA.first;
    while (denominator < 0) {
      denominator += primo;
    }
    lamda = (numerador * euclidesExtendido(primo, denominator)) % primo;
    // std::cout << "lamda2: " << lamda << std::endl;
    // std::cout << "denominator2: " << denominator << std::endl;
    // std::cout << "numerator2: " << numerador << std::endl;
  }
  x3 = ((lamda * lamda) - puntoA.first - puntoB.first);
  // std::cout << "x3 negativo " << x3<< std::endl;
  while (x3 < 0) {
    x3 += primo;
  }
  y3 = (lamda * (puntoA.first - x3) - puntoA.second);
  // std::cout << "y3 negativo " << y3<< std::endl;
  while (y3 < 0) {
    y3 += primo;
  }
  // std::cout << "x3: " << x3 << "y3:" << y3 << std::endl;

  return std::make_pair(x3 % primo, y3 % primo);
}

std::vector<std::pair<int, int>> puntos(int primo, int a, int b) {
  std::vector<int> x, y;
  std::vector<std::pair<int, int>> puntos;
  for (int i = 0; i < primo; i++) {
    int x3 = std::pow(i ,3);
    int aux = (x3 + (a * i) + b) % primo;
    x.push_back(aux);
    y.push_back((i * i) % primo);
  }

  for (int i = 0; i < primo; i++) {
    for (int j = 0; j < primo; j++) {
      if (x[i] == y[j]) {
        puntos.push_back(std::make_pair(i, j));
      }
    }
  }
  return puntos;
}

std::pair<int, int> codificar(int mensaje, int primo, std::vector<std::pair<int, int>> vectorPuntos) {
  int M = 2;
  while (M <= mensaje) {
    M *= 2;
  }
  std::cout << "M: " << M << std::endl;
  int h;
  if (primo > mensaje) {
    h = primo / M;
    std::cout << "h: " << h << std::endl;
  }

int x = 0, y = 0;
bool encontrado = false;
  for (int i = 0; i < vectorPuntos.size(); i++) {
    x = (mensaje * h) + i;
    for (int j = 0; j < vectorPuntos.size(); j++) {
      if (x == vectorPuntos[j].first) {
        y = vectorPuntos[j].second;
        encontrado = true;
        break;
      }
    }
    if (encontrado)
      break;
  }
  return std::make_pair(x, y);
}

void eliptica(int primo, int a, int b, std::pair<int, int> punto, int clavePrivadaB, int clavePrivadaA, int mensaje) {
  std::vector<std::pair<int, int>> vectorPuntos = puntos(primo, a, b);
  std::cout << "Puntos de la curva: " << std::endl;
  for (int i = 0; i < vectorPuntos.size(); i++) {
    std::cout << "(" << vectorPuntos[i].first << ", " << vectorPuntos[i].second << ") ";
  }
  std::cout << std::endl;

  std::pair<int, int> clavePublicaA = punto;
  std::pair<int, int> clavePublicaB = punto;
  for (int i = 2; i < clavePrivadaB; i+=2) {
    clavePublicaB = calcularPunto(primo, clavePublicaB, clavePublicaB, a);
  }
  std::cout << "Clave pública de B: punto dBG= (" << clavePublicaB.first << ", " << clavePublicaB.second << ") " << std::endl;
  
  for (int i = 2; i < clavePrivadaA; i+=2) {
    if (i + 1 == clavePrivadaA) {
      clavePublicaA = calcularPunto(primo, punto, clavePublicaA, a);
    } else {
      clavePublicaA = calcularPunto(primo, clavePublicaA, clavePublicaA, a);
    }
  }
  std::cout << "Clave pública de A: punto dAG= (" << clavePublicaA.first << ", " << clavePublicaA.second << ") " << std::endl;

  std::pair<int, int> claveSecretaCompartidaPorA = clavePublicaB;
  std::pair<int, int> claveSecretaCompartidaPorB = clavePublicaA;

  for (int i = 2; i < clavePrivadaA; i+=2) {
    if (i + 1 == clavePrivadaA) {
      claveSecretaCompartidaPorA = calcularPunto(primo, clavePublicaB, claveSecretaCompartidaPorA, a);
    } else {
      claveSecretaCompartidaPorA = calcularPunto(primo, claveSecretaCompartidaPorA, claveSecretaCompartidaPorA, a);
    }
  }
  std::cout << "Clave secreta compartida calculada por A: 3*(6,6)= (" << claveSecretaCompartidaPorA.first << ", " << claveSecretaCompartidaPorA.second << ") " << std::endl;

    for (int i = 2; i < clavePrivadaB; i+=2) {
    if (i + 1 == clavePrivadaB) {
      claveSecretaCompartidaPorB = calcularPunto(primo, clavePublicaA, claveSecretaCompartidaPorB, a);
    } else {
      claveSecretaCompartidaPorB = calcularPunto(primo, claveSecretaCompartidaPorB, claveSecretaCompartidaPorB, a);
    }
  }
  std::cout << "Clave secreta compartida calculada por B: 2*(0,1)= (" << claveSecretaCompartidaPorB.first << ", " << claveSecretaCompartidaPorB.second << ") "<< std::endl;
  
  std::pair<int, int> mensajeCod(0,0);
  mensajeCod = codificar(mensaje, primo, vectorPuntos);
  std::cout << "Mensaje original codificado como punto Qm =(3*2,5)= (" << mensajeCod.first << ", " << mensajeCod.second << ") " << std::endl;

  std::pair<int, int> cifrado;
  cifrado = calcularPunto(primo, claveSecretaCompartidaPorA, mensajeCod, a);
  std::cout << "Mensaje cifrado y clave pública enviados de A a B: {(" << cifrado.first << ", " << cifrado.second <<
     "), (" << clavePublicaA.first << ", " << clavePublicaA.second << ")}" << std::endl; 
}

int main (void) {
  // std::pair<int, int> punto(3, 8);
  std::pair<int, int> punto(9, 6);
  eliptica(13, 5, 3, punto, 4, 2, 2);
}