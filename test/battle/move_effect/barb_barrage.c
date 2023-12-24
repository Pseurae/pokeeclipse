#include "global.h"
#include "test/battle.h"

ASSUMPTIONS
{
    ASSUME(gBattleMoves[MOVE_BARB_BARRAGE].effect == EFFECT_BARB_BARRAGE);
    ASSUME(MoveHasMoveEffect(MOVE_BARB_BARRAGE, MOVE_EFFECT_POISON, FALSE) == TRUE);
}

SINGLE_BATTLE_TEST("Barb Barrage inflicts poison")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_BARB_BARRAGE); }
        TURN {}
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_BARB_BARRAGE, player);
        HP_BAR(opponent);
        ANIMATION(ANIM_TYPE_STATUS, B_ANIM_STATUS_PSN, opponent);
        STATUS_ICON(opponent, poison: TRUE);
    }
}

SINGLE_BATTLE_TEST("Barb Barrage's power doubles if the target is poisoned/badly poisoned", s16 damage)
{
    u32 status1;
    PARAMETRIZE { status1 = 0; }
    PARAMETRIZE { status1 = STATUS1_POISON; }
    PARAMETRIZE { status1 = STATUS1_TOXIC_POISON; }
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET) { Status1(status1); }
    } WHEN {
        TURN { MOVE(player, MOVE_BARB_BARRAGE); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_BARB_BARRAGE, player);
        HP_BAR(opponent, captureDamage: &results[i].damage);
    } FINALLY {
        EXPECT_MUL_EQ(results[0].damage, Q_4_12(2.0), results[1].damage);
        EXPECT_MUL_EQ(results[0].damage, Q_4_12(2.0), results[2].damage);
    }
}
