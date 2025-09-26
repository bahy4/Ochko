#pragma once
#include "Card.hpp"
#include <vector>
#include <random>

class Deck {
public:
    Deck();
    void shuffle(int a);
    Card drawCard();
    bool isEmpty() const;
    int remainingCards() const;
    
private:
    std::vector<Card> cards;
    size_t currentIndex;
    std::mt19937 rng;
};