#pragma once

#include <gtkmm.h>
#include <laterographics/tactiledisplayview.h>
#include <laterographics/tactileengine.h>
#include <laterographics/audioengine.h>
#include <vector>

class MainWindow : public Gtk::ApplicationWindow
{
public:
    MainWindow(latero::graphics::TactileEngine *tEngine, latero::graphics::AudioEngine *aEngine, std::vector<std::string> generators = {});
    virtual ~MainWindow();

    void AddGenerator(std::string filename);
    void AddGenerator(latero::graphics::GeneratorPtr gen, std::string name="Generator");
    
protected:
	bool OnKeyPress(guint keyval, guint keycode, Gdk::ModifierType state);
	
	void UpdateCurrentGenerator();
	void OnPageSwitch(Gtk::Widget* page, guint page_num);

	Gtk::Notebook notebook_;
	latero::graphics::TactileDisplayView preview_;
	latero::graphics::TactileEngine *tEngine_;
	latero::graphics::AudioEngine *aEngine_;
	sigc::connection switch_page_conn_;
	std::vector<latero::graphics::GeneratorPtr> list_;
	latero::graphics::GeneratorPtr currentGen_;
};
