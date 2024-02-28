#include "../headers/fruits.h"
#include "SFML/Graphics.hpp"

Fruits::Fruits(const Snake& snake): list_fruit(), number_of_fruit(0){
    for (int i=1; i<10; i++){
        addFruits(snake);
    }
}

int Fruits::getNumberOfFruits() const {
    return number_of_fruit;
}

std::vector<Fruit> Fruits::getListFruit() const{
    return list_fruit;
}

void Fruits::addFruits(const Snake& snake){
    Fruit new_fruit;

    while (snake.contains(new_fruit.getPosition(), 0)){
        new_fruit.setRandomPosition();
    }
    new_fruit.colorFruit();
    list_fruit.push_back(new_fruit);
    number_of_fruit += 1;
}

int Fruits::collision(const Snake& snake){
    int number_of_eaten_fruits = 0;
    std::vector<int> index_of_eaten_fruit;
    for (int i=0; i< number_of_fruit; i++){
        if (snake.doesTriangleIntersectCircle(list_fruit[i].getPosition(), 0, FRUIT_SIZE)){
            number_of_eaten_fruits += 1;
            index_of_eaten_fruit.push_back(i);
        }
    }
    for (int valeur : index_of_eaten_fruit){
        list_fruit.erase(list_fruit.begin() + valeur);
        number_of_fruit -= 1;
    }
    return number_of_eaten_fruits;
}


void Fruits::draw(sf::RenderTarget& target, sf::RenderStates states) const{
    for (int i=0; i<number_of_fruit; i++){
        target.draw(list_fruit[i], states);
    }
}
