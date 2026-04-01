#include "ThreeDWindow.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QWidget>

ThreeDWindow::ThreeDWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("3D View");
    resize(900, 600);

    QWidget *central = new QWidget(this);
    setCentralWidget(central);

    QLabel *label = new QLabel("3D View Screen", this);
    label->setAlignment(Qt::AlignCenter);

    label->setStyleSheet(
        "QLabel {"
        "color: white;"
        "font-size: 28px;"
        "font-weight: bold;"
        "}"
        );

    QVBoxLayout *layout = new QVBoxLayout(central);
    layout->addWidget(label);

    central->setStyleSheet(
        "background-color: #111827;"
        );
}
