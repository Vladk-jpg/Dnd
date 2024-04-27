#include "combat.h"

Combat::Combat(Player *player)
{
    this->player = player;
    enemy = new Entity;
    phase = NO_ROLL;
}

void Combat::start(Entity *entity)
{
    enemy->setup(entity);
    emit sendText("Вы вступили в битву с \"" + enemy->getName()
                  + "\"\nБросьте D20 чтобы определить, кто ходит первым");
    needRoll = D20;
    playerLastRoll = 0;
    enemyLastRoll = 0;
    yourTurn = true;
    phase = INITIATIVE_ROLL;
    round = 1;
}

int Combat::bonus()
{
    if (player->getGameClass() == "Варвар" || player->getGameClass() == "Воин"
        || player->getGameClass() == "Паладин" || player->getGameClass() == "Следопыт") {
        return player->getMod(STRENGTH);
    } else if (player->getGameClass() == "Бард") {
        return player->getMod(CHARISMA);
    } else if (player->getGameClass() == "Жрец" || player->getGameClass() == "Чародей"
               || player->getGameClass() == "Колдун" || player->getGameClass() == "Волшебник") {
        return player->getMod(INTELLEGENCE);
    } else if (player->getGameClass() == "Плут") {
        return player->getMod(DEXTERITY);
    } else if (player->getGameClass() == "Друид") {
        return player->getMod(WISDOM);
    } else {
        return 0;
    }
}

void Combat::handleRoll(int type, int roll)
{
    if (type == needRoll) {
        if (phase == INITIATIVE_ROLL) {
            needRoll = D20;
            if (yourTurn) {
                playerLastRoll = roll + player->getMod(DEXTERITY);
                emit sendText("Ваша инициатива " + QString::number(roll) + " + "
                              + QString::number(player->getMod(DEXTERITY)) + " = "
                              + QString::number(playerLastRoll));
                yourTurn = false;
                emit enemyRoll(needRoll);

            } else {
                enemyLastRoll = roll + enemy->getMod(DEXTERITY);
                emit sendText("Инициатива врага " + QString::number(roll) + " + "
                              + QString::number(enemy->getMod(DEXTERITY)) + " = "
                              + QString::number(enemyLastRoll));
                if (playerLastRoll >= enemyLastRoll) {
                    yourTurn = true;
                    firstStep = true;
                    emit sendText(
                        "\nВаша инициатива больше, поэтому вы ходите первым \nБросайте D20 на "
                        "попадание");

                } else {
                    emit sendText("\nВаша инициатива меньше, первым ходит соперник");
                    yourTurn = false;
                    firstStep = false;
                    emit enemyRoll(needRoll);
                }
                phase = HIT_ROLL;
            }
        } else if (phase == HIT_ROLL) {
            if (yourTurn) {
                emit sendText("\nРаунд " + QString::number(round) + ". Атака игрока "
                              + player->getName() + ":");
                playerLastRoll = roll + bonus();
                if (playerLastRoll >= enemy->getDefence()) {
                    emit sendText("Попадание! Бросайте D" + QString::number(player->getDamage())
                                  + " на атаку");
                    needRoll = player->getDamage();
                    phase = DAMAGE_ROLL;
                } else {
                    emit sendText("Промах");
                    yourTurn = false;
                    needRoll = D20;
                    emit enemyRoll(needRoll);
                    if (!firstStep) {
                        round++;
                    }
                }
            } else {
                emit sendText("\nРаунд " + QString::number(round) + ". Атака " + enemy->getName()
                              + ":");
                enemyLastRoll = roll + enemy->getMod(STRENGTH);
                if (enemyLastRoll >= player->getDefence()) {
                    emit sendText("Попадание!");
                    phase = DAMAGE_ROLL;
                    needRoll = enemy->getDamage();
                    emit enemyRoll(needRoll);
                } else {
                    emit sendText("Промах \n\nБросайте D20 на попадание");
                    yourTurn = true;
                    needRoll = D20;
                    if (firstStep) {
                        round++;
                    }
                }
            }
        } else if (phase == DAMAGE_ROLL) {
            if (yourTurn) {
                enemy->getHeart(roll + bonus());
                emit sendText("вы нанесли " + QString::number(roll) + " + "
                              + QString::number(bonus()) + " = " + QString::number(roll + bonus())
                              + " урона");
                if (!enemy->isAlive()) {
                    phase = NO_ROLL;
                    emit sendText("\nПоздравляю, вы победили \"" + enemy->getName() + "\"!");
                    player->addExp(enemy->getDanger() * 200);
                    emit sendText("Вы получили " + QString::number(enemy->getDanger() * 200)
                                  + " опыта");
                    emit fightEnd(enemy->getName());
                    //дать рандомную награду
                } else {
                    phase = HIT_ROLL;
                    yourTurn = false;
                    needRoll = D20;
                    emit enemyRoll(needRoll);
                }
                if (!firstStep) {
                    round++;
                }
            } else {
                player->getHeart(roll + enemy->getMod(STRENGTH));
                emit sendText("Вам нанесли " + QString::number(roll) + " + "
                              + QString::number(enemy->getMod(STRENGTH)) + " = "
                              + QString::number(roll + enemy->getMod(STRENGTH)) + " урона");
                if (!player->isAlive()) {
                    phase = NO_ROLL;
                    emit gameOver();
                } else {
                    phase = HIT_ROLL;
                    yourTurn = true;
                    needRoll = D20;
                    emit sendText("\nБросайте D20 на попадание");
                }
                if (firstStep) {
                    round++;
                }
            }
        }

    } else {
        if (phase != NO_ROLL) {
            emit sendText("\n---------------------------------\nВы бросили не ту кость!(D"
                          + QString::number(type) + ")\nВам нужен D" + QString::number(needRoll)
                          + "\n---------------------------------\n");
        }
    }
}
