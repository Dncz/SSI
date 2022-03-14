
// AUTOR: Dana B. Choque Zárate
// FECHA: 15/03/2022
// EMAIL: alu0101328348@ull.edu.es
// ASIGNATURA: Seguridad en Sistemas Informáticos
// PRÁCTICA Nº: 3º Cifrado Chacha20
// UNIVERSIDAD: Universidad de La Laguna- Escuela Superior de Ingeniería y Tecnología
// TITULACIÓN: Grado en Ingenieria informática
// ACTUALIZAR EL CMAKE: cmake .
// COMPILAR: make
// EJECUTAR: ./chacha20
// ELIMINAR EL EJECUTABLE: make clean
// Librería de colores: https://gist.github.com/Alfonzzoj/db207b89d56f24d9d0b17ff93e091be8

// #include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <iostream>
#include <iomanip> // librería para setfill y setw
#include <vector>

// #include <string>

// #include <array>

#define MAGENTA "\x1b[35m"
#define RESET   "\x1b[0m"
#define ROSE    "\x1B[38;2;255;151;203m"

#define ROTL(a, b) (((a) << (b)) | ((a) >> (32 - (b))))
#define QR(a, b, c, d) ( \
  a += b, d ^= a, d= ROTL(d, 16),   \
  c += d, b ^= c, b= ROTL(b, 12),   \
  a += b, d ^= a, d= ROTL(d,  8),   \
  c += d, b ^= c, b= ROTL(b,  7))
#define ROUNDS 10


void
imprime(std::vector<uint32_t> &PalClave) {
  int linea = 0;
  for (int  i = 0; i < PalClave.size(); i++) {
    if ( linea == 4) {
      std::cout << std::endl;
      linea = 0;
    }
    std::cout << std::hex << std::setfill('0') << std::setw(8) << PalClave[i] << " ";
    linea++;
  }
  std::cout << std::endl;
}

std::vector<uint32_t>
inicializarEstado(std::vector<uint32_t> &clave, std::vector<uint32_t> &contador, std::vector<uint32_t> &nonce) {
  std::vector<uint32_t> estado = {0x61707865, 0x3320646e, 0x79622d32, 0x6b206574};
  
  for (int i = 0; i < clave.size(); i++) {
    estado.push_back(clave[i]);
  }
  estado.push_back(contador[0]);
  for (size_t i = 0; i < nonce.size(); i++) {
    estado.push_back(nonce[i]);
  }
  return estado;
}

std::vector<uint32_t>
chacha20(std::vector<uint32_t> &bloque) {
  std::vector<uint32_t> x(16);
  std::vector<uint32_t> out(16);
  for (int i = 0; i < x.size(); i++) {
    x[i] = bloque[i];
  }
  for(int i = 0; i < ROUNDS; i++) {
    // Bloque en columnas.
    QR(x[0], x[4], x[ 8], x[12]);
    QR(x[1], x[5], x[ 9], x[13]);
    QR(x[2], x[6], x[10], x[14]);
    QR(x[3], x[7], x[11], x[15]);
    // Bloque en diagonales.
    QR(x[0], x[5], x[10], x[15]);
    QR(x[1], x[6], x[11], x[12]);
    QR(x[2], x[7], x[ 8], x[13]);
    QR(x[3], x[4], x[ 9], x[14]);
  }
  
  std::cout << ROSE << "\nEstado ChaCha después de 20 rondas:"  << RESET << std::endl;
  imprime(x);

  std::cout << ROSE << "\nEstado ChaCha al final de la operación ChaCha20: " << RESET << std::endl;
  for(int i = 0; i < x.size(); i++) {
    out[i] = x[i] + bloque[i];
  }
  return out;
}


// Problema que tuve: si usar arrays o vectores...
// Tuve que inicalizar los vectores a mano (poner los datos manualmente)...
int main() {
  std::cout << MAGENTA << "Práctica 3: Algoritmo Chacha20 " << RESET << "\U0001F47E" << std::endl;
  std::vector<uint32_t> clave(8);
  clave = {0x03020100, 0x07060504, 0x0b0a0908, 0x0f0e0d0c, 0x13121110, 0x17161514, 0x1b1a1918, 0x1f1e1d1c};

  std::cout << "Clave de 256 bits en forma de 8 palabras en hexadecimal: " << std::endl;
  imprime(clave);

  std::vector<uint32_t> contador(1);
  contador= {0x00000001};
  std::cout << "\nContador de 32 bits en forma de 1 palabra en hexadecimal:" << std::endl;
  imprime(contador);

  std::vector<uint32_t> nonce(3);
  nonce = {0x09000000, 0x4a000000, 0x00000000};
  std::cout << "\nNonce aleatorio de 96 bits en forma de 3 palabras en hexadecimal:" << std::endl;
  imprime(nonce);

  std::vector<uint32_t> estadoInicial(16);
  estadoInicial = inicializarEstado(clave, contador, nonce);

  std::cout << ROSE << "\nEstado de ChaCha con la configuración clave: " << RESET << std::endl;
  imprime(estadoInicial);

  // std::cout << "\nEstado de salida del generador=" << std::endl;
  std::vector<uint32_t> otroEstado(16);
  otroEstado = chacha20(estadoInicial);
  imprime(otroEstado);

  // POSIBLE MODIFICACIÓN: GENERAR UN NONCE ALEATORIO
  // std::vector<uint32_t> prueba(3);
  // // uint32_t bit;
  // std::string numero = "";
  // char hex_characters[]={'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'};
  // for (int i = 0; i < 8; i++) {
  //   numero += hex_characters[rand()%16];
  // }
  // std::vector<std::uint32_t> new_variable;
  // for(std::string numero: idk2)
  //   new_variable.push_back(static_cast<uint32_t>(std::stoul(numero)));
  // // bit = (numero.c_str());
  // // std::cout << bit << std::endl;
  // std::cout << numero << std::endl;
  return 0;
}