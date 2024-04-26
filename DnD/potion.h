#ifndef POTION_H
#define POTION_H

#include "item.h"

class Potion : public Item
{
public:
    Potion();
    Potion(QString name, QString descrip, int heal, int type);
    ~Potion() override = default;

    int getHeal();

    int use() override;
    void copy(Potion &other);

private:
    int heal;
};

#endif // POTION_H
