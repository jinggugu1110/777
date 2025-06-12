#ifndef PLAYER_COIN_H
#define PLAYER_COIN_H

struct Player {
    bool live;
    int coin;
    bool isPay;
};

void PlayerCoinUp(Player& player, int enemy_life);
void PlayerCoinDown(Player& player, int enemy_life);
void PlayerCoinPay(Player& player);

#endif
