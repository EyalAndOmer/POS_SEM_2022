#include "ServerSetupWindow.h"
#include "projectdefinitions.h"
#include <stdexcept>

ServerSetupWindow::ServerSetupWindow(BaseObjectType* cobject,
                                     const Glib::RefPtr<Gtk::Builder>& refBuilder)
        : Gtk::ApplicationWindow(cobject),
          m_refBuilder(refBuilder)
{
    m_test_button = m_refBuilder->get_widget<Gtk::Button>("connect_to_server_button");
    m_select_protocol = m_refBuilder->get_widget<Gtk::ComboBoxText>("protocol_select");

    m_select_protocol->append("http");
    m_select_protocol->append("https");
    m_select_protocol->append("ftp");
    m_select_protocol->append("ftps");

    m_select_protocol->set_active(0);
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