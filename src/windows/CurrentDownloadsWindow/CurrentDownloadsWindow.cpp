#include <iostream>
#include "CurrentDownloadsWindow.h"
CurrentDownloadsWindow::CurrentDownloadsWindow():
        wrapper(Gtk::Orientation::VERTICAL) {
    m_ScrolledWindow.set_policy(Gtk::PolicyType::AUTOMATIC, Gtk::PolicyType::AUTOMATIC);
    m_ScrolledWindow.set_expand();

    wrapper.append(m_ScrolledWindow);
    m_view = Gtk::TreeView();
    m_list_store = Gtk::ListStore::create(m_columns);
    m_view.set_model(m_list_store);

    m_ScrolledWindow.set_child(m_view);

    auto row = *(m_list_store->append());
    row[m_columns.col_selected] = false;
    row[m_columns.col_id] = 1;
    row[m_columns.col_file_name] = "test";
    row[m_columns.col_priority] = "najmensia";
    row[m_columns.col_download_progress] = 10;

    auto cell_toggle = new Gtk::CellRendererToggle();
    int toggle_num_cols = m_view.append_column("", *cell_toggle);
    auto p_col_toggle = m_view.get_column(toggle_num_cols - 1);
    p_col_toggle->add_attribute(cell_toggle->property_active(), m_columns.col_selected);
    cell_toggle->signal_toggled().connect(sigc::mem_fun(*this, &CurrentDownloadsWindow::on_cell_edited));

    row = *(m_list_store->append());
    row[m_columns.col_selected] = false;
    row[m_columns.col_id] = 2;
    row[m_columns.col_file_name] = "test2";
    row[m_columns.col_priority] = "stredna";
    row[m_columns.col_download_progress] = 60;

    cell_toggle = new Gtk::CellRendererToggle();
    cell_toggle->signal_toggled().connect(sigc::mem_fun(*this, &CurrentDownloadsWindow::on_cell_edited));

    m_view.append_column("Id", m_columns.col_id);
    m_view.append_column("Nazov suboru", m_columns.col_file_name);
    m_view.append_column("Priorita", m_columns.col_priority);

    auto cell_progress = Gtk::make_managed<Gtk::CellRendererProgress>();
    int num_cols = m_view.append_column("Progres", *cell_progress);
    auto p_col = m_view.get_column(num_cols - 1);
    p_col->add_attribute(cell_progress->property_value(), m_columns.col_download_progress);


    Gtk::Button pause_button = Gtk::Button();
    pause_button.set_label("Zastav stahovanie");
    wrapper.append(pause_button);

    Gtk::Button resume_button = Gtk::Button();
    resume_button.set_label("Pokracuj v stahovani");
    wrapper.append(resume_button);
}

const Gtk::Box &CurrentDownloadsWindow::get_wrapper() const {
    return wrapper;
}

CurrentDownloadsWindow *CurrentDownloadsWindow::create() {
    return new CurrentDownloadsWindow();
}

CurrentDownloadsWindow::~CurrentDownloadsWindow() = default;

void CurrentDownloadsWindow::on_cell_edited (const Glib::ustring& path_string) {
    auto row = *m_list_store->get_iter(Gtk::TreeModel::Path(path_string));

    // Invert the 'active' value in the toggled row.
    row[m_columns.col_selected] = !row[m_columns.col_selected];
}