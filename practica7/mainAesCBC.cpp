// AUTOR: Dana B. Choque Zárate
// FECHA: 00/04/2022
// EMAIL: alu0101328348@ull.edu.es
// ASIGNATURA: Seguridad en Sistemas Informáticos
// PRÁCTICA: 7º Modos de cifrado en bloque
// UNIVERSIDAD: Universidad de La Laguna- Escuela Superior de Ingeniería y Tecnología
// TITULACIÓN: Grado en Ingenieria informática
// ACTUALIZAR EL CMAKE: cmake .
// COMPILAR: make
// EJECUTAR: ./mainAES
// ELIMINAR EL EJECUTABLE: make clean
// copyright Copyright (c) 2022

#include <iostream>
#include <vector>
#include <iomanip>
#include "constantes.h"
#define RESET   "\x1b[0m"
#define MAGENTA "\x1b[35m"
#define SIZE_4 4

/**
 * @brief Función que imprime la matriz
 * 
 * @param vector Matriz a imprimir
 */
void imprime(std::vector<std::vector<unsigned char>> vector) {
  for(int i = 0; i < vector.size(); i++) {
    for(int j = 0; j < vector[i].size(); j++) {
      std::cout << std::hex << std::setfill('0') << std::setw(2) << int(vector[j][i]) << " ";
    }
  }
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

  for(int i = 0; i < matrixEstado.size(); i++) {
    for(int j = 0; j < matrixEstado[i].size(); j++) {
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
  for(int i = 0; i < texto.size(); i++) {
    for(int j = 0; j < texto[i].size(); j++) {
      aux[i][j] = clave[i][j] ^ texto[i][j];
    }
  }
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
  for (int i = 0; i < textoCifrado.size(); i++){
		for (int j = 0; j < textoCifrado[i].size(); j++){
      textoCifrado[i][j] = sCaja[textoCifrado[i][j]];
    }
  }
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
  for (int j = 0; j < matrizClaveExpandida.size(); j++) {
    aux = matrizClaveExpandida[0][j];
    for (int i = 1; i < matrizClaveExpandida[j].size(); i++) {
      matrizClaveExpandida[i -1][j] = matrizClaveExpandida[i][j];
      if (i == 3) {
        matrizClaveExpandida[3][j] = aux;
      }
    }
  }

  // SubBytes
  matrizClaveExpandida = subBytes(matrizClaveExpandida);

  // XOR con XOR(i-3)
  for(int i = 0; i < SIZE_4; i++) {
    matrizResultante[i][0] = copia[i][0] ^ matrizClaveExpandida[i][3];
  }

  // XOR con RCON: XOR del primer byte de la columna con RCON
  matrizResultante[0][0] ^= cajaRcon[iteracion];

  // XOR con la nueva columna de XOR con RCON más la copia de la clave inicial
  // desde las columnas 2, 3, y 4 de la clave inicial
  for(int i = 1; i < matrizResultante.size(); i++) {
    for(int j = 0; j < matrizResultante[i].size(); j++) {
      matrizResultante[j][i] = matrizResultante[j][i -1] ^ copia[j][i];
    }
  }
  return matrizResultante;
}

/**
 * @brief Operación Xor entre el vector de inicializacion y el texto
 * 
 * @param vectorInicializacion 
 * @param texto 
 * @return std::vector<std::vector<unsigned char>> 
 */
std::vector<std::vector<unsigned char>>
operacionXor(std::vector<std::vector<unsigned char>> vectorInicializacion, std::vector<std::vector<unsigned char>> texto) {
  for(int i = 0; i < texto.size(); i++) {
    for(int j = 0; j < texto[i].size(); j++) {
      texto[i][j] = vectorInicializacion[i][j] ^ texto[i][j];
    }
  }
  return texto;
}

/**
 * @brief Algoritmo AES
 * 
 * @param clave 
 * @param texto 
 * @return std::vector<std::vector<unsigned char>> 
 */
std::vector<std::vector<unsigned char>>
aes(std::vector<std::vector<unsigned char>> clave, std::vector<std::vector<unsigned char>> texto) {

  std::vector<std::vector<unsigned char>> matrixDeEstado;
  matrixDeEstado = addRoundKey(clave, texto);

  std::vector<std::vector<unsigned char>> claveExpandida;
  claveExpandida = expandClave(clave, 0);

  for(int i = 1; i < 10; i++) {
    if (i != 1)
      claveExpandida = expandClave(claveExpandida, i -1);
    matrixDeEstado = subBytes(matrixDeEstado);
    matrixDeEstado = shiftRow(matrixDeEstado);
    matrixDeEstado = mixColumn(matrixDeEstado);
    matrixDeEstado = addRoundKey(claveExpandida, matrixDeEstado);
  }
  matrixDeEstado = subBytes(matrixDeEstado);
  matrixDeEstado = shiftRow(matrixDeEstado);
  claveExpandida = expandClave(claveExpandida, 9);
  matrixDeEstado = addRoundKey(claveExpandida, matrixDeEstado);

  return matrixDeEstado;
}

/**
 * @brief Contador de números de elementos en la matriz
 * 
 * @param texto 
 * @param contador 
 */
void
contadorElementos(std::vector<std::vector<unsigned char>> texto, int &contador) {
  for (int i = 0; i < texto.size(); i++) {
    for (int j = 0; j < texto[i].size(); j++) {
      contador++;
    }
  }
}


int main(void) {
  std::cout << MAGENTA << "Práctica 7: Modos de cifrado en bloque " << RESET << "\U0001F47E" << std::endl;
  std::vector<std::vector<unsigned char>> clave(SIZE_4, std::vector<unsigned char>(SIZE_4));
  clave = {{0x00, 0x04, 0x08, 0x0C}, {0x01, 0x05, 0x09, 0x0D}, {0x02, 0x06, 0x0A, 0x0E}, {0x03, 0x07, 0x0B, 0x0F}};

  std::vector<std::vector<unsigned char>> primerTexto(SIZE_4, std::vector<unsigned char>(SIZE_4));
  primerTexto = {{0x00, 0x44, 0x88, 0xCC}, {0x11, 0x55, 0x99, 0xDD}, {0x22, 0x66, 0xAA, 0xEE}, {0x33, 0x77, 0xBB, 0xFF}};

  std::vector<std::vector<unsigned char>> segundoTexto;
  segundoTexto = {{0x00, 0x00, 0x00, 0x00}, {0x00, 0x00, 0x00, 0x00}, {0x00, 0x00, 0x00, 0x00}, {0x00, 0x00, 0x00}};

  std::vector<std::vector<unsigned char>> segundoTextoCBC;
  segundoTextoCBC = {{0x00, 0x00, 0x00, 0x00}, {0x00, 0x00, 0x00, 0x00}, {0x00, 0x00, 0x00, 0x00}, {0x00, 0x00, 0x00, 0x00}};

  std::vector<std::vector<unsigned char>> vectorInicializacion(SIZE_4, std::vector<unsigned char>(SIZE_4));
  vectorInicializacion = {{0x00, 0x00, 0x00, 0x00}, {0x00, 0x00, 0x00, 0x00}, {0x00, 0x00, 0x00, 0x00}, {0x00, 0x00, 0x00, 0x00}};
  
  std::vector<std::vector<unsigned char>> textoXorVI; 
  std::vector<std::vector<unsigned char>> bloqueCifrado1, bloqueCifrado2;
  int contador1 = 0, contador2  = 0;
  int posIElementoRobado = 0, posJElementoRobado = 0, elementoRobado = 0;
  int operacion = 0;
  bool salir = false;
  while (!salir) {
    std::cout << "Escoge una opcion: " << std::endl;
    std::cout << "1. Valores por defecto CBC" << std::endl;
    std::cout << "2. Valores por defecto Cipher Stealing" << std::endl;
    std::cout << "3. Salir" << std::endl;
    std::cin >> operacion;

    switch (operacion) {
      case 3:
        salir = true;
        break;
      case 1:
        std::cout << "";
        textoXorVI = operacionXor(vectorInicializacion, primerTexto);
        bloqueCifrado1 = aes(clave, textoXorVI);
        bloqueCifrado1 = operacionXor(bloqueCifrado1, segundoTextoCBC);
        bloqueCifrado2 = aes(clave, bloqueCifrado1);

        std::cout << "\nClave: "; imprime(clave); std::cout << std::endl;
        std::cout << "VI: "; imprime(vectorInicializacion); std::cout << std::endl;
        std::cout << "Bloque 1 de Texto Original: "; imprime(primerTexto); std::cout << std::endl;
        std::cout << "Bloque 2 de Texto Original "; imprime(segundoTextoCBC); std::cout << std::endl;
        std::cout << "\nBloque 1 de Texto Cifrado: "; imprime(bloqueCifrado1); std::cout << std::endl;
        std::cout << "Bloque 2 de Texto Cifrado: "; imprime(bloqueCifrado2); std::cout << std::endl;
        std::cout << std::endl;
        break;
      case 2:
        contadorElementos(primerTexto, contador1);
        contadorElementos(segundoTexto, contador2);
        textoXorVI = operacionXor(vectorInicializacion, primerTexto);
        
        bloqueCifrado1 = aes(clave, textoXorVI);

        int robado = contador1- contador2;
        textoXorVI = operacionXor(bloqueCifrado1, segundoTexto);
        for (int i = 1; i <= robado; i++) {
          posIElementoRobado = bloqueCifrado1.size() - i;
          posJElementoRobado = bloqueCifrado1[posIElementoRobado].size() - i;
          elementoRobado = bloqueCifrado1[posIElementoRobado][posJElementoRobado];
          // ultima fila de textoXorVI, insertamos el elemento
          textoXorVI[textoXorVI.size() - i].push_back(elementoRobado);
          // quitamos del primer bloque cifrado el ultimo elemento
          bloqueCifrado1[bloqueCifrado1.size() - robado].pop_back();
        }
        bloqueCifrado2 = aes(clave, textoXorVI);

        std::cout << "\nClave: "; imprime(clave); std::cout << std::endl;
        std::cout << "VI: "; imprime(vectorInicializacion); std::cout << std::endl;
        std::cout << "Bloque 1 de Texto Original: "; imprime(primerTexto); std::cout << std::endl;
        std::cout << "Bloque 2 de Texto Original "; imprime(segundoTextoCBC); std::cout << std::endl;
        std::cout << "\nBloque 1 de Texto Cifrado: "; imprime(bloqueCifrado2); std::cout << std::endl;
        std::cout << "Bloque 2 de Texto Cifrado: "; imprime(bloqueCifrado1); std::cout << std::endl;
        break;
    }
  }
  return 0;
}