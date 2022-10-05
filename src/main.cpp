#include <memory>
#include "GameBoy.h"

int main(int argc, char **argv)
{
    if ( argc < 2 ) {
        std::cout << "Usage: patboy.exe <rom>.gb";
        return -1;
    }

	std::unique_ptr<GameBoy> gameboy(new GameBoy(  argv[1] ));
	gameboy->startEmulation();
	return 0;
}