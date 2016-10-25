#include "newformdialog.h"
#include "ui_newformdialog.h"

newformdialog::newformdialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::newformdialog)
{
    ui->setupUi(this);
    ui->width_textbox->setFocus();
    ui->width_textbox->setValidator(new QIntValidator(1,250));
    ui->height_textbox->setValidator((new QIntValidator(1,250)));
}

newformdialog::~newformdialog()
{
    delete ui;
}

void newformdialog::on_okay_button_clicked()
{
    int width = (ui->width_textbox->text()).toInt();
    int height = (ui->height_textbox->text()).toInt();
    if(width == 0 )
    {
        width = 50;
    }
    if(height == 0)
    {
        height = 50;
    }

    QColor color;
    if(ui->white_background_button->isChecked())
    {
        color = QColor(255, 255, 255, 255);
    }
    else if(ui->black_background_button->isChecked())
    {
        color = QColor(0,0,0,255);
    }
    else
    {
        color = QColor(0,0,0,0);
    }

    emit SendNewFormInfo(width, height, color);
    this->close();
}

void newformdialog::on_cancel_button_clicked()
{
    this->close();
}
