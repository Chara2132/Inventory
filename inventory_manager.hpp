#ifndef ITEM_MANAGER_HPP
#define ITEM_MANAGER_HPP

#include "inventory.hpp"
#include <gtkmm.h>
#include <set>

class AddProductGUI final : public Gtk::Window {

    private:

        Gtk::Label supplier_label{"Nome fornitore"};
        Gtk::Label category_label{"Categoria"};
        Gtk::Label date_label{"Data di carico"};
        Gtk::Label quantity_label{"Quantità"};
        Gtk::Label price_label{"Prezzo"};
        Gtk::Label name_label{"Nome"};

        Gtk::Entry entry_category;
        Gtk::Entry entry_quantity;
        Gtk::Entry entry_supplier;
        Gtk::Entry entry_price;
        Gtk::Entry entry_name;
        Gtk::Entry entry_date;

        Gtk::Button send_button{"Carica il prodotto"};
        Gtk::Button now_button{"Oggi"};

        Gtk::Box vbox{Gtk::ORIENTATION_HORIZONTAL,20};
        Gtk::Box box{Gtk::ORIENTATION_VERTICAL,40};

        InventoryGUI* inventory_ptr;

        void on_send_button_clicked();
        void on_now_button_clicked();

    public:

        explicit AddProductGUI(InventoryGUI* inventory) noexcept;
};


class FilterProductGUI final: public Gtk::Window {

    private:

        Gtk::ComboBoxText combo_category;
        Gtk::ComboBoxText combo_quantity;
        Gtk::ComboBoxText combo_supplier;
        Gtk::ComboBoxText combo_price;
        Gtk::ComboBoxText combo_date;
        Gtk::ComboBoxText combo_name;
        Gtk::ComboBoxText combo_type;
        
        Gtk::Box category_box{Gtk::ORIENTATION_HORIZONTAL,20};
        Gtk::Box quantity_box{Gtk::ORIENTATION_HORIZONTAL,20};
        Gtk::Box supplier_box{Gtk::ORIENTATION_HORIZONTAL,20};
        Gtk::Box price_box{Gtk::ORIENTATION_HORIZONTAL,20};
        Gtk::Box date_box{Gtk::ORIENTATION_HORIZONTAL,20};
        Gtk::Box name_box{Gtk::ORIENTATION_HORIZONTAL,20};
        Gtk::Box type_box{Gtk::ORIENTATION_HORIZONTAL,20};
        Gtk::Box main_box{Gtk::ORIENTATION_VERTICAL, 20};

        Gtk::Label title_label{"Filtra Prodotti"};

        Gtk::Button filter_by_type_button{"Filtra per Tipologia (Scarico/Carico)"};
        Gtk::Button filter_by_supplier_button{"Filtra per il Fornitore"};
        Gtk::Button filter_by_category_button{"Filtra per Categoria"};
        Gtk::Button filter_by_quantity_button{"Filtra per Quantità"};
        Gtk::Button filter_by_price_button{"Filtra per Prezzo"};
        Gtk::Button filter_by_date_button{"Filtra per Data"};
        Gtk::Button filter_by_name_button{"Filtra per Nome"};
        Gtk::Button reset_filters_button{"Reset Filtri"};

        Gtk::Separator separator;

        Glib::RefPtr<Gtk::TreeModelFilter> filter;

        std::set<Glib::ustring> set_category;
        std::set<Glib::ustring> set_supplier;
        std::set<Glib::ustring> set_date;
        std::set<Glib::ustring> set_name;
        std::set<Glib::ustring> set_type;
        std::set<double> set_price;
        std::set<int> set_quantity;

        InventoryGUI* inventory_ptr;

        bool is_quantity=false;
        bool is_category=false;
        bool is_supplier=false;
        bool is_price=false;
        bool is_reset=false;
        bool is_date=false;
        bool is_name=false;
        bool is_type=false;

    public:

        explicit FilterProductGUI(InventoryGUI* inventory) noexcept;
};
#endif