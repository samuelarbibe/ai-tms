//
// Created by Samuel Arbibe on 07/03/2020.
//

#ifndef SIMULATORSFML_SRC_UI_WIDGETS_SIMMODEL_HPP
#define SIMULATORSFML_SRC_UI_WIDGETS_SIMMODEL_HPP

#include <QAbstractTableModel>
#include "../../sim/simulator/Set.hpp"

class SimModel : public QAbstractTableModel
{
  Q_OBJECT

  public:

	SimModel(QObject *parent = nullptr);

	void populateData(const vector<Set *> *data);

	int rowCount(const QModelIndex &parent = QModelIndex()) const override;

	int GetIdByRow(int rowNumber);

	int columnCount(const QModelIndex &parent = QModelIndex()) const override;

	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

	QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

  private:

	int row_count_;
	QVector<QVector<QString>> table_;
};
#endif //SIMULATORSFML_SRC_UI_WIDGETS_SIMMODEL_HPP
