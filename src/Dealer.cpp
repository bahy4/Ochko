#include "Dealer.hpp"

Dealer::Dealer() : Player("Dealer") {}

bool Dealer::wantsHit() const {
    return hand.getValue() < 17;
}