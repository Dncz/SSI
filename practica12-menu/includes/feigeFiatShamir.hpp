#include <iostream>

void feigeFiatShamir(int p, int q, int r, int s1, int s2, int bitA, int iteraciones) {
	int n = p * q;

	std::cout << "N: " << n << std::endl;
	// Compromiso secreto de A
	std::cout << "Número secreto r: " << r << std::endl;

	// Testigo: A envía a B
	int x = (r * r) % n;
	std::cout << "x: " << x << std::endl;

	int y, s = s1;
	int aux = 1;
	for (int i = 0; i < iteraciones; i++) {
		std::cout << "Iteración " << i << std::endl;
		// Identificación pública de A
		int v = (s * s) % n;
		std::cout << "Número secreto v" << i + 1 << ": " << s  << " = " << v << " mod (" << n << ")" << std::endl;
		std::cout << "bit a: " << bitA << std::endl;
		aux *= std::pow(v, bitA);
		// Respuesta: A envía a B 
		y = r * std::pow(s, bitA);
		s = s2;
		bitA++;
	}
	std::cout << "y: " << y << std::endl;

	int parte1 = (y * y) % n;
	int parte2 = (x * aux) % n;
	// Verificación
	if (parte1 == parte2) {
		std::cout << "correcto" << std::endl;
	} else {
		std::cout << "incorrecto" << std::endl;
	}
}

void mainFFS(void) {
	feigeFiatShamir(7, 5, 3, 16, 2, 0, 2);
}