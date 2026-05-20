#include "mainwindow.h"
#include <sstream>
#include <iostream>

MainWindow::MainWindow(latero::graphics::TactileEngine *tEngine, latero::graphics::AudioEngine *aEngine, std::vector<std::string> generators) :
	managerWidget_(tEngine, aEngine)
{
	set_title("Latero Demo");
	set_size_request(1000,800);
	set_child(managerWidget_);
	signal_realize().connect(sigc::mem_fun(*this, &MainWindow::CreateMenu));
	maximize();

	for (auto& gen : generators)
		AddGenerator(gen);
}

void MainWindow::CreateMenu()
{
	// Register actions directly on the window so they're accessible as win.*
	add_action("open",  sigc::mem_fun(*this, &MainWindow::OnOpen));
	add_action("save",  sigc::mem_fun(*this, &MainWindow::OnSave));
	add_action("close", sigc::mem_fun(*this, &MainWindow::OnClose));

	// Define the menubar using Builder XML
	auto builder = Gtk::Builder::create_from_string(R"(
	<?xml version="1.0" encoding="UTF-8"?>
	<interface>
  	<menu id="MenuBar">
    <submenu>
      <attribute name="label">File</attribute>
      <item>
        <attribute name="label">Open</attribute>
        <attribute name="action">win.open</attribute>
      </item>
      <item>
        <attribute name="label">Save</attribute>
        <attribute name="action">win.save</attribute>
      </item>
      <item>
        <attribute name="label">Close</attribute>
        <attribute name="action">win.close</attribute>
      </item>
    </submenu>
  	</menu>
	</interface>
	)");

	// Get the menu model and create a PopoverMenuBar from it
	auto menu_model = builder->get_object<Gio::Menu>("MenuBar");
	get_application()->set_menubar(menu_model);
	set_show_menubar(true);
}

MainWindow::~MainWindow()
{
}

void MainWindow::OnOpen()
{
	managerWidget_.Open();
}

void MainWindow::OnSave()
{
	managerWidget_.Save();
}

void MainWindow::OnClose()
{
	managerWidget_.Close();
}
