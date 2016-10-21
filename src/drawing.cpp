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

#include "drawing.h"
#include <laterographics/gtk/pixbufops.h>
#include <gtkmm.h>
#include <math.h>
#include <laterographics/generator.h>
#include <laterographics/positiongen.h>
#include <laterographics/visualizewidget.h>
#include <gtkmm/menu.h>
#include <gtkmm/filechooserdialog.h>
#include <iostream>
#include "boost/date_time/posix_time/posix_time.hpp"
#include "boost/date_time/local_time_adjustor.hpp"
#include "boost/date_time/c_local_time_adjustor.hpp"
#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

VirtualSurfaceArea::VirtualSurfaceArea(const latero::Tactograph *dev) :
	dev_(dev),
	tdState_(dev->GetFrameSizeX(), dev->GetFrameSizeY())
{
}

VirtualSurfaceArea::~VirtualSurfaceArea()
{
}

bool VirtualSurfaceArea::on_expose_event(GdkEventExpose* event)
{
	Cairo::RefPtr<Cairo::Context> cr = get_window()->create_cairo_context();
	if (event)
	{
		cr->rectangle(event->area.x, event->area.y, event->area.width, event->area.height);
    		cr->clip();
	}

	DrawCursor(cr);

	return true;
}

Cairo::RefPtr<Cairo::Pattern> VirtualSurfaceArea::GetCursorDrawing(Cairo::RefPtr<Cairo::Context> &cr)
{
	cr->push_group();
    double dpmm_x =  GetWidth() / GetWidthMilli();
    double dpmm_y = GetHeight() / GetHeightMilli();
    cr->scale(dpmm_x, dpmm_y);		// scale to mm
	cr->set_source(GetDisplayDrawing(cr));
    cr->paint();
	
	return cr->pop_group();
}

Cairo::RefPtr<Cairo::Pattern>
VirtualSurfaceArea::GetDisplayDrawing(Cairo::RefPtr<Cairo::Context> &mmContext)
{
	mmContext->push_group();

	mmContext->translate(GetWidthMilli()/2, GetHeightMilli()/2);

	double tdw = dev_->GetWidth()*1.4;
	double tdh = dev_->GetHeight()*1.2;
	mmContext->rectangle(-tdw/2, -tdh/2, tdw, tdh);
	mmContext->set_source_rgb(1.0, 1.0, 1.0);
	mmContext->set_line_width(1.5);
	mmContext->fill_preserve();
	mmContext->set_line_width(1.5);
	mmContext->set_source_rgb(1.0, 1.0, 1.0);
	mmContext->stroke_preserve();
	mmContext->set_line_width(0.5);
	mmContext->set_source_rgb(1.0, 0.0, 0.0);
	mmContext->stroke();

	float motionRange = 0.7 * dev_->GetPitchX();
	int hPiezo = dev_->GetContactorSizeY();

	mmContext->set_source_rgb(1.0, 0.0, 0.0);
	for (uint j=0; j< dev_->GetFrameSizeY(); ++j)
	{
		for (uint i=0; i< dev_->GetFrameSizeX(); ++i)
		{
			latero::graphics::Point p = dev_->GetActuatorOffset(i,j);
			float x = p.x + (0.5-tdState_.Get(i,j))*motionRange;
			mmContext->move_to(x, p.y - 0.5*hPiezo);
	        	mmContext->line_to(x, p.y + 0.5*hPiezo);

			mmContext->set_source_rgb(1.0, 0.0, 0.0);
			mmContext->set_line_width(0.3*dev_->GetPitchX());
			mmContext->stroke();
		}
	}
	return mmContext->pop_group();
}


void VirtualSurfaceArea::DrawCursor(Cairo::RefPtr<Cairo::Context> &cr)
{
	cr->set_source(GetCursorDrawing(cr));
	cr->paint();
}


void VirtualSurfaceArea::SetDisplayState(const latero::BiasedImg &f)
{
	assert(f.Size() ==  tdState_.Size());

    
	//tdPos_ = pos;
	tdState_ = f;
    
    Invalidate();
}




void VirtualSurfaceArea::Invalidate()
{
    Glib::RefPtr<Gdk::Window> win = get_window();
    if (win)
    {
        Gdk::Rectangle r(0, 0, get_allocation().get_width(), get_allocation().get_height());
        win->invalidate_rect(r, false);
    }
}






VirtualSurfaceWidget::VirtualSurfaceWidget(const latero::Tactograph *dev, latero::graphics::GeneratorPtr gen, bool refreshBackground) :
	BaseVirtualSurfaceWidget(dev),
	peer_(gen)
{
	// TODO: enable these timeouts only when visible?!?
	// TODO: when that's done, make sure everything 2D uses this version (e.g. Memory game)

	Glib::signal_timeout().connect(
		sigc::mem_fun(*this, &VirtualSurfaceWidget::RefreshCursor),
		(uint)33, // ms
		Glib::PRIORITY_DEFAULT_IDLE);
}



VirtualSurfaceWidget::~VirtualSurfaceWidget()
{
}

bool VirtualSurfaceWidget::RefreshCursor()
{
	if (peer_)
	{
		latero::BiasedImg frame = peer_->GetLatestFrame();
		SetDisplayState(frame);
	}
	return true;
}


void VirtualSurfaceWidget::SetGenerator(latero::graphics::GeneratorPtr gen)
{
	peer_ = gen;
}




