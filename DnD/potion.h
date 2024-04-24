#ifndef POTION_H
#define POTION_H

#include "item.h"

class Potion : public Item
{
public:
    Potion();
    ~Potion() override = default;
};

#endif // POTION_H
