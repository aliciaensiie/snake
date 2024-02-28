#ifndef SNAKE_H
#define SNAKE_H
#include "SFML/Graphics.hpp"
#include <math.h>

#ifdef MACRO_NAME
    
#else
    #define MACRO_NAME "../ressources"
#endif

#define WIDTH 600
#define HEIGTH 400
#define SNAKE_WIDTH 9.0f
#define SNAKE_LENGTH_BETWEEN_2_POINTS 12.0f
#define SNAKE_FIRST_POS_X 80
#define SNAKE_FIRST_POS_Y 60
#define COLOR_SNAKE sf::Color::Red

class Snake : public sf::Drawable{
    private:
        int number_of_points;
        sf::VertexArray body_snake;

        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
        bool doesIntersectSideScreen();
        sf::Vector2f midpoint(const sf::Vector2f& pos1, const sf::Vector2f& pos2)const;
        float distance(const sf::Vector2f& pos1, const sf::Vector2f& pos2)const;
        float crossProduct(const sf::Vector2f& A, const sf::Vector2f& B, const sf::Vector2f& C)const;
        float angleMax0() const;
        float angleMax1() const;
        float newAngle(const sf::Vector2f& pointA, const sf::Vector2f& pointB, const sf::Vector2f& point0) const;
        void corner90Degree(int side);

    public: 
        Snake();
        int getNumberOfPoints() const;
        void move(const sf::Vector2f& point);
        void move();
        void grow(int number);
        bool contains(const sf::Vector2f& point, int index_first_triangle) const;
        bool doesTriangleIntersectCircle(const sf::Vector2f& point, int index_first_triangle, int radius) const;
        bool DoesEatHimself();

    
};

#endif //SNAKE_H