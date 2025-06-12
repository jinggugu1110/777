#include "player_coin.h"

void PlayerCoinUp(Player& player, int enemy_life)
{
    if (player.live) {
        player.coin += enemy_life;
    }
}

void PlayerCoinDown(Player& player, int enemy_life)
{
    if (player.live) {
        player.coin -= enemy_life;
    }
}

void PlayerCoinPay(Player& player)
{
    if (player.live && player.isPay == 1) {
        player.isPay = 0;
        player.coin -= 50;
    }
}
