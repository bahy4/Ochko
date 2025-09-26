#pragma once
#include "Deck.hpp"
#include "Player.hpp"
#include "Dealer.hpp"
#include <vector>
#include <memory>
#include <random>

class HumanPlayer : public Player {
public:
    HumanPlayer(const std::string& name) : Player(name) {}
    
    bool wantsHit() const override;
};

class Game {
public:
    Game();
    void addPlayer(std::unique_ptr<Player> player);
    void playRound();
    void displayGameState() const;
    
private:
    Deck deck;
    Dealer dealer;
    std::vector<std::unique_ptr<Player>> players;
    bool playersTurn;
    mutable std::mt19937 rng; 
    
    void dealInitialCards();
    void playPlayerTurns();
    void playDealerTurn();
    void calculateProbabilities() const;
    void determineWinners() const;

    double simulatePlayerWinProbability(const Player* player) const;
    int simulateDealerFinalScore(int num) const;
    std::vector<Card> getRemainingCards() const;
};