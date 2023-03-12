#include "game.hpp"

#undef main

int main()
{
	try
	{
		TinyFootball ah;
		ah.start();
	}
	catch (const std::exception& ex)
	{
		std::cout << "error: " << ex.what() << std::endl;
	}

	return 0;
}
