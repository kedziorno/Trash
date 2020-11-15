#include <iostream>
#include <vector>
#include <sqlite3.h>
#include <cstring>
#include <gtkmm.h>
#include <glibmm/ustring.h>
#include <glibmm/ustring.h>
#include <gtkmm/application.h>
#include "sqlwrapper.h"
#include "examplewindow.h"
#include "openwindow.h"

using namespace std;

int main(int argc, char *argv[])
{
    auto app = Gtk::Application::create(argc, argv, "org.gtkmm.example");

    App::SqlWrapper sqlw;
    App::ExampleWindow window(&sqlw);

    //Shows the window and returns when it is closed.
    return app->run(window);
}

