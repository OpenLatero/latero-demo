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
	auto box = manage(new Gtk::Box(Gtk::Orientation::VERTICAL));
	Gtk::Expander *exp = manage(new Gtk::Expander("settings"));
	exp->set_expanded(true);
	set_child(*box);
	box->append(preview_);
	preview_.set_vexpand();
	preview_.set_hexpand();
	box->append(*exp);
	exp->set_child(notebook_);
	exp->set_vexpand(false);

	notebook_.signal_switch_page().connect(
		sigc::mem_fun(*this,&ManagerWidget::OnPageSwitch));

	auto key_controller = Gtk::EventControllerKey::create();
	key_controller->signal_key_pressed().connect(
		sigc::mem_fun(*this, &ManagerWidget::OnKeyPress), false);
	add_controller(key_controller);
};

void ManagerWidget::OnPageSwitch(Gtk::Widget* page, guint page_num)
{
	UpdateCurrentGenerator();
}

void ManagerWidget::Save()
{
	auto dialog = Gtk::FileDialog::create();
	dialog->set_title("Please select a generator file.");
	dialog->set_initial_folder(Gio::File::create_for_path(Glib::get_current_dir() + "/cards"));
	dialog->set_initial_name("card.gen");

	auto* window = dynamic_cast<Gtk::Window*>(get_root());
	dialog->save(*window, [this, dialog](Glib::RefPtr<Gio::AsyncResult>& result) {
		try {
			auto file = dialog->save_finish(result);
			if (file && currentGen_)
				currentGen_->SaveToFile(file->get_path());
		} catch (const Glib::Error&) {}
	});
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
	auto dialog = Gtk::FileDialog::create();
	dialog->set_title("Please select a generator file.");
	dialog->set_initial_folder(Gio::File::create_for_path(Glib::get_current_dir() + "/cards"));

	auto filter = Gtk::FileFilter::create();
	filter->add_pattern("*.gen");
	auto filters = Gio::ListStore<Gtk::FileFilter>::create();
	filters->append(filter);
	dialog->set_filters(filters);

	auto* window = dynamic_cast<Gtk::Window*>(get_root());
	dialog->open(*window, [this, dialog](Glib::RefPtr<Gio::AsyncResult>& result) {
		try {
			auto file = dialog->open_finish(result);
			if (file)
				AddGenerator(file->get_path());
		} catch (const Glib::Error&) {}
	});
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
	//show_all_children(); 
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

bool ManagerWidget::OnKeyPress(guint keyval, guint keycode, Gdk::ModifierType state)
{
	if (currentGen_) 
        return currentGen_->OnKeyPress(keyval, keycode, state);
    else
        return false;
}
