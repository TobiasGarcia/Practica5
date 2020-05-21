#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    layout()->setMargin(0);
    setFixedSize(width_game, height_game);

    ui->graphicsView->setBackgroundBrush(QBrush(Qt::darkCyan, Qt::SolidPattern));

    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    ui->graphicsView->setScene(pacman);
}

Widget::~Widget() {
    delete pacman;
    delete ui;
}

