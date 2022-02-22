
// AUTOR: Dana B. Choque Zárate
// FECHA: 22/02/2022
// EMAIL: alu0101328348@ull.edu.es
// ASIGNATURA: Seguridad en Sistemas Informáticos
// PRÁCTICA Nº: 1º Cifrado de Vernam
// UNIVERSIDAD: Universidad de La Laguna- Escuela Superior de Ingeniería y Tecnología
// TITULACIÓN: Grado en Ingenieria informática
// ACTUALIZAR EL CMAKE: cmake .
// COMPILAR: make
// EJECUTAR: ./NombreDelEjecutable
// ELIMINAR EL EJECUTABLE: make clean

#include "verman.hpp"


int main(void) {

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
      cifrado_binario = MSG.ClaveABinario(mensaje_original_binario,clave);
      std::cout << "Mensaje cifrado en binario: " << cifrado_binario << std::endl;
      cifrado = MSG.Encriptar(cifrado_binario);
      std::cout << "Mensaje cifrado: " <<  cifrado << std::endl;
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
  
  return 0;
}