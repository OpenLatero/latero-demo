#include "mainwindow.h"
#include <assert.h>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <laterographics/generator.h>

MainWindow::MainWindow(latero::graphics::TactileEngine *tEngine, latero::graphics::AudioEngine *aEngine, std::vector<std::string> generators) :
	preview_(tEngine->Dev(), latero::graphics::GeneratorPtr(), true),
	tEngine_(tEngine),
	aEngine_(aEngine)
{
	auto box = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::VERTICAL);
	auto exp = Gtk::make_managed<Gtk::Expander>("settings");
	exp->set_expanded(true);
	set_child(*box);
	box->append(preview_);
	preview_.set_vexpand();
	preview_.set_hexpand();
	box->append(*exp);
	exp->set_child(notebook_);
	exp->set_vexpand(false);

	switch_page_conn_ = notebook_.signal_switch_page().connect(
		sigc::mem_fun(*this,&MainWindow::OnPageSwitch));

	auto key_controller = Gtk::EventControllerKey::create();
	key_controller->signal_key_pressed().connect(
		sigc::mem_fun(*this, &MainWindow::OnKeyPress), false);
	add_controller(key_controller);

	set_title("Latero Demo");
	set_size_request(1000,800);
	maximize();

	for (auto& gen : generators)
		AddGenerator(gen);

};

MainWindow::~MainWindow()
{
	switch_page_conn_.disconnect();
}

void MainWindow::OnPageSwitch(Gtk::Widget* page, guint page_num)
{
	UpdateCurrentGenerator();
}


void MainWindow::AddGenerator(std::string filename) {
    std::string name = std::filesystem::path(filename).stem().string();
    AddGenerator(latero::graphics::Generator::Create(filename, tEngine_->Dev()), name);
}

void MainWindow::AddGenerator(latero::graphics::GeneratorPtr gen, std::string name)
{
	list_.push_back(gen);
	int i = notebook_.append_page(*gen->CreateWidget(gen), name);
	//notebook_.set_current_page(i);
	UpdateCurrentGenerator();
}

void MainWindow::UpdateCurrentGenerator()
{
	int page = notebook_.get_current_page();
	if (page<0)
		currentGen_.reset();
	else  
		currentGen_ = list_[page];

	preview_.SetGenerator(currentGen_);
	tEngine_->SetGenerator(currentGen_);
	aEngine_->SetGenerator(currentGen_);
}

bool MainWindow::OnKeyPress(guint keyval, guint keycode, Gdk::ModifierType state)
{
	if (currentGen_) 
        return currentGen_->OnKeyPress(keyval, keycode, state);
    else
        return false;
}
