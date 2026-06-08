#pragma once

#include <gtkmm.h>
#include <latero/tactileimg.h>
#include <latero/tactograph.h>
#include <laterographics/generatorfwd.h>

class VirtualSurfaceWidget : public Gtk::AspectFrame
{
public:
	VirtualSurfaceWidget(const latero::Tactograph *dev, latero::graphics::GeneratorPtr gen = latero::graphics::GeneratorPtr());
	virtual ~VirtualSurfaceWidget();
	void SetGenerator(latero::graphics::GeneratorPtr gen);
	
protected:
	bool RefreshCursor();

    inline double GetWidthMilli() { return dev_->GetWidth() * 1.5; }
    inline double GetHeightMilli() { return dev_->GetHeight() * 1.5; }

	void SetDisplayState(const latero::BiasedImg &frame);
	Cairo::RefPtr<Cairo::Pattern> GetDisplayDrawing(const Cairo::RefPtr<Cairo::Context> &mmContext);
	void OnDraw(const Cairo::RefPtr<Cairo::Context>& cr, int width, int height);

	latero::graphics::GeneratorPtr peer_;
	boost::posix_time::ptime bgUpdateTime_;
    const latero::Tactograph *dev_;
	latero::BiasedImg tdState_;
	Gtk::DrawingArea drawingArea_;

};
