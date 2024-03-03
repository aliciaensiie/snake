
#include "../headers/fruits.h"
#include <iostream>
#include <fstream>

#define TIME_UPDATE_MOVE_SNAKE sf::seconds(1.f/8.f)
#define TIME_UPDATE_NUMBER_FRUITS sf::seconds(5.f)
#define TIME_END_SCREEN sf::seconds(5.f)




int main()
{
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGTH), "Snake!", sf::Style::Close);
 
    //FONT
    sf::Font font1;
    font1.loadFromFile(std::string(MACRO_NAME) + "/fonts/SandorTrial.ttf");
    sf::Font font2;
    font2.loadFromFile(std::string(MACRO_NAME) + "/fonts/Oswald-Light.ttf");

    //score from file
    const std::string score_file = std::string(MACRO_NAME) + "/score.txt";
    std::ifstream score_file_reading(score_file);
    if (!score_file_reading.is_open()) {
        std::cerr << "Impossible to read the score file" << std::endl;
        return EXIT_FAILURE;
    }
    std::string line;
    std::getline(score_file_reading, line);
    score_file_reading.close();

    //Transparent rectangle for the end
    sf::RectangleShape transparent(sf::Vector2f(window.getSize().x, window.getSize().y));
    transparent.setFillColor(sf::Color(255, 255, 255, 200)); 

    //Time
    sf::Clock clock_to_move_snake;
    sf::Clock clock_to_add_fruits;
    sf::Clock clock_end_screen;

    //element of the game
    Snake snake;
    Fruits fruits(snake);
    
    //Text
    sf::Text end_text("Your snake ate himself !", font1 );
    end_text.setFillColor(sf::Color::Black);
    end_text.setPosition(window.getSize().x/4, window.getSize().y/2.5);
   
    sf::Text size_snake("size: "+ std::to_string(snake.getNumberOfPoints()/2)+ " / size max: " + line, font2, 20);
    size_snake.setFillColor(sf::Color::Black);
    size_snake.setPosition(window.getSize().x/2.5, 0);

    //
    bool press = false;
    sf::Vector2f mousePosition;
    bool eat_himself=false;


    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed){
                window.close();
            }

            else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                mousePosition=sf::Vector2f(sf::Mouse::getPosition(window));
                press=true;
            }
                
        }

        //move
        if(clock_to_move_snake.getElapsedTime() > TIME_UPDATE_MOVE_SNAKE && eat_himself == false){
            clock_to_move_snake.restart();

            if (press==true && 0<mousePosition.x && mousePosition.x<WIDTH && 0<mousePosition.y && mousePosition.y<HEIGTH){
                snake.move(mousePosition);
                press=false;
            }
            else{
               snake.move(); 
            }
        }

        //add fruits
        if (clock_to_add_fruits.getElapsedTime() > TIME_UPDATE_NUMBER_FRUITS){
            clock_to_add_fruits.restart();
            int diff=NUMBER_MAX_FRUITS- fruits.getNumberOfFruits();
            for(int i=0; i<diff; i++){
                fruits.addFruits(snake);
            }
        }

        //collisions
        int number_collisions=fruits.collision(snake);
        if(number_collisions>0){
            snake.grow(number_collisions);
            size_snake.setString("size: "+ std::to_string(snake.getNumberOfPoints()/2)+ " / size max: " + line);
        }

        //eat_himself, end of the game
        if (snake.DoesEatHimself() || eat_himself ==true ){

            //first round when he eats himself
            if (eat_himself==false){
                clock_end_screen.restart();
                int score_max = std::stoi(line);

                //new record in the file
                if (score_max<snake.getNumberOfPoints()/2){
                    end_text.setString(end_text.getString() + "\nYou made a new record !");
                    line = std::to_string(snake.getNumberOfPoints()/2);
                    std::ofstream file_writing(score_file);
                    if (!file_writing.is_open()) {
                        std::cerr << "Impossible to write in the score file" << std::endl;
                        return EXIT_FAILURE;
                    }
                    file_writing << line << std::endl;
                    file_writing.close();
                }
            }
            eat_himself=true;
            if(clock_end_screen.getElapsedTime() > TIME_END_SCREEN){
                window.close();
            }
        }

        window.clear(sf::Color::White);
        window.draw(fruits);
        window.draw(snake);
        window.draw(size_snake);
        if (eat_himself==true){
            window.draw(transparent);
            window.draw(end_text);
        }
        window.display();
    }

    return 0;
}
