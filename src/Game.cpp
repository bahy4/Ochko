#include "Game.hpp"
#include <iostream>
#include <cmath>

bool HumanPlayer::wantsHit() const {
    std::cout << name << ", do you want a hit? (y/n): ";
    char response;
    std::cin >> response;
    return response == 'y' || response == 'Y';
}

Game::Game() : deck() {}

void Game::addPlayer(std::unique_ptr<Player> player) {
    players.push_back(std::move(player));
}

void Game::playRound() {
    deck = Deck(); // Новая колода
    dealer.clearHand();
    for (auto& player : players) {
        player->clearHand();
    }
    
    dealInitialCards();
    displayGameState();
    playPlayerTurns();
    playDealerTurn();
    determineWinners();
}

void Game::dealInitialCards() {
    for (int i = 0; i < 2; ++i) {
        for (auto& player : players) {
            player->receiveCard(deck.drawCard());
        }
        dealer.receiveCard(deck.drawCard());
    }
}

void Game::playPlayerTurns() {
    for (auto& player : players) {
        while (!player->isBusted() && player->wantsHit()) {
            player->receiveCard(deck.drawCard());
            displayGameState();
        }
    }
}

void Game::playDealerTurn() {
    std::cout << "\nDealer's turn:\n";
    while (dealer.wantsHit()) {
        dealer.receiveCard(deck.drawCard());
        displayGameState();
    }
}

void Game::displayGameState() const {
    std::cout << "\n=== Game State ===\n";
    std::cout << "Dealer: " << dealer.getHand().toString() 
              << "(" << dealer.getHand().getValue() << ")\n";
    
    for (const auto& player : players) {
        std::cout << player->getName() << ": " << player->getHand().toString()
                  << "(" << player->getHand().getValue() << ")\n";
    }
    
    calculateProbabilities();
}

void Game::calculateProbabilities() const {
    std::cout << "\nWin Probabilities:\n";
    // Более точный расчет вероятности выигрыша
    int remainingCards = deck.remainingCards();
    if (remainingCards == 0) return;
    
    for (const auto& player : players) {
        if (player->isBusted()) {
            std::cout << player->getName() << ": 0%\n";
            continue;
        }
        
        int playerScore = player->getHand().getValue();
        int dealerScore = dealer.getHand().getValue();
        
        // Упрощенная модель: считаем вероятность, что дилер проиграет или наберет меньше
        double winProb = 0.0;
        
        if (dealerScore > 21) {
            winProb = 100.0; // Дилер уже перебрал
        } else {
            // Вероятность, что дилер наберет больше игрока, но не больше 21
            // Это упрощенная модель - в реальности нужно учитывать все возможные карты
            int neededPoints = playerScore - dealerScore;
            if (neededPoints <= 0) {
                winProb = 0.0;
            } else {
                // Каждая карта от 1 до 13 имеет примерно равную вероятность
                double probPerCard = 1.0 / 13.0; // Упрощение
                winProb = (10 - neededPoints + 1) * probPerCard * 100;
                winProb = std::max(0.0, std::min(100.0, winProb));
            }
        }
        
        std::cout << player->getName() << ": " << std::round(winProb) << "%\n";
    }
}

void Game::determineWinners() const {
    std::cout << "\n=== Results ===\n";
    int dealerScore = dealer.isBusted() ? 0 : dealer.getHand().getValue();
    
    for (const auto& player : players) {
        int playerScore = player->isBusted() ? 0 : player->getHand().getValue();
        
        if (player->isBusted()) {
            std::cout << player->getName() << " busts! Dealer wins.\n";
        } else if (dealer.isBusted()) {
            std::cout << player->getName() << " wins! Dealer busts.\n";
        } else if (playerScore > dealerScore) {
            std::cout << player->getName() << " wins! " << playerScore 
                      << " vs " << dealerScore << "\n";
        } else if (playerScore < dealerScore) {
            std::cout << player->getName() << " loses! " << playerScore 
                      << " vs " << dealerScore << "\n";
        } else {
            std::cout << player->getName() << " pushes with dealer.\n";
        }
    }
}