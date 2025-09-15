#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>

#include "Game.h"
#include "Board.h"
#include "Piece.h"
#include <iostream>
#include <string>
#include <map>
#include <thread>
#include <chrono>
#include <optional>

// Function declaration for drawIndicator
void drawIndicator(sf::RenderWindow& window, int row, int col);

int main()
{
    // create the window
    sf::RenderWindow window(sf::VideoMode({600, 600}), "Michael's Chess App");
    
    // start game
    Game theGame;
    
    // A flag to keep track of the indicator's state
    bool showIndicator = false;
    int indicatorRow = -1;
    int indicatorCol = -1;
    
    // Load all textures once at the beginning
        std::map<std::string, sf::Texture> textures;
        for (int row = 0; row < 8; ++row) {
            for (int col = 0; col < 8; ++col) {
                Piece curr = theGame.getBoard().getPiece(row, col);
                if (curr.getSymbol() != ' ') {
                    char symbol = curr.getSymbol();
                    char color = curr.getColor();
                    std::string path;
                    if (color == 'b') {
                        path = "Desktop/ChessApp/sprites/" + std::string(1, symbol) + "1.png";
                    } else {
                        path = "Desktop/ChessApp/sprites/" + std::string(1, symbol) + ".png";
                    }

                    if (textures.find(path) == textures.end()) {
                        sf::Texture tempTexture;
                        if (!tempTexture.loadFromFile(path)) {
                            std::cerr << "Error: Failed to load texture from " << path << std::endl;
                        }
                        textures[path] = tempTexture;
                    }
                }
            }
        }


    // run the program as long as the window is open
    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        while (const std::optional event = window.pollEvent())
        {
            // "close requested" event: we close the window
            if (event->is<sf::Event::Closed>())
                window.close();
            
            if (const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>())
            {
                if (mouseButtonPressed->button == sf::Mouse::Button::Left)
                {
                    // Retrieve position and calculate the tile
                    int row = 7-(mouseButtonPressed->position.y / 75);
                    int col = mouseButtonPressed->position.x / 75;
                    
                    std::cout << "Clicked (" << row << "," << col << ")" << std::endl;
                    
                    if (showIndicator) {
                        bool res = theGame.makeMove(indicatorRow, indicatorCol, row, col);
                        std::cout << "Move Success: " << res <<std::endl;
                        showIndicator = false;
                    }
                    
                    // Set the indicator's position and make it visible
                    indicatorRow = row;
                    indicatorCol = col;
                    
                    if (theGame.getBoard().getPiece(row, col).getSymbol() != ' ' && theGame.getBoard().getPiece(row, col).getColor() == theGame.getTurn()) {
                        showIndicator = true;
                    }
                }
            }
        }

        // clear the window with black color
        window.clear(sf::Color::Black);
        
        // define a 120x75 rectangle
        sf::RectangleShape white_square({75.f, 75.f});
        white_square.setFillColor(sf::Color(255, 255, 255));
        
        sf::RectangleShape dark_square({75.f, 75.f});
        dark_square.setFillColor(sf::Color(139,69,19));
        
        // draw everything here...
        
        // draw board
        for (int row = 0; row < 8; row++) {
            for (int col = 0; col < 8; col++) {
                if (((row % 2) == 0 && (col % 2) == 0) || ((row % 2) == 1 && (col % 2) == 1)) { // white
                    white_square.setPosition(sf::Vector2f(75.f * col, 75.f * row));
                    window.draw(white_square);
                } else { // dark
                    dark_square.setPosition(sf::Vector2f(75.f * col, 75.f * row));
                    window.draw(dark_square);
                }
            }
        }
        
        // draw pieces
        for (int row = 7; row >= 0; row--) {
            for (int col = 0; col < 8; col++) {
                Piece curr = theGame.getBoard().getPiece(row, col);
                
                // Summon sprite if not empty
                if (curr.getSymbol() != ' ') {
                    char symbol = curr.getSymbol();
                    char color = curr.getColor();
                    std::string path;
                    
                    // Adjust path to sprite based on color and symbol
                    if (color == 'b') {
                        path = "Desktop/ChessApp/sprites/" + std::string(1, symbol) + "1.png";
                    } else {
                        path = "Desktop/ChessApp/sprites/" + std::string(1, symbol) + ".png";
                    }
                    
                    // The sprite must be a local variable to be drawn correctly.
                    textures[path].setSmooth(true);
                    sf::Sprite sprite(textures[path]);
                    
                    // Set the position using a single sf::Vector2f object
                    sprite.setPosition(sf::Vector2f(75.f * col, 75.f * (7 - row)));
                    
                    // Set the scale using a single sf::Vector2f object
                    sprite.setScale(sf::Vector2f(42.f / textures[path].getSize().x, 42.f / textures[path].getSize().y));
                    
                    // Optional: Center the piece on the tile
                    float offsetX = (75.f - 42.f) / 2.f;
                    float offsetY = (75.f - 42.f) / 2.f;
                    
                    // Move the sprite using a single sf::Vector2f object
                    sprite.move(sf::Vector2f(offsetX, offsetY));
                    
                    window.draw(sprite);
                }
            }
        }
        
        // Draw the indicator if it should be visible
        if (showIndicator) {
            drawIndicator(window, indicatorRow, indicatorCol);
        }

        // end the current frame
        window.display();
    }
}

// Global function definition for drawIndicator
void drawIndicator(sf::RenderWindow& window, int row, int col) {
    sf::CircleShape indicator(7);
    indicator.setFillColor(sf::Color(237, 41, 57));
    // The position calculation needs to be consistent with your board drawing
    float posX = 75.f * col + (75.f / 2.f) - 15.f; // Center the indicator on the tile
    float posY = 75.f * (7 - row) + (75.f / 2.f) - 15.f; // The 7-row is for the coordinate system
    
    // Optional: Center the piece on the tile
    float offsetX = (75.f - 58.f) / 2.f;
    float offsetY = (75.f - 58.f) / 2.f;
    
    indicator.setPosition(sf::Vector2f(posX, posY));
    indicator.move(sf::Vector2f(offsetX, offsetY));
    window.draw(indicator);
}
