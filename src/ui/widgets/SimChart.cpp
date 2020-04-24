//
// Created by Samuel Arbibe on 12/04/2020.
//

#include "SimChart.hpp"

SimChart::SimChart(QWidget * parent) : QChart(){

	parent_ = parent;

	series_ = new QLineSeries();

	this->legend()->hide();
	this->addSeries(series_);
	this->createDefaultAxes();
	this->setMargins(QMargins(0,0,0,0));
	this->setTitle("Training Progress");
	this->axes(Qt::Vertical).front()->setMax(0.8);
}

void SimChart::populateData(Set *data) {

	series_->clear();
	int count = 0;
	int max = 0;

	for(Simulation * s : *data->GetSimulations())
	{
		series_->append(s->GetSimulationNumber(), s->GetResult());
		if(s->GetResult() > max)
		{
			max = s->GetResult();
		}
		count ++;
	}
	this->axes(Qt::Vertical).front()->setMax(max + 0.2);
	this->axes(Qt::Horizontal).front()->setRange(0, count);
}