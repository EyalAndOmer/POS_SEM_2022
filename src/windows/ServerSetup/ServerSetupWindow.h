#include <gtkmm.h>
#include "../MainAppWindow/MainAppWindow.h"

class ServerSetupWindow : public Gtk::ApplicationWindow {
public:
    ServerSetupWindow(BaseObjectType* cobject,
                      const Glib::RefPtr<Gtk::Builder>&  refBuilder);
    static ServerSetupWindow* create();

protected:
    Glib::RefPtr<Gtk::Builder> m_refBuilder;
    Gtk::Button* m_test_button {nullptr};
    Gtk::ComboBoxText* m_select_protocol {nullptr};
};