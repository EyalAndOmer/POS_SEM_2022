#include <gtkmm.h>

class CurrentDownloadsWindow : public Gtk::Widget{
public:
    CurrentDownloadsWindow();
    ~CurrentDownloadsWindow() override;
    static CurrentDownloadsWindow* create();

protected:
    class ModelColumns : public Gtk::TreeModel::ColumnRecord {
    public:
        Gtk::TreeModelColumn<bool> col_selected;
        Gtk::TreeModelColumn<unsigned int> col_id;
        Gtk::TreeModelColumn<Glib::ustring> col_file_name;
        Gtk::TreeModelColumn<Glib::ustring> col_priority;
        Gtk::TreeModelColumn<unsigned int> col_download_progress;
        ModelColumns() {
            add(col_selected);
            add(col_id);
            add(col_file_name);
            add(col_priority);
            add(col_download_progress);
        }
    };

    ModelColumns m_columns;
    Glib::RefPtr<Gtk::ListStore> m_list_store;
    Gtk::TreeView m_view;
    Gtk::Box wrapper;

public:
    const Gtk::Box &get_wrapper() const;

protected:
    Gtk::ScrolledWindow m_ScrolledWindow;

    void on_cell_edited(const Glib::ustring& path_string);
};
