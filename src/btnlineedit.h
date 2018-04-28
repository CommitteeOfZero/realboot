#pragma once

#include <QLineEdit>

class BtnLineEdit : public QLineEdit {
    Q_OBJECT

   public:
    explicit BtnLineEdit(QWidget *parent = 0) : QLineEdit(parent) {
        setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
        setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        setFixedSize(56, 24);
        setReadOnly(true);
    }
    ~BtnLineEdit() {}
};