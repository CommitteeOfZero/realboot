#include "graphicstab.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>

GraphicsTab::GraphicsTab(QWidget *parent) : QWidget(parent) {
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setSpacing(8);
    setLayout(mainLayout);

    QSizePolicy colSp(QSizePolicy::Preferred, QSizePolicy::Preferred);
    colSp.setHorizontalStretch(1);

    QWidget *left = new QWidget(this);
    left->setSizePolicy(colSp);
    QVBoxLayout *leftLayout = new QVBoxLayout(this);
    left->setLayout(leftLayout);
    leftLayout->setSpacing(8);
    leftLayout->setMargin(0);
    mainLayout->addWidget(left);

    QLabel *leftLabel = new QLabel(left);
    leftLabel->setText("<b>Left label</b>");
    leftLayout->addWidget(leftLabel);

    QWidget *right = new QWidget(this);
    right->setSizePolicy(colSp);
    QVBoxLayout *rightLayout = new QVBoxLayout(this);
    right->setLayout(rightLayout);
    rightLayout->setSpacing(8);
    rightLayout->setMargin(0);
    mainLayout->addWidget(right);

    QLabel *rightLabel = new QLabel(right);
    rightLabel->setText("<b>Right label</b>");
    rightLayout->addWidget(rightLabel);

    leftLayout->addStretch(1);
    rightLayout->addStretch(1);
}

void GraphicsTab::setConfig() {}