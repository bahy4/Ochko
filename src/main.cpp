#include "Game.hpp"
#include <iostream>
#include <memory>

int main() {
    Game game;
    
    // Добавляем игроков
    game.addPlayer(std::make_unique<HumanPlayer>("Player1"));
    game.addPlayer(std::make_unique<HumanPlayer>("Player2"));
    
    char playAgain;
    do {
        game.playRound();
        std::cout << "\nPlay again? (y/n): ";
        std::cin >> playAgain;
    } while (playAgain == 'y' || playAgain == 'Y');
    
    return 0;
}