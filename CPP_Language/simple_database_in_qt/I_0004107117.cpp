#include "adddatabase.h"
#include "ui_adddatabase.h"
#include "database.h"

addDatabase::addDatabase(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addDatabase)
{
    ui->setupUi(this);

    // set info
    ui->labelAddDatabase->setText("Podaj nazwę bazy");

    this->parentMW = MW;
}

addDatabase::~addDatabase()
{
    delete ui;
}

void addDatabase::on_editAddDatabase_textEdited(const QString &arg1)
{
    // if we have some text
    if(arg1.length())
        ui->labelAddDatabase->setText("");
}

void addDatabase::on_buttonAddDatabase_clicked()
{
    // if database name field is empty
    if(ui->editAddDatabase->text().isEmpty())
    {
        ui->labelAddDatabase->setText("Nie podałeś nazwy bazy !");

        // we do nothing
        return;
    }

    // or setup connection
    QString dbName(ui->editAddDatabase->text());

    globalSqliteDb = new sqliteDB();
    globalSqliteDb->setName(dbName);
    globalSqliteDb->openConnection();

    // after connection we fill model object and the table
    this->parentMW->fillTableMain();

    // kick this window
    this->destroy();
}

