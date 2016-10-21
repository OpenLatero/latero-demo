// -----------------------------------------------------------
//
// Copyright (c) 2015 by Vincent Levesque. All Rights Reserved.
//
// This file is part of latero-graphics.
//
//    latero-graphics is free software: you can redistribute it and/or modify
//    it under the terms of the GNU Lesser General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    latero-graphics is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU Lesser General Public License for more details.
//
//    You should have received a copy of the GNU Lesser General Public License
//    along with latero-graphics.  If not, see <http://www.gnu.org/licenses/>.
//
// -----------------------------------------------------------

#ifndef DRAWING_H
#define DRAWING_H

#include <gtkmm/uimanager.h>
#include <gtkmm/drawingarea.h>
#include <gtkmm/alignment.h>
#include <gtkmm/aspectframe.h>
#include <gtkmm/eventbox.h>
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

	
	inline uint GetWidth() { return get_allocation().get_width(); };
	inline uint GetHeight() { return get_allocation().get_height(); };

	Cairo::RefPtr<Cairo::Pattern> GetDisplayDrawing(Cairo::RefPtr<Cairo::Context> &mmContext);
    
protected:

    inline double GetWidthMilli() { return dev_->GetWidth() * 1.5; }
    inline double GetHeightMilli() { return dev_->GetHeight() * 1.5; }
    
	// invalidate the entire window
	void Invalidate();

	Cairo::RefPtr<Cairo::Pattern> GetCursorDrawing(Cairo::RefPtr<Cairo::Context> &cr);

	void DrawCursor(Cairo::RefPtr<Cairo::Context> &cr);

	virtual bool on_expose_event(GdkEventExpose* event);
    
    

protected:

    const latero::Tactograph *dev_;
	latero::BiasedImg tdState_;
};

/**
 * This widget represents the virtual surface explored by a tactile display. It is implement as an AspectFrame enclosing a 
 * DrawingArea so that the aspect ratio can be maintained.
 */
class BaseVirtualSurfaceWidget : public Gtk::EventBox
{
public:
	BaseVirtualSurfaceWidget(const latero::Tactograph *dev) :
 		frame_("", 0.5, 0.5, dev->GetWidth()/dev->GetHeight(), false),
		surface_(dev)
	{
		add(frame_);
		frame_.unset_label(); // this is necessary to remove blank above surface
		frame_.set_shadow_type(Gtk::SHADOW_NONE); // this removes the border
		frame_.add(surface_);
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


#endif
