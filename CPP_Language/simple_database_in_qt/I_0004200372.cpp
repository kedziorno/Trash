#include <iostream>
#include <vector>
#include <sqlite3.h>
#include <cstring>
#include <gtkmm.h>
#include <glibmm/ustring.h>
#include <glibmm/ustring.h>
#include <gtkmm/application.h>
#include "openwindow.h"
#include "examplewindow.h"

namespace App {
    OpenWindow::OpenWindow(void *a) {
        App::ExampleWindow *ew = (App::ExampleWindow *)a;

        set_parent(*ew);
        set_title("Open Dialog");
        set_default_size(1, 1);

        get_content_area()->pack_start(m_FrameHorizontal);

        Gtk::Frame *f = Gtk::manage(new Gtk::Frame("File"));
        Gtk::Box *b = Gtk::manage(new Gtk::Box);
        Gtk::Button *open = Gtk::manage(new Gtk::Button());

        m_Entry.set_hexpand(true);
        m_Entry.set_width_chars(15);

        open->set_label("Open File");
        open->signal_clicked().connect(sigc::bind<Gtk::Entry*,void*>(sigc::mem_fun(*this, &OpenWindow::open_on_clicked), &m_Entry, (void*)ew));

        b->add(m_Entry);
        b->add(*open);

        f->add(*b);

        m_Grid.attach(*f,1,1,1,1);
        m_FrameHorizontal.add(m_Grid);

        add_button("_Close", Gtk::RESPONSE_CLOSE);
        signal_response().connect(sigc::mem_fun(*this, &OpenWindow::on_dialog_response));

        set_default_response(Gtk::RESPONSE_CLOSE);

        show_all_children();
    }

    OpenWindow::~OpenWindow() {}

    std::string OpenWindow::getFileName() {
        return this->m_fileName;
    }

    void OpenWindow::on_dialog_response(int response_id) {
        switch (response_id) {
            case Gtk::RESPONSE_CLOSE:
            case Gtk::RESPONSE_DELETE_EVENT:
                hide();
            break;
            default:
                std::cout << "Unexpected response_id=" << response_id << std::endl;
            break;
        }
    }

    void OpenWindow::open_on_clicked(Gtk::Entry *data1, void *data2) {
        App::ExampleWindow *ew = (App::ExampleWindow *)data2;
        m_fileName = data1->get_text();
        on_dialog_response(Gtk::RESPONSE_CLOSE);
        fprintf(stderr, "%s fn -> %s\n",__FUNCTION__, m_fileName.c_str());
        std::string frameTitle("Database: ");
        frameTitle += m_fileName;
        ew->getFrame()->set_label(frameTitle);
        //redraw ew window
        std::vector<Gtk::Widget*> asd = ew->getGrid()->get_children();
        for(Gtk::Widget *w : asd)
            ew->getGrid()->remove(*w);
        ew->getSqlWrapper()->addDb(m_fileName);
        std::string dbDemo1("CREATE TABLE demo1 (id integer primary key, name varchar(128), surename varchar(128), age int)");
        ew->getSqlWrapper()->createExampleDb(dbDemo1, ew->getSqlWrapper()->option::ONE_DB);
        ew->renderGrid(ew->getGrid());
        ew->show_all_children();
    }
}

