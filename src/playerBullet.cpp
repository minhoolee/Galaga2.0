#include "playerBullet.hpp"
#include "galagaGame.hpp"
#include <iostream>

PlayerBullet::PlayerBullet() : GameObject() 
{
	playerBulletTextureRect = sf::IntRect(373, 50, 5, 10);
	sprite.setTextureRect(playerBulletTextureRect);
	sprite.setScale(3, 3);
}

void PlayerBullet::update(sf::Time timeElapsed)
{
	sprite.setTexture(game->mainTexture);

	// Make sprite move upwards (negative)
	sprite.move(-velocity * timeElapsed.asSeconds());

	if (sprite.getPosition().y < 0)
	{
		enabled = false;
	}
	
	for (Enemy &e : game->enemies)
	{
		if (isColliding(&e))
		{
			enabled = false;
			e.enabled = false;
			e.point_num = 0;
			game->player1Score+=100;
			game->p1ScoreText.setString(std::to_string(game->player1Score));
		}
	}
}
