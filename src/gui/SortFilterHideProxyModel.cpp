/*
 *  Copyright (C) 2012 Felix Geyer <debfx@fobos.de>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 or (at your option)
 *  version 3 of the License.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "SortFilterHideProxyModel.h"
#include <stdio.h>

QRegExp bignum("^\\d+([\\.,]\\d+){,1}$");
QRegExp num("^\\d+([\\.,]\\d+){,1}$");

SortFilterHideProxyModel::SortFilterHideProxyModel(QObject* parent)
    : QSortFilterProxyModel(parent)
{
}

void SortFilterHideProxyModel::hideColumn(int column, bool hide)
{
    m_hiddenColumns.resize(column + 1);
    m_hiddenColumns[column] = hide;

    invalidateFilter();
}

bool SortFilterHideProxyModel::filterAcceptsColumn(int sourceColumn, const QModelIndex& sourceParent) const
{
    Q_UNUSED(sourceParent);

    return sourceColumn >= m_hiddenColumns.size() || !m_hiddenColumns.at(sourceColumn);
}
bool SortFilterHideProxyModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
  QVariant leftData = sourceModel()->data(left);
  QVariant rightData = sourceModel()->data(right);
  if (num.exactMatch(leftData.toString()))
    {
      if(num.exactMatch(rightData.toString()))
	{
	  return leftData.toDouble() < rightData.toDouble();
	}
      else if(bignum.exactMatch(rightData.toString()))
	{
	  return leftData.toDouble() < rightData.toString().replace(QString("."), QString("")).toDouble();
	}
    }
  else if(bignum.exactMatch(leftData.toString()))
    {
      if(num.exactMatch(rightData.toString()))
	{
	  return leftData.toString().replace(QString("."), QString("")).toDouble() < rightData.toDouble();
	}
      else if(bignum.exactMatch(rightData.toString()))
	{
	  return leftData.toString().replace(QString("."), QString("")).toDouble() < rightData.toString().replace(QString("."), QString("")).toDouble();
	}
    }
  return QSortFilterProxyModel::lessThan(left, right);
}
