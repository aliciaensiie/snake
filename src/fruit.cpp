#include "../headers/fruit.h"
#include <random> 
#include <chrono>

//PRIVATE
std::default_random_engine createRandomEngine(){
    unsigned long seed=static_cast<unsigned long> (std::time(nullptr));
    return std::default_random_engine(seed);
}

std::default_random_engine RandomEngine=createRandomEngine();

int Fruit::random(int nombre){
    std::uniform_int_distribution<> distr(0, nombre);
    return distr(RandomEngine);
}

void Fruit::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.transform *=getTransform();
    target.draw(visual_fruit, states);
}


//PUBLIC

Fruit::Fruit(): visual_fruit(FRUIT_SIZE){
    setPosition(random(WIDTH), random(HEIGTH));
    colorFruit();
}

sf::CircleShape Fruit::getVisualFruit() const{
    return visual_fruit;
}

void Fruit::setRandomPosition(){
    setPosition(random(WIDTH), random(HEIGTH));
}

void Fruit::colorFruit(){
    int number=random(5);
    switch (number){
        case 1: 
            visual_fruit.setFillColor(sf::Color::Black);
            break;
        case 2:
            visual_fruit.setFillColor(sf::Color::Green);
            break;
        case 3:
            visual_fruit.setFillColor(sf::Color::Blue);
            break;
        case 4:
            visual_fruit.setFillColor(sf::Color::Cyan);
            break;
        case 5:
            visual_fruit.setFillColor(sf::Color::Magenta);
            break;
    }
}