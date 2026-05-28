#pragma once

#include <gtkmm.h>
#include <latero/tactileimg.h>
#include <latero/tactograph.h>
#include <laterographics/gtk/animation.h>
#include <laterographics/point.h>
#include <laterographics/generatorfwd.h>


/** Use VirtualSurfaceWidget instead! */
class VirtualSurfaceArea : public Gtk::DrawingArea
{
public:
	VirtualSurfaceArea(const latero::Tactograph *dev);
	virtual ~VirtualSurfaceArea();
	
	void SetDisplayState(const latero::BiasedImg &frame);

	
	inline uint GetWidth() { return get_width(); };
	inline uint GetHeight() { return get_height(); };

	Cairo::RefPtr<Cairo::Pattern> GetDisplayDrawing(const Cairo::RefPtr<Cairo::Context> &mmContext);
    
protected:

    inline double GetWidthMilli() { return dev_->GetWidth() * 1.5; }
    inline double GetHeightMilli() { return dev_->GetHeight() * 1.5; }
    
	// invalidate the entire window
	void Invalidate();

	Cairo::RefPtr<Cairo::Pattern> GetCursorDrawing(const Cairo::RefPtr<Cairo::Context> &cr);

	void DrawCursor(const Cairo::RefPtr<Cairo::Context> &cr);

	void OnDraw(const Cairo::RefPtr<Cairo::Context>& cr, int width, int height);
	//virtual bool on_expose_event(GdkEventExpose* event);
    
    

protected:

    const latero::Tactograph *dev_;
	latero::BiasedImg tdState_;
};

/**
 * This widget represents the virtual surface explored by a tactile display. It is implement as an AspectFrame enclosing a 
 * DrawingArea so that the aspect ratio can be maintained.
 */
class BaseVirtualSurfaceWidget : public Gtk::Box
{
public:
	BaseVirtualSurfaceWidget(const latero::Tactograph *dev) :
 		frame_(0.5, 0.5, dev->GetWidth()/dev->GetHeight(), false),
		surface_(dev)
	{
		append(frame_);
		//frame_.unset_label(); // this is necessary to remove blank above surface
		//frame_.set_shadow_type(Gtk::SHADOW_NONE); // this removes the border
		frame_.set_child(surface_);
		surface_.set_expand();
	}

	virtual ~BaseVirtualSurfaceWidget()
	{
	}

	inline void SetDisplayState(const latero::BiasedImg &frame) {
		surface_.SetDisplayState(frame);
	}

protected:
	Gtk::AspectFrame frame_;
	VirtualSurfaceArea surface_;
};

class VirtualSurfaceWidget : public BaseVirtualSurfaceWidget
{
public:
	VirtualSurfaceWidget(const latero::Tactograph *dev, latero::graphics::GeneratorPtr gen = latero::graphics::GeneratorPtr(), bool refreshBackground=false);
	virtual ~VirtualSurfaceWidget();
	void SetGenerator(latero::graphics::GeneratorPtr gen);
	bool RefreshCursor();
    

private:
	latero::graphics::GeneratorPtr peer_;
	boost::posix_time::ptime bgUpdateTime_;
};
