#include "backgroundParticles.hpp"
#include "galagaGame.hpp"

void BackgroundParticles::animateParticlesIdle()
{
	for (int particle_num = 0; particle_num < 30; particle_num++)
	{
		float random_x = 0.0f;
		float random_y = 0.0f;

		// Set random positions that are not within a border of 20 from the left/right and 10 from the top/bottom
		random_x = (float)(rand() % (GAME_WIDTH - 49)) + 20;
		random_y = (float)(rand() % (GAME_HEIGHT - 49)) + 10;

		sf::Vector2f coord(random_x, random_y);

		backgroundParticles[particle_num].setPosition(coord);

		int rand_r = rand() % 257;
		int rand_g = rand() % 257;
		int rand_b = rand() % 257;

		backgroundParticles[particle_num].setFillColor(sf::Color(rand_r, rand_g, rand_b));

		float random_radius = 0.0f;

		// Some particles are bigger than others in order to have foreground and backgrounds
		random_radius = (rand() % 3) + 1;

		backgroundParticles[particle_num].setRadius(random_radius);
	}
}

void BackgroundParticles::animateParticlesMovement()
{
	std::mutex animate_particles_movement_mutex;
	animate_particles_movement_mutex.lock();

	for (int particle_num = 0; particle_num < 30; particle_num++)
	{
		int offset = 0;
		
		if (backgroundParticles[particle_num].getRadius() == 1)
			offset = 1;
		else if (backgroundParticles[particle_num].getRadius() == 2)
			offset = 2;
		else if (backgroundParticles[particle_num].getRadius() == 3)
			offset = 3;
		
		backgroundParticles[particle_num].setPosition ( backgroundParticles[particle_num].getPosition().x, backgroundParticles[particle_num].getPosition().y + offset);
	}

	animate_particles_movement_mutex.unlock();
}