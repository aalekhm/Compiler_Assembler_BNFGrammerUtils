#include <iostream>

int main(int argc, char* argv[])
{
	int32_t iInt32 = 0;
	iInt32 |= (44 & 0xFF) << 24;
	iInt32 |= (33 & 0xFF) << 16;
	iInt32 |= (22 & 0xFF) << 8;
	iInt32 |= (11 & 0xFF);

	int8_t* i = (int8_t*)&iInt32;
	int8_t* j = (((int8_t*)&iInt32) + 1);
	int8_t* k = (((int8_t*)&iInt32) + 2);
	int8_t* l = (((int8_t*)&iInt32) + 3);

	std::cout << iInt32 << " " << *i << " " << *j << " " << *k << " " << *l << "";

	exit(EXIT_SUCCESS);
}