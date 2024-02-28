#ifndef FRUITS_H
#define FRUITS_H

#include "fruit.h"
#define NUMBER_MAX_FRUITS 10

class Fruits: public sf::Drawable{
    private: 
        std::vector<Fruit> list_fruit;
        int number_of_fruit;

    public: 
        Fruits(const Snake& snake);
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
        int getNumberOfFruits() const;
        std::vector<Fruit> getListFruit() const;
        void addFruits(const Snake& snake);
        int collision(const Snake& snake);
};

#endif //FRUITS_H