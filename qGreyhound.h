//##########################################################################
//#                                                                        #
//#                       CLOUDCOMPARE PLUGIN: qGreyhound                  #
//#                                                                        #
//#  This program is free software; you can redistribute it and/or modify  #
//#  it under the terms of the GNU General Public License as published by  #
//#  the Free Software Foundation; version 2 of the License.               #
//#                                                                        #
//#  This program is distributed in the hope that it will be useful,       #
//#  but WITHOUT ANY WARRANTY; without even the implied warranty of        #
//#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         #
//#  GNU General Public License for more details.                          #
//#                                                                        #
//#                             COPYRIGHT: Thomas Montaigu                 #
//#                                                                        #
//##########################################################################

#ifndef Q_GREYHOUND_PLUGIN_HEADER
#define Q_GREYHOUND_PLUGIN_HEADER

#include <ccPointCloud.h>

//qCC
#include "ccStdPluginInterface.h"

#include "ccGreyhoundResource.h"
#include "ccGreyhoundCloud.h"


class qGreyhound : public QObject, public ccStdPluginInterface
{
	Q_OBJECT
		Q_INTERFACES(ccStdPluginInterface)
		Q_PLUGIN_METADATA(IID "cccorp.cloudcompare.plugin.qGreyhound" FILE "info.json")

public:

	//! Default constructor
	explicit qGreyhound(QObject* parent = 0);
	virtual ~qGreyhound() = default;

	//inherited from ccPluginInterface
	QString getName() const override { return "qGreyhound"; }
	QString getDescription() const override { return "Greyhound things in cloudcompare"; }
	QIcon getIcon() const override;

	//inherited from ccStdPluginInterface
	void onNewSelection(const ccHObject::Container& selectedEntities) override;
	QList<QAction*> getActions() override;


protected slots:

	void connect_to_resource() const;
	void download_bounding_box() const;

protected:
	QAction* m_download_bounding_box;
	QAction* m_connect_to_resource;


	void download_more_dimensions(ccGreyhoundCloud* cloud) const;
};

#endif
