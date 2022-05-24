#include <iostream>
#include "../practica12-menu/includes/funciones.hpp"
#include "../practica12-menu/includes/feigeFiatShamir.hpp"


int main(void) {
  int option = 0;
  std::cout << "1. Verman" << std::endl;
  std::cout << "2. RC4" << std::endl;
  std::cout << "3. Chacha20" << std::endl;
  std::cout << "4. CA GPS" << std::endl;
  std::cout << "5. Multiplicación Binaria Snow AES" << std::endl;
  std::cout << "6. Rijndael" << std::endl;
  std::cout << "7. AES CBC" << std::endl;
  std::cout << "8 DH ElGamal" << std::endl;
  std::cout << "9. RSA" << std::endl;
  std::cout << "10. Criptología elítica" << std::endl;
  std::cout << "11. Feige-Fiat-Shamir" << std::endl;
  std::cin >> option;
  switch (option) {
    case 1:
      mainVernam();
      break;
    case 2:
      mainRc4();
      break;
    case 3:
    mainChacha();
      break;
    case 4:
    mainCaGps();
      break;
    case 5:
    mainSnow3GAES();
      break;
    case 6:
    mainAES();
      break;
    case 7:
    mainAesCBC();
      break;
    case 8:
    mainDhElGamal();
      break;
    case 9:
    mainRSA();
      break;
    case 10:
    mainCripEliptica();
      break;
    case 11:
    mainFFS();
      break;
  }
}
