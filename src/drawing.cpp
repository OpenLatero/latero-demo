#include "drawing.h"
#include <laterographics/gtk/pixbufops.h>
#include <gtkmm.h>
#include <math.h>
#include <laterographics/generator.h>
#include <laterographics/positiongen.h>
#include <laterographics/visualizewidget.h>
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
    set_draw_func(sigc::mem_fun(*this, &VirtualSurfaceArea::OnDraw));
}


VirtualSurfaceArea::~VirtualSurfaceArea()
{
}


void VirtualSurfaceArea::OnDraw(const Cairo::RefPtr<Cairo::Context>& cr, int /*width*/, int /*height*/)
{
	DrawCursor(cr);
}


Cairo::RefPtr<Cairo::Pattern> VirtualSurfaceArea::GetCursorDrawing(const Cairo::RefPtr<Cairo::Context> &cr)
{
	int w = GetWidth();
	int h = GetHeight();

	if ((w<=0)||(h<=0))
	{
		cr->push_group();
		return cr->pop_group(); // TODO: cleaner way?
	}
	
	cr->push_group();
    double dpmm_x =  w / GetWidthMilli();
    double dpmm_y = h / GetHeightMilli();
    cr->scale(dpmm_x, dpmm_y);		// scale to mm
	cr->set_source(GetDisplayDrawing(cr));
    cr->paint();
	return cr->pop_group();
}


Cairo::RefPtr<Cairo::Pattern>
VirtualSurfaceArea::GetDisplayDrawing(const Cairo::RefPtr<Cairo::Context> &mmContext)
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


void VirtualSurfaceArea::DrawCursor(const Cairo::RefPtr<Cairo::Context> &cr)
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
    queue_draw();
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