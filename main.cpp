#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <thread>
#include "map.h"
#include "view.h"
#include <vector>
#include <queue>
#include <iostream>
#include <sstream>
#include <list>

using namespace sf;
using namespace std;
//Constants
const int Block = 32;
enum PlayerMove {
	Up = 3,
	Down = 2,
	Left = 1,
	Right = 0

};
//////////////////Point and graphs///////////////////////////////
struct point { int x; int y; int num; };

struct graph {
	graph() = default;
	graph(size_t nodes) : m_adjacency_list(nodes) {
	}

	size_t number_of_nodes() const {
		return m_adjacency_list.size();
	}

	std::vector<size_t> const& neighbours_of(size_t node) const {
		return m_adjacency_list.at(node);
	}

	void add_edge(size_t from, size_t to) {
		vector<size_t>& al = m_adjacency_list.at(from);
		for (size_t i = 0; i < al.size(); ++i) if (al[i] == to) return;
		al.push_back(to);
	}

private:
	vector<vector<size_t>> m_adjacency_list;
};

point searchGraphCoords(int needNum) {
	int freeNum = 0;
	for (int i = 0; i < HEIGHT_MAP; i++)
		for (int j = 0; j < WIDTH_MAP; j++)
		{
			if (TileMap[i][j] == ' ' || TileMap[i][j] == 'g' ||
				TileMap[i][j] == 'o' || TileMap[i][j] == 'w' || TileMap[i][j] == 's' ||
				TileMap[i][j] == 'h') {
				freeNum++;
				if (needNum == freeNum) {
					point coords;
					coords.x = j * Block;
					coords.y = i * Block;
					coords.num = freeNum;
					return coords;

				}
			}
		}
}

//////////////////Player Class////////////////////////
class Player {
private:

	const int Stay = 0;
	const int BarMax = 100;
	const int BarMin = 0;
	const int Tick = 15;

	const float HpRegen = 0.002;
	const float ManaRegen = 0.004;
	const float ScoreAdd = 0.02;
	const float AttackCoolDown = 0.005;



	int playerGetDamageTime = 0;

	Image PlayerIm;
	Image PlayerImD;

	Texture PlayerTex;
	Texture PlayerTexD;

	graph GraphMap;
	String File;


public:
	//Init Variables

	float ManaCost;
	float Width, Heigth;
	float PlayerX, PlayerY;
	float CurrentPX, CurrentPY;
	float CDAttack, CDFireBall;
	float CrosshairW, CrosshairH;
	float Speed, Health, Mana, playerScore;
	
	const float Ready = 0;

	int Direction, BDir;
	int mouseX, mouseY;

	bool life, isShoot;
	bool NotGetDamageState;

	Image CrossHairImg;

	Texture CrossHairTex;

	Sprite PlayerSpr;
	Sprite CrossHairSpr;


	Player() = default;
	Player(float X, float Y, float W, float H, graph GraphMap, float CSW, float CSH) {
		//Variables
		ManaCost = 2;
		playerScore = 0;
		Health = 100; Mana = 100;
		Direction = 0; Speed = 0;
		CurrentPX = 0; CurrentPY = 0;

		life = true;
		isShoot = false;
		NotGetDamageState = true;

		this->GraphMap = GraphMap;
		Width = W; Heigth = H;
		CrosshairW = CSW; CrosshairH = CSH;

		//Set Texture
		PlayerIm.loadFromFile("img/Hero.png");
		CrossHairImg.loadFromFile("img/Crosshair.png");
		PlayerImD.loadFromFile("img/heroGetDamage.png");

		PlayerIm.createMaskFromColor(Color::White);
		PlayerImD.createMaskFromColor(Color::White);

		PlayerTex.loadFromImage(PlayerIm);
		PlayerTexD.loadFromImage(PlayerImD);
		CrossHairTex.loadFromImage(CrossHairImg);

		PlayerSpr.setTexture(PlayerTex);
		CrossHairSpr.setTexture(CrossHairTex);

		//Set Player Position
		PlayerX = X;
		PlayerY = Y;

		PlayerSpr.setOrigin(Width / 2, Heigth / 2);
		CrossHairSpr.setOrigin(CrosshairW / 2, CrosshairH / 2);
	}

	void update(float time)
	{
		// Set Sprite Texture
		if (NotGetDamageState) {
			PlayerSpr.setTexture(PlayerTex);
		}
		else {
			PlayerSpr.setTexture(PlayerTexD);
		}



		//Player Direction
		switch (Direction)
		{
		case Up: CurrentPX = Stay; CurrentPY = -Speed; break;
		case Down: CurrentPX = Stay; CurrentPY = Speed; break;
		case Left: CurrentPX = -Speed; CurrentPY = Stay; break;
		case Right: CurrentPX = Speed; CurrentPY = Stay; break;
		}

		//scoreTimer
		playerScore += ScoreAdd *time;

		//Player Speed
		PlayerX += CurrentPX * time;
		PlayerY += CurrentPY * time;
		Speed = Stay;
		PlayerSpr.setPosition(PlayerX, PlayerY);

		//Collision
		interactionWithMap(time);

		//Life Check
		if (Health <= BarMin) { life = false; }

		//ManaRegen
		if (Mana + ManaRegen*time <= BarMax) {
			Mana += ManaRegen *time;
		}
		else {
			Mana = BarMax;
		}

		//HPRegen
		if (Health + HpRegen*time <= BarMax) {
			Health += HpRegen *time;
		}
		else {
			Health = BarMax;
		}

		//CoolDowns
		if (CDAttack - AttackCoolDown *time >= Ready) {
			CDAttack -= AttackCoolDown *time;
		}
		else {
			CDAttack = Ready;
		}

		// set texture back
		playerGetDamageTime++;
		if (playerGetDamageTime > Tick) {
			NotGetDamageState = true;
			playerGetDamageTime = Ready;
		}

	}

	//X Player
	float getplayercoordinateX() {
		return PlayerX;
	}

	//Y Player
	float getplayercoordinateY() {
		return PlayerY;
	}

	//Collision
	void interactionWithMap(int time)
	{
		for (int i = PlayerY / Block; i < (PlayerY + Heigth / 4) / Block; i++)
			for (int j = PlayerX / Block; j < (PlayerX + Width / 4) / Block; j++)
			{
				if (TileMap[i][j] == '0')
				{
					if (CurrentPY > 0)
					{
						Speed = 0;
						PlayerY = i * Block - Heigth / 4;
					}
					if (CurrentPY < 0)
					{
						Speed = 0;
						PlayerY = i * Block + Block;
					}
					if (CurrentPX > 0)
					{
						Speed = 0;
						PlayerX = j * Block - Width / 4;
					}
					if (CurrentPX < 0)
					{
						Speed = 0;
						PlayerX = j * Block + Block;
					}
				}

				if (TileMap[i][j] == 's') {
					Mana += 20;
					TileMap[i][j] = ' ';
				}
				if (TileMap[i][j] == 'f') {
					if (Health >= 0) {
						Health -= 0.005*time;
					}
					else {
						Health = 0;
					}

				}
				if (TileMap[i][j] == 'h') {
					if (Health + 20 <= BarMax) {
						Health += 20;
						TileMap[i][j] = ' ';
					}
					else {
						Health = BarMax;
						TileMap[i][j] = ' ';
					}
				}
				if (TileMap[i][j] == 'w') {
					Health -= 20;
					TileMap[i][j] = ' ';
				}

			}
	}

	// Graph Works
	point getPlayerGraph() {

		int freeNum = 0;
		int size = Block / 2;

		// Player Graph Find
		for (int i = 0; i < HEIGHT_MAP; i++)
			for (int j = 0; j < WIDTH_MAP; j++)
			{
				if (TileMap[i][j] == ' ' || TileMap[i][j] == 'g' ||
					TileMap[i][j] == 'o' || TileMap[i][j] == 'w' || TileMap[i][j] == 's' ||
					TileMap[i][j] == 'h') {
					freeNum++;
					if (PlayerX - size < j * Block && PlayerX + size > j * Block &&
						PlayerY - size < i * Block && PlayerY + size > i * Block) {
						point coords;
						coords.x = i * Block;
						coords.y = j * Block;
						coords.num = freeNum;

						return coords;

					}
				}
			}

		// If path dont find
		point tempcoord;
		tempcoord.x = 1;
		tempcoord.y = 1;
		tempcoord.num = 1;
		return tempcoord;

	}

	// Operator reloading
	Player operator =(Player p) {
		return p;
	}

};

/////////////////Enemy Class///////////////////////////
class Enemy {
private:

	int needGraphNum;
	int enemyGetDamageTime = 0;

	float time;
	float playerX;
	float playerY;
	float updateTime;

	bool SoundIsOn = true;
	bool firstAttempt = true;

	Sound sound_die;
	Sound sound_attack;

	SoundBuffer buffer_die;
	SoundBuffer buffer_attack;

	Image EnemyIm;
	Image EnemyImD;

	Texture EnemyTex;
	Texture EnemyTexD;

	point needGraph;
	point playerGraph;

	graph graphMap;
	Player* p;



public:
	float Width, Heigth, CurrentPX, CurrentPY,
		eX, eY, health;
	bool life;
	Sprite EnemySpr;
	bool NotGetDamageState;

	Enemy(float X, float Y, float W, float H, graph graphMap, Player* p) {
		updateTime += 0;

		//Variables
		this->p = p;
		Width = W; Heigth = H;
		NotGetDamageState = true;
		this->graphMap = graphMap;
		CurrentPY = 0; life = true;
		health = 100; CurrentPX = 0;

		// sounds
		sound_die.setBuffer(buffer_die);
		sound_attack.setBuffer(buffer_attack);

		buffer_die.loadFromFile("Sound/ZombieDie.flac");
		buffer_attack.loadFromFile("Sound/ZombieS.flac");

		//Set Texture
		EnemyIm.loadFromFile("img/Enemy.png");
		EnemyIm.createMaskFromColor(Color::White);

		EnemyImD.loadFromFile("img/EnemyGetDamage.png");
		EnemyImD.createMaskFromColor(Color::White);

		EnemyTex.loadFromImage(EnemyIm);

		EnemySpr.setTexture(EnemyTex);

		// Set Texture Damage
		EnemyTexD.loadFromImage(EnemyImD);

		//Set Player Position
		eX = X;
		eY = Y;
		EnemySpr.setOrigin(Width / 2, Heigth / 2);
		needGraph.num = 0;
		needGraph.x = 0;
		needGraph.y = 0;


	}

	void getArgs(float time, point playerGraph, float playerX, float playerY) {

		this->time = time;
		this->playerGraph = playerGraph;

		this->playerX = playerX;
		this->playerY = playerY;
	}

	void update() {
		// Update Timer
		updateTime += time;

		// Damage Sprite
		if (NotGetDamageState) {
			EnemySpr.setTexture(EnemyTex);
		}
		else {
			EnemySpr.setTexture(EnemyTexD);
		}

		// Get Player Graph
		point enemyGraph = getEnemyGraph();
		if (enemyGraph.num != playerGraph.num) {
			goToPlayer(enemyGraph, playerGraph, time);
		}

		////Rotation Funct
		float dX = playerX - eX;
		float dY = playerY - eY;
		float rotation = (atan2(dY, dX)) * 180 / 3.14159265;
		EnemySpr.setRotation(rotation);

		//Damage Show
		enemyGetDamageTime++;
		if (enemyGetDamageTime > 10) {
			NotGetDamageState = true;
			enemyGetDamageTime = 0;
		}

		// Life Check
		if (health <= 0) {
			life = false;
			p->playerScore += 10;

		}

		//Move Enemy
		EnemySpr.setPosition(eX, eY);

		// Attack in Graph
		if (enemyGraph.num == playerGraph.num) {
			attackPlayer();
		}

	}
	//Attack
	void attackPlayer() {
		if (sound_attack.getStatus() != 2) {
			if (SoundIsOn) {
				sound_attack.play();
			}
			p->Health -= 10;
			p->NotGetDamageState = false;
		}




	}

	//Move Funct
	void goToPlayer(point enemyGraph, point playerGraph, float time) {
		float distance;
		if ((enemyGraph.num == needGraphNum && updateTime > 300) || firstAttempt) {
			updateTime = 0;

			needGraphNum = searchPos(enemyGraph.num, playerGraph.num, graphMap);
			needGraph = searchGraphCoords(needGraphNum);
			firstAttempt = false;
		}


		distance = sqrt((needGraph.x - eX)*(needGraph.x - eX) + (needGraph.y - eY)*(needGraph.y - eY));
		eX += 0.12*time*(needGraph.x - eX) / distance;
		eY += 0.12*time*(needGraph.y - eY) / distance;


	}

	//Get Graph Funct
	point getEnemyGraph() {
		int freeNum = 0;
		int size = 16;
		for (int i = 0; i < HEIGHT_MAP; i++)
			for (int j = 0; j < WIDTH_MAP; j++)
			{
				if (TileMap[i][j] == ' ' || TileMap[i][j] == 'g' ||
					TileMap[i][j] == 'o' || TileMap[i][j] == 'w' || TileMap[i][j] == 's' ||
					TileMap[i][j] == 'h') {
					freeNum++;
					if (eX - size < j * 32 && eX + size > j * 32 &&
						eY - size < i * 32 && eY + size > i * 32) {
						point coords;
						coords.x = i * Block;
						coords.y = j * Block;
						coords.num = freeNum;
						return coords;
					}
				}

			}

	}

	//Search Funct
	size_t searchPos(size_t start, size_t target, graph& GraphMap)
	{
		vector<size_t> reached_by(GraphMap.number_of_nodes(), GraphMap.number_of_nodes());
		queue<size_t> q;

		size_t need_num = 0;


		reached_by[start] = start;
		q.push(start);

		while (!q.empty()) {
			size_t node = q.front();
			q.pop();
			for (size_t i = 0; i < GraphMap.neighbours_of(node).size(); ++i) {
				size_t candidate = GraphMap.neighbours_of(node)[i];
				if (reached_by[candidate] == GraphMap.number_of_nodes()) {

					reached_by[candidate] = node;
					if (candidate == target) break;
					q.push(candidate);
				}
			}
		}

		if (reached_by[target] == GraphMap.number_of_nodes()) {
			return 1;
		}
		else {
			for (size_t node = target; node != start; node = reached_by[node]) {
				need_num = node;
			}

			return need_num;
		}
	}
};

/////////////////Entity Class/////////////////////////
class FireBall {
private:
	Texture FireBallTex;
	int damage = 20;
	int lifeNum;
	float x;
	float y;

	float plusX;
	float plusY;
	bool firstAttempt = true;

	float directionX;
	float directionY;



public:

	bool active;
	Sprite FireBallSpr;

	//Init
	FireBall(Image &image, float X, float Y, float directionX, float directionY) {
		this->x = X - Block;
		this->y = Y - Block;
		this->directionX = directionX - Block;
		this->directionY = directionY - Block;
		active = true;
		lifeNum = 0;

		FireBallTex.loadFromImage(image);
		FireBallSpr.setTexture(FireBallTex);


	}

	//Update
	void update(float time, list<Enemy*>& enemyList) {
		float distance;

		lifeNum++;
		if (lifeNum > 65) {
			active = false;
		}

		if (firstAttempt) {
			distance = sqrt((directionX - x)*(directionX - x) + (directionY - y)*(directionY - y));
			plusX = 0.4*time*(directionX - x) / distance;
			plusY = 0.4*time*(directionY - y) / distance;
			firstAttempt = false;
		}
		x += plusX;
		y += plusY;
		interactionWithMap(time);

		FireBallSpr.setPosition(x, y);

		attack(enemyList, plusX, plusY);
	}

	//Attack
	void attack(list<Enemy*>& enemyList, float xChange, float yChange)
	{
		list<Enemy*>::iterator it;
		int size = 32;
		for (it = enemyList.begin(); it != enemyList.end(); ++it) {
			Enemy* obj = (*it);
			int tempObjX = obj->eX - Block;
			int tempObjY = obj->eY - Block;
			if (tempObjX - 32 < x && tempObjX + 32 > x
				&& tempObjY - 32 < y && tempObjY + 32 > y) {
				obj->eX += xChange;
				obj->eY += yChange;
				obj->NotGetDamageState = false;
				obj->health -= damage;
				active = false;
			}
		}
	}

	//Collision
	void interactionWithMap(int time)
	{
		for (int i = y / Block; i < (y - 32) / Block; i++)
			for (int j = x / Block; j < (x - 32) / Block; j++)
			{
				if (TileMap[i][j] == '0')
				{
					active = false;
				}
				else { return; }
			}
	}
	~FireBall() {}
};


bool GameStart() {

	//Init Variables
	int tempX = 0;
	int tempY = 0;
	int BarsMin = 0;
	int menuNum = 0;
	int gameTime = 0;
	int sizeM = 0, Resolution = 0;
	int createObjectForMapTimer = 0;
	int massiveTile[HEIGHT_MAP][WIDTH_MAP];

	const int TSizeBig = 20;
	const int TSizeSmall = 15;
	const int TSizeGiant = 50;
	const int EnemyTimeToSpawn = 2000;
	const int BonusTimeToSpawn = 3000;

	float dX = 0;
	float dY = 0;
	float loadTime = 3;
	float distance = 0;
	float currentFrame = 0;
	float AttackCoolDown = 1;
	float timeToEndLoading = 0;
	float objectSpawnTimer = 0;

	bool Load = false;
	bool Pause = true;
	bool loading = true;
	bool SoundIsOn = true;

	SoundBuffer buffer;
	SoundBuffer bufferButton;
	SoundBuffer bufferAtmosphere;

	Sound sound;
	Sound soundButton;
	Sound soundAtmosphere;

	Font BarsFont, MenuFont;

	Text BarsH("", BarsFont, TSizeBig);
	Text BarsM("", BarsFont, TSizeBig);
	Text BarsS("", BarsFont, TSizeBig);
	Text Score("", BarsFont, TSizeSmall);
	Text Game("", BarsFont, TSizeGiant);
	Text Restart("", BarsFont, TSizeGiant);

	Image MenuImg;
	Image MapImg;
	Image GraveImg;
	Image BackGroundImg;
	Image fireballimage;

	Texture MapTex;
	Texture MenuBgT;
	Texture GraveTex;
	Texture BackGroundTex;

	Sprite MenuBgS;
	Sprite MapSprite;
	Sprite GraveSprite;
	Sprite BackGroundSpr;

	Clock clock;
	Clock gameTimeClock;

	list<FireBall*> firaBallList;
	list<Enemy*> enemyList;


	srand(time(NULL));

	//Window
	RenderWindow window(VideoMode(600, 600), "Game" , Style::Fullscreen);
	view.reset(FloatRect(0, 0, 640, 480));

	//Sound
	buffer.loadFromFile("Sound/fb.flac");
	bufferButton.loadFromFile("Sound/button.flac");
	bufferAtmosphere.loadFromFile("Sound/AtmoSphere.flac");

	sound.setBuffer(buffer);
	soundButton.setBuffer(bufferButton);
	soundAtmosphere.setBuffer(bufferAtmosphere);

	sound.setVolume(30);
	soundButton.setVolume(30);
	//FireBall
	fireballimage.loadFromFile("img/Fireball.png");
	fireballimage.createMaskFromColor(Color::White);

	//Font
	MenuFont.loadFromFile("fonts/GUI.otf");
	BarsFont.loadFromFile("fonts/CyrillicOldBold.ttf");

	//Map Elements
	MapImg.loadFromFile("img/map.png");
	GraveImg.loadFromFile("img/Grave.png");
	BackGroundImg.loadFromFile("img/BG.jpg");

	MapTex.loadFromImage(MapImg);
	GraveTex.loadFromImage(GraveImg);
	BackGroundTex.loadFromImage(BackGroundImg);

	MapSprite.setTexture(MapTex);
	GraveSprite.setTexture(GraveTex);
	BackGroundSpr.setTexture(BackGroundTex);

	// Creating Graph Map
	for (int i = 0; i < HEIGHT_MAP; i++) {
		for (int j = 0; j < WIDTH_MAP; j++) {
			if (TileMap[i][j] == ' ' || TileMap[i][j] == 'g' ||
				TileMap[i][j] == 'o' || TileMap[i][j] == 'w' || TileMap[i][j] == 's' ||
				TileMap[i][j] == 'h') {
				sizeM++;
				massiveTile[i][j] = sizeM;
			}
		}
	}

	//Player and Graph Init
	graph GraphMap(sizeM + 1 * 4);
	Player* p = new Player(205, 1100, 32, 32, GraphMap, 16, 16);

	for (int i = 0; i < HEIGHT_MAP; i++) {
		for (int j = 0; j < WIDTH_MAP; j++) {
			if (massiveTile[i][j] < 0) continue;

			if (massiveTile[i][j + 1] > 0) {
				GraphMap.add_edge(massiveTile[i][j], massiveTile[i][j + 1]);
			}
			if (massiveTile[i + 1][j] > 0) {
				GraphMap.add_edge(massiveTile[i][j], massiveTile[i + 1][j]);
			}
			if (massiveTile[i][j - 1] > 0) {
				GraphMap.add_edge(massiveTile[i][j], massiveTile[i][j - 1]);
			}
			if (massiveTile[i - 1][j] > 0) {
				GraphMap.add_edge(massiveTile[i][j], massiveTile[i - 1][j]);
			}
		}
	}

	//Window loop
	while (window.isOpen())
	{

		float time = clock.getElapsedTime().asMicroseconds();
		const float Load = 3000;

		Vector2i pixelPos = Mouse::getPosition(window);
		Vector2f pos = window.mapPixelToCoords(pixelPos);

		//Text init variables
		ostringstream playerHealthString;
		ostringstream playerManaString;
		ostringstream task;
		ostringstream score;

		///////Time
		if (p->life == true) { gameTime = gameTimeClock.getElapsedTime().asSeconds(); }
		if (p->life == false) {
			gameTime = 0;
			time = 0;
		}
		clock.restart();
		time = time / 800;

		////////Event Loop
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed) { window.close(); }
			if (Keyboard::isKeyPressed(Keyboard::Enter)) { return true; }
			if (event.type == Event::KeyPressed) {
				if ((event.key.code == Keyboard::Escape)) {
					switch (Pause) {
					case true: {
						Pause = false;
						break;
					}
					case false: {
						Pause = true;
						break;
					}
					}
				}
			}
		}

		//Bars inits
		playerHealthString << (int)p->Health;
		playerManaString << (int)p->Mana;
		score << (int)p->playerScore;
		//Bars Fill

		BarsH.setFillColor(Color::Red);
		BarsM.setFillColor(Color::Blue);
		Score.setFillColor(Color::White);
		//Bars Set

		Score.setString("Score :" + score.str());
		BarsH.setString("HP:" + playerHealthString.str());
		BarsM.setString("Mana:" + playerManaString.str());
		//Bars Set Postion

		BarsM.setPosition(view.getCenter().x - 300, view.getCenter().y - 220);
		BarsH.setPosition(view.getCenter().x - 300, view.getCenter().y - 240);
		Score.setPosition(view.getCenter().x + 200, view.getCenter().y - 240);

		//Hero Control
		if (p->life == true) { 
			if (Pause) {

				//Show Cursor
				window.setMouseCursorVisible(true);

				//Init Variables
				Image PlayImg;
				Image ExitImg;

				Texture PlayTex;
				Texture ExitTex;

				Sprite PlaySpr;
				Sprite ExitSpr;


				PlayImg.loadFromFile("img/Play.png");
				ExitImg.loadFromFile("img/Exit.png");
				MenuImg.loadFromFile("img/Menu2.jpg");

				PlayTex.loadFromImage(PlayImg);
				ExitTex.loadFromImage(ExitImg);
				MenuBgT.loadFromImage(MenuImg);

				PlaySpr.setTexture(PlayTex);
				ExitSpr.setTexture(ExitTex);
				MenuBgS.setTexture(MenuBgT);

				PlaySpr.setPosition(view.getCenter().x - 45, view.getCenter().y - 50);
				ExitSpr.setPosition(view.getCenter().x - 45, view.getCenter().y);
				MenuBgS.setPosition(view.getCenter().x - 350, view.getCenter().y - 250);


				if (IntRect(810, 400, 300, 120).contains(Mouse::getPosition(window))) {
					if (soundButton.getStatus() != 2 && menuNum != 1) {
						soundButton.play();
					}
					PlaySpr.setColor(Color::Blue);
					menuNum = 1;
				}
				if (IntRect(810, 550, 300, 100).contains(Mouse::getPosition(window)))
				{
					if (soundButton.getStatus() != 2 && menuNum != 2) {
						soundButton.play();
					}
					ExitSpr.setColor(Color::Blue);
					menuNum = 2;
				}

				if (Mouse::isButtonPressed(Mouse::Left))
				{
					if (menuNum == 1) { Pause = false; }
					if (menuNum == 2) { window.close(); }

				}

				window.draw(MenuBgS);
				window.draw(PlaySpr);
				window.draw(ExitSpr);

				window.display();
			}
			else if (loading) {

				Game.setFillColor(Color::Red);

				Game.setString("Loading. . . ");

				Game.setPosition(view.getCenter().x - 110, view.getCenter().y - 200);

				window.draw(BackGroundSpr);
				window.draw(Game);

				window.display();

				timeToEndLoading += time;
				if (timeToEndLoading > Load) {
					loading = false;
				}
			}
			else {

				window.setMouseCursorVisible(false);
				if (Keyboard::isKeyPressed(Keyboard::A)) {
					p->Direction = Left; p->Speed = 0.1;
				}
				if (Keyboard::isKeyPressed(Keyboard::D)) {
					p->Direction = Right; p->Speed = 0.1;
				}
				if (Keyboard::isKeyPressed(Keyboard::W)) {
					p->Direction = Up; p->Speed = 0.1;
				}
				if (Keyboard::isKeyPressed(Keyboard::S)) {
					p->Direction = Down; p->Speed = 0.1;
				}
				if (Mouse::isButtonPressed(Mouse::Left)) {
					if (p->CDAttack == p->Ready) {
						if (p->Mana - p->ManaCost >= BarsMin) {
							if (SoundIsOn) {
								sound.play();
							}
							p->Mana -= p->ManaCost;
							firaBallList.push_back(new FireBall(fireballimage, p->PlayerX, p->PlayerY, pos.x, pos.y));
							p->CDAttack = AttackCoolDown;
						}
					}

				}

				//Rotation Funct
				float dX = pos.x - p->PlayerX;
				float dY = pos.y - p->PlayerY;
				float rotation = (atan2(dY, dX)) * 180 / 3.14159265;
				p->PlayerSpr.setRotation(rotation);
				p->CrossHairSpr.setPosition(pos.x, pos.y);

				///////////////////View
				getplayercoordinateforview(p->getplayercoordinateX(), p->getplayercoordinateY());

				//Player&Window funct
				p->update(time);

				//Random Gen
				createObjectForMapTimer += time;//наращиваем таймер
				if (createObjectForMapTimer > EnemyTimeToSpawn && enemyList.size() < 12) {
					point randomGraph;
					if (p->getPlayerGraph().num > 100) {
						randomGraph = searchGraphCoords(p->getPlayerGraph().num - (rand() % 10 + 3));

					}
					else {
						randomGraph = searchGraphCoords(p->getPlayerGraph().num + (rand() % 10 + 3));

					}

					enemyList.push_back(new Enemy(randomGraph.x, randomGraph.y, 32, 32, GraphMap, p));
					createObjectForMapTimer = 0;
				}

				objectSpawnTimer += time;
				if (objectSpawnTimer > BonusTimeToSpawn) {
					randomMapGenerate();
					objectSpawnTimer = 0;
				}


				// Отрисовывааем обьекты

				// Отрисовка enemy
				point player_graph = p->getPlayerGraph();
				if (enemyList.size())
					for (auto it = enemyList.begin(); it != enemyList.end(); it++) {
						if ((*it)->life) {
							(*it)->getArgs(time, player_graph, p->PlayerX, p->PlayerY);
							std::thread(&Enemy::update, (*it)).detach();
							window.draw((*it)->EnemySpr);
						}
						else
						{
							delete *it;
							enemyList.erase(it);
							break;
						}
					}

				// Отрисовка фаерболлов
				if (firaBallList.size()) // Если на карте имеются фаерболлы
					for (auto it = firaBallList.begin(); it != firaBallList.end(); it++) {
						if ((*it)->active) {
							(*it)->update(time, enemyList);
							window.draw((*it)->FireBallSpr); // Отрисовываем/update все спрайты
						}
						else {
							delete *it;
							firaBallList.erase(it);
							break; // Если не активно - удаляем
						}
					}

				window.draw(Score);
				window.draw(BarsS);
				window.draw(BarsH);
				window.draw(BarsM);
				window.draw(p->PlayerSpr);
				window.draw(p->CrossHairSpr);
				window.display();
				window.clear();
			}
		}

		// Включаем саунд
		if (soundAtmosphere.getStatus() != 2) {
			soundAtmosphere.setVolume(30);
			soundAtmosphere.play();
		}

		window.setView(view);
		window.clear();
		//Map objects
		for (int i = 0; i < HEIGHT_MAP; i++)
			for (int j = 0; j < WIDTH_MAP; j++)
			{
				if (TileMap[i][j] == ' ')  MapSprite.setTextureRect(IntRect(0, 0, Block, Block));
				if (TileMap[i][j] == 's')  MapSprite.setTextureRect(IntRect(32, 0, Block, Block));
				if ((TileMap[i][j] == 'w')) MapSprite.setTextureRect(IntRect(64, 0, Block, Block));
				if ((TileMap[i][j] == 'f')) MapSprite.setTextureRect(IntRect(96, 0, Block, Block));
				if ((TileMap[i][j] == 'h')) MapSprite.setTextureRect(IntRect(128, 0, Block, Block));
				if ((TileMap[i][j] == 'o'))MapSprite.setTextureRect(IntRect(160, 0, Block, Block));
				if ((TileMap[i][j] == 'g'))MapSprite.setTextureRect(IntRect(192, 0, Block, Block));
				if ((TileMap[i][j] == '0')) MapSprite.setTextureRect(IntRect(224, 0, Block, Block));
				if ((TileMap[i][j] == '1')) MapSprite.setTextureRect(IntRect(256, 0, Block, Block));
				MapSprite.setPosition(j * Block , i * Block);

				window.draw(MapSprite);
			}


		if (!p->life) {
			Restart.setFillColor(Color::Red);
			Game.setFillColor(Color::Red);
			Score.setFillColor(Color::Black);

			Game.setString("Game Over");
			Restart.setString("Press Enter to Restart");
			Score.setString("Yours Score :\n\t\t\t" + score.str());

			Score.setPosition(view.getCenter().x - 75, view.getCenter().y + 100);
			Game.setPosition(view.getCenter().x - 110, view.getCenter().y - 200);
			Restart.setPosition(view.getCenter().x - 250, view.getCenter().y - 150);
			GraveSprite.setPosition(view.getCenter().x - 100, view.getCenter().y - 50);
			BackGroundSpr.setPosition(view.getCenter().x - 400, view.getCenter().y - 300);

			window.draw(BackGroundSpr);
			window.draw(GraveSprite);
			window.draw(Game);
			window.draw(Restart);
			window.draw(Score);

			window.display();
		}
	}
}
void GameRunning() {
	if (GameStart()) { GameRunning(); }
}

int main()
{
	GameRunning();
	return 0;
}
