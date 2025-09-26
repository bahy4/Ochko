#pragma once
#include <string>

enum class Suit {
    HEARTS, DIAMONDS, CLUBS, SPADES
};

enum class Rank {
    ACE = 1, TWO, THREE, FOUR, FIVE, SIX, SEVEN,
    EIGHT, NINE, TEN, JACK, QUEEN, KING
};

class Card {
public:
    Card(Suit suit, Rank rank);
    int getValue() const;
    std::string toString() const;
    
private:
    Suit suit;
    Rank rank;
};