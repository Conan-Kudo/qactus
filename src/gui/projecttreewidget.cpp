/*
 *  Qactus - A Qt based OBS notifier
 *
 *  Copyright (C) 2018 Javier Llorente <javier@opensuse.org>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 of the License, or
 *  (at your option) version 3.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "projecttreewidget.h"

ProjectTreeWidget::ProjectTreeWidget(QWidget *parent) :
    QTreeView(parent)
{
    setContextMenuPolicy(Qt::CustomContextMenu);
    sourceModelProjects = new QStringListModel(this);
    proxyModelProjects = new QSortFilterProxyModel(this);
    setModel(proxyModelProjects);
}

void ProjectTreeWidget::scrollToCurrentIndex()
{
    scrollTo(currentIndex(), QAbstractItemView::PositionAtTop);
}

QString ProjectTreeWidget::getCurrentProject() const
{
    return currentIndex().data().toString();
}

bool ProjectTreeWidget::setCurrentProject(const QString &project)
{
    QModelIndexList itemList = model()->match(model()->index(0, 0),
                                              Qt::DisplayRole,
                                              QVariant::fromValue(QString(project)), 1, Qt::MatchExactly);
    if (!itemList.isEmpty()) {
        auto itemIndex = itemList.at(0);
        selectionModel()->setCurrentIndex(itemIndex, QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
        scrollTo(itemIndex, QAbstractItemView::PositionAtTop);
        return true;
    }
    return false;
}

void ProjectTreeWidget::addProjectList(const QStringList &projectList)
{
    sourceModelProjects->setStringList(projectList);
    proxyModelProjects->setSourceModel(sourceModelProjects);
    setModel(proxyModelProjects);
}

bool ProjectTreeWidget::removeProject(const QString &project)
{
    QModelIndexList itemList = model()->match(model()->index(0, 0),
                                              Qt::DisplayRole,
                                              QVariant::fromValue(QString(project)), 1, Qt::MatchExactly);
    if(!itemList.isEmpty()) {
        auto item = itemList.at(0);
        model()->removeRow(item.row(), item.parent());
        return true;
    }
    return false;
}

void ProjectTreeWidget::filterProjects(const QString &item)
{
    proxyModelProjects->setFilterRegExp(QRegExp(item, Qt::CaseInsensitive));
    proxyModelProjects->setFilterKeyColumn(0);

    scrollToCurrentIndex();
}

