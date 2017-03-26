#ifndef PLAYER_BULLET_HPP
#define PLAYER_BULLET_HPP

#include "gameObject.hpp"
#include "enemy.hpp"

class PlayerBullet : public GameObject
{
public:
	bool enabled = false;
	sf::Vector2f velocity;
	
	// Rectangle for taking texture off of main texture
	sf::IntRect playerBulletTextureRect;

	PlayerBullet();

	void update(sf::Time time);
};

#endif // PLAYER_BULLET_HPP