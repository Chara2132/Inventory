#ifndef INVENTORY_HPP
#define INVENTORY_HPP

#include <gtkmm.h>
#include <vector>

class InventoryGUI final : public Gtk::Window {

    private:

        class ModelColumns final : public Gtk::TreeModel::ColumnRecord {

            public:

                Gtk::TreeModelColumn<Glib::ustring> supplier;
                Gtk::TreeModelColumn<Glib::ustring> category;
                Gtk::TreeModelColumn<Glib::ustring> name;
                Gtk::TreeModelColumn<Glib::ustring> date;
                Gtk::TreeModelColumn<Glib::ustring> type;
                Gtk::TreeModelColumn<double> price;
                Gtk::TreeModelColumn<int> quantity;

                explicit inline ModelColumns(void) noexcept {
                    add(category);
                    add(quantity);
                    add(supplier);
                    add(price);
                    add(name);
                    add(date);
                    add(type);
                }
        };

        Gtk::Button filter_button{"Guarda i prodotti filtrati"};
        Gtk::Button add_button{"Carica prodotto"};
        Gtk::Button client_button{"Clienti"};

        Gtk::Box tablebox{Gtk::ORIENTATION_HORIZONTAL};
        Gtk::Box hbox{Gtk::ORIENTATION_HORIZONTAL};
        Gtk::Box vbox{Gtk::ORIENTATION_VERTICAL};   

        Gtk::Label title_of_tabel;
        Gtk::Label title;
        
        Gtk::Separator separator2;
        Gtk::Separator separator;

        Gtk::TreeView tree_view;

        Glib::RefPtr<Gtk::ListStore> list;

        ModelColumns columns;
        
        std::vector<std::shared_ptr<Gtk::Window>> child_windows;

        void on_filter_button_pressed(void) noexcept;

        void on_add_button_pressed(void) noexcept;
 
        void on_client_button_pressed(void) noexcept; 

        bool on_tree_view_click(GdkEventButton* event) noexcept;

    public:

        explicit InventoryGUI(void) noexcept;

        Glib::RefPtr<Gtk::ListStore> get_liststore(void) noexcept;

        ModelColumns& get_columns(void) noexcept;

        void show_context_menu(GdkEventButton* event) noexcept; 

        void add_item(const Glib::ustring& name, const Glib::ustring& category, const int& quantity, const double& price, const Glib::ustring& date, const Glib::ustring& supplier) const noexcept;
        
        void filter_for(const Glib::RefPtr<Gtk::TreeModelFilter> filter) noexcept;
};

#endif
