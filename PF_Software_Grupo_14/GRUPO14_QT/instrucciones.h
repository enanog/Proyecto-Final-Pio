#ifndef INSTRUCCIONES_H
#define INSTRUCCIONES_H

#include <QDialog>
#include <QPixmap>

namespace Ui {
class Instrucciones;
}

class Instrucciones : public QDialog
{
    Q_OBJECT

public:
    explicit Instrucciones(QWidget *parent = nullptr);
    ~Instrucciones();

private slots:
    void on_pushButton_clicked();

private:
    Ui::Instrucciones *ui;
};

#endif // INSTRUCCIONES_H
