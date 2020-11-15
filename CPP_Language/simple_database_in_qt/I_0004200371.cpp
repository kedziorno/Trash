#include <iostream>
#include <vector>
#include <sqlite3.h>
#include <cstring>
#include <gtkmm.h>
#include <glibmm/ustring.h>
#include <glibmm/ustring.h>
#include <gtkmm/application.h>
#include "insertwindow.h"
#include "examplewindow.h"

namespace App {
    InsertWindow::InsertWindow(void *a) {
        App::ExampleWindow *ew = (App::ExampleWindow *)a;

        set_parent(*ew);
        set_title("Insert Dialog");
        set_default_size(1, 1);

        get_content_area()->pack_start(m_FrameVertical);

        Gtk::Frame *f = Gtk::manage(new Gtk::Frame("Insert data"));
        Gtk::Grid *g = Gtk::manage(new Gtk::Grid());
        Gtk::Box *b1 = Gtk::manage(new Gtk::Box);
        Gtk::Box *b2 = Gtk::manage(new Gtk::Box);
        Gtk::Box *b3 = Gtk::manage(new Gtk::Box);
        Gtk::Box *ib = Gtk::manage(new Gtk::Box);
        Gtk::Button *insert = Gtk::manage(new Gtk::Button());

        Gtk::Label *label1 = Gtk::manage(new Gtk::Label("Name"));
        Gtk::Label *label2 = Gtk::manage(new Gtk::Label("SureName"));
        Gtk::Label *label3 = Gtk::manage(new Gtk::Label("Age"));

        m_EntryName.set_hexpand(true);
        m_EntryName.set_width_chars(15);
        m_EntrySureName.set_hexpand(true);
        m_EntrySureName.set_width_chars(15);
        m_EntryAge.set_hexpand(true);
        m_EntryAge.set_width_chars(15);

        b1->add(*label1);
        b1->add(m_EntryName);
        b2->add(*label2);
        b2->add(m_EntrySureName);
        b3->add(*label3);
        b3->add(m_EntryAge);

        insert->set_label("Insert Record");
        insert->signal_clicked().connect\
                    (sigc::bind<void*,Gtk::Entry*,Gtk::Entry*,Gtk::Entry*>\
                     (sigc::mem_fun(*this, &InsertWindow::insert_on_clicked), \
                      (void*)ew, &m_EntryName, &m_EntrySureName, &m_EntryAge));
        ib->add(*insert);

        g->set_vexpand(true);
        g->attach(*b1,1,1,1,1);
        g->attach(*b2,1,2,1,1);
        g->attach(*b3,1,3,1,1);
        g->attach(*ib,1,4,1,1);


        f->add(*g);

        m_Grid.attach(*f,1,1,1,1);
        m_FrameVertical.add(m_Grid);

        add_button("_Close", Gtk::RESPONSE_CLOSE);
        signal_response().connect(sigc::mem_fun(*this, &InsertWindow::on_dialog_response));

        set_default_response(Gtk::RESPONSE_CLOSE);

        show_all_children();
    }

    InsertWindow::~InsertWindow() {}

    void InsertWindow::on_dialog_response(int response_id) {
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

    void InsertWindow::insert_on_clicked(void *data1, Gtk::Entry *data2, Gtk::Entry *data3, Gtk::Entry *data4) {
        App::ExampleWindow *ew = (App::ExampleWindow *)data1;
        std::vector<std::string> values;
        if(data2->get_text().empty() || data3->get_text().empty() || data4->get_text().empty()) {
            Gtk::MessageDialog dialog(*this, "Insert");
            dialog.set_secondary_text("Fill all entries");
            dialog.run();
        } else {
            values.push_back(data2->get_text());
            values.push_back(data3->get_text());
            values.push_back(data4->get_text());
            ew->getSqlWrapper()->insertData(values);
            on_dialog_response(Gtk::RESPONSE_CLOSE);
            //redraw ew window
            std::vector<Gtk::Widget*> asd = ew->getGrid()->get_children();
            for(Gtk::Widget *w : asd)
                ew->getGrid()->remove(*w);
            ew->renderGrid(ew->getGrid());
            ew->show_all_children();
        }
    }
};

