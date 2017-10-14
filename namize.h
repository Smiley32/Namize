#ifndef NAMIZE_H
#define NAMIZE_H

#include <QMainWindow>

namespace Ui {
class Namize;
}

class Namize : public QMainWindow
{
    Q_OBJECT

public:
    explicit Namize(QWidget *parent = 0);
    ~Namize();

private slots:
    void on_pushButton_clicked();

private:
    Ui::Namize *ui;
};

#endif // NAMIZE_H
