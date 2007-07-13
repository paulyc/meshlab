/****************************************************************************
* MeshLab                                                           o o     *
* An extendible mesh processor                                    o     o   *
*                                                                _   O  _   *
* Copyright(C) 2005, 2006                                          \/)\/    *
* Visual Computing Lab                                            /\/|      *
* ISTI - Italian National Research Council                           |      *
*                                                                    \      *
* All rights reserved.                                                      *
*                                                                           *
* This program is free software; you can redistribute it and/or modify      *
* it under the terms of the GNU General Public License as published by      *
* the Free Software Foundation; either version 2 of the License, or         *
* (at your option) any later version.                                       *
*                                                                           *
* This program is distributed in the hope that it will be useful,           *
* but WITHOUT ANY WARRANTY; without even the implied warranty of            *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
* GNU General Public License (http://www.gnu.org/licenses/gpl.txt)          *
* for more details.                                                         *
*                                                                           *
****************************************************************************/
/****************************************************************************
History

$Log: stdpardialog.cpp,v $

****************************************************************************/

#include <GL/glew.h>
#include <QtGui>

#include "alignDialog.h"
#include <meshlab/glarea.h>
#include "editalign.h"


AlignDialog::AlignDialog(QWidget *parent )    : QDockWidget(parent)    
{ 
 // setWindowFlags( windowFlags() | Qt::WindowStaysOnTopHint | Qt::SubWindow);
	//setVisible(false);
	AlignDialog::ui.setupUi(this);
	this->setWidget(ui.frame);
	this->setFeatures(QDockWidget::AllDockWidgetFeatures);
	this->setAllowedAreas(Qt::LeftDockWidgetArea);
	QPoint p=parent->mapToGlobal(QPoint(0,0));
	this->setFloating(true);
	this->setGeometry(p.x()+(parent->width()-width()),p.y()+40,width(),height() );
	
	connect(	ui.alignTreeWidget, SIGNAL(itemClicked(QTreeWidgetItem * , int  )) , this,  SLOT(setCurrent(QTreeWidgetItem * , int ) ) );
}

void AlignDialog::updateTree()
{
	gla=edit->gla;
	QList<MeshModel *> &meshList=gla->meshDoc.meshList;
	qDebug("Items in list: %d", meshList.size());
	ui.alignTreeWidget->clear();
	//ui.alignTreeWidget->setColumnCount(1);
	M2T.clear();
	for(int i=0;i<meshList.size();++i)
	 {
		QTreeWidgetItem *item;
		qDebug("Filename %s", meshList.at(i)->fileName.c_str());
		item = new QTreeWidgetItem(QStringList (QFileInfo(meshList.at(i)->fileName.c_str()).fileName()));
		item->setData(1,Qt::DisplayRole,i);
		M2T[meshList.at(i)]=item;
		
		ui.alignTreeWidget->insertTopLevelItem(0,item);
	}
	

	AlignPair::Result *A;
	for(int i=0;i< edit->ResVec.size();++i)
	{
	  A=&(edit->ResVec[i]);
		QString buf=QString("Arc: %1 -> %2 A: %3 Err: %4 Sample %5 (%6)")
		.arg((*A).FixName)
		.arg((*A).MovName)
		.arg(edit->OG.SVA[i].norm_area, 6,'f',3)
		.arg((*A).err,                  6,'f',3)
		.arg((*A).ap.SampleNum,6)
		.arg((*A).as.LastSampleUsed() );// LPCTSTR lpszItem
		QTreeWidgetItem *parent=M2T[meshList.at((*A).FixName)];
		
		QTreeWidgetItem *item = new QTreeWidgetItem(parent);
		item->setText(0,buf);
		
		parent=M2T[meshList.at((*A).MovName)];
		item = new QTreeWidgetItem(parent);
		item->setText(0,buf);
	
	
	}
	
}

void AlignDialog::setCurrent(QTreeWidgetItem * item, int column )
{
  int row = item->data(1,Qt::DisplayRole).toInt();
  gla->meshDoc.setCurrentMesh(row);
	gla->update();
}