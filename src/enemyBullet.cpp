#include "enemyBullet.hpp"
#include "galagaGame.hpp"

EnemyBullet::EnemyBullet() : GameObject()
{
	enemyBulletTextureRect = sf::IntRect(364, 216, 5, 10);
	sprite.setTextureRect(enemyBulletTextureRect);
	sprite.setOrigin(2.5, 4);
	sprite.setScale(3, 3);
}

void EnemyBullet::update(sf::Time timeElapsed)
{
	sprite.move(velocity * timeElapsed.asSeconds());

	if (sprite.getPosition().y > GAME_HEIGHT)
	{
		enabled = false;
	}

	if (game->player1.enabled && isColliding(&game->player1))
	{
		//enabled = false;
		//game->player1.enabled = false;
		game->player1.sprite.setPosition((GAME_WIDTH / 2), ((GAME_HEIGHT * 7) / 8));
	}

	if (game->MULTI_PLAYER)
	{
		if (game->player2.enabled && isColliding(&game->player2))
		{
			//enabled = false;
			//game->player2.enabled = false;
			game->player2.sprite.setPosition((GAME_WIDTH / 2), ((GAME_HEIGHT * 7) / 8));
		}
	}
}
