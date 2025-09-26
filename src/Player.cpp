#include "Player.hpp"

Player::Player(const std::string& name) : name(name) {}

void Player::receiveCard(Card card) {
    hand.addCard(card);
}

void Player::clearHand() {
    hand.clear();
}

std::string Player::getName() const {
    return name;
}

const Hand& Player::getHand() const {
    return hand;
}

bool Player::isBusted() const {
    return hand.isBusted();
}