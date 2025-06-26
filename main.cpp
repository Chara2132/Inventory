#include "inventory.hpp"
#include "inventory_manager.hpp"
#include "client.hpp"
#include "client_manager.hpp"

void InventoryGUI::on_add_button_pressed(void) noexcept {
    auto window = std::make_shared<AddProductGUI>(this);
    window->set_transient_for(*this);
    window->show();
    child_windows.push_back(window);
}

void InventoryGUI::on_filter_button_pressed(void) noexcept {
    auto window = std::make_shared<FilterProductGUI>(this);
    window->set_transient_for(*this);
    window->show();
    child_windows.push_back(window);
}

void InventoryGUI::on_client_button_pressed(void) noexcept {
    auto window = std::make_shared<ClientGUI>();
    window->set_transient_for(*this);
    window->show();
    child_windows.push_back(window);
}

void ClientGUI::on_add_client_pressed(void) noexcept {
    auto window = std::make_shared<AddClientGUI>(this);
    window->set_transient_for(*this);
    window->show();
    child_windows.push_back(window);
}

int main(int argc, char* argv[]) {

    auto app = Gtk::Application::create(argc, argv, "org.gtkmm.inventory");
    InventoryGUI window;
    
    return app->run(window);
}
