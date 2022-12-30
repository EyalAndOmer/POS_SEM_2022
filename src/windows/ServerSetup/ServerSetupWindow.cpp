#include "ServerSetupWindow.h"
#include "projectdefinitions.h"
#include <stdexcept>
#include <iostream>

ServerSetupWindow::ServerSetupWindow(BaseObjectType* cobject,
                                     const Glib::RefPtr<Gtk::Builder>& refBuilder)
        : Gtk::ApplicationWindow(cobject),
          m_refBuilder(refBuilder)
{
    m_connect_to_server_button = m_refBuilder->get_widget<Gtk::Button>("connect_to_server_button");
    m_select_protocol = m_refBuilder->get_widget<Gtk::ComboBoxText>("protocol_select");

    m_select_protocol->append("http");
    m_select_protocol->append("https");
    m_select_protocol->append("ftp");
    m_select_protocol->append("ftps");

    m_select_protocol->set_active(0);

    m_connect_to_server_button->signal_clicked().connect(
            sigc::mem_fun(*this, &ServerSetupWindow::on_connect_to_server));

    main_app_window = MainAppWindow::create();

}

//static
ServerSetupWindow* ServerSetupWindow::create()
{
    // filename is relative to cmake-build-debug directory
    auto refBuilder = Gtk::Builder::create_from_resource(projectdefinitions::getApplicationPrefix() + "ui/main_window.ui");

    auto window = Gtk::Builder::get_widget_derived<ServerSetupWindow>(refBuilder, "connect_to_server_window");

    if (!window)
        throw std::runtime_error("No \"window\" object in window.ui");

    return window;
}

void ServerSetupWindow::on_close_server() {
    this->set_sensitive(true);
}

void ServerSetupWindow::on_connect_to_server() {
    main_app_window->set_modal(true);
    main_app_window->set_hide_on_close();
    main_app_window->show();
    main_app_window->signal_hide().connect(
            sigc::mem_fun(*this, &ServerSetupWindow::on_close_server));
    this->set_sensitive(false);
}
