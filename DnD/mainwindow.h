#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDebug>
#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QMainWindow>

#include "armor.h"
#include "combat.h"
#include "creationform.h"
#include "dice.h"
#include "menu.h"
#include "player.h"
#include "weapon.h"
#include "world.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void closeWindow();
    void closeMenu();
    void handleDiceRoll(int, int, bool);
    void handlePlayerCreate(Player *);
    void on_menuButton_clicked();
    void on_rollButton_clicked();
    void on_enterButton_clicked();
    void updateWindow();
    void handleTextReceived(QString);
    void handleEnemyRoll(int);
    void handleGameOver();

    void on_useButton_clicked();

private:
    Ui::MainWindow *ui;
    Menu *menu;
    CreationForm *form;
    Dice *dice;
    World *world = new World;
    bool canRoll = true;
    QPair<int, int> currRoll;
    QGraphicsScene *scene;
    Player *player = new Player();
    bool isFight = false;
    int gameState{CREATION};
    QTimer timer;
    Combat *combat;
signals:
    void DiceRolled(int, int);
};
#endif // MAINWINDOW_H
