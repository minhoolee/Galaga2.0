#ifndef ENEMY_BULLET_HPP
#define ENEMY_BULLET_HPP

#include "gameObject.hpp"

class EnemyBullet : public GameObject
{
public:
	bool enabled = false;

	sf::Vector2f velocity;

	// Rectangle for taking texture off of main texture
	sf::IntRect enemyBulletTextureRect;

	EnemyBullet();

	void update(sf::Time time);
};

#endif // ENEMY_BULLET_HPP
