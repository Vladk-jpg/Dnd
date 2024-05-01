#ifndef CONSTANTS_H
#define CONSTANTS_H

enum modificators { STRENGTH, DEXTERITY, CONSTITUTION, INTELLEGENCE, WISDOM, CHARISMA };

enum gameStates { CREATION, FIGHT, DIALOG, FREE };

enum itemTypes { HEAL, DAMAGE, DEFENCE, TRINKET };

enum battleRolls { INITIATIVE_ROLL, HIT_ROLL, DAMAGE_ROLL, NO_ROLL };

enum placeTypes { CITY, BUILDING, ENVIRONMENT };

enum events {
    NO_EVENT,
    STRENGTH_EVENT,
    DEXTERITY_EVENT,
    CONSTITUTION_EVENT,
    INTELLEGENCE_EVENT,
    WISDOM_EVENT,
    CHARISMA_EVENT,
    FIGHT_EVENT,
    TRAP_EVENT,
    PRICE_EVENT,
    END_EVENT
};

#define DEFAULT_ARM 10;

const int D4 = 4;
const int D6 = 6;
const int D8 = 8;
const int D10 = 10;
const int D12 = 12;
const int D20 = 20;
const int MAX_LVL = 20;
const int DEF_EXP = 300;

#endif // CONSTANTS_H
