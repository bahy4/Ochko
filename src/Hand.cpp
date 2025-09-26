#include "Hand.hpp"

void Hand::addCard(Card card) {
    cards.push_back(card);
}

int Hand::getValue() const {
    int value = 0;
    int aces = 0;
    
    for (const auto& card : cards) {
        value += card.getValue();
        if (card.getValue() == 1) aces++;
    }
    
    while (aces > 0 && value + 10 <= 21) {
        value += 10;
        aces--;
    }
    
    return value;
}

void Hand::clear() {
    cards.clear();
}

bool Hand::isBusted() const {
    return getValue() > 21;
}

bool Hand::hasBlackjack() const {
    return cards.size() == 2 && getValue() == 21;
}

std::string Hand::toString() const {
    std::string result;
    for (const auto& card : cards) {
        result += card.toString() + " ";
    }
    return result;
}

const std::vector<Card>& Hand::getCards() const {
    return cards;
}