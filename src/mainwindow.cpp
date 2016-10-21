#include "mainwindow.h"
#include <sstream>
#include <iostream>

MainWindow::MainWindow(latero::graphics::TactileEngine *tEngine, latero::graphics::AudioEngine *aEngine) :
	managerWidget_(tEngine, aEngine)
{
	set_title("STReSS2 Tactile Graphics Studio");
	set_border_width(10);
	set_size_request(1000,800);

	Gtk::VBox *box = new Gtk::VBox;

	add(*manage(box));
	box->pack_start(*manage(CreateMenu()), Gtk::PACK_SHRINK);
	box->pack_start(managerWidget_);

	maximize();
	show_all_children();
}

Gtk::Widget *MainWindow::CreateMenu()
{
	Glib::RefPtr<Gtk::ActionGroup> group = Gtk::ActionGroup::create();

	group->add(Gtk::Action::create("FileMenu", "File"));
	group->add(Gtk::Action::create("FileOpen", Gtk::Stock::OPEN),
  		sigc::mem_fun(*this, &MainWindow::OnOpen));
	group->add(Gtk::Action::create("FileSave", Gtk::Stock::SAVE),
  		sigc::mem_fun(*this, &MainWindow::OnSave));
	group->add(Gtk::Action::create("FileClose", Gtk::Stock::CLOSE),
  		sigc::mem_fun(*this, &MainWindow::OnClose));

	uiManager_ = Gtk::UIManager::create();
	uiManager_->insert_action_group(group);
 	add_accel_group(uiManager_->get_accel_group());

	std::stringstream fileMenuUI;
	fileMenuUI << "<menu action='FileMenu'>";
	fileMenuUI << 	"<menuitem action='FileOpen'/>";
	fileMenuUI << 	"<menuitem action='FileSave'/>";
	fileMenuUI << 	"<menuitem action='FileClose'/>";
	fileMenuUI << "</menu>";

	std::stringstream buf;
	buf << "<ui>";
	buf << "<menubar name='MenuBar'>";
	buf << fileMenuUI.str();
	buf << "</menubar>";
	buf << "</ui>";

	try
	{	
		uiManager_->add_ui_from_string(buf.str());
	}
	catch(const Glib::Error& ex)
	{
		std::cerr << "building menus failed: " <<  ex.what();
		exit(1);
	}

	return uiManager_->get_widget("/MenuBar");
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


	
