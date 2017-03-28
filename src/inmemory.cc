#include "inmemory.h"
#include "idatabase.h"

#include <iostream>

int main() {
	InMemory t;
	t.count = 1;
	std::cout << "t.count = " << t.count << std::endl;
}
