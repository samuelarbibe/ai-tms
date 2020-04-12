//
// Created by Samuel Arbibe on 12/04/2020.
//

#ifndef TMS_SRC_UI_WIDGETS_GRAPHVIEW_HPP
#define TMS_SRC_UI_WIDGETS_GRAPHVIEW_HPP

#include <QApplication>
#include <QWidget>
#include <QtCharts/QtCharts>
#include <src/sim/simulator/Simulation.hpp>

class GraphView
{
  public:
	GraphView(QWidget *parent = nullptr);

	void populateData(const vector<Simulation *> *data);
	void resize();

  private:

	QWidget *parent_;

	QLineSeries *series_;
	QChart *chart_;
	QValueAxis * x_axis_;
	QChartView *chart_view_;
};

#endif //TMS_SRC_UI_WIDGETS_GRAPHVIEW_HPP
