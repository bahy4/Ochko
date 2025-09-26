#include "Card.hpp"
#include <map>

Card::Card(Suit s, Rank r) : suit(s), rank(r) {}

int Card::getValue() const {
    if (static_cast<int>(rank) >= 10) return 10;
    return static_cast<int>(rank);
}

std::string Card::toString() const {
    static std::map<Suit, std::string> suitStrings = {
        {Suit::HEARTS, "H"}, {Suit::DIAMONDS, "D"},
        {Suit::CLUBS, "C"}, {Suit::SPADES, "S"}
    };
    
    static std::map<Rank, std::string> rankStrings = {
        {Rank::ACE, "A"}, {Rank::JACK, "J"},
        {Rank::QUEEN, "Q"}, {Rank::KING, "K"}
    };
    
    std::string rankStr;
    if (rank == Rank::ACE || rank == Rank::JACK || 
        rank == Rank::QUEEN || rank == Rank::KING) {
        rankStr = rankStrings[rank];
    } else {
        rankStr = std::to_string(static_cast<int>(rank));
    }
    
    return rankStr + suitStrings[suit];
}