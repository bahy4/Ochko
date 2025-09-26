#pragma once
#include "Player.hpp"

class Dealer : public Player {
public:
    Dealer();
    bool wantsHit() const override;
};