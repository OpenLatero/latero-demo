#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include "managerwidget.h"
#include <gtkmm.h>

class MainWindow : public Gtk::Window
{
public:
	MainWindow(latero::graphics::TactileEngine *tEngine,
               latero::graphics::AudioEngine *aEngine);

    virtual ~MainWindow();

    void AddGenerator(std::string filename) { managerWidget_.AddGenerator(filename); }
    void AddGenerator(latero::graphics::GeneratorPtr gen) { managerWidget_.AddGenerator(gen); }
    
protected:
	void OnOpen();
	void OnSave();
	void OnClose();

	Gtk::Widget *CreateMenu();

	Glib::RefPtr<Gtk::UIManager> uiManager_;
	ManagerWidget managerWidget_;
};

#endif
