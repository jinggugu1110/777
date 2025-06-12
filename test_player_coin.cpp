#include "test_framework.h"
#include "player_coin.h"

TEST_CASE(TestCoinUp) {
    Player p{true, 10, 0};
    PlayerCoinUp(p, 5);
    REQUIRE(p.coin == 15);
}

TEST_CASE(TestCoinDown) {
    Player p{true, 20, 0};
    PlayerCoinDown(p, 7);
    REQUIRE(p.coin == 13);
}

TEST_CASE(TestCoinPay) {
    Player p{true, 100, 1};
    PlayerCoinPay(p);
    REQUIRE(p.isPay == 0);
    REQUIRE(p.coin == 50);
}

int main() {
    return run_tests();
}
