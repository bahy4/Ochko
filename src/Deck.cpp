#include "Deck.hpp"
#include <algorithm>

Deck::Deck() : currentIndex(0), rng(std::random_device{}()) {
    for (int s = 0; s < 4; ++s) {
        for (int r = 1; r <= 13; ++r) {
            cards.emplace_back(static_cast<Suit>(s), static_cast<Rank>(r));
        }
    }
    shuffle();
}

void Deck::shuffle() {
    std::shuffle(cards.begin(), cards.end(), rng);
    currentIndex = 0;
}

Card Deck::drawCard() {
    return cards[currentIndex++];
}

bool Deck::isEmpty() const {
    return currentIndex >= cards.size();
}

int Deck::remainingCards() const {
    return cards.size() - currentIndex;
}