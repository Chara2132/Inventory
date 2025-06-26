#include "client.hpp"
#include "client_manager.hpp"
#include <gtkmm.h>

ClientGUI::ClientGUI(void) noexcept {
    set_title("Gestione Clienti");
    maximize();

    list = Gtk::ListStore::create(columns);
    tree_view.set_model(list);

    // Colonne
    tree_view.append_column("Tipo", columns.client_type);
    tree_view.append_column("Nome", columns.name);
    tree_view.append_column("Cognome", columns.last_name);
    tree_view.append_column("Codice Fiscale", columns.tax_id_code);
    tree_view.append_column("IVA", columns.IVA);
    tree_view.append_column("Nome dell'Azienda", columns.agency_name);
    tree_view.append_column("Indirizzo legale", columns.registered_office);
    tree_view.append_column("Nome Referente", columns.referent_name);
    tree_view.append_column("Indirizzo Consegna", columns.address);
    tree_view.append_column("Codice PEC/SDI", columns.PEC_code);
    tree_view.append_column("Numero di telefono", columns.cell_number);
    tree_view.append_column("Email", columns.email);
    tree_view.append_column("Metodo di pagamento", columns.payment_method);
    tree_view.append_column("Termini di pagamento", columns.payments_terms);
    tree_view.append_column("Note", columns.note);

    // 🔧 Scrolled window per gestire lo scroll orizzontale e verticale
    scrolled_window.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
    scrolled_window.add(tree_view);
    scrolled_window.set_hexpand(true);
    scrolled_window.set_vexpand(true);

    // Pulsante per aggiungere cliente
    add_client_button.signal_clicked().connect(sigc::mem_fun(*this, &ClientGUI::on_add_client_pressed));

    // Titoli e layout
    title.set_markup("<b><big>Gestione dei clienti</big></b>");
    title.set_halign(Gtk::ALIGN_CENTER);
    title.set_margin_bottom(20);

    table_title.set_markup("<b><big>Tabella Clienti</big></b>");
    table_title.set_halign(Gtk::ALIGN_CENTER);
    table_title.set_margin_bottom(10);

    main_box.set_spacing(20);
    main_box.set_margin_top(20);
    main_box.set_margin_bottom(20);
    main_box.set_margin_start(20);
    main_box.set_margin_end(20);

    vbox.set_spacing(10);
    vbox.set_margin_bottom(10);

    tablebox.set_spacing(10);
    tablebox.set_margin_top(10);

    // Layout aggiornato con scrolled window
    vbox.pack_start(add_client_button, Gtk::PACK_SHRINK);
    tablebox.pack_start(scrolled_window, Gtk::PACK_EXPAND_WIDGET);  // << Cambiato qui

    main_box.pack_start(title, Gtk::PACK_SHRINK);
    main_box.pack_start(separator, Gtk::PACK_SHRINK);
    main_box.pack_start(vbox, Gtk::PACK_SHRINK);
    main_box.pack_start(table_title, Gtk::PACK_SHRINK);
    main_box.pack_start(separator2, Gtk::PACK_SHRINK);
    main_box.pack_start(tablebox, Gtk::PACK_EXPAND_WIDGET);

    add(main_box);
    show_all_children();
}

void ClientGUI::add_client(const Glib::ustring& registered_office, const Glib::ustring& payment_method, const Glib::ustring& payments_terms, const Glib::ustring& referent_name, const Glib::ustring& client_type, const Glib::ustring& tax_id_code, const Glib::ustring& cell_number, const Glib::ustring& agency_name, const Glib::ustring& last_name, const Glib::ustring& PEC_code, const Glib::ustring& address, const Glib::ustring& email, const Glib::ustring& name, const Glib::ustring& note, const Glib::ustring& IVA) noexcept{

    auto row=*(list->append());
    row[columns.registered_office] = registered_office;
    row[columns.payment_method] = payment_method;
    row[columns.payments_terms] = payments_terms;
    row[columns.referent_name] = referent_name;
    row[columns.client_type] = client_type;
    row[columns.tax_id_code] = tax_id_code;
    row[columns.cell_number] = cell_number;
    row[columns.agency_name] = agency_name;
    row[columns.last_name] = last_name;
    row[columns.PEC_code] = PEC_code;
    row[columns.address] = address;
    row[columns.email] = email;
    row[columns.name] = name;
    row[columns.note] = note;
    row[columns.IVA] = IVA;
}

bool ClientGUI::on_tree_view_click(GdkEventButton* event) noexcept {

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

void ClientGUI::show_context_menu(GdkEventButton* event) noexcept{
    auto menu = Gtk::make_managed<Gtk::Menu>();

    auto modify_client = Gtk::make_managed<Gtk::MenuItem>("Modifica");
    modify_client->signal_activate().connect([this](void) {
        auto selected = tree_view.get_selection()->get_selected();
        if (selected) {
            Gtk::Dialog dialog("Inserisci i nuovi campi", *this);

            Gtk::Entry new_client_type;
            Gtk::Entry new_registered_office;
            Gtk::Entry new_payments_terms;
            Gtk::Entry new_payments_method;
            Gtk::Entry new_referent_name;
            Gtk::Entry new_agency_name;
            Gtk::Entry new_cell_number;
            Gtk::Entry new_tax_id_code;
            Gtk::Entry new_name;
            Gtk::Entry new_IVA;
            Gtk::Entry new_note;
            Gtk::Entry new_email;
            Gtk::Entry new_address;
            Gtk::Entry new_PEC_code;
            Gtk::Entry new_last_name;

            Gtk::Label registered_office_label{"Indirizzo legale"};
            Gtk::Label client_type_label{"Tipo cliente (Privato/Azienda)"};
            Gtk::Label payments_terms_label{"Termini pagamento"};
            Gtk::Label payment_method_label{"Metodo di pagamento"};
            Gtk::Label referent_name_label{"Nome referente"};
            Gtk::Label agency_name_label{"Nome dell' Agenzia/Azienda"};
            Gtk::Label cell_number_label{"Numero di telefono"};
            Gtk::Label tax_id_code_label{"Codice fiscale"};
            Gtk::Label last_name_label{"Cognome"};
            Gtk::Label PEC_code_label{"Codice PEC/SDI"};
            Gtk::Label address_label{"Indirizzo consegna"};
            Gtk::Label email_label{"Email"};
            Gtk::Label name_label{"Nome"};
            Gtk::Label note_label{"Note"};
            Gtk::Label IVA_label{"Partita IVA"};

            dialog.set_default_size(700, 500);
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
        
            main_box->pack_start(*create_field(client_type_label, new_client_type), Gtk::PACK_SHRINK);
            main_box->pack_start(*create_field(name_label,new_name), Gtk::PACK_SHRINK);
            main_box->pack_start(*create_field(last_name_label, new_last_name), Gtk::PACK_SHRINK);
            main_box->pack_start(*create_field(tax_id_code_label,new_tax_id_code), Gtk::PACK_SHRINK);
            main_box->pack_start(*create_field(IVA_label,new_IVA), Gtk::PACK_SHRINK);
            main_box->pack_start(*create_field(agency_name_label,new_agency_name), Gtk::PACK_SHRINK);
            main_box->pack_start(*create_field(registered_office_label,new_registered_office), Gtk::PACK_SHRINK);
            main_box->pack_start(*create_field(referent_name_label,new_referent_name), Gtk::PACK_SHRINK);
            main_box->pack_start(*create_field(address_label,new_address), Gtk::PACK_SHRINK);
            main_box->pack_start(*create_field(PEC_code_label,new_PEC_code), Gtk::PACK_SHRINK);
            main_box->pack_start(*create_field(cell_number_label,new_cell_number), Gtk::PACK_SHRINK);
            main_box->pack_start(*create_field(email_label,new_email), Gtk::PACK_SHRINK);
            main_box->pack_start(*create_field(payment_method_label,new_payments_method), Gtk::PACK_SHRINK);
            main_box->pack_start(*create_field(payments_terms_label,new_payments_terms), Gtk::PACK_SHRINK);
            main_box->pack_start(*create_field(note_label,new_note), Gtk::PACK_SHRINK);

            dialog.get_content_area()->pack_start(*main_box);
            dialog.add_button("OK", Gtk::RESPONSE_OK);
            dialog.add_button("Annulla", Gtk::RESPONSE_CANCEL);
            dialog.show_all_children();

            int respose=dialog.run();

            if(respose==Gtk::RESPONSE_OK){
                if (
                    !new_client_type.get_text().empty() &&
                    !new_registered_office.get_text().empty() &&
                    !new_payments_terms.get_text().empty() &&
                    !new_payments_method.get_text().empty() &&
                    !new_referent_name.get_text().empty() &&
                    !new_agency_name.get_text().empty() &&
                    !new_cell_number.get_text().empty() &&
                    !new_tax_id_code.get_text().empty() &&
                    !new_name.get_text().empty() &&
                    !new_IVA.get_text().empty() &&
                    !new_note.get_text().empty() &&
                    !new_email.get_text().empty() &&
                    !new_address.get_text().empty() &&
                    !new_PEC_code.get_text().empty() &&
                    !new_last_name.get_text().empty()
                ){
                    (*selected)[columns.client_type] = new_client_type.get_text();
                    (*selected)[columns.registered_office] = new_registered_office.get_text();
                    (*selected)[columns.payments_terms] = new_payments_terms.get_text();
                    (*selected)[columns.payment_method] = new_payments_method.get_text();
                    (*selected)[columns.referent_name] = new_referent_name.get_text();
                    (*selected)[columns.agency_name] = new_agency_name.get_text();
                    (*selected)[columns.cell_number] = new_cell_number.get_text();
                    (*selected)[columns.tax_id_code] = new_tax_id_code.get_text();
                    (*selected)[columns.name] = new_name.get_text();
                    (*selected)[columns.IVA] = new_IVA.get_text();
                    (*selected)[columns.note] = new_note.get_text();
                    (*selected)[columns.email] = new_email.get_text();
                    (*selected)[columns.address] = new_address.get_text();
                    (*selected)[columns.PEC_code] = new_PEC_code.get_text();
                    (*selected)[columns.last_name] = new_last_name.get_text();
                }else{
                    Gtk::MessageDialog dialog(*this,"Inserire dei valori corretti");
                    dialog.run();
                }
            }else return;
        }
    });
    auto remove_client = Gtk::make_managed<Gtk::MenuItem>("Elimina");
    remove_client->signal_activate().connect([this](void){

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
    
    menu->append(*modify_client);
    menu->append(*remove_client);
    menu->show_all();

    menu->popup_at_pointer((GdkEvent*)event);
}

AddClientGUI::AddClientGUI(ClientGUI* clients) noexcept : clients{clients} {

    set_default_size(700,500);
    set_title("Aggiungi un cliente");

    auto main_box = Gtk::make_managed<Gtk::Box>(Gtk::ORIENTATION_VERTICAL, 1);
    auto button_box = Gtk::make_managed<Gtk::Box>(Gtk::ORIENTATION_HORIZONTAL,2);
    auto create_field = [](Gtk::Label& label, Gtk::Entry& entry) -> Gtk::Box* {
        auto field_box = Gtk::make_managed<Gtk::Box>(Gtk::ORIENTATION_VERTICAL, 2);
        field_box->pack_start(label, Gtk::PACK_SHRINK);
        field_box->pack_start(entry, Gtk::PACK_SHRINK);
        return field_box;
    };

    send_button.set_margin_bottom(30);
    send_button.set_margin_start(30);
    send_button.set_margin_end(30);
    send_button.set_margin_top(30);

    deny_button.set_margin_bottom(30);
    deny_button.set_margin_end(30);
    deny_button.set_margin_start(30);
    deny_button.set_margin_top(30);

    button_box->pack_start(deny_button,Gtk::PACK_EXPAND_WIDGET);
    button_box->pack_start(send_button,Gtk::PACK_EXPAND_WIDGET);

    main_box->pack_start(*create_field(client_type_label, new_client_type), Gtk::PACK_SHRINK);
    main_box->pack_start(*create_field(name_label,new_name), Gtk::PACK_SHRINK);
    main_box->pack_start(*create_field(last_name_label, new_last_name), Gtk::PACK_SHRINK);
    main_box->pack_start(*create_field(tax_id_code_label,new_tax_id_code), Gtk::PACK_SHRINK);
    main_box->pack_start(*create_field(IVA_label,new_IVA), Gtk::PACK_SHRINK);
    main_box->pack_start(*create_field(agency_name_label,new_agency_name), Gtk::PACK_SHRINK);
    main_box->pack_start(*create_field(registered_office_label,new_registered_office), Gtk::PACK_SHRINK);
    main_box->pack_start(*create_field(referent_name_label,new_referent_name), Gtk::PACK_SHRINK);
    main_box->pack_start(*create_field(address_label,new_address), Gtk::PACK_SHRINK);
    main_box->pack_start(*create_field(PEC_code_label,new_PEC_code), Gtk::PACK_SHRINK);
    main_box->pack_start(*create_field(cell_number_label,new_cell_number), Gtk::PACK_SHRINK);
    main_box->pack_start(*create_field(email_label,new_email), Gtk::PACK_SHRINK);
    main_box->pack_start(*create_field(payment_method_label,new_payments_method), Gtk::PACK_SHRINK);
    main_box->pack_start(*create_field(payments_terms_label,new_payments_terms), Gtk::PACK_SHRINK);
    main_box->pack_start(*create_field(note_label,new_note), Gtk::PACK_SHRINK);
    main_box->pack_start(*button_box);

    send_button.signal_clicked().connect(sigc::mem_fun(*this,&AddClientGUI::on_send_button_clicked));
    deny_button.signal_clicked().connect([](void){this->close();});

    add(*main_box);

    show_all_children();
}

void AddClientGUI::on_send_button_clicked(void) noexcept{

    clients->add_client(
        new_registered_office.get_text(),  
        new_payments_method.get_text(),    
        new_payments_terms.get_text(),     
        new_referent_name.get_text(),      
        new_client_type.get_text(),        
        new_tax_id_code.get_text(),        
        new_cell_number.get_text(),        
        new_agency_name.get_text(),        
        new_last_name.get_text(),          
        new_PEC_code.get_text(),                   
        new_address.get_text(),            
        new_email.get_text(),              
        new_name.get_text(),               
        new_note.get_text(),               
        new_IVA.get_text()                 
    );
    this->close();
}
