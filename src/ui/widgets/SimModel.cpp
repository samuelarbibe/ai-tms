//
// Created by Samuel Arbibe on 07/03/2020.
//

#include "SimModel.hpp"

SimModel::SimModel(QObject *parent)
	: QAbstractTableModel(parent) {
	row_count_ = 0;
}

void SimModel::populateData(const vector<Simulation *> *data)
{
	table_.clear();
	row_count_ = 0;

	for(Simulation * s : *data)
	{
		table_.append({QString::number(s->GetSimulationNumber()),
		               QString::fromUtf8(ctime(s->GetStartTime())),
		               QString::fromUtf8(ctime(s->GetEndTime())),
		               QString::number(s->GetElapsedTime()),
		               QString::number(s->GetVehicleCount()),
		               QString::number(s->GetResult()),
		              });
		this->row_count_ ++;
	}
}

int SimModel::GetIdByRow(int rowNumber){

	if(table_.size() >= (rowNumber+1))
	{
		return table_[rowNumber][0].toInt();
	}
	return 0;
}


int SimModel::rowCount(const QModelIndex & /*parent*/) const {
	return row_count_;
}

int SimModel::columnCount(const QModelIndex & /*parent*/) const {
	return 6;
}

QVariant SimModel::data(const QModelIndex &index, int role) const {
	if (role == Qt::DisplayRole)
	{
		return table_.at(index.row()).at(index.column());
	}
	return QVariant();
}

QVariant SimModel::headerData(int section, Qt::Orientation orientation, int role) const {
	if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
	{
		switch (section)
		{
		case 0: return QString("ID");
		case 1: return QString("Start Time");
		case 2: return QString("End Time");
		case 3: return QString("Simulated Time");
		case 4: return QString("Vehicle Count");
		case 5: return QString("Score");
		default: return QString("#");
		}
	}
	return QVariant();
}