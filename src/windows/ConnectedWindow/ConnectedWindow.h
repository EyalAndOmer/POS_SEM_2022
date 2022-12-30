#include "../FileDownloadWindow/FileDownloadWindow.h"
#include <gtkmm.h>

#include <utility>

class ConnectedWindow : public Gtk::Widget
{
public:
    ConnectedWindow();
    ~ConnectedWindow() override;
    static ConnectedWindow* create();

protected:
    class ModelColumns : public Glib::Object {
    private:
        Glib::ustring m_display_name;
        const Glib::ustring image_path = "/gridview/gnome-fs-regular.png";
    public:
        const Glib::ustring &getImagePath() const {
            return image_path;
        }
        static Glib::RefPtr<ModelColumns> create(const Glib::ustring& display_name) {
            return Glib::make_refptr_for_instance<ModelColumns>(new ModelColumns(display_name));
        }

        const Glib::ustring &getMDisplayName() const {
            return m_display_name;
        }
    protected:
        explicit ModelColumns(Glib::ustring  display_name):  m_display_name(std::move(display_name)) { }
    };

    void fill_store();

    void on_gridview_item_activated(unsigned int position);
    void on_setup_listitem(const Glib::RefPtr<Gtk::ListItem>& list_item);
    void on_bind_listitem(const Glib::RefPtr<Gtk::ListItem>& list_item);

    Glib::RefPtr<Gio::ListStore<ModelColumns>> m_dir_model;
    Glib::RefPtr<Gtk::SingleSelection> m_selection_model;
    Glib::RefPtr<Gtk::SignalListItemFactory> m_factory;

    std::string m_parent;

    Gtk::Box m_VBox;
    Gtk::ScrolledWindow m_ScrolledWindow;
    Gtk::GridView m_GridView;
    FileDownloadWindow* file_window;

    void close_window();

public:
    const Gtk::Box &getMVBox() const;
};