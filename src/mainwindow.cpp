#include "mainwindow.h"
#include <sstream>
#include <iostream>

MainWindow::MainWindow(latero::graphics::TactileEngine *tEngine, latero::graphics::AudioEngine *aEngine, std::vector<std::string> generators) :
	managerWidget_(tEngine, aEngine)
{
	set_title("Latero Demo");
	set_size_request(1000,800);

	auto box = new Gtk::Box(Gtk::Orientation::VERTICAL);
	box->set_margin(10);

	set_child(*Gtk::manage(box));
	box->append(*Gtk::manage(CreateMenu()));
	box->append(managerWidget_);
	managerWidget_.set_expand();

	maximize();

	for (auto& gen : generators)
		AddGenerator(gen);
}

Gtk::Widget *MainWindow::CreateMenu()
{
	// Create action group and add actions
	auto action_group = Gio::SimpleActionGroup::create();
	action_group->add_action("open",  sigc::mem_fun(*this, &MainWindow::OnOpen));
	action_group->add_action("save",  sigc::mem_fun(*this, &MainWindow::OnSave));
	action_group->add_action("close", sigc::mem_fun(*this, &MainWindow::OnClose));
	insert_action_group("file", action_group);

	// Define the menubar using Builder XML
	auto builder = Gtk::Builder::create_from_string(R"(
	<?xml version="1.0" encoding="UTF-8"?>
	<interface>
  	<menu id="MenuBar">
    <submenu>
      <attribute name="label">File</attribute>
      <item>
        <attribute name="label">Open</attribute>
        <attribute name="action">file.open</attribute>
      </item>
      <item>
        <attribute name="label">Save</attribute>
        <attribute name="action">file.save</attribute>
      </item>
      <item>
        <attribute name="label">Close</attribute>
        <attribute name="action">file.close</attribute>
      </item>
    </submenu>
  	</menu>
	</interface>
	)");

	// Get the menu model and create a PopoverMenuBar from it
	auto menu_model = builder->get_object<Gio::Menu>("MenuBar");
	auto menubar = Gtk::manage(new Gtk::PopoverMenuBar(menu_model));
	return menubar;

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
