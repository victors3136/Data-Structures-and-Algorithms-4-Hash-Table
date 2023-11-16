#include <iostream>
#include "ExtendedTest.h"
#include "ShortTest.h"


int main() {
	testAll();
	testAllExtended();
	std::cout << "That's all... " << std::endl;
	system("pause");
}