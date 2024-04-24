#include "weapon.h"

Weapon::Weapon() = default;

Weapon::Weapon(QString name, QString descrip, int damage, int type)
    : damage(damage)
{
    this->name = name;
    description = descrip;
    this->type = type;
}

int Weapon::getDam()
{
    return damage;
}

void Weapon::setDam(int damage)
{
    this->damage = damage;
}

void Weapon::copy(Weapon &other)
{
    this->name = other.name;
    this->damage = other.damage;
    this->description = other.description;
    this->type = other.type;
}

int Weapon::use()
{
    return damage;
}
