#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "gameObject.hpp"
#include "playerBullet.hpp"

class Player : public GameObject
{
public:
	// Rectangle for taking texture off of main texture
	sf::IntRect playerTextureRect;

	// Player 1 controls : ARROW KEYS
	bool isPlayerMovingUp = false;
	bool isPlayerMovingDown = false;
	bool isPlayerMovingLeft = false;
	bool isPlayerMovingRight = false;
	bool isPlayerFiring = false;

	static const int PLAYER_SPEED = 400;
	static const int BULLET_SPEED = 1500;

	sf::Clock shotTimer;

	Player();
	Player(sf::Vector2f position, GalagaGame *game);

	void update(sf::Time t);
	void fire();
};

#endif // PLAYER_HPP
