#ifndef NEWFORMDIALOG_H
#define NEWFORMDIALOG_H

#include <QDialog>
#include <QWidget>

namespace Ui {
class newformdialog;
}

class newformdialog : public QDialog
{
    Q_OBJECT

public:
    explicit newformdialog(QWidget *parent = 0);
    ~newformdialog();

signals:
    void SendNewFormInfo(int width, int height, QColor color);

private slots:

    void on_okay_button_clicked();

    void on_cancel_button_clicked();

private:
    Ui::newformdialog *ui;
};

#endif // NEWFORMDIALOG_H
