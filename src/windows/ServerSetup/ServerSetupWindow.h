#include <gtkmm.h>
#include "../MainAppWindow/MainAppWindow.h"

class ServerSetupWindow : public Gtk::ApplicationWindow {
public:
    ServerSetupWindow(BaseObjectType* cobject,
                      const Glib::RefPtr<Gtk::Builder>&  refBuilder);
    static ServerSetupWindow* create();

protected:
    Glib::RefPtr<Gtk::Builder> m_refBuilder;
    Gtk::Button* m_connect_to_server_button {nullptr};
    Gtk::ComboBoxText* m_select_protocol {nullptr};

    MainAppWindow* main_app_window;

    void on_connect_to_server();

    void on_close_server();
};