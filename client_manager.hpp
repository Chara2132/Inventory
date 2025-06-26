#ifndef CLIENT_MANAGER_HPP
#define CLIENT_MANAGER_HPP

class AddClientGUI final : public Gtk::Window{

    private:

        Gtk::Entry new_registered_office;
        Gtk::Entry new_payments_method;
        Gtk::Entry new_payments_terms;
        Gtk::Entry new_referent_name;
        Gtk::Entry new_client_type;
        Gtk::Entry new_agency_name;
        Gtk::Entry new_cell_number;
        Gtk::Entry new_tax_id_code;
        Gtk::Entry new_last_name;
        Gtk::Entry new_PEC_code;
        Gtk::Entry new_address;
        Gtk::Entry new_email;
        Gtk::Entry new_name;
        Gtk::Entry new_note;
        Gtk::Entry new_IVA;

        Gtk::Label client_type_label{"Tipo cliente (Privato/Azienda)"};
        Gtk::Label agency_name_label{"Nome dell' Agenzia/Azienda"};
        Gtk::Label registered_office_label{"Indirizzo legale"};
        Gtk::Label payment_method_label{"Metodo di pagamento"};
        Gtk::Label payments_terms_label{"Termini pagamento"};
        Gtk::Label cell_number_label{"Numero di telefono"};
        Gtk::Label referent_name_label{"Nome referente"};
        Gtk::Label tax_id_code_label{"Codice fiscale"};
        Gtk::Label address_label{"Indirizzo consegna"};
        Gtk::Label PEC_code_label{"Codice PEC/SDI"};
        Gtk::Label last_name_label{"Cognome"};
        Gtk::Label IVA_label{"Partita IVA"};
        Gtk::Label email_label{"Email"};
        Gtk::Label name_label{"Nome"};
        Gtk::Label note_label{"Note"};

        Gtk::Button send_button{"Aggiungi cliente"};
        Gtk::Button deny_button{"Annulla"};
        
        ClientGUI* clients;


        void on_send_button_clicked(void) noexcept;
        
    public:

        explicit AddClientGUI(ClientGUI* clients) noexcept;
};
#endif