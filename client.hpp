#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <gtkmm.h>
#include <vector>

class ClientGUI final : public Gtk::Window{

    private:

        class ModelColumns final : public Gtk::TreeModel::ColumnRecord {

            public:

                Gtk::TreeModelColumn<Glib::ustring> registered_office;
                Gtk::TreeModelColumn<Glib::ustring> payment_method;
                Gtk::TreeModelColumn<Glib::ustring> payments_terms;
                Gtk::TreeModelColumn<Glib::ustring> referent_name;
                Gtk::TreeModelColumn<Glib::ustring> agency_name;
                Gtk::TreeModelColumn<Glib::ustring> cell_number;
                Gtk::TreeModelColumn<Glib::ustring> tax_id_code;
                Gtk::TreeModelColumn<Glib::ustring> last_name;
                Gtk::TreeModelColumn<Glib::ustring> PEC_code;
                Gtk::TreeModelColumn<Glib::ustring> address;
                Gtk::TreeModelColumn<Glib::ustring> email;
                Gtk::TreeModelColumn<Glib::ustring> name;
                Gtk::TreeModelColumn<Glib::ustring> note;
                Gtk::TreeModelColumn<Glib::ustring> IVA;
                Gtk::TreeModelColumn<Glib::ustring> client_type;


                explicit inline ModelColumns(void) noexcept {
                    add(registered_office);
                    add(payments_terms);
                    add(payment_method);
                    add(referent_name);
                    add(client_type);
                    add(agency_name);
                    add(cell_number);
                    add(tax_id_code);
                    add(last_name);
                    add(PEC_code);
                    add(address);
                    add(email);
                    add(name);
                    add(note);
                    add(IVA);
                }
        };

        Gtk::Label title;
        Gtk::Label table_title;

        Gtk::Button add_client_button{"Aggiungi un cliente"};

        Gtk::Box main_box{Gtk::ORIENTATION_VERTICAL,40};
        Gtk::Box vbox{Gtk::ORIENTATION_HORIZONTAL,40};
        Gtk::Box tablebox{Gtk::ORIENTATION_HORIZONTAL,40};

        Gtk::Separator separator;
        Gtk::Separator separator2;

        Gtk::TreeView tree_view;

        Glib::RefPtr<Gtk::ListStore> list;

        Gtk::ScrolledWindow scrolled_window;

        ModelColumns columns;

        std::vector<std::shared_ptr<Gtk::Window>> child_windows;

        void on_add_client_pressed(void) noexcept;

        bool on_tree_view_click(GdkEventButton* event) noexcept;

    public:

        explicit ClientGUI(void) noexcept;

        void add_client(const Glib::ustring& registered_office, const Glib::ustring& payment_method, const Glib::ustring& payments_terms, const Glib::ustring& referent_name, const Glib::ustring& client_type, const Glib::ustring& tax_id_code, const Glib::ustring& cell_number, const Glib::ustring& agency_name, const Glib::ustring& last_name, const Glib::ustring& PEC_code, const Glib::ustring& address, const Glib::ustring& email, const Glib::ustring& name, const Glib::ustring& note, const Glib::ustring& IVA) noexcept;

        void show_context_menu(GdkEventButton* event) noexcept;
};

#endif 