#pragma once
#include "Card.hpp"
#include <vector>

class Hand {
public:
    void addCard(Card card);
    int getValue() const;
    void clear();
    bool isBusted() const;
    bool hasBlackjack() const;
    std::string toString() const;
    const std::vector<Card>& getCards() const;
    
private:
    std::vector<Card> cards;
};