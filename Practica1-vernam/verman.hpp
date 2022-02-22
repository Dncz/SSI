#include <iostream>
#include <bitset>
#include <vector>
#include <sstream>

class verman {
// private:
//   std::string clave_, mensaje_, cifrado_;
    
public:
  verman();
  ~verman();
  std::string ConvertirABinario(std::string &mensaje);
  std::string ClaveABinario(std::string &mensaje, std::string &clave);
  std::string Encriptar(std::string &mensajeEncriptadoBinario);
  std::string Desencriptar(std::string &mensaje);

};


