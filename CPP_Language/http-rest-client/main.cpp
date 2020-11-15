#include <QApplication>

#include "restclient.hpp"
#include "user.hpp"
#include "accesses.hpp"
#include "container.hpp"

QString GetRandomString();
QString GetRandomTag();
int GetRandomPin();
int GetRandomNumericLogin();

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    qSetMessagePattern("%{function}:%{line} => %{message}");

    QUrl url;
    url.setScheme("http");
    url.setHost("aaa.pl");
    url.setUserName("sss");
    url.setPassword("ddd");
    url.setPath("/api/v1/");
    RestClient rest(url);

    User user(&rest);
    //user.create("k123", "o123", 3, "PL", "kologin3", 1988, "kopass", 1986, "2015-09-01", "deadbeafa0dcafe");
    //user.get(13);
    //user.update(U_validDateFromAndTo, "2015-10-11 14:14:14#2015-12-13 14:14:14");
    //user.remove();
    qDebug() << user;

//    Accesses accesses(&rest);

//    User item(&rest);
//    item.get(5);
//    qDebug() << item;

//    Container<User> containerOfUsers(&rest, "users", 100, "tags");
//    containerOfUsers.get();
//    containerOfUsers.get(1);
//    containerOfUsers.remove(15);
//    qDebug() << containerOfUsers.getData();
//    User newUser;
//    newUser.create("Sylwia", "BaBa", 3, "PL", "loginDSD", 123444, "haselko1234", 1234, "2015-08-11", "BadC0de");
//    containerOfUsers.create(newUser);
//    qDebug() << containerOfUsers.getData();
//    Container<Tags> containerOfTags(&rest, "tags");
//    containerOfTags.get();

//    qsrand(QDateTime::currentMSecsSinceEpoch());
//    QString postfix = GetRandomString();
//    User newUser(&rest);
//    newUser.create("Name" + postfix, "Surname" + postfix, 1, "PL", "login" + postfix, GetRandomNumericLogin(), "Haselko1234", GetRandomPin(), "2015-08-01", "deadbeefcafe");
//    Tags newTag1;
//    newTag1.create(newUser.getId(), GetRandomTag(), "2015-07-31 12:00:00", "2015-08-31 12:00:00");
//    Tags newTag2;
//    newTag2.create(newUser.getId(), GetRandomTag(), "2015-07-31 12:00:00", "2015-08-31 12:00:00");
//    Tags newTag3;
//    newTag3.create(newUser.getId(), GetRandomTag(), "2015-07-31 12:00:00", "2015-08-31 12:00:00");
//    qDebug() << newUser;
//    newUser.insertTag(newTag1);
//    newUser.insertTag(newTag2);
//    newUser.insertTag(newTag3);
//    foreach (const Tags& tag, newUser.getTags())
//        qDebug() << tag;
//    newUser.updateTags();
//    newUser.update(U_externalKey, QVariant("NewExternalKey"));
//    newUser.update(U_validDateFromAndTo, \
//                   QVariant("2015-10-01 12:12:12|2016-02-29 12:23:41"));
//    newUser.update(U_firstName, QVariant("NewFirstName"));
//    newUser.update(U_language, QVariant("PL"));
//    newUser.update(U_lastName, QVariant("NewLastname"));
//    newUser.update(U_login, QVariant("NewLoginAAA"));
//    newUser.update(U_numericLogin, QVariant(1234512380));
//    newUser.update(U_pin, QVariant(1988));
//    newUser.update(U_privileges, QVariant(3));
//    newUser.update(U_secondName, QVariant("NewSecondName"));
//    qDebug() << newUser;
//    foreach (const Tags& tag, newUser.getTags())
//        qDebug() << tag;

    exit(0);
    return a.exec();
}

QString GetRandomString()
{
   const QString possibleCharacters("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");
   const int randomStringLength = 6; // assuming you want random strings of 12 characters

   QString randomString;
   for(int i=0; i<randomStringLength; ++i)
   {
       int index = qrand() % possibleCharacters.length();
       QChar nextChar = possibleCharacters.at(index);
       randomString.append(nextChar);
   }
   return randomString;
}

QString GetRandomTag()
{
   const QString possibleCharacters("0123456789");
   const int randomStringLength = 12; // assuming you want random strings of 12 characters

   QString randomString;
   for(int i=0; i<randomStringLength; ++i)
   {
       int index = qrand() % possibleCharacters.length();
       QChar nextChar = possibleCharacters.at(index);
       randomString.append(nextChar);
   }
   return randomString;
}

int GetRandomPin()
{
   const QString possibleCharacters("0123456789");
   const int randomStringLength = 4; // assuming you want random strings of 12 characters

   QString randomString;
   for(int i=0; i<randomStringLength; ++i)
   {
       int index = qrand() % possibleCharacters.length();
       QChar nextChar = possibleCharacters.at(index);
       randomString.append(nextChar);
   }
   return randomString.toInt();
}

int GetRandomNumericLogin()
{
   const QString possibleCharacters("0123456789");
   const int randomStringLength = 6; // assuming you want random strings of 12 characters

   QString randomString;
   for(int i=0; i<randomStringLength; ++i)
   {
       int index = qrand() % possibleCharacters.length();
       QChar nextChar = possibleCharacters.at(index);
       randomString.append(nextChar);
   }
   return randomString.toUInt();
}
