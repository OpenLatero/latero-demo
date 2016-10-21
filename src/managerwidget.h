#ifndef PIEZO_MANAGER_WIDGET_H
#define PIEZO_MANAGER_WIDGET_H

#include <gtkmm/notebook.h>
#include <gtkmm/frame.h>
#include "drawing.h"
#include <laterographics/tactileengine.h>
#include <laterographics/audioengine.h>

class ManagerWidget : public Gtk::Frame
{
public:
    ManagerWidget(latero::graphics::TactileEngine *tEngine, latero::graphics::AudioEngine *aEngine);
    virtual ~ManagerWidget() {};

	void Save();
	void Open();
	void Close();

    void AddGenerator(std::string filename);
    void AddGenerator(latero::graphics::GeneratorPtr gen, std::string name="Generator");
    
protected:
	bool OnKeyPress(GdkEventKey* event);
	
	void UpdateCurrentGenerator();
	void OnPageSwitch(GtkNotebookPage* page, guint idx);

	Gtk::Notebook notebook_;
	VirtualSurfaceWidget preview_;
	latero::graphics::TactileEngine *tEngine_;
	latero::graphics::AudioEngine *aEngine_;
	std::vector<latero::graphics::GeneratorPtr> list_;
	latero::graphics::GeneratorPtr currentGen_;
};

#endif
