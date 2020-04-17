//
// Created by Samuel Arbibe on 12/04/2020.
//

#ifndef TMS_SRC_UI_WIDGETS_SIMCHART_HPP
#define TMS_SRC_UI_WIDGETS_SIMCHART_HPP

#include <QApplication>
#include <QWidget>
#include <QtCharts/QtCharts>
#include "../../sim/simulator/Set.hpp"

class SimChart : public QChart
{
	Q_OBJECT
  public:
	SimChart(QWidget * parent = nullptr);

	void populateData(Set *data);

  private:

	QWidget * parent_;

	QLineSeries *series_;
};

#endif //TMS_SRC_UI_WIDGETS_SIMCHART_HPP
