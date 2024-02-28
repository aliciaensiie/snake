#ifndef FRUIT_H
#define FRUIT_H
#include "SFML/Graphics.hpp"
#include "snake.h"

#define FRUIT_SIZE 4

class Fruit : public sf::Drawable, public sf::Transformable{
    private: 
        sf::CircleShape visual_fruit;
        int random(int nombre);
        void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    public: 
        Fruit();
        sf::CircleShape getVisualFruit() const;
        void setRandomPosition();
        void colorFruit();
};



#endif //FRUIT_H
