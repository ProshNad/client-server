#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialog.h"
#include "dialog2.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{

    Dialog *dialog = new Dialog();
    dialog->show();
}

void MainWindow::on_pushButton_2_clicked()
{
   Dialog2 *dial = new Dialog2();
   dial->show();
}
