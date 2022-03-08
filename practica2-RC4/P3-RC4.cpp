
// AUTOR: Dana B. Choque Zárate
// FECHA: XX/03/2022
// EMAIL: alu0101328348@ull.edu.es
// ASIGNATURA: Seguridad en Sistemas Informáticos
// PRÁCTICA Nº: 1º Cifrado RC4
// UNIVERSIDAD: Universidad de La Laguna- Escuela Superior de Ingeniería y Tecnología
// TITULACIÓN: Grado en Ingenieria informática
// ACTUALIZAR EL CMAKE: cmake .
// COMPILAR: make
// EJECUTAR: ./NombreDelEjecutable
// ELIMINAR EL EJECUTABLE: make clean

#include <iostream>
#include <vector>
#include<bits/stdc++.h>
#define TAM 256

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


int main(void) {
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