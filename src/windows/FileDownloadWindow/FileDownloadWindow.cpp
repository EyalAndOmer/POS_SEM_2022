#include "FileDownloadWindow.h"

#include <utility>
#include <iostream>
#include "projectdefinitions.h"

FileDownloadWindow::FileDownloadWindow(BaseObjectType* cobject,
                                       Glib::RefPtr<Gtk::Builder>  refBuilder)
        : Gtk::Window(cobject),
          m_refBuilder(std::move(refBuilder)) {
    main_wrapper = m_refBuilder->get_widget<Gtk::Box>("wrapper");

    main_wrapper->set_margin(10);

    hour_spin_button = m_refBuilder->get_widget<Gtk::SpinButton>("hour_spin_button");
    minute_spin_button = m_refBuilder->get_widget<Gtk::SpinButton>("minute_spin_button");

    time_t now = time(nullptr);
    tm* ltm = localtime(&now);

    hour_spin_button->set_range(0, 23);
    hour_spin_button->set_increments(1, 1);
    hour_spin_button->set_value(ltm->tm_hour);

    minute_spin_button->set_range(0, 59);
    minute_spin_button->set_increments(1, 1);
    minute_spin_button->set_value(ltm->tm_min);

    file_name_label = m_refBuilder->get_widget<Gtk::Label>("file_name_label");

    priority_combo_box = m_refBuilder->get_widget<Gtk::ComboBoxText>("priority_combo_button");
    priority_combo_box->append("Najvyssia");
    priority_combo_box->append("Vyssia");
    priority_combo_box->append("Stredna");
    priority_combo_box->append("Mala");
    priority_combo_box->append("Najmensia");

    priority_combo_box->set_active(0);
}

FileDownloadWindow* FileDownloadWindow::create() {
    auto refBuilder = Gtk::Builder::create_from_resource(
            projectdefinitions::getApplicationPrefix() + "ui/file_download_modal.ui");
    auto window = Gtk::Builder::get_widget_derived<FileDownloadWindow>(
            refBuilder,  "file_start_download_window");

    if (!window) {
        throw std::runtime_error("No \"window\" object in file_download_modal.ui");
    }
    return window;
}

void FileDownloadWindow::set_file_name(const std::string& p_file_name) {
    file_name_label->set_label(p_file_name);
}

