#include "MainAppWindow.h"

MainAppWindow::MainAppWindow()
:   v_box(Gtk::Orientation::VERTICAL),
    text("Second page!"),
    text2("Third page!"),
    file_download_window(ConnectedWindow::create()),
    current_downloads_window(CurrentDownloadsWindow::create())
{
    set_title("Vitajte na servery !");
    set_icon_name("media-flash");
    set_default_size(640, 480);
    set_child(v_box);

    main_notebook.set_expand();

    v_box.append(main_notebook);

    main_notebook.append_page((Widget &) file_download_window->getMVBox(), "Subory");
    main_notebook.append_page((Widget &) current_downloads_window->get_wrapper(), "Aktualne stahovanie");
    main_notebook.append_page(text2, "Historia stahovania");

}

MainAppWindow *MainAppWindow::create() {
    return new MainAppWindow();
}

MainAppWindow::~MainAppWindow() {

}
