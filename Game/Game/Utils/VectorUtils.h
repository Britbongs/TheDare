#ifndef VECTORUTILS_H
#define VECTORUTILS_H
#include <SFML\Graphics.hpp>

sf::Vector2f subtractVector(sf::Vector2f a, sf::Vector2f b);

sf::Vector2f addVector(sf::Vector2f a, sf::Vector2f b);

sf::Vector2i subtractVector(sf::Vector2i a, sf::Vector2i b);

sf::Vector2i addVector(sf::Vector2i a, sf::Vector2i b);

double dotProduct(sf::Vector2f a, sf::Vector2f b);

sf::Vector2f normalize(sf::Vector2f a);
#endif