#include <SFML/Graphics.hpp>
using namespace sf;

sf::View view;//объ€вили sfml объект "вид", который и €вл€етс€ камерой

void getplayercoordinateforview(float x, float y) { //функци€ дл€ считывани€ координат игрока
	float tempX = x; float tempY = y;//считываем коорд игрока и провер€ем их, чтобы убрать кра€
    //убираем из вида
	if (x < 320) tempX = 320;//левую сторону
	if (x > 4798) tempX = 4798;//правую сторону
	if (y < 240) tempY = 240;//верхнюю сторону
	if (y > 1010) tempY = 1010;//нижнюю сторону	

	view.setCenter(tempX, tempY); //следим за игроком, передава€ его координаты. 

}


void viewmap(float time) { //функци€ дл€ перемещени€ камеры по карте. принимает врем€ sfml


	if (Keyboard::isKeyPressed(Keyboard::D)) {
		view.move(0.1*time, 0);//скроллим карту вправо
	}

	if (Keyboard::isKeyPressed(Keyboard::S)) {
		view.move(0, 0.1*time);//скроллим карту вниз
	}

	if (Keyboard::isKeyPressed(Keyboard::A)) {
		view.move(-0.1*time, 0);//скроллим карту влево
	}
	if (Keyboard::isKeyPressed(Keyboard::W)) {
		view.move(0, -0.1*time);//скроллим карту вправо 
	}

}

void changeview() {

	if (Keyboard::isKeyPressed(Keyboard::U)) {
		view.zoom(1.0100f); //масштабируем, уменьшение
							//view.zoom(1.0006f); //тоже самое помедленнее соответственно
	}
	if (Keyboard::isKeyPressed(Keyboard::R)) {
		view.setRotation(90);//сразу же задает поворот камере
	}
	if (Keyboard::isKeyPressed(Keyboard::I)) {
		view.setSize(640, 480);//устанавливает размер камеры (наш исходный)
	}
	if (Keyboard::isKeyPressed(Keyboard::P)) {
		view.setSize(540, 380);//например другой размер
	}
	if (Keyboard::isKeyPressed(Keyboard::Q)) {
		view.setViewport(sf::FloatRect(0, 0, 0.5f, 1));//таким образом делаетс€ раздельный экран дл€ игры на двоих. нужно только создать ещЄ один объект View и прив€зывать к нему координаты игрока 2.
	}

}