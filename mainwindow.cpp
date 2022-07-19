#include "mainwindow.h"
#include <QGridLayout>
#include <QGroupBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QGroupBox *mainWidget = new QGroupBox("Chat");
    QGridLayout *mainLayout = new QGridLayout;

    w_chat = new ChatUI;
    mainLayout->addWidget(w_chat);

    mainWidget->setLayout(mainLayout);
    this->setCentralWidget(mainWidget);
}

MainWindow::~MainWindow()
{
}
