#include "drawing.h"
#include <laterographics/generator.h>
#include <iostream>

VirtualSurfaceWidget::VirtualSurfaceWidget(const latero::Tactograph *dev, latero::graphics::GeneratorPtr gen) :
 	Gtk::AspectFrame(0.5, 0.5, dev->GetWidth()/dev->GetHeight(), false),
	peer_(gen),
	dev_(dev),
	tdState_(dev->GetFrameSizeX(), dev->GetFrameSizeY())
{
	set_child(drawingArea_);
	drawingArea_.set_expand();

	Glib::signal_timeout().connect(
		sigc::mem_fun(*this, &VirtualSurfaceWidget::RefreshCursor),
		(uint)33, // ms
		Glib::PRIORITY_DEFAULT_IDLE);

	drawingArea_.set_draw_func(sigc::mem_fun(*this, &VirtualSurfaceWidget::OnDraw));
}


VirtualSurfaceWidget::~VirtualSurfaceWidget()
{
}


void VirtualSurfaceWidget::OnDraw(const Cairo::RefPtr<Cairo::Context>& cr, int /*width*/, int /*height*/)
{
	int w = drawingArea_.get_width();
	int h = drawingArea_.get_height();

	if ((w<=0)||(h<=0))
		return;

	cr->push_group();
    double dpmm_x =  w / GetWidthMilli();
    double dpmm_y = h / GetHeightMilli();
    cr->scale(dpmm_x, dpmm_y);		// scale to mm
	cr->set_source(GetDisplayDrawing(cr));
    cr->paint();
	auto drawing = cr->pop_group();

	cr->set_source(drawing);
	cr->paint();
}




Cairo::RefPtr<Cairo::Pattern> VirtualSurfaceWidget::GetDisplayDrawing(const Cairo::RefPtr<Cairo::Context> &mmContext)
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


void VirtualSurfaceWidget::SetDisplayState(const latero::BiasedImg &f)
{
	assert(f.Size() ==  tdState_.Size());
	tdState_ = f;
    drawingArea_.queue_draw();
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