#include <gtkmm.h>
#include "../ConnectedWindow/ConnectedWindow.h"
#include "../CurrentDownloadsWindow/CurrentDownloadsWindow.h"

class MainAppWindow: public Gtk::Window {
public:
    static MainAppWindow* create();
    MainAppWindow();
    ~MainAppWindow() override;

protected:
    Gtk::Box v_box;
    ConnectedWindow* file_download_window;
    CurrentDownloadsWindow* current_downloads_window;
    Gtk::Label text;
    Gtk::Label text2;
    Gtk::Notebook main_notebook;
};
