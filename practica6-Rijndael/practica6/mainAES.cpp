// AUTOR: Dana B. Choque Zárate
// FECHA: 29/03/2022
// EMAIL: alu0101328348@ull.edu.es
// ASIGNATURA: Seguridad en Sistemas Informáticos
// PRÁCTICA: 6º ALGORITMO RIJNDAEL
// UNIVERSIDAD: Universidad de La Laguna- Escuela Superior de Ingeniería y Tecnología
// TITULACIÓN: Grado en Ingenieria informática
// ACTUALIZAR EL CMAKE: cmake .
// COMPILAR: make
// EJECUTAR: ./mainAES
// ELIMINAR EL EJECUTABLE: make clean
// REFERENCIAS:
// Algoritmo Rijndael: https://www.youtube.com/watch?v=tzj1RoqRnv0
// Librería de colores: https://gist.github.com/Alfonzzoj/db207b89d56f24d9d0b17ff93e091be8
// copyright Copyright (c) 2022

#include "constantes.h"

#include <iostream>
#include <vector>
#include <iomanip>

#define RESET   "\x1b[0m"
#define BLUE    "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define ROSE    "\x1B[38;2;255;151;203m"
#define CYAN    "\x1b[36m"
#define CLEAR "\x1B[2J\x1B[H"
#define SIZE_4 4

/**
 * @brief Función que imprime la matriz
 * 
 * @param vector Matriz a imprimir
 */
void imprime(std::vector<std::vector<unsigned char>> vector) {
  for(int i = 0; i < SIZE_4; i++) {
    for(int j = 0; j < SIZE_4; j++) {
      // para imprimir en forma de matriz
      // std::cout << std::hex << std::setfill('0') << std::setw(2) << int(vector[i][j]) << " ";
      std::cout << std::hex << std::setfill('0') << std::setw(2) << int(vector[j][i]) << " ";
    }
    // std::cout << std::endl;
  }
  // std::cout << std::endl;
}


/**
 * @brief mixColumn: se encarga de multiplicar cada columna
 * de la matriz de estado por un polinomio fijo
 * 
 * @param matrixEstado 
 * @return std::vector<std::vector<unsigned char>> 
 */
std::vector<std::vector<unsigned char>>
mixColumn(std::vector<std::vector<unsigned char>> matrixEstado) {
 unsigned char a[SIZE_4];
 unsigned char b[SIZE_4];
 unsigned char h;

  for(int i = 0; i < SIZE_4; i++) {
    for(int j = 0; j < SIZE_4; j++) {
      a[j] = matrixEstado[j][i];
      h = matrixEstado[j][i] & 0x80;
      b[j] = matrixEstado[j][i] << 1;

      if (h == 0x80) {
        b[j] ^= 0x1b;
      }
    }
    matrixEstado[0][i] = b[0] ^ a[3] ^ a[2] ^ b[1] ^ a[1]; /* 2 * a0 + a3 + a2 + 3 * a1 */
    matrixEstado[1][i] = b[1] ^ a[0] ^ a[3] ^ b[2] ^ a[2]; /* 2 * a1 + a0 + a3 + 3 * a2 */
    matrixEstado[2][i] = b[2] ^ a[1] ^ a[0] ^ b[3] ^ a[3]; /* 2 * a2 + a1 + a0 + 3 * a3 */
    matrixEstado[3][i] = b[3] ^ a[2] ^ a[1] ^ b[0] ^ a[0]; /* 2 * a3 + a2 + a1 + 3 * a0 */
  }

  // std::cout << "mixColumn: " << std::endl;
  // imprime(matrixEstado);
  return matrixEstado;
} 


/**
 * @brief AddRoundKey: realiza la suma exclusiva (XOR) entre la clave
 * (clave inicial o clave expandida) con el mensaje
 * 
 * @param clave Clave inicial o clave expandida
 * @param texto Mensaje original o matriz de estado
 * @return std::vector<std::vector<unsigned char>> Vector resultante
 *         del XOR entre los dos parámetros
 */
std::vector<std::vector<unsigned char>>
addRoundKey(std::vector<std::vector<unsigned char>> clave, std::vector<std::vector<unsigned char>> texto) {
  std::vector<std::vector<unsigned char>> aux = texto;
  for(int i = 0; i < SIZE_4; i++) {
    for(int j = 0; j < SIZE_4; j++) {
      aux[i][j] = clave[i][j] ^ texto[i][j];
    }
  }
  // std::cout << "AddRoundKey: " << std::endl;
  imprime(aux);
  return aux;
}


/**
 * @brief SubBytes: se realiza la sustitución de los bytes de la matriz
 * estado con los bytes de la matriz SCaja
 * 
 * @param textoCifrado 
 * @return std::vector<std::vector<unsigned char>> 
 */
std::vector<std::vector<unsigned char>>
subBytes(std::vector<std::vector<unsigned char>> textoCifrado) {
  for (int i = 0; i < SIZE_4; i++){
		for (int j = 0; j < SIZE_4; j++){
      textoCifrado[i][j] = sCaja[textoCifrado[i][j]];
    }
  }
  // std::cout << "SubBytes: " << std::endl;
  // imprime(textoCifrado);
  return textoCifrado;
}


/**
 * @brief ShiftRow: permutación de las filas del estado
 * La primera fila no rota, la segunda fila rota 1 bytes,
 * la segunda, 2 bytes y la tercera 3 bytes
 * 
 * @param textoCifrado Matriz de estado
 * @return std::vector<std::vector<unsigned char>> 
 */
std::vector<std::vector<unsigned char>>
shiftRow(std::vector<std::vector<unsigned char>> textoCifrado) {
  unsigned char auxCopia;

  auxCopia = textoCifrado[1][0];
	textoCifrado[1][0] = textoCifrado[1][1];
	textoCifrado[1][1] = textoCifrado[1][2];
	textoCifrado[1][2] = textoCifrado[1][3];
	textoCifrado[1][3] = auxCopia;

	auxCopia = textoCifrado[2][0];
	textoCifrado[2][0] = textoCifrado[2][2];
	textoCifrado[2][2] = auxCopia;
	auxCopia = textoCifrado[2][1];
	textoCifrado[2][1] = textoCifrado[2][3];
	textoCifrado[2][3] = auxCopia;

	auxCopia = textoCifrado[3][0];
	textoCifrado[3][0] = textoCifrado[3][3];
	textoCifrado[3][3] = textoCifrado[3][2];
	textoCifrado[3][2] = textoCifrado[3][1];
	textoCifrado[3][1] = auxCopia;
  
  // std::cout << "ShiftRow: " << std::endl;
  // imprime(textoCifrado);
  return textoCifrado;
}


/**
 * @brief expancionClave: modificamos la matriz de estado de la clave
 * inicial con los siguientes pasos:
 *  1- rotWord: rota el primer byte de la columna
 *              con el último byte de la matrix
 *  2- SubBytes: aplicamos a la matriz resultante la función SubBytes
 *  3- XOR con XOR(i-3): se realiza la OR exclusivo (XOR) con la matriz resultado
 *                       (en nuestro caso siempre será la primera columna) con la
 *                       posición que se encuentra 3 posiciones más atrás
 *  4- XOR con rcon: se realiza el xor con la cajaRcon de la posicón del parámetro
 *                   iteracion.
 * 
 * @param matrizClaveExpandida clave
 * @param iteracion estado en que se encuentra
 * @return std::vector<std::vector<unsigned char>> clave expandida del estado iteracion
 */
std::vector<std::vector<unsigned char>>
expandClave(std::vector<std::vector<unsigned char>> matrizClaveExpandida, int iteracion) {
  unsigned char aux;
  std::vector<std::vector<unsigned char>> copia = matrizClaveExpandida, matrizResultante(SIZE_4, std::vector<unsigned char>(SIZE_4));
  
  // rotWord
  for (int j = 0; j < SIZE_4; j++) {
    aux = matrizClaveExpandida[0][j];
    for (int i = 1; i < SIZE_4; i++) {
      matrizClaveExpandida[i -1][j] = matrizClaveExpandida[i][j];
      if (i == 3) {
        matrizClaveExpandida[3][j] = aux;
      }
    }
  }
  // std::cout << "rotWord:" << std::endl;
  // imprime(matrizClaveExpandida);

  // SubBytes
  matrizClaveExpandida = subBytes(matrizClaveExpandida);

  // XOR con XOR(i-3)
  for(int i = 0; i < SIZE_4; i++) {
    matrizResultante[i][0] = copia[i][0] ^ matrizClaveExpandida[i][3];
  }
  // std::cout << "XOR con XOR(i-3)" << std::endl;
  // imprime(matrizResultante);

  // XOR con RCON: XOR del primer byte de la columna con RCON
  matrizResultante[0][0] ^= cajaRcon[iteracion];
  // std::cout << "XOR con RCON" << std::endl;
  // imprime(matrizResultante);

  // XOR con la nueva columna de XOR con RCON más la copia de la clave inicial
  // desde las columnas 2, 3, y 4 de la clave inicial
  for(int i = 1; i < SIZE_4; i++) {
    for(int j = 0; j < SIZE_4; j++) {
      matrizResultante[j][i] = matrizResultante[j][i -1] ^ copia[j][i];
    }
  }
  // std::cout << "SUBCLAVE" << std::endl;
  // imprime(matrizResultante);
  return matrizResultante;
}


int main(void) {
  std::cout << MAGENTA << "Práctica 6: Algoritmo Rijndael " << RESET << "\U0001F47E" << std::endl;
  std::vector<std::vector<unsigned char>> clave(SIZE_4, std::vector<unsigned char>(SIZE_4));
  clave[0][0] = 0x00; clave[1][0] = 0x01; clave[2][0] = 0x02; clave[3][0] = 0x03;
	clave[0][1] = 0x04;	clave[1][1] = 0x05;	clave[2][1] = 0x06;	clave[3][1] = 0x07;
	clave[0][2] = 0x08;	clave[1][2] = 0x09;	clave[2][2] = 0x0A;	clave[3][2] = 0x0B;
	clave[0][3] = 0x0C;	clave[1][3] = 0x0D;	clave[2][3] = 0x0E;	clave[3][3] = 0x0F;

  std::cout << BLUE << "Clave: " << RESET; imprime(clave); std::cout << std::endl;

  std::vector<std::vector<unsigned char>> texto(SIZE_4, std::vector<unsigned char>(SIZE_4));
	texto[0][0] = 0x00;	texto[1][0] = 0x11;	texto[2][0] = 0x22;	texto[3][0] = 0x33;
	texto[0][1] = 0x44;	texto[1][1] = 0x55;	texto[2][1] = 0x66;	texto[3][1] = 0x77;
	texto[0][2] = 0x88;	texto[1][2] = 0x99;	texto[2][2] = 0xAA;	texto[3][2] = 0xBB;
	texto[0][3] = 0xCC;	texto[1][3] = 0xDD;	texto[2][3] = 0xEE;	texto[3][3] = 0xFF;

  std::cout <<BLUE << "Bloque del Texto Original: " << RESET; imprime(texto); std::cout << std::endl;
  std::cout << std::endl;

  std::cout << CYAN << "R0 (Subclave = " << RESET; imprime(clave); std::cout << CYAN << ")= " << RESET;
  std::vector<std::vector<unsigned char>> matrixDeEstado;
  matrixDeEstado = addRoundKey(clave, texto);
  std::cout << std::endl;

  std::vector<std::vector<unsigned char>> claveExpandida;
  claveExpandida = expandClave(clave, 0);

  for(int i = 1; i < 10; i++) {
    std::cout << CYAN << "R" << i << " (Subclave = " << RESET; imprime(claveExpandida); std::cout << CYAN << ")= " << RESET; 
    if (i != 1)
      claveExpandida = expandClave(claveExpandida, i -1);
    matrixDeEstado = subBytes(matrixDeEstado);
    matrixDeEstado = shiftRow(matrixDeEstado);
    matrixDeEstado = mixColumn(matrixDeEstado);
    matrixDeEstado = addRoundKey(claveExpandida, matrixDeEstado);
    std::cout << std::endl;
  }
  matrixDeEstado = subBytes(matrixDeEstado);
  matrixDeEstado = shiftRow(matrixDeEstado);
  claveExpandida = expandClave(claveExpandida, 9);
  std::cout << CYAN << "R10 (Subclave = " << RESET; imprime(claveExpandida); std::cout << CYAN << ")= " << RESET;
  matrixDeEstado = addRoundKey(claveExpandida, matrixDeEstado);
  std::cout << std::endl;

  std::cout << ROSE << "\nBloque del Texto Cifrado: " << RESET; imprime(matrixDeEstado); std::cout << std::endl;
}
