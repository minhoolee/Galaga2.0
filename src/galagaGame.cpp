#include "galagaGame.hpp"
#include <iostream>

GalagaGame::GalagaGame() : window(sf::VideoMode(GAME_WIDTH, GAME_HEIGHT), "Galaga 2.0")
{
	window.setFramerateLimit(60);

	// TODO: TAKE OFF BACKGROUND? DEFAULT = BLACK
	backgroundTexture.loadFromFile("Textures/black_background.png");
	backgroundSprite.setTexture(backgroundTexture);
	backgroundSprite.setScale(10000, 10000);

	// Main texture to pull others off of
	mainTexture.loadFromFile("Textures/mainTexture.png");

	player1 = Player(sf::Vector2f((GAME_WIDTH / 2), (GAME_HEIGHT * 7 / 8)), this);
	player2 = Player(sf::Vector2f((GAME_WIDTH / 2), (GAME_HEIGHT * 7 / 8)), this);

	font.loadFromFile("Fonts/galaga_fonts.ttf");

	p1ScoreText.setPosition(50, 30);
	p1ScoreText.setFont(font);
	p1ScoreText.setColor(sf::Color::White);
	p1ScoreText.setCharacterSize(24);

	helpText.setPosition(sf::Vector2f(GAME_WIDTH/2.0f, 30));
	helpText.setFont(font);
	helpText.setColor(sf::Color::White);
	helpText.setString("          Galaga 2.0\
      \n Written by Min Hoo Lee (2016)\
      \n\nPress \"v\" for Multiplayer Mode!\
      \n  Press \"q\" to quit the game!\
      \n\n Player WHITE: Arrows + SPACE");
	helpText.setCharacterSize(12);

  sf::FloatRect textRect = helpText.getLocalBounds();
	helpText.setOrigin(textRect.left + textRect.width/2.0f,
    textRect.top);
}

void GalagaGame::run()
{
	srand( (unsigned int) time(NULL) );

	sf::Time timeSinceLastUpdate = sf::Time::Zero;

	// Start some game elements on startup
#if MULTI_THREADING
	std::thread spawn_enemy_thread(&GalagaGame::spawnEnemies, this, MAX_NUM_ENEMIES);
	spawn_enemy_thread.detach();
#else
	spawnEnemies(MAX_NUM_ENEMIES);
#endif

	particle.animateParticlesIdle();
	particle2.animateParticlesIdle();

  int i = 0;
	while (window.isOpen())
	{
    if (i == 1)
      std::this_thread::sleep_for(std::chrono::milliseconds(10000));
    i++;
    p1ScoreText.setString(std::to_string(player1Score));

    if (MULTI_PLAYER)
    {
      p2ScoreText.setString(std::to_string(player2Score));
      helpText.setString("          Galaga 2.0\
          \n Written by Min Hoo Lee (2016)\
          \n\nPress \"v\" for Singleplayer Mode!\
          \n  Press \"q\" to quit the game!\
          \n\n       MULTI PLAYER MODE\
          \nPlayer WHITE: Arrows + RControl\
          \nPlayer YELLOW:  WASD + LControl");
    }
    else
    {
      helpText.setString("          Galaga 2.0\
        \n Written by Min Hoo Lee (2016)\
        \n\nPress \"v\" for Multiplayer Mode!\
        \n  Press \"q\" to quit the game!\
        \n\n       SINGLE PLAYER MODE\
        \n Player WHITE: Arrows + SPACE");
    }

		processEvents();
		timeSinceLastUpdate = clock.restart();
		update(timeSinceLastUpdate);

		render(); // Draw all the game objects, textures, and fonts
	}
}

void GalagaGame::update(sf::Time time)
{
	//float time_f = time.asSeconds();
	//void *thread_arg = &time_f;

	if (player1.enabled)
	{
#if MULTI_THREADING
		std::thread player1_thread (&Player::update, &player1, time);
		player1_thread.detach(); // Allow thread to execute separately from the main thread
#else
		player1.update(time);
#endif
	}

	if (MULTI_PLAYER)
	{
		if (player2.enabled)
		{

#if MULTI_THREADING
			std::thread player2_thread (&Player::update, &player2, time);
			player2_thread.detach();
#else
			player2.update(time);
#endif
		}
	}

	for (PlayerBullet &b : this->playerBullets)
	{
		if (b.enabled)
		{
#if MULTI_THREADING
			std::thread player_bullet_thread(&PlayerBullet::update, &b, time);
			player_bullet_thread.detach();
#else
			b.update(time);
#endif
		}
	}

	for (EnemyBullet &b : enemyBullets)
	{
		if (b.enabled)
		{
#if MULTI_THREADING
			std::thread enemy_bullet_thread(&EnemyBullet::update, &b, time);
			enemy_bullet_thread.detach();
#else
			b.update(time);
#endif
		}
	}

	enemiesLeft = 0;
	for (Enemy &e : enemies)
	{
		if (e.enabled)
		{
#if MULTI_THREADING
			std::thread enemy_thread(&Enemy::update, &e, time);
			enemy_thread.detach();
#else
			e.update(time);
#endif
			enemiesLeft++;
		}
	}

	if (enemiesLeft == 0)
	{
		enemyNum = 0;
#if MULTI_THREADING
		std::thread spawn_enemy_thread(&GalagaGame::spawnEnemies, this, MAX_NUM_ENEMIES);
		spawn_enemy_thread.detach();
#else
		spawnEnemies(MAX_NUM_ENEMIES);
#endif
	}
	
	if (enemySpawnTimer.getElapsedTime().asSeconds() > 0.05f)
	{
		if (enemyNum < enemiesLeft)
		{
			if (enemyNum < (enemiesLeft / 2))
			{
				enemies[enemyNum] = Enemy(sf::Vector2f(GAME_WIDTH + 100, (GAME_HEIGHT / 2)), this);
				enemies[enemyNum].destination_x += (100.0f * enemyNum); // Set the enemies in a grid like formation
				enemies[enemyNum].enabled = true;
				enemies[enemyNum].update(time);
				enemyNum++;
			}
		}
	}

	if (enemySpawnTimer.getElapsedTime().asSeconds() > 0.1f)
	{
		enemySpawnTimer.restart();

		if (enemyNum < enemiesLeft)
		{
			if (enemyNum >= (enemiesLeft / 2))
			{
				enemies[enemyNum] = Enemy(sf::Vector2f(GAME_WIDTH + 100, (GAME_HEIGHT / 2) + 100), this);
				enemies[enemyNum].destination_x += (100.0f * (enemyNum - (enemiesLeft / 2))); // Next row of enemies
				enemies[enemyNum].enabled = true;
				enemies[enemyNum].update(time);
				enemyNum++;
			}
		}
	}

	if (animateParticlesTimer.getElapsedTime().asSeconds() > 3.0f)
	{
		animateParticlesTimer.restart();

		particle.animateParticlesIdle();
		particle2.animateParticlesIdle();
	}


#if MULTI_THREADING
	std::thread particle_thread(&BackgroundParticles::animateParticlesMovement, &particle);
	std::thread particle2_thread(&BackgroundParticles::animateParticlesMovement, &particle2);

	particle_thread.detach();
	particle2_thread.detach();
#else
	particle.animateParticlesMovement();
	particle2.animateParticlesMovement();
#endif
}

void GalagaGame::processEvents()
{
	sf::Event event;

	while (window.pollEvent(event))
	{
		// Click 'x' to exit
		if (event.type == sf::Event::Closed)
		{
			window.close();
		}

		if (event.type == sf::Event::KeyPressed)
		{
			// Press 'V' to switch modes (from Singleplayer -> Multiplayer and vice versa)
			if (event.key.code == sf::Keyboard::V)
			{
				SINGLE_PLAYER = !SINGLE_PLAYER;
				MULTI_PLAYER = !MULTI_PLAYER;
			}
			// Press 'Q' to quit
			if (event.key.code == sf::Keyboard::Q)
			{
				window.close();
			}
		}

		// Control player one and player two
		if (MULTI_PLAYER)
		{
			if (event.type == sf::Event::KeyPressed)
			{
				// First player
				if (event.key.code == sf::Keyboard::Up)
				{
					player1.isPlayerMovingUp = true;
				}
				if (event.key.code == sf::Keyboard::Down)
				{
					player1.isPlayerMovingDown = true;
				}
				if (event.key.code == sf::Keyboard::Left)
				{
					player1.isPlayerMovingLeft = true;
				}
				if (event.key.code == sf::Keyboard::Right)
				{
					player1.isPlayerMovingRight = true;
				}
				if (event.key.code == sf::Keyboard::RControl)
				{
					player1.isPlayerFiring = true;
				}
				
				// Second player
				if (event.key.code == sf::Keyboard::W)
				{
					player2.isPlayerMovingUp = true;
				}
				if (event.key.code == sf::Keyboard::S)
				{
					player2.isPlayerMovingDown = true;
				}
				if (event.key.code == sf::Keyboard::A)
				{
					player2.isPlayerMovingLeft = true;
				}
				if (event.key.code == sf::Keyboard::D)
				{
					player2.isPlayerMovingRight = true;
				}
				if (event.key.code == sf::Keyboard::LControl)
				{
					player2.isPlayerFiring = true;
				}
			}

			if (event.type == sf::Event::KeyReleased)
			{
				// First player
				if (event.key.code == sf::Keyboard::Up)
				{
					player1.isPlayerMovingUp = false;
				}
				if (event.key.code == sf::Keyboard::Down)
				{
					player1.isPlayerMovingDown = false;
				}
				if (event.key.code == sf::Keyboard::Left)
				{
					player1.isPlayerMovingLeft = false;
				}
				if (event.key.code == sf::Keyboard::Right)
				{
					player1.isPlayerMovingRight = false;
				}
				if (event.key.code == sf::Keyboard::RControl)
				{
					player1.isPlayerFiring = false;
				}

				// Second player
				if (event.key.code == sf::Keyboard::W)
				{
					player2.isPlayerMovingUp = false;
				}
				if (event.key.code == sf::Keyboard::S)
				{
					player2.isPlayerMovingDown = false;
				}
				if (event.key.code == sf::Keyboard::A)
				{
					player2.isPlayerMovingLeft = false;
				}
				if (event.key.code == sf::Keyboard::D)
				{
					player2.isPlayerMovingRight = false;
				}
				if (event.key.code == sf::Keyboard::Space)
				{
					player2.isPlayerFiring = false;
				}
			}
		}

		// Single player mode
		else
		{
			// Control player one
			if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Up)
				{
					player1.isPlayerMovingUp = true;
				}
				if (event.key.code == sf::Keyboard::Down)
				{
					player1.isPlayerMovingDown = true;
				}
				if (event.key.code == sf::Keyboard::Left)
				{
					player1.isPlayerMovingLeft = true;
				}
				if (event.key.code == sf::Keyboard::Right)
				{
					player1.isPlayerMovingRight = true;
				}
				if (event.key.code == sf::Keyboard::Space)
				{
					player1.isPlayerFiring = true;
				}
			}

			if (event.type == sf::Event::KeyReleased)
			{
				if (event.key.code == sf::Keyboard::Up)
				{
					player1.isPlayerMovingUp = false;
				}
				if (event.key.code == sf::Keyboard::Down)
				{
					player1.isPlayerMovingDown = false;
				}
				if (event.key.code == sf::Keyboard::Left)
				{
					player1.isPlayerMovingLeft = false;
				}
				if (event.key.code == sf::Keyboard::Right)
				{
					player1.isPlayerMovingRight = false;
				}
				if (event.key.code == sf::Keyboard::Space)
				{
					player1.isPlayerFiring = false;
				}
			}
		}
	}
}

void GalagaGame::render()
{
	window.clear();
	window.draw(backgroundSprite);

	if (player1.enabled)
		window.draw(player1.sprite);

	if (MULTI_PLAYER)
		if (player2.enabled)
			window.draw(player2.sprite);

	for (PlayerBullet &b : playerBullets)
	{
		if (b.enabled)
		{
			window.draw(b.sprite);
		}
	}

	// Draw all bullets for existing enemies
	for (int i = 0; i < MAX_NUM_ENEMIES; i++)
	{
		for (EnemyBullet &b : enemyBullets)
		{
			if (b.enabled)
			{
				window.draw(b.sprite);
			}
		}
	}

	for (Enemy &e : enemies)
	{
		if (e.enabled)
		{
			window.draw(e.sprite);
		}
	}

	window.draw(p1ScoreText);

	if (MULTI_PLAYER)
		window.draw(p2ScoreText);

	for (int i = 0; i < 30; i++)
	{
		window.draw(particle.backgroundParticles[i]);
		window.draw(particle2.backgroundParticles[i]);
	}

	window.draw(helpText);
	window.display();
}

void GalagaGame::spawnEnemies(int count)
{
	for (int enemyNum = 0; enemyNum < count; enemyNum++)
	{
		// First half
		if (enemyNum < (count / 2))
		{
			enemies[enemyNum] = Enemy(sf::Vector2f( GAME_WIDTH + 100, (GAME_HEIGHT / 2) ), this);
			enemies[enemyNum].enabled = true;
			enemiesLeft++;
		}
	}

	for (int enemyNum = 0; enemyNum < count; enemyNum++)
	{
		if (enemyNum >= (count / 2))
		{
			enemies[enemyNum] = Enemy(sf::Vector2f(GAME_WIDTH + 100, (GAME_HEIGHT / 2) + 100), this);
			enemies[enemyNum].enabled = true;
			enemiesLeft++;
		}
	}
}
