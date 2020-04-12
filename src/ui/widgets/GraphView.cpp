//
// Created by Samuel Arbibe on 12/04/2020.
//

#include "GraphView.hpp"

GraphView::GraphView(QWidget * Parent){

	parent_ = Parent;
	parent_->setContentsMargins(0, 0, 0, 0);

	series_ = new QLineSeries();
	chart_ = new QChart();

	chart_->legend()->hide();
	chart_->addSeries(series_);
	chart_->createDefaultAxes();
	chart_->setTitle("Training Progress");

	x_axis_ = new QValueAxis;
	chart_->setAxisX(x_axis_, series_);

	chart_view_ = new QChartView(chart_, Parent);
	chart_view_->setRenderHint(QPainter::Antialiasing);
}

void GraphView::populateData(const vector<Simulation *> *data) {

	series_->clear();
	int count = 0;

	for(Simulation * s : *data)
	{
		series_->append(s->GetSimulationNumber(), s->GetResult());
		count ++;
	}

	x_axis_->setRange(0, count);
	x_axis_->setTickCount(10);
	x_axis_->setLabelFormat("%.0f");
}

void GraphView::resize(){
	chart_view_->resize(parent_->size());
}