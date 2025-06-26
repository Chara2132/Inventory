#include "inventory_manager.hpp"
#include "client.hpp"
#include "inventory.hpp"
#include <cstdlib> 
#include <ctime>

InventoryGUI::InventoryGUI(void) noexcept{

    set_title("Inventario");
    maximize();

    list = Gtk::ListStore::create(columns);
    tree_view.set_model(list);

    tree_view.append_column("Nome", columns.name);
    tree_view.append_column("Categoria", columns.category);
    tree_view.append_column("Quantità", columns.quantity);
    tree_view.append_column("Prezzo", columns.price);
    tree_view.append_column("Data", columns.date);
    tree_view.append_column("Fornitore", columns.supplier);
    tree_view.append_column("Tipologia", columns.type);

    tree_view.add_events(Gdk::BUTTON_PRESS_MASK);
    tree_view.signal_button_press_event().connect(sigc::mem_fun(*this, &InventoryGUI::on_tree_view_click), false);

    add_button.signal_clicked().connect(sigc::mem_fun(*this, &InventoryGUI::on_add_button_pressed));
    filter_button.signal_clicked().connect(sigc::mem_fun(*this, &InventoryGUI::on_filter_button_pressed));
    client_button.signal_clicked().connect(sigc::mem_fun(*this, &InventoryGUI::on_client_button_pressed));

    vbox.set_spacing(20);
    vbox.set_margin_bottom(40);
    vbox.set_margin_start(40);
    vbox.set_margin_top(40);
    vbox.set_margin_end(40);

    hbox.set_spacing(20);
    hbox.set_margin_top(40);

    tablebox.set_margin_top(40);
    tablebox.set_spacing(20);

    title.set_markup("<b><big>Inventario</big></b>");
    title.set_halign(Gtk::ALIGN_CENTER);
    title.set_margin_bottom(20);

    title_of_tabel.set_markup("<b><big>Tabella Carico/Scarico prodotti</big></b>");
    title_of_tabel.set_halign(Gtk::ALIGN_CENTER);
    title_of_tabel.set_margin_bottom(10);
    title_of_tabel.set_margin_top(40);

    tablebox.pack_start(tree_view);

    hbox.pack_start(add_button, Gtk::PACK_EXPAND_WIDGET);
    hbox.pack_start(filter_button, Gtk::PACK_EXPAND_WIDGET);
    hbox.pack_start(client_button, Gtk::PACK_EXPAND_WIDGET);

    vbox.pack_start(title, Gtk::PACK_SHRINK);
    vbox.pack_start(separator, Gtk::PACK_SHRINK);
    vbox.pack_start(hbox, Gtk::PACK_SHRINK);
    vbox.pack_start(title_of_tabel, Gtk::PACK_SHRINK);
    vbox.pack_start(separator2, Gtk::PACK_SHRINK);
    vbox.pack_start(tablebox, Gtk::PACK_EXPAND_WIDGET);

    add(vbox);
    show_all_children();
}

void InventoryGUI::add_item(const Glib::ustring& name, const Glib::ustring& category, const int& quantity, const double& price, const Glib::ustring& date, const Glib::ustring& supplier) const noexcept {
    auto row = *(list->append());
    row[columns.category] = category;
    row[columns.quantity] = quantity;
    row[columns.price] = price;
    row[columns.name] = name;
    row[columns.date] = date;
    row[columns.supplier] = supplier;
    row[columns.type] = "Carico";
}

bool InventoryGUI::on_tree_view_click(GdkEventButton* event) noexcept {

    if (event->type == GDK_BUTTON_PRESS && event->button == 1) {
        Gtk::TreeModel::Path path;
        Gtk::TreeViewColumn* column = nullptr;
        int cell_x, cell_y;

        if (tree_view.get_path_at_pos((int)event->x, (int)event->y, path, column, cell_x, cell_y)) {
            tree_view.set_cursor(path);
            show_context_menu(event);
            return true;
        }
    }
    return false;
}

void InventoryGUI::show_context_menu(GdkEventButton* event) noexcept {
    auto menu = Gtk::make_managed<Gtk::Menu>();

    auto modify_item = Gtk::make_managed<Gtk::MenuItem>("Modifica");
    modify_item->signal_activate().connect([this](void) {
        auto selected = tree_view.get_selection()->get_selected();
        if (selected) {

           Gtk::Dialog dialog("Inserisci i nuovi campi", *this);

            Gtk::Entry new_name;
            Gtk::Entry new_category;
            Gtk::Entry new_quantity;
            Gtk::Entry new_price;
            Gtk::Entry new_supplier;
            Gtk::Entry new_type;

            Gtk::Label name_label{"Nome"};
            Gtk::Label category_label{"Categoria"};
            Gtk::Label quantity_label{"Quantità"};
            Gtk::Label price_label{"Prezzo"};
            Gtk::Label supplier_label{"Fornitore"};
            Gtk::Label type_label{"Scarico/Carico"};

            dialog.set_default_size(400, 300);
            dialog.set_resizable(true);

            auto main_box = Gtk::make_managed<Gtk::Box>(Gtk::ORIENTATION_VERTICAL, 8);

            auto create_field = [](Gtk::Label& label, Gtk::Entry& entry) -> Gtk::Box* {
                auto field_box = Gtk::make_managed<Gtk::Box>(Gtk::ORIENTATION_VERTICAL, 10);
                label.set_margin_bottom(5);
                label.set_margin_top(5);
                entry.set_margin_bottom(5);
                field_box->pack_start(label, Gtk::PACK_SHRINK);
                field_box->pack_start(entry, Gtk::PACK_SHRINK);
                return field_box;
            };

            main_box->pack_start(*create_field(name_label, new_name), Gtk::PACK_SHRINK);
            main_box->pack_start(*create_field(category_label, new_category), Gtk::PACK_SHRINK);
            main_box->pack_start(*create_field(quantity_label, new_quantity), Gtk::PACK_SHRINK);
            main_box->pack_start(*create_field(price_label, new_price), Gtk::PACK_SHRINK);
            main_box->pack_start(*create_field(supplier_label, new_supplier), Gtk::PACK_SHRINK);
            main_box->pack_start(*create_field(type_label, new_type), Gtk::PACK_SHRINK);

            dialog.get_content_area()->pack_start(*main_box);
            dialog.add_button("OK", Gtk::RESPONSE_OK);
            dialog.add_button("Annulla", Gtk::RESPONSE_CANCEL);
            dialog.show_all_children();

            int respose=dialog.run();

            if(respose==Gtk::RESPONSE_OK){
                int quantity=std::stoi(new_quantity.get_text());
                double price=std::stod(new_price.get_text());
                if(price>0 && quantity>0 && new_name.get_text()!="" && new_category.get_text()!="" && new_supplier.get_text()!="" && new_type.get_text()!=""){

                (*selected)[columns.name]=new_name.get_text();
                (*selected)[columns.category]=new_category.get_text();
                (*selected)[columns.quantity]=quantity;
                (*selected)[columns.price]=price;
                (*selected)[columns.supplier]=new_supplier.get_text();
                (*selected)[columns.type]=new_type.get_text();

                }else{
                    Gtk::MessageDialog error(*this,"Campi errati, inserire valori validi");
                    error.run();
                }
            }else return;
        }
    });

    auto uncharge_item = Gtk::make_managed<Gtk::MenuItem>("Scarica");
    uncharge_item->signal_activate().connect([this](void) {
        auto selected = tree_view.get_selection()->get_selected();
        if (selected) {

            Glib::ustring name = (*selected)[columns.name];
            Glib::ustring category = (*selected)[columns.category];
            Glib::ustring supplier = (*selected)[columns.supplier];
            Glib::ustring date = (*selected)[columns.date];

            int quantity = (*selected)[columns.quantity];

            Gtk::Dialog dialog("Quantità da scaricare", *this);
            Gtk::Entry entry;

            dialog.set_default_size(600,100);
            dialog.set_resizable(true);
            dialog.get_content_area()->pack_start(entry);
            dialog.add_button("OK", Gtk::RESPONSE_OK);
            dialog.add_button("Annulla", Gtk::RESPONSE_CANCEL);

            entry.set_text("1");
            entry.show();

            int response = dialog.run();

            if (response == Gtk::RESPONSE_OK) {
                try {
                    int to_remove = std::stoi(entry.get_text());
                    if (to_remove <= 0) throw std::invalid_argument("Negativo");

                    if (to_remove > quantity) {
                        Gtk::MessageDialog error(*this, "Non puoi scaricare più di quanto disponibile");
                        error.run();
                        return;
                    }
                    double price=(*selected)[columns.price];
                    auto new_row = *(list->append());

                    new_row[columns.name] = name;
                    new_row[columns.category] = category;
                    new_row[columns.supplier] = supplier;
                    new_row[columns.date] = date;
                    new_row[columns.quantity] = to_remove;
                    new_row[columns.price] = price;
                    new_row[columns.type] = "Scarico";
                    (*selected)[columns.quantity] = quantity - to_remove;

                } catch (...) {
                    Gtk::MessageDialog error(*this, "Quantità non valida");
                    error.run();
                }
            }else return;
        }
    });

    auto remove_item = Gtk::make_managed<Gtk::MenuItem>("Elimina");
    remove_item->signal_activate().connect([this](void) {
        auto selected = tree_view.get_selection()->get_selected();
        if (selected) {

            auto model = tree_view.get_model();

            Glib::RefPtr<Gtk::TreeModelFilter> filter_model = Glib::RefPtr<Gtk::TreeModelFilter>::cast_dynamic(model);

            Gtk::TreeModel::iterator iter;
            if (filter_model) {

                Gtk::TreeModel::Path path = model->get_path(selected);
                Gtk::TreeModel::Path child_path = filter_model->convert_path_to_child_path(path);
                iter = list->get_iter(child_path);

            } else {
                iter = selected;
            }

            if (iter) {
                list->erase(iter);
            }
        }
    });


    menu->append(*modify_item);
    menu->append(*uncharge_item);
    menu->append(*remove_item);
    menu->show_all();

    menu->popup_at_pointer((GdkEvent*)event);
}

void InventoryGUI::filter_for(const Glib::RefPtr<Gtk::TreeModelFilter> filter) noexcept {
    tree_view.set_model(filter);
}

InventoryGUI::ModelColumns& InventoryGUI::get_columns(void) noexcept {
    return columns;
}

Glib::RefPtr<Gtk::ListStore> InventoryGUI::get_liststore(void) noexcept {
    return list;
}

AddProductGUI::AddProductGUI(InventoryGUI* inventory) noexcept : inventory_ptr(inventory) {

    set_title("Aggiungi prodotto");
    set_default_size(400, 300);

    box.set_spacing(10);
    box.set_margin_top(10);
    box.set_margin_bottom(10);
    box.set_margin_start(10);
    box.set_margin_end(10);

    box.pack_start(name_label);
    box.pack_start(entry_name);
    box.pack_start(category_label);
    box.pack_start(entry_category);
    box.pack_start(quantity_label);
    box.pack_start(entry_quantity);
    box.pack_start(price_label);
    box.pack_start(entry_price);
    box.pack_start(supplier_label);
    box.pack_start(entry_supplier);
    box.pack_start(date_label);
    
    vbox.pack_start(entry_date,Gtk::PACK_EXPAND_WIDGET);
    vbox.pack_start(now_button,Gtk::PACK_SHRINK);

    box.pack_start(vbox);
    box.pack_start(send_button);

    send_button.signal_clicked().connect(sigc::mem_fun(*this, &AddProductGUI::on_send_button_clicked));
    now_button.signal_clicked().connect(sigc::mem_fun(*this,&AddProductGUI::on_now_button_clicked));

    add(box);

    show_all_children();
}

void AddProductGUI::on_send_button_clicked(void) {

    Glib::ustring supplier = entry_supplier.get_text();
    Glib::ustring category = entry_category.get_text();
    Glib::ustring name = entry_name.get_text();
    Glib::ustring date = entry_date.get_text();

    std::string raw_date = date;
    Glib::ustring date_formattata;
    int giorno, mese, anno;
    double price = 0.0;
    int quantity = 0;

    if (name.empty()) {
        Gtk::MessageDialog dialog(*this, "Inserire un name per il prodotto", false, Gtk::MESSAGE_ERROR, Gtk::BUTTONS_OK, true);
        dialog.run();
        entry_name.set_text("");
        return;
    }

    if (category.empty()) {
        Gtk::MessageDialog dialog(*this, "Inserire una category per il prodotto", false, Gtk::MESSAGE_ERROR, Gtk::BUTTONS_OK, true);
        dialog.run();
        entry_category.set_text("");
        return;
    }

    try {
        price = std::stod(entry_price.get_text());
    } catch (...) {
        Gtk::MessageDialog dialog(*this, "Inserire un valore valido (es. 10.99)", false, Gtk::MESSAGE_ERROR, Gtk::BUTTONS_OK, true);
        dialog.run();
        entry_price.set_text("");
        return;
    }

    try {
        quantity = std::stoi(entry_quantity.get_text());
    } catch (...) {
        Gtk::MessageDialog dialog(*this, "Inserire una quantità valida (numero intero)", false, Gtk::MESSAGE_ERROR, Gtk::BUTTONS_OK, true);
        dialog.run();
        entry_quantity.set_text("");
        return;
    }
    
    if(supplier.empty()){
        Gtk::MessageDialog dialog(*this, "Inserire il fornitore del prodotto", false, Gtk::MESSAGE_ERROR, Gtk::BUTTONS_OK, true);
        dialog.run();
        entry_name.set_text("");
        return;
    }

    if (sscanf(raw_date.c_str(), "%d/%d/%d", &giorno, &mese, &anno) == 3) {
        char formatted_date[11];
        snprintf(formatted_date, sizeof(formatted_date), "%02d/%02d/%04d", giorno, mese, anno);
        date_formattata = formatted_date;
    } else {
        Gtk::MessageDialog dialog(*this, "Formato date non valido. Usa GG/MM/AAAA.", false, Gtk::MESSAGE_ERROR, Gtk::BUTTONS_OK, true);
        dialog.run();
        entry_date.set_text("");
        return;
    }

    inventory_ptr->add_item(name, category, quantity, price, date_formattata,supplier);
    close();
}


void AddProductGUI::on_now_button_clicked(void){

    std::time_t now=std::time(nullptr);
    std::tm* local_time=std::localtime(&now);
    int y=local_time->tm_year+1900;
    int m=local_time->tm_mon+1;
    int g=local_time->tm_mday;

    entry_date.set_text(std::to_string(g)+"/"+std::to_string(m)+"/"+std::to_string(y));
}

FilterProductGUI::FilterProductGUI(InventoryGUI* inventory) noexcept: inventory_ptr(inventory){

    set_title("Filtri Prodotti");
    set_border_width(20);
    set_default_size(500, 400);

    title_label.set_markup("<b><big>Seleziona un filtro</big></b>");
    title_label.set_halign(Gtk::ALIGN_CENTER);
    
    combo_category.remove_all();
    combo_quantity.append("");
    combo_category.append("");
    combo_supplier.append("");
    combo_price.append("");
    combo_date.append("");
    combo_name.append("");
    combo_type.append("");

    auto& columns=inventory->get_columns();
    auto list=inventory->get_liststore();
    filter=Gtk::TreeModelFilter::create(list);
    
    for(auto iter=list->children().begin();iter!=list->children().end();++iter){

        set_category.insert((*iter)[columns.category]);
        set_quantity.insert((*iter)[columns.quantity]);
        set_supplier.insert((*iter)[columns.supplier]);
        set_price.insert((*iter)[columns.price]);
        set_date.insert((*iter)[columns.date]);
        set_name.insert((*iter)[columns.name]);
    }

    for(auto& categories : set_category) combo_category.append(categories);
    for(auto& dates : set_date)          combo_date.append(dates);
    for(auto& prices : set_price)        combo_price.append(std::to_string(prices));
    for(auto& sizes : set_quantity)      combo_quantity.append(std::to_string(sizes));
    for(auto& names : set_name)          combo_name.append(names);
    for(auto& types : set_type)          combo_type.append(types);
    for(auto& suppliers : set_supplier)  combo_supplier.append(suppliers);

    main_box.pack_start(title_label, Gtk::PACK_SHRINK);
    main_box.pack_start(separator, Gtk::PACK_SHRINK);

    category_box.pack_start(filter_by_category_button,Gtk::PACK_EXPAND_WIDGET);
    category_box.pack_start(combo_category,Gtk::PACK_EXPAND_WIDGET);

    date_box.pack_start(filter_by_date_button,Gtk::PACK_EXPAND_WIDGET);
    date_box.pack_start(combo_date,Gtk::PACK_EXPAND_WIDGET);

    price_box.pack_start(filter_by_price_button,Gtk::PACK_EXPAND_WIDGET);
    price_box.pack_start(combo_price,Gtk::PACK_EXPAND_WIDGET);

    quantity_box.pack_start(filter_by_quantity_button,Gtk::PACK_EXPAND_WIDGET);
    quantity_box.pack_start(combo_quantity,Gtk::PACK_EXPAND_WIDGET);

    name_box.pack_start(filter_by_name_button,Gtk::PACK_EXPAND_WIDGET);
    name_box.pack_start(combo_name,Gtk::PACK_EXPAND_WIDGET);

    type_box.pack_start(filter_by_type_button,Gtk::PACK_EXPAND_WIDGET);
    type_box.pack_start(combo_type,Gtk::PACK_EXPAND_WIDGET);

    supplier_box.pack_start(filter_by_supplier_button,Gtk::PACK_EXPAND_WIDGET);
    supplier_box.pack_start(combo_supplier,Gtk::PACK_EXPAND_WIDGET);

    main_box.pack_start(name_box);
    main_box.pack_start(category_box);
    main_box.pack_start(quantity_box);
    main_box.pack_start(price_box);
    main_box.pack_start(date_box);
    main_box.pack_start(supplier_box);
    main_box.pack_start(type_box);
    main_box.pack_start(reset_filters_button);

    filter_by_category_button.signal_clicked().connect([this](void){

        is_category=!is_category;
        filter->refilter();
    });
    
    filter_by_quantity_button.signal_clicked().connect([this](void){

        is_quantity=!is_quantity;
        filter->refilter();
    });

    filter_by_price_button.signal_clicked().connect([this](void){

        is_price=!is_price;
        filter->refilter();
    });
    
    filter_by_date_button.signal_clicked().connect([this](void){

        is_date=!is_date;
        filter->refilter();
    });

    filter_by_name_button.signal_clicked().connect([this](void){

        is_name=!is_name;
        filter->refilter();
    });

    filter_by_type_button.signal_clicked().connect([this](void){

        is_type=!is_type;
        filter->refilter();
    });

    filter_by_supplier_button.signal_clicked().connect([this](void){

        is_supplier=!is_supplier;
    });

    reset_filters_button.signal_clicked().connect([this](void){

        is_reset=!is_reset;
        filter->refilter();
    });

    filter->set_visible_func([&](const Gtk::TreeModel::const_iterator& iter) -> bool {

        if (!iter) return false;
        if (is_reset) return true;

        if (is_category) {

            auto selected_category = combo_category.get_active_text();
            if (selected_category.empty() || (*iter)[columns.category] != selected_category) return false;
        }

        if (is_date) {

            auto selected_date = combo_date.get_active_text();
            if ((*iter)[columns.date] != selected_date) return false;
        }

        if (is_price) {

            double price_filter = std::stod(combo_price.get_active_text());
            if ((*iter)[columns.price] == price_filter) return false;
        }

        if (is_quantity) {

            int quantity_filter = std::stoi(combo_quantity.get_active_text());
            if ((*iter)[columns.quantity] == quantity_filter) return false;
        }

        if (is_name){

            auto name_filter = combo_name.get_active_text();
            if(name_filter.empty() || (*iter)[columns.name] != name_filter) return false;
        }

        if (is_type){

            auto type_filter = combo_type.get_active_text();
            if(type_filter.empty() || (*iter)[columns.type] != type_filter) return false;
        }
        
        if (is_supplier){

            auto supplier_filter = combo_supplier.get_active_text();
            if(supplier_filter.empty() || (*iter)[columns.supplier] != supplier_filter) return false;
        }

        return true;
    });

    inventory->filter_for(filter);

    add(main_box);

    show_all_children();
} 


