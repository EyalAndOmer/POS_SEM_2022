#include <iostream>
#include "ConnectedWindow.h"

ConnectedWindow::ConnectedWindow()
        : m_VBox(Gtk::Orientation::VERTICAL) {
    m_ScrolledWindow.set_policy(Gtk::PolicyType::AUTOMATIC, Gtk::PolicyType::AUTOMATIC);
    m_ScrolledWindow.set_expand();

    m_VBox.append(m_ScrolledWindow);

    m_dir_model = Gio::ListStore<ModelColumns>::create();
    m_selection_model = Gtk::SingleSelection::create(m_dir_model);

    m_factory = Gtk::SignalListItemFactory::create();
    m_factory->signal_setup().connect(
            sigc::mem_fun(*this, &ConnectedWindow::on_setup_listitem));
    m_factory->signal_bind().connect(
            sigc::mem_fun(*this, &ConnectedWindow::on_bind_listitem));

    m_parent = "/home/klacek/Desktop/test/server_files";
    fill_store();

    m_GridView.set_model(m_selection_model);
    m_GridView.set_factory(m_factory);

    //Connect signals:
    m_GridView.signal_activate().connect(
            sigc::mem_fun(*this, &ConnectedWindow::on_gridview_item_activated));

    m_ScrolledWindow.set_child(m_GridView);

    m_GridView.grab_focus();

    file_window = FileDownloadWindow::create();
}


ConnectedWindow* ConnectedWindow::create() {
    return new ConnectedWindow();
}

void ConnectedWindow::on_setup_listitem(const Glib::RefPtr<Gtk::ListItem>& list_item) {
    auto vBox = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::VERTICAL);
    vBox->append(*Gtk::make_managed<Gtk::Image>());
    vBox->append(*Gtk::make_managed<Gtk::Label>());
    list_item->set_child(*vBox);
}

void ConnectedWindow::on_bind_listitem(const Glib::RefPtr<Gtk::ListItem>& list_item) {
    auto col = std::dynamic_pointer_cast<ModelColumns>(list_item->get_item());
    if (!col) {
        return;
    }
    auto vBox = dynamic_cast<Gtk::Box*>(list_item->get_child());
    if (!vBox) {
        return;
    }
    auto image = dynamic_cast<Gtk::Image*>(vBox->get_first_child());
    if (!image) {
        return;
    }
    auto label = dynamic_cast<Gtk::Label*>(image->get_next_sibling());
    if (!label) {
        return;
    }
    image->set_from_resource(col->getImagePath());
    label->set_text(col->getMDisplayName());
}

ConnectedWindow::~ConnectedWindow() = default;

void ConnectedWindow::fill_store() {
    m_dir_model->remove_all();
    try
    {
        Glib::Dir dir(m_parent);

        auto name = dir.read_name();
        while (!name.empty())
        {
            if (name[0] != '.')
            {
                const auto display_name = Glib::filename_to_utf8(name);

                m_dir_model->append(ModelColumns::create(display_name));
            }
            name = dir.read_name();
        }
    }
    catch (const Glib::FileError& ex)
    {
        std::cout << ex.what() << std::endl;
    }
}

void ConnectedWindow::close_window() {
    this->set_sensitive(true);
}

void ConnectedWindow::on_gridview_item_activated(unsigned int position) {
    auto curr_item = m_dir_model->get_item(position);
    file_window->set_file_name(curr_item->getMDisplayName());
    file_window->set_modal(true);
    file_window->set_hide_on_close(true);
    file_window->signal_hide().connect(sigc::mem_fun(*this, &ConnectedWindow::close_window));
    file_window->show();
    this->set_sensitive(false);
}

const Gtk::Box &ConnectedWindow::getMVBox() const {
    return m_VBox;
}
