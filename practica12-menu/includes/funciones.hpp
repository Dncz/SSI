#ifndef _FUNCIONES_
#define _FUNCIONES_
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <iostream>
#include <iomanip> // librería para setfill y setw
#include <vector>
#include <array>
#include <utility>
#include <math.h>
#include <algorithm>
#include<bits/stdc++.h>
#include "./verman.hpp"
#include "./constantes.h"

#define TAM 256
#define RESET   "\x1b[0m"
#define BLUE    "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define ROSE    "\x1B[38;2;255;151;203m"
#define CYAN    "\x1b[36m"
#define CLEAR "\x1B[2J\x1B[H"
#define SIZE_4 4
#define CteSnow3g "A9"
#define CteAES "1B"

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

void mainVernam(void) {
std::cout << "Selecciona una de las opciones:" << std::endl;
  std::cout << "1. Encriptar" << std::endl;
  std::cout << "2. Desencriptar" << std::endl;
  char opcion;
  std::cin >> opcion;

  if(opcion == '1') {
      std::string mensaje, mensaje_original_binario;
      verman MSG;
      std::cout << "Introduce el mensaje a cifrar: ";
      std::cin >> mensaje;
      mensaje_original_binario = MSG.ConvertirABinario(mensaje);
      std::cout << "Mensaje original en binario: " << 
                    mensaje_original_binario << std::endl;
      std::cout << "Longitud del mensaje binario: " << 
                    mensaje_original_binario.length() << std::endl;

      std::string clave, cifrado_binario, cifrado;
      std::cout << "\nIntroduce la clave aleatoria: ";
      std::cin >> clave;
      if(mensaje_original_binario.length() != clave.length())
        std::cout << "¡La clave tiene que ser de igual tamaño a mensaje binario original!" << std::endl;
      else {
        cifrado_binario = MSG.ClaveABinario(mensaje_original_binario,clave);
        std::cout << "Mensaje cifrado en binario: " << cifrado_binario << std::endl;
        cifrado = MSG.Encriptar(cifrado_binario);
        std::cout << "Mensaje cifrado: " <<  cifrado << std::endl;
      }
  }
  else if(opcion == '2') {
      verman MSG2;
      std::cout << "Introduce el mensaje cifrado: ";
      std::string mensaje_cifrado, mensaje_cifrado_binario;
      std::cin >> mensaje_cifrado;
      mensaje_cifrado_binario = MSG2.ConvertirABinario(mensaje_cifrado);

      std::cout << "Mensaje cifrado en binario: " << 
                    mensaje_cifrado_binario << std::endl;
      std::cout<< "Longitud dek mensaje binario: " << 
                    mensaje_cifrado_binario.length()<< std::endl;

      std::string clave_aleatoria, mensaje_original_binario, mensaje_original;
      std::cout << "Introduce la clave aleatoria: ";
      std::cin >> clave_aleatoria;
      mensaje_cifrado_binario = MSG2.ClaveABinario(mensaje_cifrado_binario, clave_aleatoria);
      std::cout << "Mensaje original en binario: " <<  mensaje_cifrado_binario << std::endl;
      mensaje_original = MSG2.Desencriptar(mensaje_cifrado_binario);
      std::cout << "Mensaje original: " << mensaje_original << std::endl;
  }
}

std::string aBinario(int numero) {
  std::string binario = "";
  binario.resize(8);
  while (numero != 0) {
    binario += (numero % 2 == 0 ? "0" : "1");
    numero /= 2;
  }

  std::reverse(binario.begin(),binario.end());
  return binario;
}

void mainRc4(void) {
int aux_semilla;
  std::vector<int> vector_semilla_clave(TAM);

  std::cout << "Práctica 3: cifrado RC4" << std::endl;
  std::cout << "Introduca la semilla de clave" << std::endl
            << "(introduce -1 para dejar de introducir datos)" << std::endl;
  // Lectura de la semilla clave
  int tam_semilla;
  for (int i = 0; aux_semilla != -1; i++) {
      std::cin >> aux_semilla;
      if(aux_semilla != -1) {
          vector_semilla_clave[i] = aux_semilla;
          tam_semilla = i + 1;
      }
  }
  vector_semilla_clave.resize(tam_semilla);

  // lectura del mensaje original
  std::vector<int> vector_mensaje(TAM);
  std::cout << "Introduca el mensaje original" << std::endl
            << "(introduce -1 para dejar de introducir datos)" << std::endl;
  int tam_mensaje;
  int aux_mensaje;
  for (int i = 0; aux_mensaje != -1; i++) {
      std::cin >> aux_mensaje;
      if(aux_mensaje != -1) {
          vector_mensaje[i] = aux_mensaje;
          tam_mensaje = i + 1;
      }
  }
  vector_mensaje.resize(tam_mensaje);

  // incialización: KSA
  std::vector<int> K(TAM);
  std::vector<int> S(TAM);
  for (int i = 0; i < K.size(); i++) {
      S[i] = i;
      K[i] = vector_semilla_clave[i%vector_semilla_clave.size()];
  }

  int j  = 0;
  int aux_intercambio;
  for(int i = 0; i < TAM; i++) {
    j = (j + S[i] + K[i]) % TAM;
    // intercambiar;
    aux_intercambio = S[i];
    S[i] = S[j];
    S[j] = aux_intercambio;
  }

  // Generacion de secuencia cifrante: PRGA
  int aux_intercambio2;
  int  indice_i = 0, indice_j = 0, indice_t = 0;
  for (int i = 0; i < vector_mensaje.size(); i++) {
    indice_i = (indice_i + 1) % TAM;
    indice_j = (indice_j + S[indice_i]) % TAM;
    // intercambiar
    aux_intercambio2 = S[indice_i];
    S[indice_i] = S[indice_j];
    S[indice_j] = aux_intercambio2;

    indice_t = (S[indice_i] + S[indice_j]) % TAM;

    std::cout << "Byte " << i + 1<< " de secuencia cifrante: Salida S[" 
              << indice_t <<"]= " << S[indice_t] << std::endl;
    std::cout << "En binario: " << aBinario(S[indice_t]) << std::endl;
    std::cout << "Byte " << i + 1<< " de texto original: Entrada M["<< i + 1 
              <<"]= " << vector_mensaje[i] << std::endl;
    std::cout << "En binario: " << aBinario(vector_mensaje[i]) << std::endl;
    int operacion_xor = S[indice_t] ^ vector_mensaje[i];
    std::cout << "Byte " << i + 1<< " de texto cifrado: Salida C["<< i + 1 
              << "]= " << operacion_xor << std::endl;
    std::cout << "En binario: " << aBinario(operacion_xor) << std::endl;
    std::cout << std::endl;
  }

  for ( int i = 0; i < S.size(); i++) {
    std::cout << S[i] << " ";
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

void mainChacha(void) {
std::cout << MAGENTA << "Práctica 3: Algoritmo Chacha20 " << RESET << "\U0001F47E" << std::endl;
  std::vector<uint32_t> clave(8);
  // clave = {0x03020100, 0x07060504, 0x0b0a0908, 0x0f0e0d0c, 0x13121110, 0x17161514, 0x1b1a1918, 0x1f1e1d1c};
  clave = {0x97a3990e, 0x1beb533c, 0xad6b42e2, 0x242d312f, 0x2b76c2d9, 0xd7f45e53, 0xa975178e, 0xa5683a45};

  std::cout << "Clave de 256 bits en forma de 8 palabras en hexadecimal: " << std::endl;
  imprime(clave);

  std::vector<uint32_t> contador(1);
  contador= {0x00000001};
  std::cout << "\nContador de 32 bits en forma de 1 palabra en hexadecimal:" << std::endl;
  imprime(contador);

  std::vector<uint32_t> nonce(3);
  // nonce = {0x09000000, 0x4a000000, 0x00000000};
  nonce = {0xacde9eb6, 0x0544ee73, 0x8e9afad3};
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
}

std::pair<int, int> tablaIDs(int idSatelite) {
  std::pair<int, int> idSat;
  switch (idSatelite) {
    case 1:
      idSat.first = 2;
      idSat.second = 6;
      break;
    case 2:
      idSat.first = 3;
      idSat.second = 7;
      break;
    case 3:
      idSat.first = 4;
      idSat.second = 8;
      break;
    case 4:
      idSat.first = 5;
      idSat.second = 9;
      break;
    case 5:
      idSat.first = 1;
      idSat.second = 9;
      break;
    case 6:
      idSat.first = 2;
      idSat.second = 10;
      break;
    case 7:
      idSat.first = 1;
      idSat.second = 8;
      break;
    case 8:
      idSat.first = 2;
      idSat.second = 9;
      break;
    case 9:
      idSat.first = 3;
      idSat.second = 10;
      break;
    case 10:
      idSat.first = 2;
      idSat.second = 3;
      break;
    case 11:
      idSat.first = 3;
      idSat.second = 4;
      break;
    case 12:
      idSat.first = 5;
      idSat.second = 6;
      break;
    case 13:
      idSat.first = 6;
      idSat.second = 7;
      break;
    case 14:
      idSat.first = 7;
      idSat.second = 8;
      break;
    case 15:
      idSat.first = 8;
      idSat.second = 9;
      break;
    case 16:
      idSat.first = 9;
      idSat.second = 10;
      break;
    case 17:
      idSat.first = 1;
      idSat.second = 4;
      break;
    case 18:
      idSat.first = 2;
      idSat.second = 5;
      break;
    case 19:
      idSat.first = 3;
      idSat.second = 6;
      break;
    case 20:
      idSat.first = 4;
      idSat.second = 7;
      break;
    case 21:
      idSat.first = 5;
      idSat.second = 8;
      break;
    case 22:
      idSat.first = 6;
      idSat.second = 9;
      break;
    case 23:
      idSat.first = 1;
      idSat.second = 3;
      break;
    case 24:
      idSat.first = 4;
      idSat.second = 6;
      break;
    case 25:
      idSat.first = 5;
      idSat.second = 7;
      break;
    case 26:
      idSat.first = 6;
      idSat.second = 8;
      break;
    case 27:
      idSat.first = 7;
      idSat.second = 9;
      break;
    case 28:
      idSat.first = 8;
      idSat.second = 10;
      break;
    case 29:
      idSat.first = 1;
      idSat.second = 6;
      break;
    case 30:
      idSat.first = 2;
      idSat.second = 7;
      break;
    case 31:
      idSat.first = 3;
      idSat.second = 8;
      break;
    case 32:
      idSat.first = 4;
      idSat.second = 9;
      break;
    default:
      break;
  }
  return idSat;
}

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
cifradorCA(int longitudSalida, std::pair<int, int> idSat) {
  std::vector<u_int> registro1 = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
  std::vector<u_int> registro2 = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
  std::vector<u_int> resultado(longitudSalida);
  std::cout << CYAN << "\t\tRegistro1 \t\t\t\t" << "\t\t Registro2"  << RESET << std::endl;
  for(int i = 0; i < longitudSalida; i++) {
    u_int bitRealimentacion1 = registro1[2] ^ registro1[9];
    u_int bitRealimentacion2 = registro2[1] ^ registro2[2] ^ registro2[5] ^ registro2[7] ^ registro2[8] ^ registro2[9];
    imprimeRegistros(registro1, registro2, bitRealimentacion1, bitRealimentacion2);
    u_int bitPrn1 = registro2[idSat.first - 1] ^ registro2[idSat.second - 1];
    resultado[i] = bitPrn1 ^ registro1[9];
    for(int j = registro1.size() - 1; j > 0 ; j--) {
      registro1[j] = registro1[j - 1];
      registro2[j] = registro2[j - 1];
    }
    registro1[0] = bitRealimentacion1;
    registro2[0] = bitRealimentacion2;
  }
  return resultado;
}

void mainCaGps(void) {
  std::cout << MAGENTA << "Práctica 4: Generador C/A de GPS " << RESET << "\U0001F47E" << std::endl;

  int idSatelite = 0;
  std::cout << "Introduce el ID del satélite: ";
  std::cin >> idSatelite;

  std::pair<int, int> idSat;
  idSat = tablaIDs(idSatelite);

  int longitudSalida = 0;
  std::cout << "Introduce la longitud de la salida: ";
  std::cin >> longitudSalida;
  
  std::vector<u_int> salida(longitudSalida);
  salida = cifradorCA(longitudSalida, idSat);
  imprimeResultado(salida);
}

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

void mainSnow3GAES(void) {
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

void imprimeAES(std::vector<std::vector<unsigned char>> vector) {
  for(int i = 0; i < vector.size(); i++) {
    for(int j = 0; j < vector[i].size(); j++) {
      std::cout << std::hex << std::setfill('0') << std::setw(2) << int(vector[j][i]) << " ";
    }
  }
  // std::cout << std::endl;
}
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

std::vector<std::vector<unsigned char>>
subBytes(std::vector<std::vector<unsigned char>> textoCifrado) {
  for (int i = 0; i < textoCifrado.size(); i++){
		for (int j = 0; j < textoCifrado[i].size(); j++){
      textoCifrado[i][j] = sCaja[textoCifrado[i][j]];
    }
  }
  return textoCifrado;
}

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
  for(int i = 1; i < matrizResultante.size(); i++) {
    for(int j = 0; j < matrizResultante[i].size(); j++) {
      matrizResultante[j][i] = matrizResultante[j][i -1] ^ copia[j][i];
    }
  }
  // std::cout << "SUBCLAVE" << std::endl;
  // imprime(matrizResultante);
  return matrizResultante;
}

void mainAES(void) {
  std::vector<std::vector<unsigned char>> clave(SIZE_4, std::vector<unsigned char>(SIZE_4));
  clave[0][0] = 0x00; clave[1][0] = 0x01; clave[2][0] = 0x02; clave[3][0] = 0x03;
	clave[0][1] = 0x04;	clave[1][1] = 0x05;	clave[2][1] = 0x06;	clave[3][1] = 0x07;
	clave[0][2] = 0x08;	clave[1][2] = 0x09;	clave[2][2] = 0x0A;	clave[3][2] = 0x0B;
	clave[0][3] = 0x0C;	clave[1][3] = 0x0D;	clave[2][3] = 0x0E;	clave[3][3] = 0x0F;

  std::cout << BLUE << "Clave: " << RESET; imprimeAES(clave); std::cout << std::endl;

  std::vector<std::vector<unsigned char>> texto(SIZE_4, std::vector<unsigned char>(SIZE_4));
	texto[0][0] = 0x00;	texto[1][0] = 0x11;	texto[2][0] = 0x22;	texto[3][0] = 0x33;
	texto[0][1] = 0x44;	texto[1][1] = 0x55;	texto[2][1] = 0x66;	texto[3][1] = 0x77;
	texto[0][2] = 0x88;	texto[1][2] = 0x99;	texto[2][2] = 0xAA;	texto[3][2] = 0xBB;
	texto[0][3] = 0xCC;	texto[1][3] = 0xDD;	texto[2][3] = 0xEE;	texto[3][3] = 0xFF;

  std::cout <<BLUE << "Bloque del Texto Original: " << RESET; imprimeAES(texto); std::cout << std::endl;
  std::cout << std::endl;

  std::cout << CYAN << "R0 (Subclave = " << RESET; imprimeAES(clave); std::cout << CYAN << ")= " << RESET;
  std::vector<std::vector<unsigned char>> matrixDeEstado;
  matrixDeEstado = addRoundKey(clave, texto);
  std::cout << std::endl;

  std::vector<std::vector<unsigned char>> claveExpandida;
  claveExpandida = expandClave(clave, 0);

  for(int i = 1; i < 10; i++) {
    std::cout << CYAN << "R" << i << " (Subclave = " << RESET; imprimeAES(claveExpandida); std::cout << CYAN << ")= " << RESET; 
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
  std::cout << CYAN << "R10 (Subclave = " << RESET; imprimeAES(claveExpandida); std::cout << CYAN << ")= " << RESET;
  matrixDeEstado = addRoundKey(claveExpandida, matrixDeEstado);
  std::cout << std::endl;

  std::cout << ROSE << "\nBloque del Texto Cifrado: " << RESET; imprimeAES(matrixDeEstado); std::cout << std::endl;
}

std::vector<std::vector<unsigned char>>
operacionXor(std::vector<std::vector<unsigned char>> vectorInicializacion, std::vector<std::vector<unsigned char>> texto) {
  for(int i = 0; i < texto.size(); i++) {
    for(int j = 0; j < texto[i].size(); j++) {
      texto[i][j] = vectorInicializacion[i][j] ^ texto[i][j];
    }
  }
  return texto;
}

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

void
contadorElementos(std::vector<std::vector<unsigned char>> texto, int &contador) {
  for (int i = 0; i < texto.size(); i++) {
    for (int j = 0; j < texto[i].size(); j++) {
      contador++;
    }
  }
}

void mainAesCBC(void) {
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

        std::cout << "\nClave: "; imprimeAES(clave); std::cout << std::endl;
        std::cout << "VI: "; imprimeAES(vectorInicializacion); std::cout << std::endl;
        std::cout << "Bloque 1 de Texto Original: "; imprimeAES(primerTexto); std::cout << std::endl;
        std::cout << "Bloque 2 de Texto Original "; imprimeAES(segundoTextoCBC); std::cout << std::endl;
        std::cout << "\nBloque 1 de Texto Cifrado: "; imprimeAES(bloqueCifrado1); std::cout << std::endl;
        std::cout << "Bloque 2 de Texto Cifrado: "; imprimeAES(bloqueCifrado2); std::cout << std::endl;
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

        std::cout << "\nClave: "; imprimeAES(clave); std::cout << std::endl;
        std::cout << "VI: "; imprimeAES(vectorInicializacion); std::cout << std::endl;
        std::cout << "Bloque 1 de Texto Original: "; imprimeAES(primerTexto); std::cout << std::endl;
        std::cout << "Bloque 2 de Texto Original "; imprimeAES(segundoTextoCBC); std::cout << std::endl;
        std::cout << "\nBloque 1 de Texto Cifrado: "; imprimeAES(bloqueCifrado2); std::cout << std::endl;
        std::cout << "Bloque 2 de Texto Cifrado: "; imprimeAES(bloqueCifrado1); std::cout << std::endl;
        break;
    }
  }
}

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

void imprimirTraza(std::vector<int> &x, std::vector<int> &z) {
  std::cout << std::endl;
  for(int i = 0; i < x.size() && i < z.size(); i++) {
    std::cout << x[i] << " " << z[i + 1] << std::endl;
  }
}

int euclidesExtendido(int dividendo, int divisor) {
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
  mensajeDesencriptado = (kInversa * numeroCifrado) % numeroPrimo;
  std::cout << "yA: " << yA << ", yB= " << yB << ", kA= " << kA << ", kB= " << kB
            << ", Numero cifrado: " << numeroCifrado << ", k-1: " << kInversa
            << ", M: " << mensajeDesencriptado << std::endl << std::endl;
}

void mainDhElGamal(void) {
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

void mainRSA(void) {
  rsa("MANDA DINEROS", 421, 7, 1619);
  std::cout << std::endl;
  rsa("AMIGO MIO", 2347, 347, 5);
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

void mainCripEliptica(void) {
  std::pair<int, int> punto(9, 6);
  eliptica(13, 5, 3, punto, 4, 2, 2);
}

#endif