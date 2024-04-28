#include "creationform.h"
#include "ui_creationform.h"

CreationForm::CreationForm(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::CreationForm)
{
    world = new World;
    ui->setupUi(this);
    //setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
    setWindowFlags(Qt::FramelessWindowHint | Qt::Window | Qt::WindowStaysOnTopHint);
}

CreationForm::~CreationForm()
{
    delete ui;
}

void CreationForm::on_createButton_clicked()
{
    QString name = ui->name->text();
    QString race = ui->race->currentText();
    QString gameClass = ui->gameClass->currentText();
    int str = 0, dex = 0, con = 0, intel = 0, wis = 0, cha = 0, health = 0;

    str = QRandomGenerator::global()->bounded(-2, 3);
    dex = QRandomGenerator::global()->bounded(-2, 3);
    con = QRandomGenerator::global()->bounded(-2, 3);
    intel = QRandomGenerator::global()->bounded(-2, 3);
    wis = QRandomGenerator::global()->bounded(-2, 3);
    cha = QRandomGenerator::global()->bounded(-2, 3);

    if (gameClass == "Варвар") {
        health = D12;
    } else if (gameClass == "Бард") {
        health = D8;
    } else if (gameClass == "Жрец") {
        health = D8;
    } else if (gameClass == "Друид") {
        health = D8;
    } else if (gameClass == "Воин") {
        health = D10;
        player->inventory.push_back(world->longSword);
        player->inventory.push_back(world->chain);
        player->inventory.push_back(world->SmallHealPotion);
        player->setDamage(player->inventory[0]->use());
        player->setDefence(player->inventory[1]->use());
        str = 2;
        intel = -3;
    } else if (gameClass == "Монах") {
        health = D8;
    } else if (gameClass == "Паладин") {
        health = D10;
    } else if (gameClass == "Следопыт") {
        health = D10;
    } else if (gameClass == "Плут") {
        health = D8;
    } else if (gameClass == "Чародей") {
        health = D6;
    } else if (gameClass == "Колдун") {
        health = D8;
    } else if (gameClass == "Волшебник") {
        health = D6;
    }

    //раса
    if (race == "Человек") {
    } else if (race == "Дварф") {
        str += 2;
        con += 2;
    } else if (race == "Эльф") {
        dex += 2;
        intel += 1;
        wis += 1;
    } else if (race == "Полурослик") {
        dex += 2;
        con += 1;
    } else if (race == "Драконорожденный") {
        str += 2;
        cha += 1;
    } else if (race == "Гном") {
        intel += 2;
        con += 1;
    } else if (race == "Полуэльф") {
        cha += 2;
    } else if (race == "Полуорк") {
        str += 2;
        con += 1;
    } else if (race == "Тифлинг") {
        intel += 1;
        cha += 2;
    }
    health += con;

    player->setHealth(health);
    player->setGameClass(gameClass);
    player->setRace(race);
    player->setName(name);
    player->setMod(STRENGTH, str);
    player->setMod(DEXTERITY, dex);
    player->setMod(CONSTITUTION, con);
    player->setMod(INTELLEGENCE, intel);
    player->setMod(WISDOM, wis);
    player->setMod(CHARISMA, cha);
    emit Completed(player);
}
