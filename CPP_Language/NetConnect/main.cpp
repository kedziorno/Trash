#include "mainwindow.h"
#include <QApplication>

#include "netcode.h"
#include "urldata.h"

using namespace std;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    qSetMessagePattern("%{function}:%{line} => %{message}");

    StormPath object(\
                "",\
                ""\
                );

    object.getApplications();

    object.addUser("");

//    urlData urldata;
//    urldata.setScheme("https");
//    urldata.setUser("");
//    urldata.setPassword("");
//    urldata.setHost("api.stormpath.com");
//    urldata.setPath("/v1/tenants/current");
//    qDebug() << urldata.getBasicAuthorizationHeader();
//    qDebug() << urldata.getUrlObject();
//    qDebug() << urldata.getUrlObjectWithoutUserInfo();
//    qDebug() << urldata.getUrlPath();
//    qDebug() << urldata.getUrlPathWithAccess();

    exit(0);
    return a.exec();
}
