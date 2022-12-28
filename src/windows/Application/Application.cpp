#include "Application.h"

#include <iostream>
#include <gtkmm/builder.h>

#include "projectdefinitions.h"

Application::Application() : Gtk::Application(projectdefinitions::getApplicationID() + ".application") {
}

Application::~Application() = default;

Glib::RefPtr<Application> Application::create() {
    return Glib::RefPtr<Application>(new Application());
}

ServerSetupWindow* Application::createWindow() {
    auto window = ServerSetupWindow::create();
    add_window(*window);
    window->signal_hide().connect(sigc::bind(sigc::mem_fun(*this, &Application::on_hide_window), window));
    window->show();
    return window;
}

void Application::on_activate() {
    try {
        auto window = createWindow();
        window->present();
    } catch (const Glib::Error &ex) {
        std::cerr << "Application::on_activate(): " << ex.what() << std::endl;
    } catch (const std::exception &ex) {
        std::cerr << "Application::on_activate(): " << ex.what() << std::endl;
    }
}

void Application::on_startup() {
    Gtk::Application::on_startup();
    add_action("quit", sigc::mem_fun(*this, &Application::on_action_quit));
}

void Application::on_hide_window(Gtk::Window *window) {
    delete window;
}

void Application::on_action_quit() {
    auto windows = get_windows();
    for (auto window : windows) {
        window->hide();
    }
    quit();
}
