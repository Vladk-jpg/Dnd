#include "item.h"

Item::Item() = default;

Item::Item(QString itemName, QString descrip, int type)
    : name(itemName)
    , description(descrip)
    , type(type)
{}

QString Item::getName()
{
    return name;
}

void Item::setName(QString name)
{
    this->name = name;
}

QString Item::getDescription()
{
    return description;
}

void Item::setDescription(QString descrip)
{
    description = descrip;
}

void Item::setType(int type)
{
    this->type = type;
}

int Item::getType()
{
    return type;
}

int Item::use()
{
    qDebug() << this->getName();
    return 0;
}

void Item::copy(Item &other)
{
    this->name = other.name;
    this->description = other.description;
    this->type = other.type;
}
