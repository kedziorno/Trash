#include <iostream>
#include <vector>
#include <sqlite3.h>
#include <cstring>
#include <gtkmm.h>
#include <glibmm/ustring.h>
#include <glibmm/ustring.h>
#include <gtkmm/application.h>
#include "examplewindow.h"

namespace App {
    ExampleWindow::ExampleWindow(void *a) {
        this->m_sqlw = (SqlWrapper *)a;
        this->m_ow = new App::OpenWindow(this);

        set_title("sqlite3 and c++/gtkmm example");
        set_border_width(10);
        set_default_size(1, 1);

        Gtk::MenuBar *menubar = Gtk::manage(new Gtk::MenuBar());
        Gtk::Menu *filemenu = Gtk::manage(new Gtk::Menu());
        Gtk::MenuItem *menuitem_file = Gtk::manage(new Gtk::MenuItem("_File", true));
        Gtk::MenuItem *menuitem_open = Gtk::manage(new Gtk::MenuItem("_Open", true));
        Gtk::MenuItem *menuitem_insert = Gtk::manage(new Gtk::MenuItem("_Insert", true));

        get_content_area()->pack_start(*menubar, Gtk::PACK_SHRINK, 0);
        menubar->append(*menuitem_file);
        menuitem_file->set_submenu(*filemenu);
        filemenu->append(*menuitem_open);
        filemenu->append(*menuitem_insert);
        menuitem_open->signal_activate().connect(sigc::mem_fun(*this, &ExampleWindow::on_open_click));
        menuitem_insert->signal_activate().connect(sigc::mem_fun(*this, &ExampleWindow::on_insert_click));

        get_content_area()->pack_start(m_FrameHorizontal);
        m_FrameHorizontal.add(m_Grid);

        add_button("_Close", Gtk::RESPONSE_CLOSE);
        set_default_response(Gtk::RESPONSE_CLOSE);
        signal_response().connect(sigc::mem_fun(*this, &ExampleWindow::on_dialog_response));

        std::string dbName(this->m_ow->getFileName());
        std::string dbDemo1("CREATE TABLE demo1 (id integer primary key, name varchar(128), surename varchar(128), age int)");

        std::cout << __FUNCTION__ << " database name from open dialog " << this->m_ow->getFileName() << "\n";

        std::string frameTitle("Database: ");
        frameTitle += dbName;
        m_FrameHorizontal.set_label(frameTitle);

        m_sqlw->addDb(dbName);
        m_sqlw->createExampleDb(dbDemo1, m_sqlw->option::ONE_DB);

        this->renderGrid(&m_Grid);

        show_all_children();
    }

    void ExampleWindow::renderGrid(Gtk::Grid *grid0) {
        int nrcol = m_sqlw->getNr(m_sqlw->option::NR_COLS);
        int nrrow = m_sqlw->getNr(m_sqlw->option::NR_ROWS);

        fprintf(stderr, "[%s] (row,col) (%d,%d)\n", __FUNCTION__, nrrow, nrcol);

        m_sqlw->getAllTable();
        m_sqlw->dumpAllTable();

        Glib::ustring uColumnName;
        Glib::ustring oldTuple;

       for(int i = 0; i < nrrow; i++) {
          char buffer[64];
          int rowid = std::stoi(m_sqlw->getTupleAtIndex(i, 0, m_sqlw->option::FROM_ZERO).c_str());
          snprintf(buffer, 64, "Row nr. %d ( id: %d )", i+1, rowid);
          Gtk::Frame *f = Gtk::manage(new Gtk::Frame(buffer));
          snprintf(buffer, 64, "IRow%d", rowid);
          f->set_name(buffer);
          Gtk::Box *b = Gtk::manage(new Gtk::Box);
          for(int j = 1; j < nrcol; j++) {
              std::string *sColumnName = new std::string();
              sColumnName->assign(m_sqlw->getColumnAt(j));
              std::string *tuple = new std::string();
              tuple->assign(m_sqlw->getTupleAtIndex(i, j, m_sqlw->option::FROM_ZERO));
              Gtk::Entry *label = Gtk::manage(new Gtk::Entry());
              label->set_hexpand(true);
              label->set_width_chars(5);
              label->set_text(*tuple);
              uColumnName = *sColumnName;
              oldTuple = *tuple;
              label->signal_activate().connect(sigc::bind<sqlite3*,Gtk::Entry*,Glib::ustring,Glib::ustring,int>(sigc::mem_fun(*this, &ExampleWindow::tuple_on_press_enter), m_sqlw->getContext(), label, uColumnName, oldTuple, rowid));
              b->add(*label);
          }
          f->add(*b);
          Gtk::Frame *deleteFrame = Gtk::manage(new Gtk::Frame());
          Gtk::Box *deleteBox = Gtk::manage(new Gtk::Box());
          Gtk::Button *deleteButton = Gtk::manage(new Gtk::Button());
          deleteButton->set_label("X");
          deleteButton->signal_clicked().connect(sigc::bind<sqlite3*,Gtk::Grid*,int>(sigc::mem_fun(*this, &ExampleWindow::delete_on_clicked), m_sqlw->getContext(), grid0, rowid));
          deleteBox->add(*deleteButton);
          deleteFrame->add(*deleteBox);
          deleteFrame->set_name(buffer);
          grid0->attach(*f,1,i,1,1);
          grid0->attach(*deleteFrame,2,i,1,1);
        }
    }

    ExampleWindow::~ExampleWindow() {}

    void ExampleWindow::on_dialog_response(int response_id) {
        m_sqlw->closeAllConnections();

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

    void ExampleWindow::tuple_on_press_enter(sqlite3 *dbCtx, Gtk::Entry *entry, Glib::ustring data1, Glib::ustring data2, int rowid) {
        // TODO:old!=new?update:noupdate
        std::cout << entry->get_text().c_str() << " - " << data2.c_str() << "\n";
        if(strcmp(entry->get_text().c_str(), data2.c_str()) != 0) {
            char buf[512];
            snprintf(buf, 512, "UPDATE demo1 SET %s='%s' WHERE id=%d", data1.c_str(), entry->get_text().c_str(), rowid);
            fprintf(stderr, "%s\n", buf);
            int ret = sqlite3_exec(dbCtx, buf, NULL, NULL, 0);
            m_sqlw->getAllTable();
            entry->set_text(entry->get_text());
            if(ret == SQLITE_OK) {
                fprintf(stderr, "Row %d ipdated\n", rowid);
                // XXX:afterupdate
            } else {
                m_sqlw->showError();
            }
        }
    }

    void ExampleWindow::delete_on_clicked(sqlite3 *dbCtx, Gtk::Grid *grid, int rowid) {
        char buf[512];
        snprintf(buf, 512, "DELETE FROM demo1 WHERE id=%d", rowid);
        int ret = sqlite3_exec(dbCtx, buf, NULL, NULL, 0);
        if(ret == SQLITE_OK) {
            fprintf(stderr, "Row %d deleted\n", rowid);
        } else {
            m_sqlw->showError();
        }

        snprintf(buf, 64, "IRow%d", rowid);

        std::vector<Gtk::Widget*> vW = grid->get_children();
        for(Gtk::Widget *w : vW) {
            std::cout << "Name: " << w->get_name() << "\n";
            if(strcmp(w->get_name().c_str(), buf) == 0) {
                grid->remove(*w);
            }
        }
    }

    void ExampleWindow::on_open_click() {
        std::cout << __FUNCTION__ << " open window " << "\n";
        this->m_ow = new OpenWindow(this);
        this->m_ow->show();
    }

    void ExampleWindow::on_insert_click() {
        std::cout << __FUNCTION__ << " insert window " << "\n";
        this->m_iw = new InsertWindow(this);
        this->m_iw->show();
    }
}
