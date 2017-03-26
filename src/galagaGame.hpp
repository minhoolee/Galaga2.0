#ifndef GALAGA_GAME_HPP
#define GALAGA_GAME_HPP

#include <SFML/Graphics.hpp>
#include <SFML/System/String.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <thread>

#include "gameObject.hpp"
#include "player.hpp"
#include "enemy.hpp"
#include "backgroundParticles.hpp"

#define MULTI_THREADING 1

class GalagaGame;

class GalagaGame
{
public:
	sf::RenderWindow window;

	sf::Clock clock;
	sf::Clock animateParticlesTimer;
	sf::Clock enemySpawnTimer;
	
	// Game window properties
	static const int GAME_WIDTH = 1600;
	static const int GAME_HEIGHT = 1000;
	static const int NUM_THREADS = 5;
  const int MAX_NUM_ENEMIES = 50;

	int SINGLE_PLAYER = 1;
	int MULTI_PLAYER = 0;

	int enemyNum = 0;
	int enemiesLeft = 0;

	// Two settings, SINGLE_PLAYER and MULTI_PLAYER
	Player player1;
	Player player2;
	Enemy enemies[100];

	BackgroundParticles particle;
	BackgroundParticles particle2;

	int currentBullet = 0;
	PlayerBullet playerBullets[50];

	int currentEnemyBullet = 0;
	EnemyBullet enemyBullets[50];

	// Background texture
	sf::Sprite backgroundSprite;
	sf::Texture backgroundTexture;

	sf::CircleShape backgroundParticles[30];

	// Main texture to pull other textures off of
	sf::Texture mainTexture;

	sf::Font font;

	sf::Text p1ScoreText;
	sf::Text p2ScoreText;
	sf::Text helpText;

	unsigned long long int player1Score = 0;
	unsigned long long int player2Score = 0;

	GalagaGame();

	void run();
	void update(sf::Time time);
	void processEvents();
	void render();

	void spawnEnemies(int count);
};

#endif // GALAGA_GAME_HPP
