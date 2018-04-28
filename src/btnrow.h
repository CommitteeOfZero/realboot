#pragma once

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QMap>
#include "controllerconfig.h"
#include "btnlineedit.h"

extern QMap<ControllerConfig::Bind, QString> bindLabelTexts;

class BtnRow : public QWidget {
    Q_OBJECT

   public:
    explicit BtnRow(ControllerConfig::Bind bind, QWidget *parent = 0)
        : QWidget(parent) {
        _bind = bind;
        QHBoxLayout *lay = new QHBoxLayout;
        lay->setSpacing(0);
        lay->setMargin(0);
        lay->setAlignment(Qt::AlignVCenter);
        QLabel *lbl = new QLabel(bindLabelTexts[bind]);
        lay->addWidget(lbl);
        lay->addStretch(1);
        _le = new BtnLineEdit(this);
        lay->addWidget(_le);
        setLayout(lay);
    }
    ~BtnRow() {}

    ControllerConfig::Bind bind() const { return _bind; }
    BtnLineEdit *le() { return _le; }

   private:
    ControllerConfig::Bind _bind;
    BtnLineEdit *_le;
};