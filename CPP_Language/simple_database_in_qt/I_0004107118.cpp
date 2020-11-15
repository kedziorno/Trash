#ifndef ADDDATABASE_H
#define ADDDATABASE_H

#include "mainwindow.h"

#include <QDialog>

namespace Ui {
class addDatabase;
}

class addDatabase : public QDialog
{
    Q_OBJECT

public:
    explicit addDatabase(QWidget *parent = 0);
    ~addDatabase();

private slots:
    void on_editAddDatabase_textEdited(const QString &arg1);

    void on_buttonAddDatabase_clicked();

private:
    Ui::addDatabase *ui;

    // for parent window
    class MainWindow *parentMW;
};

#endif // ADDDATABASE_H

