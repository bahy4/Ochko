#include "Game.hpp"
#include <iostream>
#include <cmath>
#include <algorithm>
#include <chrono>

bool HumanPlayer::wantsHit() const {
    std::cout << name << ", do you want a hit? (y/n): ";
    char response;
    std::cin >> response;
    return response == 'y' || response == 'Y';
}

Game::Game() : deck(), playersTurn(false), rng(std::chrono::steady_clock::now().time_since_epoch().count()) {}

void Game::addPlayer(std::unique_ptr<Player> player) {
    players.push_back(std::move(player));
}

void Game::playRound() {
    deck = Deck(); // Новая колода
    dealer.clearHand();
    for (auto& player : players) {
        player->clearHand();
    }
    
    playersTurn = true;
    dealInitialCards();
    displayGameState();
    playPlayerTurns();
    playersTurn = false;
    playDealerTurn();
    determineWinners();
}

void Game::dealInitialCards() {
    for (int i = 0; i < 2; ++i) {
        for (auto& player : players) {
            player->receiveCard(deck.drawCard());
        }
        
    }
    dealer.receiveCard(deck.drawCard());
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
    std::cout << "\nWin Probabilities (Monte Carlo simulation):\n";
    
    if (deck.isEmpty()) {
        std::cout << "Deck is empty, cannot calculate probabilities.\n";
        return;
    }
    
    for (const auto& player : players) {
        double winProbability = simulatePlayerWinProbability(player.get());
        std::cout << player->getName() << ": " << (winProbability) << "%\n";
    }
}

double Game::simulatePlayerWinProbability(const Player* player) const {
    const int NUM_SIMULATIONS = 10000;
    int winCount = 0;
    
    if (player->isBusted()) {
        return 0.0;
    }

    if (dealer.isBusted()){
        return 100.0;
    }
    
    int playerScore = player->getHand().getValue();
    bool playerHasBlackjack = player->getHand().hasBlackjack();
    
    // Получаем открытую карту дилера
    const auto& dealerCards = dealer.getHand().getCards();
    if (dealerCards.empty()) {
        return 0.0;
    }
    Card dealerOpenCard = dealerCards[0];
    
    for (int i = 0; i < NUM_SIMULATIONS; ++i) {
        // Симулируем финальный счет дилера
        int dealerScore = simulateDealerFinalScore(i);
        
        // Определяем победителя
        if (dealerScore > 21) {
            // Дилер перебрал - игрок выигрывает (кроме случая, когда игрок тоже перебрал)
            if (!player->isBusted()) {
                winCount++;
            }
        } else if (player->isBusted()) {
            // Игрок перебрал - проигрыш
            // winCount не увеличиваем
        } else if (playerHasBlackjack) {
            // У игрока blackjack
            if (dealerScore == 21 && dealerCards.size() == 2) {
                // У дилера тоже blackjack - ничья
                // winCount не увеличиваем
            } else {
                // Дилер не имеет blackjack - игрок выигрывает
                winCount++;
            }
        } else if (dealerScore == 21 && dealerCards.size() == 2) {
            // У дилера blackjack, у игрока нет - проигрыш
            // winCount не увеличиваем
        } else if (playerScore > dealerScore) {
            // Игрок имеет больший счет
            winCount++;
        } else if (playerScore == dealerScore) {
            // Ничья - не считается победой
            // winCount не увеличиваем
        }
        // Во всех остальных случаях игрок проигрывает
    }
    return (1.0*winCount*(1.0/ NUM_SIMULATIONS)) * 100.0;
}

int Game::simulateDealerFinalScore(int num) const {
    // Создаем копию текущей колоды для симуляции
    Deck simulationDeck = deck;
    simulationDeck.shuffle(num);
    
    // Создаем руку дилера для симуляции
    Hand dealerHandSim;
    
    // Добавляем открытую карту дилера
    const auto& dealerCards = dealer.getHand().getCards();
    if (!dealerCards.empty()) {
        dealerHandSim.addCard(dealerCards[0]);
    }
    
    // Симулируем скрытую карту дилера из оставшейся колоды
    if (!simulationDeck.isEmpty()) {
        dealerHandSim.addCard(simulationDeck.drawCard());
    }
    
    // Симулируем добор карт дилером по правилам (до 17 или больше)
    while (dealerHandSim.getValue() < 17 && !simulationDeck.isEmpty()) {
        dealerHandSim.addCard(simulationDeck.drawCard());
        
        // Если дилер перебрал, останавливаемся
        if (dealerHandSim.isBusted()) {
            break;
        }
    }
    
    return dealerHandSim.isBusted() ? 0 : dealerHandSim.getValue();
}

std::vector<Card> Game::getRemainingCards() const {
    // Этот метод можно использовать для более сложной симуляции
    // В текущей реализации мы просто используем копию колоды
    return std::vector<Card>(); // Заглушка
}

void Game::determineWinners() const {
    std::cout << "\n=== Results ===\n";
    int dealerScore = dealer.isBusted() ? 0 : dealer.getHand().getValue();
    bool dealerHasBlackjack = dealer.getHand().hasBlackjack();
    
    for (const auto& player : players) {
        int playerScore = player->isBusted() ? 0 : player->getHand().getValue();
        bool playerHasBlackjack = player->getHand().hasBlackjack();
        
        if (player->isBusted()) {
            std::cout << player->getName() << " busts! Dealer wins.\n";
        } else if (dealer.isBusted()) {
            std::cout << player->getName() << " wins! Dealer busts.\n";
        } else if (playerHasBlackjack && !dealerHasBlackjack) {
            std::cout << player->getName() << " wins with BLACKJACK!\n";
        } else if (!playerHasBlackjack && dealerHasBlackjack) {
            std::cout << player->getName() << " loses! Dealer has BLACKJACK!\n";
        } else if (playerHasBlackjack && dealerHasBlackjack) {
            std::cout << player->getName() << " pushes with dealer (both have BLACKJACK).\n";
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