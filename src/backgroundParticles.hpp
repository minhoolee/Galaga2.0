#ifndef BACKGROUND_PARTICLES_HPP
#define BACKGROUND_PARTICLES_HPP

#include <SFML/Graphics.hpp>

class BackgroundParticles
{
public:
	// Game properties
	static const int GAME_WIDTH = 1600;
	static const int GAME_HEIGHT = 1000;

	sf::CircleShape backgroundParticles[30];

	void animateParticlesIdle();
	void animateParticlesMovement();
};

#endif // BACKGROUND_PARTICLES_HPP