#include <SFML\Graphics.hpp>
const int HEIGHT_MAP = 40;//размер карты высота
const int WIDTH_MAP = 160;//размер карты ширина 


sf::String TileMap[HEIGHT_MAP] = {
	"0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000",
	"0                                                                                                                       w                                      0",
	"0    w       w                                                                                                          w                                      0",
	"0    w       w                           w                                                                              w                                      0",
	"0          f       f                     w                                                                              w                                      0",
	"0                                        w                                                                              w                                      0",
	"0         ww                             w                                                                              w                                      0",
	"0                                        w                                                                              w                                      0",
	"0                                        w                                                                              w                                      0",
	"0                                        w                                                                              w                                      0",
	"0                                        w                                                                              w                                      0",
	"0                                        w                                                                              w                                      0",
	"0                                        w                                                                              w                                      0",
	"0                                        w                                                                              w                                      0",
	"0                                        w                                                                              w                                      0",
	"0                                        w                                                                              w                                      0",
	"0                                        w                                                                              w                                      0",
	"0                                        w                                                                              w                                      0",
	"0                                        wwwwwwwwwwww                                                                   w                                      0",
	"0                                        w                                                                              w                                      0",
	"0                                        w                                                                              w                                      0",
	"0                                        w                                                                              w                                      0",
	"0                                        w                                                                              w                                      0",
	"0                                        w                                                                              w                                      0",
	"0                                        w                                                                              w                                      0",
	"0                                        w                                                                              w                                      0",
	"0                                        w                                                                              w                                      0",
	"0                                        w                                                                              w                                      0",
	"0                                        w                                                                              w                                      0",
	"0                                        w                                                                              w                                      0",
	"0                                        w                                                                              w                                      0",
	"0                                        w                                                                              w                                      0",
	"0                                        w                                                                              w                                      0",
	"0                                        w                                                                              w                                      0",
	"0                                        w                                                                              w                                      0",
	"0                                        w                                                                              w                                      0",
	"0                                        w                                                                              w                                      0",
	"0                                        w                                                                                                                     0",
	"0                                        w                                                                                                                     0",
	"0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000", };

void randomMapGenerate() {//рандомно расставляем камни


	int randomElementX = 0;//случайный элемент по горизонтали
	int randomElementY = 0;//случ эл-т по вертикали
	srand(time(0));//рандом	
	int countStone = 1;//количество камней 1

	while (countStone > 0) {
		randomElementX = 1 + rand() % (WIDTH_MAP - 1);//рандомное по иксу от 1 до ширина карты-1, чтобы не получать числа бордюра карты
		randomElementY = 1 + rand() % (HEIGHT_MAP - 1);//по игреку так же
		if (TileMap[randomElementY][randomElementX] == ' ') {//если встретили символ пробел, 
			TileMap[randomElementY][randomElementX] = 's'; //то ставим туда камень.
														   //std::cout << "coordinate of Stone X:" << randomElementX << "\n" << "coordinate of Stone Y:" << randomElementY << "\n\n";
			countStone--;//создали камень=>счетчик камней будет "текущий минус 1"
		}
	}
}