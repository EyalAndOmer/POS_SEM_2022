#include <gtkmm.h>

class FileDownloadWindow: public Gtk::Window{
public:
    FileDownloadWindow(BaseObjectType* cobject,
                       Glib::RefPtr<Gtk::Builder>   refBuilder);
    static FileDownloadWindow *create();
    void set_file_name(const std::string& p_file_name);

protected:
    Glib::RefPtr<Gtk::Builder> m_refBuilder;
    Gtk::SpinButton* hour_spin_button {nullptr};
    Gtk::SpinButton* minute_spin_button {nullptr};
    Gtk::Label* file_name_label {nullptr};
    Gtk::ComboBoxText* priority_combo_box {nullptr};
    Gtk::Box* main_wrapper {nullptr};
};
