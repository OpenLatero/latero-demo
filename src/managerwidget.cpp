#include "managerwidget.h"
#include <assert.h>
#include <gtkmm.h>
#include <iostream>
#include <fstream>
#include <laterographics/generator.h>

ManagerWidget::ManagerWidget(latero::graphics::TactileEngine *tEngine, latero::graphics::AudioEngine *aEngine) :
	preview_(tEngine->Dev(), latero::graphics::GeneratorPtr(), true),
	tEngine_(tEngine),
	aEngine_(aEngine)
{
	Gtk::VBox *box = manage(new Gtk::VBox);
	Gtk::Expander *exp = manage(new Gtk::Expander("settings"));
	exp->set_expanded(true);
	add(*box);
	box->pack_start(preview_);
	box->pack_start(*exp, Gtk::PACK_SHRINK);
	exp->add(notebook_);
	show_all_children();

	notebook_.signal_switch_page().connect(
		sigc::mem_fun(*this,&ManagerWidget::OnPageSwitch));

	//AddGenerator(gen);

	signal_key_press_event().connect(
		sigc::mem_fun(*this, &ManagerWidget::OnKeyPress));


};

void ManagerWidget::OnPageSwitch(GtkNotebookPage* page, guint idx)
{
	UpdateCurrentGenerator();
}

void ManagerWidget::Save()
{
	Gtk::FileChooserDialog dialog("Please select a generator file.", Gtk::FILE_CHOOSER_ACTION_SAVE);

	std::string dir = Glib::get_current_dir();
 
	dir += "/cards"; // TODO
	dialog.set_current_folder(dir);
	dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
	dialog.add_button(Gtk::Stock::SAVE, Gtk::RESPONSE_OK);
	dialog.set_default_response(Gtk::RESPONSE_CANCEL);
	dialog.set_current_name("card.gen");

	if (Gtk::RESPONSE_OK == dialog.run())		
	{
		std::string filename = dialog.get_filename();
		if (currentGen_) currentGen_->SaveToFile(dialog.get_filename());
	}
}

void ManagerWidget::Close()
{
	int page = notebook_.get_current_page();
	if (page>=0)
	{
		list_.erase(list_.begin()+page);
		notebook_.remove_page(page);
		UpdateCurrentGenerator();
		//preview_.Clear(0xffffffff);
	}
}


void ManagerWidget::Open()
{
	Gtk::FileChooserDialog dialog("Please select a generator file.", Gtk::FILE_CHOOSER_ACTION_OPEN);

	Gtk::FileFilter filter;
	filter.add_pattern("*.gen");
 
	std::string dir = Glib::get_current_dir();
	dir += "/cards"; // TODO

	dialog.set_current_folder(dir);
	dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
	dialog.add_button(Gtk::Stock::OPEN, Gtk::RESPONSE_OK);
	dialog.set_default_response(Gtk::RESPONSE_OK);
	dialog.add_filter(filter);

	if (Gtk::RESPONSE_OK == dialog.run())		
	{
		AddGenerator(dialog.get_filename());
	}
}

void ManagerWidget::AddGenerator(std::string filename) {
    std::string name = filename;
    size_t dot = name.find_last_of(".");
    if (dot != std::string::npos)
        name = name.substr(0, dot);
    AddGenerator(latero::graphics::Generator::Create(filename, tEngine_->Dev()), name);
}

void ManagerWidget::AddGenerator(latero::graphics::GeneratorPtr gen, std::string name)
{
	list_.push_back(gen);
	int i = notebook_.append_page(*manage(gen->CreateWidget(gen)), name);
	show_all_children(); 
	//notebook_.set_current_page(i);
	UpdateCurrentGenerator();
}

void ManagerWidget::UpdateCurrentGenerator()
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

bool ManagerWidget::OnKeyPress(GdkEventKey* event)
{
	if (currentGen_) 
        return currentGen_->OnKeyPress(event);
    else
        return false;
}
