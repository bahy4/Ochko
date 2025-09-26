#pragma once
#include "Hand.hpp"
#include <string>

class Player {
public:
    Player(const std::string& name);
    virtual ~Player() = default;
    
    virtual bool wantsHit() const = 0;
    void receiveCard(Card card);
    void clearHand();
    std::string getName() const;
    const Hand& getHand() const;
    bool isBusted() const;
    
protected:
    std::string name;
    Hand hand;
};