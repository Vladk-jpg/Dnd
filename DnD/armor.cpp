#include "armor.h"

Armor::Armor() = default;

Armor::Armor(QString armorName, QString descrip, int def, int type)
    : defence(def)
{
    this->name = armorName;
    description = descrip;
    this->type = type;
}

int Armor::getDef()
{
    return defence;
}

void Armor::setDef(int def)
{
    defence = def;
}

void Armor::copy(Armor &other)
{
    this->name = other.name;
    this->defence = other.defence;
    this->description = other.description;
    this->type = other.type;
}

int Armor::use()
{
    return defence;
}
