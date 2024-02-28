#include "../headers/snake.h"
#include "iostream"
#include <math.h>



//PUBLIC
Snake::Snake():number_of_points(10), body_snake(sf::TriangleStrip, 10){
    for(int i=0; i<number_of_points; i+=1){
        body_snake[i].position = sf::Vector2f(SNAKE_FIRST_POS_X + SNAKE_LENGTH_BETWEEN_2_POINTS * (i/2) , SNAKE_FIRST_POS_Y + SNAKE_WIDTH * (i%2));
        body_snake[i].color = COLOR_SNAKE;
    }
}

int Snake::getNumberOfPoints() const{
    return number_of_points;
}

//move toward point
void  Snake::move(const sf::Vector2f& point){

    sf::Vector2f midpoint0 = midpoint(body_snake[0].position , body_snake[1].position);
    sf::Vector2f vector_midpoint_point = point-midpoint0;
    float distance_midpoint_point = distance(midpoint0, point);

    //to find the new midpoint
    float coeff = SNAKE_LENGTH_BETWEEN_2_POINTS / distance_midpoint_point;
    sf::Vector2f new_midpoint = midpoint0 + coeff * vector_midpoint_point;

     
    sf::Vector2f vector_midpoints = new_midpoint - midpoint0;

    //sum of vectors and projections
    sf::Vector2f new_point0;
    new_point0 = midpoint0 + vector_midpoints + (SNAKE_WIDTH/2) / SNAKE_LENGTH_BETWEEN_2_POINTS * sf::Vector2f(-vector_midpoints.y, vector_midpoints.x);

    sf::Vector2f new_point1;
    new_point1 = midpoint0 + vector_midpoints + (SNAKE_WIDTH/2) / SNAKE_LENGTH_BETWEEN_2_POINTS * sf::Vector2f(vector_midpoints.y, -vector_midpoints.x);
    

    //Verification
    float angle_max_0 = angleMax0();
    float angle_max_1 = angleMax1();
    float angle0 = newAngle(body_snake[2].position, new_point0, body_snake[0].position);
    float angle1 = newAngle(body_snake[3].position, new_point1, body_snake[1].position);

    if(angle0 > angle_max_0 && angle1 > angle_max_1){ //if ok
       for (int i = 1; i < number_of_points-1; ++i) {
            body_snake[number_of_points-i].position = body_snake[number_of_points-i-2].position;
        } 
        body_snake[0].position = new_point0;
        body_snake[1].position = new_point1;
    }
    else{ // if not ok: the snake turns of 90degrees first and go towards the 'point' after
        if(angle0 > angle_max_0){
            corner90Degree(0);
        }
        else{
            corner90Degree(1);
        }
        move(point);
    }
    
}

//move in front of him
void Snake::move(){

    sf::Vector2f midpoint0 = midpoint(body_snake[0].position, body_snake[1].position);
    sf::Vector2f midpoint1 = midpoint(body_snake[2].position, body_snake[3].position);
    sf::Vector2f vector_between_midpoint = midpoint0 - midpoint1;

    //move in front of him
    for (int i = 1; i < number_of_points-1; ++i) {
        body_snake[number_of_points-i].position = body_snake[number_of_points-i-2].position;
    }
    body_snake[0].position += vector_between_midpoint;
    body_snake[1].position += vector_between_midpoint;


    bool check_sides_screen = false;
    
    while( check_sides_screen == false ){
        check_sides_screen = doesIntersectSideScreen();
    }
   
}


void Snake::grow(int number){
    
    for (int i=0; i<number; i++){
        sf::Vector2f last_midpoint = midpoint(body_snake[number_of_points-2].position , body_snake[number_of_points-1].position);

        // last_midpoint+ vector projection 
        sf::Vector2f new_point1;
        new_point1 = last_midpoint + SNAKE_LENGTH_BETWEEN_2_POINTS / (SNAKE_WIDTH/2) * sf::Vector2f(last_midpoint.y - body_snake[number_of_points-2].position.y, last_midpoint.x - body_snake[number_of_points-2].position.x) - (last_midpoint - body_snake[number_of_points-2].position);

        sf::Vector2f new_point2;
        new_point2 = last_midpoint + SNAKE_LENGTH_BETWEEN_2_POINTS / (SNAKE_WIDTH/2) * sf::Vector2f(last_midpoint.y - body_snake[number_of_points-2].position.y, last_midpoint.x - body_snake[number_of_points-2].position.x) + (last_midpoint - body_snake[number_of_points-2].position);

        body_snake.append(new_point1);
        body_snake.append(new_point2);
        body_snake[number_of_points].color = COLOR_SNAKE;
        body_snake[number_of_points+1].color = COLOR_SNAKE;
        number_of_points += 2;  
    }
}

// to see if the point is in the snake
bool Snake::contains(const sf::Vector2f& point, int index_first_triangle)const{
    //for each triangle of the snake
    for (int i=index_first_triangle; i< number_of_points-3 ; i++){
        sf::Vector2f pointA = body_snake[i].position;
        sf::Vector2f pointB = body_snake[i+1].position;
        sf::Vector2f pointC = body_snake[i+2].position;

        double produitABxAP = crossProduct(pointA, pointB, point);
        double produitBCxBP = crossProduct(pointB, pointC, point);
        double produitCAxCP = crossProduct(pointC, pointA, point);

        if( (produitABxAP >= 0 && produitBCxBP >= 0 && produitCAxCP >= 0) || (produitABxAP <= 0 && produitBCxBP <= 0 && produitCAxCP <= 0)){
            return true;
        }
    }
    return false;
}

//to see if a circle of center point is in the snake
bool Snake::doesTriangleIntersectCircle(const sf::Vector2f& point, int index_first_triangle, int radius) const {
    
    if (contains(point, 0)){
        return true;
    }

    for (int i=index_first_triangle; i < number_of_points-3 ; i++){
        float distanceA = distance(point, body_snake[i].position);
        float distanceB = distance(point, body_snake[i+1].position);
        float distanceC = distance(point, body_snake[i+2].position);
        
        if (distanceA < radius || distanceB < radius || distanceC < radius){
            return true;
        }
    }
    return false;
}


bool Snake::DoesEatHimself(){
    sf::Vector2f head1 = body_snake[0].position;
    sf::Vector2f head2 = body_snake[1].position;
    return (contains(head1, 4) || contains(head2, 4));
}



//PRIVATE
void Snake::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(body_snake, states);
}


bool Snake::doesIntersectSideScreen(){
    sf::Vector2f pos0 = body_snake[0].position;
    sf::Vector2f pos1 = body_snake[1].position;

    sf::Vector2f midpoint1 = midpoint(body_snake[2].position, body_snake[3].position);

    if (pos0.x <= 0 && pos0.x<=pos1.x){
        float y = midpoint1.y+SNAKE_LENGTH_BETWEEN_2_POINTS;
        body_snake[0].position.x = body_snake[2].position.x;
        body_snake[0].position.y = y;
        
        float xo = body_snake[0].position.x + SNAKE_WIDTH;
        body_snake[1].position.x = xo;
        body_snake[1].position.y = y;
        
        return false;
    }
    else if (pos0.x >= WIDTH && pos0.x >= pos1.x){
        float y = midpoint1.y - SNAKE_LENGTH_BETWEEN_2_POINTS;
        body_snake[0].position.x = body_snake[2].position.x;
        body_snake[0].position.y = y;
        float xo = body_snake[0].position.x-SNAKE_WIDTH;
        body_snake[1].position.x = xo;
        body_snake[1].position.y = y;
        
        return false;
    }
    else if (pos0.y <= 0 && pos0.y <= pos1.y){
        float x = midpoint1.x - SNAKE_LENGTH_BETWEEN_2_POINTS;
        body_snake[0].position.y = body_snake[2].position.y;
        body_snake[0].position.x = x;
        float yo = body_snake[0].position.y + SNAKE_WIDTH;
        body_snake[1].position.y = yo;
        body_snake[1].position.x = x;
        
        
        return false;
    }
    else if (pos0.y >= HEIGTH && pos0.y >= pos1.y){
        float x = midpoint1.x + SNAKE_LENGTH_BETWEEN_2_POINTS;
        body_snake[0].position.y = body_snake[2].position.y;
        body_snake[0].position.x = x;
        float yo = body_snake[0].position.y - SNAKE_WIDTH;
        body_snake[1].position.y = yo;
        body_snake[1].position.x = x;
        
        return false;
    }
    else if (pos1.x <= 0){
        float y = midpoint1.y - SNAKE_LENGTH_BETWEEN_2_POINTS;
        body_snake[1].position.x = body_snake[3].position.x;
        body_snake[1].position.y = y;
        float x1 = body_snake[1].position.x + SNAKE_WIDTH;
        body_snake[0].position.x = x1;
        body_snake[0].position.y = y;
        
        
        return false;
    }
    else if (pos1.x >= WIDTH){
        float y = midpoint1.y + SNAKE_LENGTH_BETWEEN_2_POINTS;
        body_snake[1].position.x = body_snake[3].position.x;
        body_snake[1].position.y = y;
        float x1 = body_snake[1].position.x-SNAKE_WIDTH;
        body_snake[0].position.x = x1;
        body_snake[0].position.y = y;
        return false;
    }
    else if (pos1.y <= 0){
        float x = midpoint1.x + SNAKE_LENGTH_BETWEEN_2_POINTS;
        body_snake[1].position.y = body_snake[3].position.y;
        body_snake[1].position.x = x;
        float y1 = body_snake[1].position.y + SNAKE_WIDTH;
        body_snake[0].position.y = y1;
        body_snake[0].position.x = x;
        return false;
    }
    else if (pos1.y >= HEIGTH){
        float x = midpoint1.x - SNAKE_LENGTH_BETWEEN_2_POINTS;
        body_snake[1].position.y = body_snake[3].position.y;
        body_snake[1].position.x = x;
        float y1 = body_snake[1].position.y - SNAKE_WIDTH;
        body_snake[0].position.y = y1;
        body_snake[0].position.x = x;
        return false;
    }
    return true;
}

sf::Vector2f Snake::midpoint(const sf::Vector2f& pos1, const sf::Vector2f& pos2) const{
    return sf::Vector2f(std::floor((pos1.x + pos2.x)/2), std::floor((pos1.y + pos2.y)/2));
}

float Snake::distance(const sf::Vector2f& pos1, const sf::Vector2f& pos2) const{
    return sqrt((pos1.x - pos2.x) * (pos1.x - pos2.x) + (pos1.y - pos2.y) * (pos1.y - pos2.y));
}

float Snake::crossProduct(const sf::Vector2f& A, const sf::Vector2f& B, const sf::Vector2f& C) const {
    return (B.x - A.x) * (C.y - A.y) - (C.x - A.x) * (B.y - A.y);
}

float Snake::angleMax0() const{
    sf::Vector2f diagonal = (body_snake[1].position - body_snake[0].position) + (body_snake[3].position - body_snake[1].position);
    sf::Vector2f side = body_snake[2].position -body_snake[0].position;

    float cosO=((side.x * diagonal.x) + (side.y * diagonal.y)) / (sqrt(side.x * side.x + side.y * side.y) * sqrt(diagonal.x * diagonal.x + diagonal.y * diagonal.y));
    if(cosO>=1.f){
        return 2.f*M_PI;
    }
    return acos(cosO);
}

float Snake::angleMax1() const{
    sf::Vector2f diagonal = (body_snake[0].position - body_snake[1].position) + (body_snake[2].position - body_snake[0].position);
    sf::Vector2f side = body_snake[3].position - body_snake[1].position;

    float cosO=((side.x * diagonal.x) + (side.y * diagonal.y)) / (sqrt(side.x * side.x + side.y * side.y) * sqrt(diagonal.x * diagonal.x + diagonal.y * diagonal.y));
    if(cosO>=1.f){
        return 2.f*M_PI;
    }
    return acos(cosO);
}

float Snake::newAngle(const sf::Vector2f& pointA, const sf::Vector2f& pointB, const sf::Vector2f& point0) const{
    sf::Vector2f vectorOA = pointA - point0;
    sf::Vector2f vectorOB = pointB - point0;
    float cosO= (vectorOA.x * vectorOB.x + vectorOA.y * vectorOB.y) / (sqrt(vectorOA.x * vectorOA.x + vectorOA.y * vectorOA.y) * sqrt(vectorOB.x * vectorOB.x + vectorOB.y * vectorOB.y));
    if(cosO>=1.f){
        return 2.f*M_PI;
    }
    return acos(cosO);
}

void Snake::corner90Degree(int side){
    sf::Vector2f new_point_side;
    sf::Vector2f new_point_other_side;
    sf::Vector2f new_midpoint;

    sf::Vector2f midpoint0 = midpoint(body_snake[0].position, body_snake[1].position);
    sf::Vector2f midpoint1 = midpoint(body_snake[2].position, body_snake[3].position);
    sf::Vector2f vector_mid01 = midpoint1 - midpoint0;

    if (side==0){
        new_midpoint.x = midpoint0.x - vector_mid01.y;
        new_midpoint.y = midpoint0.y + vector_mid01.x;
    }
    else{
        new_midpoint.x =midpoint0.x + vector_mid01.y ;
        new_midpoint.y = midpoint0.y - vector_mid01.x;
    }

    new_point_side = new_midpoint + (SNAKE_WIDTH/2) * vector_mid01 / SNAKE_LENGTH_BETWEEN_2_POINTS;

    new_point_other_side = new_midpoint - (SNAKE_WIDTH/2) * vector_mid01 / SNAKE_LENGTH_BETWEEN_2_POINTS;
    
    for (int i = 1; i < number_of_points-1; ++i) {
        body_snake[number_of_points-i].position = body_snake[number_of_points-i-2].position;
     } 
     if (side == 0){
        body_snake[0].position = new_point_side;
        body_snake[1].position = new_point_other_side;
     }
     else{
        body_snake[0].position = new_point_other_side;
        body_snake[1].position = new_point_side;
     }
        

}