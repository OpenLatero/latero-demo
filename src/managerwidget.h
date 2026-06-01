#pragma once

#include <gtkmm.h>
#include "drawing.h"
#include <laterographics/tactileengine.h>
#include <laterographics/audioengine.h>
#include <vector>

class ManagerWidget : public Gtk::ApplicationWindow
{
public:
    ManagerWidget(latero::graphics::TactileEngine *tEngine, latero::graphics::AudioEngine *aEngine, std::vector<std::string> generators = {});
    virtual ~ManagerWidget();

    void AddGenerator(std::string filename);
    void AddGenerator(latero::graphics::GeneratorPtr gen, std::string name="Generator");
    
protected:
	bool OnKeyPress(guint keyval, guint keycode, Gdk::ModifierType state);
	
	void UpdateCurrentGenerator();
	void OnPageSwitch(Gtk::Widget* page, guint page_num);

	bool destroying_ = false;
	Gtk::Notebook notebook_;
	VirtualSurfaceWidget preview_;
	latero::graphics::TactileEngine *tEngine_;
	latero::graphics::AudioEngine *aEngine_;
	std::vector<latero::graphics::GeneratorPtr> list_;
	latero::graphics::GeneratorPtr currentGen_;
};
