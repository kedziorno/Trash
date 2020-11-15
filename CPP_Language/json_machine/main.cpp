#include <QCoreApplication>

#include <QDebug>
#include <QFile>
#include <QByteArray>
#include <QDate>

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QJsonValue>
#include <QJsonValuePtr>
#include <QJsonValueRef>
#include <QJsonValueRefPtr>

class JsonD540;
class D540ObjectJson;
class D540Card;
class D540Machines;

class D540Machines {
public:
    D540Machines(quint32 mId);

    quint32 get();

private:
    quint32 machineId;
};

D540Machines::D540Machines(quint32 mId) {
    machineId = mId;
}

quint32 D540Machines::get() {
    return machineId;
}

class D540Card {
public:
    D540Card(QDate &DateTo, int Id, int Number);

    QDate       getDateTo();
    quint32     Id();
    quint32     Number();

private:
    QDate       DateTo;
    quint32     ID;
    quint32     NumberCard;
};

D540Card::D540Card(QDate &dt, int id, int number) {
    DateTo = dt;
    ID = id;
    NumberCard = number;
}

QDate D540Card::getDateTo() {
    return DateTo;
}

quint32 D540Card::Id() {
    return ID;
}

quint32 D540Card::Number() {
    return NumberCard;
}

class D540ObjectJson {
public:
    D540ObjectJson();
    //D540ObjectJson(quint32 &AccessGroupId, QList<D540Card> &Card, \
                   QDate &DateTo, quint32 &DefaultMpkId, QString FirstName, \
                   quint32 &Id, QString &Language, QString &Lastname, \
                   QString &Login, QList<D540Machines> &Machines, \
                   QString &Password, quint32 &Privileges);

    bool setAccessGroupId(quint32 value);
    bool setCard(D540Card &value);
    bool setDateTo(QDate &value);
    bool setDefaultMpkId(quint32 value);
    bool setFirstName(QString value);
    bool setId(quint32 value);
    bool setLanguage(QString value);
    bool setLastName(QString value);
    bool setLogin(QString value);
    bool setMachine(D540Machines value);
    bool setPassword(QString value);
    bool setPrivileges(quint32 value);

    quint32         getAccessGroupId();
    QList<D540Card> getCard();
    QDate           getDateTo();
    quint32         getDefaultMpkId();
    QString         getFirstName();
    quint32         getId();
    QString         getLanguage();
    QString         getLastName();
    QString         getLogin();
    QList<D540Machines>    getMachine();
    QString         getPassword();
    quint32         getPrivileges();

private:
    quint32             AccessGroupId;
    QList<D540Card>     Card;
    QDate               DateTo;
    quint32             DefaultMpkId;
    QString             FirstName;
    quint32             Id;
    QString             Language;
    QString             LastName;
    QString             Login;
    QList<D540Machines> Machines;
    QString             Password;
    quint32             Privileges;
};

//D540ObjectJson::D540ObjectJson(quint32 &AccessGroupId, QList<D540Card> &Card, \
                               QDate &DateTo, quint32 &DefaultMpkId, \
                               QString FirstName, quint32 &Id, \
                               QString &Language, QString &Lastname, \
                               QString &Login, QList<D540Machines> &Machines, \
                               QString &Password, quint32 &Privileges) { }

D540ObjectJson::D540ObjectJson() { }

bool D540ObjectJson::setAccessGroupId(quint32 value) {
    AccessGroupId = value;
    return true;
}

bool D540ObjectJson::setCard(D540Card &value) {
    Card.append(value);
    return true;
}

bool D540ObjectJson::setDateTo(QDate &value) {
    DateTo = value;
    return true;
}

bool D540ObjectJson::setDefaultMpkId(quint32 value) {
    DefaultMpkId = value;
    return true;
}

bool D540ObjectJson::setFirstName(QString value) {
    FirstName = value;
    return true;
}

bool D540ObjectJson::setId(quint32 value) {
    Id = value;
    return true;
}

bool D540ObjectJson::setLanguage(QString value)  {
    Language = value;
    return true;
}

bool D540ObjectJson::setLastName(QString value) {
    LastName = value;
    return true;
}

bool D540ObjectJson::setLogin(QString value) {
    Login = value;
    return true;
}

bool D540ObjectJson::setMachine(D540Machines value) {
    Machines.append(value);
    return true;
}

bool D540ObjectJson::setPassword(QString value) {
    Password = value;
    return true;
}

bool D540ObjectJson::setPrivileges(quint32 value) {
    Privileges = value;
    return true;
}

quint32 D540ObjectJson::getAccessGroupId() {
    qDebug() << AccessGroupId;
    return AccessGroupId;
}

QList<D540Card> D540ObjectJson::getCard() {
    QList<D540Card> list = Card;
    for(quint8 i = 0; i < list.length(); i++) {
        D540Card card = list.at(i);
        qDebug() << card.getDateTo() << card.Id() << card.Number();
    }
    return Card;
}

QDate D540ObjectJson::getDateTo() {
    qDebug() << DateTo;
    return DateTo;
}

quint32 D540ObjectJson::getDefaultMpkId() {
    qDebug() << DefaultMpkId;
    return DefaultMpkId;
}

QString D540ObjectJson::getFirstName() {
    qDebug() << FirstName;
    return FirstName;
}

quint32 D540ObjectJson::getId() {
    qDebug() << Id;
    return Id;
}

QString D540ObjectJson::getLanguage() {
    qDebug() << Language;
    return Language;
}

QString D540ObjectJson::getLastName() {
    qDebug() << LastName;
    return LastName;
}

QString D540ObjectJson::getLogin() {
    qDebug() << Login;
    return Login;
}

QList<D540Machines> D540ObjectJson::getMachine() {
    QList<D540Machines> list = Machines;
    for(quint8 i = 0; i < list.length(); i++) {
        D540Machines machine = list.at(i);
        qDebug() << machine.get();
    }
    return Machines;
}

QString D540ObjectJson::getPassword() {
    qDebug() << Password;
    return Password;
}

quint32 D540ObjectJson::getPrivileges() {
    qDebug() << Privileges;
    return Privileges;
}

class JsonD540 {
public:
    JsonD540(const QString &source);
    ~JsonD540();

    bool            convertToJson();
    bool            parseJsonToObjectsList();
    bool            dumpJsonToObjectsList();

    bool            JsonIsNull();

    bool            JsonIsArray();
    quint8          JsonArrayLength();
    QJsonArray*     JsonGetArray();

    bool            JsonIsObject();
    quint8          JsonObjectLength();
    QJsonObject*    JsonGetObject();

    bool            JsonGetKeys();
    bool            JsonDumpKeys();
    bool            JsonInKeysTable(const QString &key, QJsonValue &value);

    bool            JsonDumpObject();
    bool            JsonDumpArray();

private:
    QFile           *file;

    QByteArray      *data;

    QJsonDocument   *jd;
    QJsonObject     jo;
    QJsonArray      ja;

    QStringList     keys;

    QList<D540ObjectJson> objJsonList;

};

/* ************************************************************************* */

JsonD540::JsonD540(const QString &source) {
//    qSetMessagePattern("(%{appname}) [%{file}/%{function}:%{line}] => %{if-debug}%{message}%{endif}");
    qSetMessagePattern("[%{function}:%{line}] => %{if-debug}%{message}%{endif}");
//    qSetMessagePattern("");
    file = new QFile(source);
    if(file->exists()) {
        if(file->open(QIODevice::ReadWrite | QIODevice::Text)) {
            if(file->isOpen()) {
                if(file->isReadable()) {
                    data = new QByteArray();
                    data->append(file->readAll());
                    jd = new QJsonDocument();
                    *jd = jd->fromJson(data->data());
                    parseJsonToObjectsList();
                    if(JsonIsNull()) {
                        qDebug() << "!!! JSON is NULL !!!";
                        if(file->isOpen()) {
                            file->close();
                        } else {
                            qDebug() << "Error: QFile::isOpen() false";
                        }
                    }
                } else {
                    qDebug() << "Error: QFile::isReadable() false";
                }
            } else {
                qDebug() << "Error: QFile::isOpen() false";
            }
        } else {
            qDebug() << "Error: QFile::open() false";
        }
    } else {
        qDebug() << "Error: QFile::exists() false";
    }
}

JsonD540::~JsonD540() {
    if(file->isOpen()) {
        file->close();
    } else {
        qDebug() << "Error: QFile::isOpen() false";
    }
}

bool JsonD540::parseJsonToObjectsList() {
    if(JsonIsObject()) {
        D540ObjectJson oj;

        QJsonValue value;

        JsonInKeysTable("AccessGroupId", value);
        oj.setAccessGroupId(value.toInt());

        JsonInKeysTable("Card", value);
        QJsonArray ArrayCards = value.toArray();
        for(quint8 i = 0; i < ArrayCards.size(); i++) {
            QJsonValue value = ArrayCards.at(i);
            QJsonObject vObject = value.toObject();
            qDebug() << "ArrayCard Object:" << vObject;
//            QStringList date = vObject.value("DateTo").toString().split("-");
//            int year = date.at(0).toInt() - 2000;
//            int month = date.at(1).toInt();
//            int day = date.at(2).toInt();
//qDebug() << year << month << day;

            QString dateString = vObject.value("DateTo").toString();
            QDate date;
            date.fromString(dateString);

            qDebug() << date;
            //D540Card newCard(newDate);
            //oj.setCard(value.toInt());
        }

        JsonInKeysTable("DateTo", value);
        QStringList dateTo = value.toString().split("-");
        quint16 year = dateTo.at(0).toInt();
        quint8 month = dateTo.at(1).toInt();
        quint8 day = dateTo.at(2).toInt();
        QDate newDate(year, month, day);
        oj.setDateTo(newDate);

        JsonInKeysTable("DefaultMpkId", value);
        oj.setDefaultMpkId(value.toInt());

        JsonInKeysTable("FirstName", value);
        oj.setFirstName(value.toString());

        JsonInKeysTable("Id", value);
        oj.setId(value.toInt());

        JsonInKeysTable("Language", value);
        oj.setLanguage(value.toString());

        JsonInKeysTable("LastName", value);
        oj.setLastName(value.toString());

        JsonInKeysTable("Login", value);
        oj.setLogin(value.toString());

        JsonInKeysTable("Machines", value);
        QJsonArray ArrayMachines = value.toArray();
        for(quint8 i = 0; i < ArrayMachines.size(); i++) {
            D540Machines machineId(ArrayMachines.at(i).toInt());
            oj.setMachine(machineId);
        }

        JsonInKeysTable("Password", value);
        oj.setPassword(value.toString());

        JsonInKeysTable("Privilages", value);
        oj.setPrivileges(value.toInt());

        objJsonList.append(oj);

//        JsonGetKeys();

//        QStringList::Iterator start = keys.begin();
//        QStringList::Iterator end = keys.end();

//        while(start != end) {
//            qDebug() << *start;
//            start++;
//        }

    }
    return false;
}

bool JsonD540::dumpJsonToObjectsList() {
    quint16 size = objJsonList.length();
    for(quint16 i = 0; i < size; i++) {
        D540ObjectJson val = objJsonList.at(i);
        val.getAccessGroupId();
        val.getCard();
        val.getDateTo();
        val.getDefaultMpkId();
        val.getFirstName();
        val.getId();
        val.getLanguage();
        val.getLastName();
        val.getLogin();
        val.getMachine();
        val.getPassword();
        val.getPrivileges();
    }
    return true;
}

/* ************************************************************************* */

bool JsonD540::JsonIsNull() {
    if(jd->isNull()) {
        return true;
    }
    return false;
}

bool JsonD540::JsonIsArray() {
    if(jd->isArray()) {
        ja = jd->array();
        return true;
    }
    return false;
}

quint8 JsonD540::JsonArrayLength() {
    if(JsonIsArray()) {
        quint8 length = ja.size();
        qDebug() << "JSON array have" << length << "elements";
        return length;
    }
    return false;
}

bool JsonD540::JsonDumpArray() {
    if(!JsonIsArray())
        return false;

    JsonArrayLength();

    QJsonArray::Iterator jai;
    for(jai = ja.begin(); jai != ja.end(); jai++) {
        qDebug() << "\t" << (*jai);
    }

    return true;
}

/* ************************************************************************* */

bool JsonD540::JsonIsObject() {
    if(jd->isObject()) {
        jo = jd->object();
        return true;
    }
    return false;
}

quint8 JsonD540::JsonObjectLength() {
    if(JsonIsObject()) {
        quint8 length = jo.size();
        qDebug() << "JSON object have" << length << "elements";
        return length;
    }
    return false;
}

bool JsonD540::JsonDumpObject() {
    if(!JsonIsObject())
        return false;

    JsonObjectLength();

    QJsonObject::Iterator joi;
    for(joi = jo.begin(); joi != jo.end(); joi++) {
        qDebug() << "\t" << (joi.key()) << " - " << (*joi);
    }

    return true;
}

bool JsonD540::JsonGetKeys() {
    if(JsonIsObject()) {
        keys = jo.keys();
        return true;
    }
    return false;
}

bool JsonD540::JsonInKeysTable(const QString &key, QJsonValue &value) {
    if(JsonGetKeys()) {
        quint8 start = 0;
        quint8 end = keys.length();

        while(start != end) {
            QString currentKey = keys.at(start);
            if(!QString::compare(currentKey, key)) {
                value = jo.value(key);
                qDebug() << "Found key:" << key << "with value:" << value;
                return true;
            }
            start++;
        }
        qDebug() << "Not found key:" << key;
        return false;
    }
    return false;
}

//bool JsonD540::GetCardList() {
//    QJsonValue CardList;
//    JsonInKeysTable("Card", CardList);
//    if(CardList.isArray()) {
//        QJsonArray CardListArray = CardList.toArray();

//    }
//}

bool JsonD540::JsonDumpKeys() {
    if(JsonGetKeys()) {
        QStringList::Iterator start;
        QStringList::Iterator end;

        start = keys.begin();
        end = keys.end();

        while(start != end) {
            qDebug() << (*start);
            start++;
        }

        return true;
    }
    return false;
}

/* ************************************************************************* */

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QJsonValue value;
    JsonD540 jo("example_object.json");
    jo.JsonDumpObject();
    jo.JsonDumpKeys();
    jo.JsonInKeysTable("Card", value);
    jo.dumpJsonToObjectsList();

    //JsonD540 ja("example_array.json");
    //ja.JsonDumpArray();


    return a.exec();
}
