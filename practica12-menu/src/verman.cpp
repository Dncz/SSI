#include "../includes/verman.hpp"

verman::verman(void) {}


verman::~verman(void) {}


std::string
verman::ConvertirABinario(std::string &mensaje) {
  std::string resultado;
  for (int i = 0; i < mensaje.length(); ++i) 
    resultado += std::bitset<8> (mensaje[i]).to_string();
      
  return resultado;
}


std::string
verman::ClaveABinario(std::string &mensaje, std::string &clave) {
  std::string resultado;
  
  for (int i = 0; i < mensaje.size(); i++){
    // tabla xor:
    // cuando los números son iguales --> 0
    // caso contrario, 1.
    if (mensaje[i] == clave[i])
      resultado += '0';
    else
      resultado += '1';
  }
  
  return resultado;
}


std::string
verman::Encriptar(std::string &mensaje_encriptado_binario) {
  //  cifrado será del tamaño del mensaje en binario / 8 bits que ocupa 1 palabra
  std::string cifrado("", mensaje_encriptado_binario.size()/8);
  
  // meto en el bufer el mensaje encriptado
	std::stringstream mensaje_stream(mensaje_encriptado_binario);
  // leo el mensaje. Creo un bitset para luego convertirlo a ASCII
	for (int i=0; i < mensaje_encriptado_binario.length()/8;i++){
		std::bitset<8> mensaje_bit;
    // desplazo 8 bits para leer el primer numero
		mensaje_stream >> mensaje_bit;
    // std::cout << mensaje_bit << std::endl;
    // convierto los 8 bits que se lea a char
		cifrado[i] = char(mensaje_bit.to_ulong());
	}

	return cifrado;
}


std::string
verman::Desencriptar(std::string &mensaje_cifrado) {
  //  cifrado será del tamaño del mensaje en binario / 8 bits que ocupa 1 palabra
  std::string cifrado("", mensaje_cifrado.size()/8);
  
  // meto en el bufer el mensaje encriptado
  std::stringstream mensaje_stream(mensaje_cifrado);
  // leo el mensaje. Creo un bitset para luego convertirlo a ASCII
	for (int i=0; i < mensaje_cifrado.length()/8;i++){
		std::bitset<8> mensaje_bit;
    // desplazo 8 bits para leer el primer numero
    mensaje_stream >> mensaje_bit;
    // std::cout << mensaje_bit << std::endl;
    // convierto los 8 bits que se lea a char
		cifrado[i] = char(mensaje_bit.to_ulong());
	}
	return cifrado;
}