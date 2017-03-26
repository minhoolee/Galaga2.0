#include "enemy.hpp"
#include "galagaGame.hpp"

Enemy::Enemy() : GameObject() {}

Enemy::Enemy(sf::Vector2f position, GalagaGame *game) : GameObject(position, game)
{
	sprite.setTexture(game->mainTexture);
	enemyTextureRect = sf::IntRect(18, 152, 13, 15);
	sprite.setTextureRect(enemyTextureRect);
	sprite.setScale(3, 3);
	sprite.setOrigin(6.5, 7.5);
	sprite.setRotation(90); // Image starts pointed left, so rotate it 90 degrees clockwise
}

void Enemy::update(sf::Time timeElapsed)
{
	sf::Vector2f velocity(0, 0);

	// Cause the enemies to go back to the top of the screen when hitting the bottom
	if (this->sprite.getPosition().y > GAME_HEIGHT + 200)
	{
		this->sprite.setPosition(sprite.getPosition().x, -200);
		this->hasFired = false;
	}

	// Only execute once
	if (order < 4)
	{
		if (moveTimer.getElapsedTime().asSeconds() > 0.005f)
		{
			moveTimer.restart();

			switch (order)
			{
			case (0) :
				amplitude = 200;
				period = 1.0f;

				if (point_num == 0)
				{
					center_x = this->sprite.getPosition().x;
					center_y = this->sprite.getPosition().y;
				}

				sprite.setPosition(point_x, point_y);

				if (point_num <= 90)
				{
					moveSequence(timeElapsed, velocity, point_num, SINUSOID, 90, -2 * 360);
					break;
				}

				order++;
				point_num = 0;
				theta = 90;

			case (1) :
				semi_axis_x = 350;
				semi_axis_y = 200;

				// Use sprite's current position to start sequence (only for starting point)
				if (point_num == 0)
				{
					center_x = this->sprite.getPosition().x - (semi_axis_x * std::cos(theta * (PI / 180)));
					center_y = this->sprite.getPosition().y - (semi_axis_y * std::sin(theta * (PI / 180)));
				}

				if (point_num <= 30)
				{
					//moveSequence(this->shape2, timeElapsed, velocity, point_num, ELLIPSE, 30, 90);
					moveSequence(timeElapsed, velocity, point_num, ELLIPSE, 30, 90);
					sprite.setPosition(point_x, point_y);
					//moveSequence(timeElapsed, velocity, point_num, ELLIPSE, 15, 90);
					break;
				}

				// Move onto next sequence
				order++;
				point_num = 0; // Reset counter
				theta = 180; // New initial theta
				center_x -= (semi_axis_x - radius); // Move shape

			case (2) :
				radius = 200;

				point_x = center_x + (radius * std::cos(theta * (PI / 180)));
				point_y = center_y + (radius * std::sin(theta * (PI / 180)));

				sprite.setPosition(point_x, point_y);

				if (point_num <= 60)
				{
					//moveSequence(this->shape3, timeElapsed, velocity, point_num, CIRCLE, 60, 270);
					moveSequence(timeElapsed, velocity, point_num, CIRCLE, 60, 270);
					break;
				}

				// Move onto next sequence
				order++;
				point_num = 0;
				theta = 90;
				center_y -= (semi_axis_x - radius);

			case (3) :
				semi_axis_x = 200;
				semi_axis_y = 350;

				point_x = center_x + (semi_axis_x * std::cos(theta * (PI / 180)));
				point_y = center_y + (semi_axis_y * std::sin(theta * (PI / 180)));

				sprite.setPosition(point_x, point_y);

				if (point_num <= 30)
				{
					//moveSequence(this->shape4, timeElapsed, velocity, point_num, ELLIPSE, 30, 90);
					moveSequence(timeElapsed, velocity, point_num, ELLIPSE, 30, 90);
					break;
				}

				// Reset values
				order++;
				point_num = 0;
				theta = 0;

				// Go from position into grid formation
				//moveToFormation(timeElapsed);
			}
		}
	}

	// Execute other animations when not doing movement sequence
	else
	{
		// Break position and attack player after a certain amount of time
		if (moveTimer.getElapsedTime().asSeconds() > 5.0f)
		{
			attackPlayer(timeElapsed);
		}

		else
		{
			if (idleAnimateTimer.getElapsedTime().asSeconds() > 0.5f)
			{
				idleAnimateTimer.restart();
				hasWingsOpen = !hasWingsOpen;
				idleAnimation();
			}

			// Execute idle movement pattern
			if (idleMovementTimer.getElapsedTime().asSeconds() > 0.7f)
			{
				idleMovementTimer.restart();
				isEnemyMovingRight = !isEnemyMovingRight;
			}

			if (isEnemyMovingRight)
				velocity.x = ENEMY_SPEED / 2;

			else if (!isEnemyMovingRight)
				velocity.x = -ENEMY_SPEED / 2;

			sprite.move(velocity * timeElapsed.asSeconds());
		}
	}
}

// FAILED ATTEMPT TO MOVE THEM INTO A GRID FORMATION
void Enemy::moveToFormation(sf::Time &timeElapsed)
{
	sf::Clock movementTimer;

	for (int i = 0; i < 20; i++)
	{
		if (movementTimer.getElapsedTime().asSeconds() >= 0.01f)
		{
			movementTimer.restart();

			this->seg_x = (float)(this->destination_x - this->sprite.getPosition().x) / 20;
			this->seg_y = (float)(this->destination_y - this->sprite.getPosition().y) / 20;

			sprite.move(sf::Vector2f(seg_x, seg_y));
		}
	}

	order++;
}

void Enemy::idleAnimation()
{
	if (hasWingsOpen)
	{
		// Texture is facing left
		enemyTextureRect = sf::IntRect(18, 152, 13, 15);
		sprite.setTextureRect(enemyTextureRect);
		sprite.setScale(3, 3);
		sprite.setOrigin(6.5, 7.5);
		sprite.setRotation(90);
	}

	else
	{
		// Texture is facing upwards
		enemyTextureRect2 = sf::IntRect(186, 153, 13, 12);
		sprite.setTextureRect(enemyTextureRect2);
		sprite.setRotation(0);
		sprite.setScale(3, 3);
	}
}

void Enemy::attackPlayer(sf::Time &timeElapsed)
{
	sf::Vector2f velocity(0, 0);
	double degrees = 0;
	
	// Rotate sprite so that it faces downwards
	// degrees = 270;
	// sprite.setRotation(degrees);

	// Rotate sprite so that it faces downwards (255 <= degrees <= 285; 30 degree arc)
	// degrees = rand() % 31 + 255;
	// sprite.setRotation(degrees);

	float enemy_x = rand() % (ENEMY_SPEED) - ENEMY_SPEED / 2; 
	float enemy_y = sqrt (pow(ENEMY_SPEED, 2) - pow(enemy_x, 2)); // Use pythagorean theorem to calculate the y distance the enmey must travel

	velocity.x = enemy_x;
	velocity.y = enemy_y;

  degrees = atan(velocity.y / velocity.x);
	sprite.setRotation(degrees + 270);

	sprite.move(velocity * timeElapsed.asSeconds());
	
	float maxFiringPos = 0.0f;
	maxFiringPos = rand() % (GAME_HEIGHT / 4);

	if (sprite.getPosition().y > (GAME_HEIGHT / 4))
	{
		if (!hasFired)
		{
			fire();
			hasFired = true;
		}
	}
}

void Enemy::moveSequence(sf::Time timeElapsed, sf::Vector2f &velocity, int point_num, int select_mode, int point_total, double degrees_total)
{
	// Range of theta (degrees) is 0 <= theta < 360
	if (theta >= 360)
		theta = theta - 360;

	sprite.setRotation(theta + 270);

	// Select flight path
	if (select_mode == CIRCLE)
	{
		point_x = center_x + (radius * std::cos (theta * (PI / 180)));
		point_y = center_y + (radius * std::sin (theta * (PI / 180)));
	}

	else if (select_mode == ELLIPSE)
	{
		point_x = center_x + (semi_axis_x * std::cos (theta * (PI / 180)));
		point_y = center_y + (semi_axis_y * std::sin (theta * (PI / 180)));
	}

	else if (select_mode == SINUSOID)
	{
		point_x = center_x + theta;
		point_y = center_y + (amplitude * (std::sin ( period * (theta * (PI / 180)) )));
	}

	velocity.x = point_x;
	velocity.y = point_y;

	//shape.setPoint(point_num, sf::Vector2f(point_x, point_y));

	this->point_num++;

	this->theta += ( degrees_total / point_total ); // Increment by number of degrees necessary to create a total number of degrees
}

void Enemy::fire()
{
	// When enemy fires, make it turn to the bottom and move off the screen
  // Fire a random number (up until a maximum) of bullets at random positions
	for (int bullet_num = 0; bullet_num < rand() * MAX_NUM_BULLETS + 1; bullet_num++)
	{
		game->enemyBullets[game->currentEnemyBullet].sprite.setPosition(sf::Vector2f((this->sprite.getPosition().x), (this->sprite.getPosition().y) + 10.5 + 10));
		game->enemyBullets[game->currentEnemyBullet].enabled = true;

		float enemy_x = rand() % (BULLET_SPEED / 6) - ((BULLET_SPEED / 12) + 1); // Bullets can fly in any direction of a certain range
		float enemy_y = sqrt( pow(BULLET_SPEED, 2) - pow(enemy_x, 2) ); // Use pythagorean theorem to calculate the y distance the bullet must travel

		game->enemyBullets[game->currentEnemyBullet].velocity = sf::Vector2f(enemy_x, enemy_y);

		// If bullet not set, set a couple values on it
		if (game->enemyBullets[game->currentEnemyBullet].game == NULL)
		{
			game->enemyBullets[game->currentEnemyBullet].game = game;
			game->enemyBullets[game->currentEnemyBullet].sprite.setTexture(game->mainTexture); // 0th index because all bullet textures are same
		}

		game->currentEnemyBullet++;

		if (game->currentEnemyBullet == 50)
		{
			game->currentEnemyBullet = 0;
		}
	}
}
