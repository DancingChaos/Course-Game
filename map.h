#include <SFML\Graphics.hpp>
#include <iostream>
const int HEIGHT_MAP = 40;//размер карты высота
const int WIDTH_MAP = 76;//размер карты ширина 


sf::String TileMap[HEIGHT_MAP] = {
	"000000000000000000000001111111111111111111111111111111111111111111111111111",
	"000000000000000000000001111111111111111111111111111111111111111111111111111",
	"00ffff  o       fffff001111111111111111111111111111111111111111111111111111",
	"00ff gffffff   g ffff001111111111111111111111111111111111111111111111111111",
	"00ffffffgfffo      ff001111111111111111111111111111111111111111111111111111",
	"00  g     o   ffff  f001111111111111111111111111111111111111111111111111111",
	"00ffffffffffffffff  f001111111111111111111111111111111111111111111111111111",
	"000000000000000000  f000000000000000000000000000000000000000000000000000000",
	"000000000000000000  f000000000000000000000000000000000000000000000000000000",
	"111111111111111100                                               ffffffff00",
	"111111111111111100                                                   ffff00",
	"111111111111111100   fffffffffffffffffffffffff  fffffffffff   fffffffffff00",
	"111111111111111100  f0000000000000000000000000  00000000000   f000000000000",
	"111111111111111100  f000000000                  00000000000   f000000000000",
	"111111111111111100  f00fffffffffff              ffffffffff0   fffffffffff00",
	"111111111111111100  f00 g      o     g fffffff            0   f0         00",
	"111111111111111100  f00    ff       o  fffofff00                         00",
	"111111111111111100  f00ffffff o   g     ffffff00 o   o   g     g   o   g 00",
	"111111111111111100         g            offfff00                         00",
	"111111111111111100               o           f00                         00",
	"111111111111111100   ff      o            o  f00 o   g   o     g   o   g 00",
	"111111111111111100  f00fffffff      g        f00                         00",
	"111111111111111100  f00fffgff    offffffffffff00                         00",
	"111111111111111100  f00ffff  o     g    offfff00 g   g   g    fo   g   g 00",
	"111111111111111100  f00ff               ffffff00             f0          00",
	"111111111111111100  f00fffffffff fffffffffffff00fffffffffff  f0ffffffffff00",
	"000000000000000000  f00000000000 0f00000000000000000000000f  f0000000000000",
	"000000000000000000  f00000000000 0f00000000000000000000000f  f0000000000000",
	"000fffffffffff                                                      fffff00",
	"000fffffffffff                  ffff                                 ffff00",
	"000ffffff       fffffffffffffffffffffffffffffffffffffffffffffff        ff00",
	"000ffffff       f0000000000000000000000000000000000000000000000         f00",
	"000fffffff      f0000000000000000000000000000000000000000000000         f00",
	"000             f0011111111111111111111111111111111111111111100         f00",
	"000         fffff0011111111111111111111111111111111111111111100       fff00",
	"000          ffff0011111111111111111111111111111111111111111100     fffff00",
	"000       fffffff0011111111111111111111111111111111111111111100    ffffff00",
	"000ffffffffffffff0011111111111111111111111111111111111111111100ffffffffff00",
	"000000000000000000000000000000000000000000000000000000000000000000000000000",
	"000000000000000000000000000000000000000000000000000000000000000000000000000", };
void randomMapGenerate() {//рандомно расставляем камни


	int randomElementX = 0;//случайный элемент по горизонтали
	int randomElementY = 0;//случ эл-т по вертикали

	
	randomElementX = 1 + rand() % (WIDTH_MAP - 1);
	randomElementY = 1 + rand() % (HEIGHT_MAP - 1);

	if (TileMap[randomElementY][randomElementX] == ' ') {
		TileMap[randomElementY][randomElementX] = 's'; 
	}

	randomElementX = 1 + rand() % (WIDTH_MAP - 1);
	randomElementY = 1 + rand() % (HEIGHT_MAP - 1);
	if (TileMap[randomElementY][randomElementX] == ' ') {
		TileMap[randomElementY][randomElementX] = 'h';
	}

	randomElementX = 1 + rand() % (WIDTH_MAP - 1);
	randomElementY = 1 + rand() % (HEIGHT_MAP - 1);
	if (TileMap[randomElementY][randomElementX] == ' ') {
		TileMap[randomElementY][randomElementX] = 'w';
	}
	
}