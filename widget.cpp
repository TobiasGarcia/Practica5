#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    width_game = 596;
    height_game = 646;

    layout()->setMargin(0);
    setFixedSize(width_game, height_game);

    ui->graphicsView->setBackgroundBrush(QBrush(Qt::black, Qt::SolidPattern));

    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    pacman = new Pacman(width_game, height_game);
    ui->graphicsView->setScene(pacman);
}

Widget::~Widget() {
    delete pacman;
    delete ui;
}

