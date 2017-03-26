#include "gameObject.hpp"

GameObject::GameObject() : GameObject::GameObject(sf::Vector2f(0, 0), NULL) {}

GameObject::GameObject(sf::Vector2f position, GalagaGame *game)
{
	this->game = game;
	this->sprite.setPosition(position);
}

void GameObject::update(sf::Time timeElapsed) {}

bool GameObject::isColliding(GameObject* obj)
{
	return (obj->enabled && sprite.getGlobalBounds().intersects(obj->sprite.getGlobalBounds()));
}
