#include "logged.h"
#include "ui_logged.h"

logged::logged(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::logged)
{
    ui->setupUi(this);
}

logged::~logged()
{
    delete ui;
}
