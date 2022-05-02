// AUTOR: Dana B. Choque Zárate
// FECHA: 03/05/2022
// EMAIL: alu0101328348@ull.edu.es
// ASIGNATURA: Seguridad en Sistemas Informáticos
// PRÁCTICA: 9º Cifrado de clave pública RSA
// UNIVERSIDAD: Universidad de La Laguna- Escuela Superior de Ingeniería y Tecnología
// TITULACIÓN: Grado en Ingenieria informática
// ACTUALIZAR EL CMAKE: cmake .
// COMPILAR: make
// EJECUTAR: ./mainRSA
// ELIMINAR EL EJECUTABLE: make clean
// copyright Copyright (c) 2022

#include <math.h>
#include <vector>
#include <iostream>
#include <stdlib.h>
#include <algorithm>

long exponenciacionRapida(long base, long exponente, long modulo) {
  long resultado = 1;
  long y = base % modulo;
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


long euclidesExtendido(long &dividendo, long &divisor) {
  long resto = 1;
  
  std::vector<long> z, x;
  x = {dividendo, divisor};
  z = {0, 1};
  // iteramos desde el final del vector hasta que el resto sea 1,
  // lo que indica que los números son primos
  for(int i = x.size(); x[i -1] > 1; i++) {
    if (x[i - 1] > 0) {
    // en el vector x se van guarda el resto
    x.push_back(x[i -2] % x[i - 1]);
    // en zi los valores que se calculan con la formula
    long zi = -(x[i-2] / x[i -1]) * z[z.size() - 1] + z[z.size() - 2 % dividendo];
    // en caso de que zi de negativo, se suma el módulo hasta que sea positivo
    while (zi < 0 && zi < dividendo)
      zi += dividendo;
    z.push_back(zi);
    }
  }
  // último valor del vector z
  return z[z.size() - 1];
}


bool testLehmanPeralta(long p) {
  // comprobamos que no es divisible por un numero pequeño
  std::vector<long> primos = {2, 3, 5, 7, 11, 13, 17, 19};
  for (int i = 0; i < primos.size(); i++) {
    if ((primos[i] != p) && (p % primos[i] == 0))
      return false;
  }

  // elegir enteros aleatorios a1 entre 2 y p-1
  std::vector<long> aleatorios; // vector ai
  for (int i = 0; i < 6; i++) {
    long numeroA = 2 + std::rand() % (p - 1);
    aleatorios.push_back(numeroA);
  }

  // calcular ai^((p-1) / 2) mod p
  bool compuesto = true;
  for (int i = 0; i < aleatorios.size(); i++) {
    if (exponenciacionRapida(aleatorios[i], (p-1) / 2, p) != 1)
      compuesto = false;
  }

  if (compuesto) {
    return false;
  }

  for (int i = 0; i < aleatorios.size(); i++) {
    long aux = exponenciacionRapida(aleatorios[i], (p-1) / 2, p);
    if (aux != 1)
      aux -= p;
    
    if (aux != 1 && aux != -1)
      return false;
  }
  return true;
}


long logBase(long a, long base) {
  return std::log(a) / std::log(base);
}


void imprimirVector(const std::vector<long> &v) {
  for (int i = 0; i < v.size(); i++)
    std::cout << v[i] << " ";
}


long codificar(long tamBloque, std::string mensaje, long n) {
  std::string alfabeto = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  long resultado = 0;
  for (int i = 0; i < tamBloque; i++) {
    resultado += alfabeto.find(mensaje[i]) * exponenciacionRapida(alfabeto.size(), tamBloque - (i + 1), n);
  }
  return resultado;
}


void rsa(std::string mensaje, long p, long q, long d) {
  // quitamos los espacios
  mensaje.erase(std::remove(mensaje.begin(), mensaje.end(), ' '), mensaje.end());

  bool esPrimoP = testLehmanPeralta(p);
  bool esPrimoQ = testLehmanPeralta(q);
  if (esPrimoP && esPrimoQ)
    std::cout << "P y q son primos\np = " << p << " q = " << q << std::endl;
  else
    std::cout << "P y q no primos\np = " << p << " q = " << q << std::endl;

  long fi = (p - 1) * (q - 1);
  std::cout << "fi: " << fi << std::endl;
  long n = p * q;
  std::cout << "n: " << n << std::endl;

  long e = euclidesExtendido(fi, d);
  std::cout << "e: " << e << std::endl;

  std::string alfabeto = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  long tamBloque = logBase(n, alfabeto.size());
  std::cout << "El texto en bloques de: " << tamBloque << std::endl;

  // comprobamos que el mensaje es multiplo del tamaño del bloque
  long aux = mensaje.size() % tamBloque;
  if (aux == 1) {
    for (int i = 0; i < (tamBloque - aux); i++) {
      mensaje.push_back('X');
    }
  }

  std::vector<long> mensajeCodificado, mensajeCifrado;
  for (int i = 0; i < mensaje.size(); i++) {
    // si i es multiplo del tamaño del subloque
    if (i % tamBloque == 0) {
      // dividir por bloques el mensaje: subloques
      std::string subloqueMensaje = mensaje.substr(i, tamBloque);
      long aux = codificar(tamBloque, subloqueMensaje, n);
      mensajeCodificado.push_back(aux);
      mensajeCifrado.push_back(exponenciacionRapida(aux, e, n));
    }
  }
  std::cout << "Mensaje codificado: "; imprimirVector(mensajeCodificado); std::cout << std::endl;
  std::cout << "Mensaje cifrado: "; imprimirVector(mensajeCifrado); std::cout << std::endl;
}


int main(void) {
  rsa("MANDA DINEROS", 421, 7, 1619);
  std::cout << std::endl;
  rsa("AMIGO MIO", 2347, 347, 5);
}

