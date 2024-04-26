#ifndef WORLD_H
#define WORLD_H

#include "armor.h"
#include "entity.h"
#include "potion.h"
#include "weapon.h"

class World
{
public:
    World();

    Weapon *longSword = new Weapon("Длинный меч", "Урон 1D8", D8, DAMAGE);

    Armor *chain = new Armor("Кольчуга", "Защита 16", 16, DEFENCE);
    Armor *leatherArmor = new Armor("Кожаная броня", "Защита 12", 12, DEFENCE);

    Entity *goblin = new Entity("Голлум", "Гоблин", 7, -1, 2, 0, 0, -1, -1, 4);

    Potion *SmallHealPotion = new Potion("Малое лечебное зелье", "Восстанавливает 4 HP", 4, HEAL);
};

#endif // WORLD_H
