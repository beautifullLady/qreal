/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#pragma once

#include <QtCore/QMimeData>

#include <qrrepo/repoApi.h>

#include "qrgui/plugins/pluginManager/editorManagerInterface.h"
#include "qrgui/models/details/modelsImplementation/logicalModelItem.h"
#include "qrgui/models/details/modelsImplementation/abstractModel.h"
#include "qrgui/models/details/graphicalModelView.h"
#include "qrgui/models/logicalModelAssistApi.h"

namespace qReal {

namespace models {
class LogicalModelAssistApi;

namespace details {

class GraphicalModel;

class QRGUI_MODELS_EXPORT LogicalModel : public modelsImplementation::AbstractModel
{
	Q_OBJECT

public:
	LogicalModel(qrRepo::LogicalRepoApi *repoApi, const EditorManagerInterface &editorManagerInterface);
	virtual ~LogicalModel();

	void connectToGraphicalModel(GraphicalModel * const graphicalModel);
	void updateElements(const Id &logicalId, const QString &name);
	virtual QMimeData* mimeData(const QModelIndexList &indexes) const;
	void addElementToModel(const Id &parent, const Id &id,const Id &logicalId, const QString &name
			, const QPointF &position);
	virtual QVariant data(const QModelIndex &index, int role) const;
	virtual bool setData(const QModelIndex &index, const QVariant &value, int role);
	virtual void changeParent(const QModelIndex &element, const QModelIndex &parent, const QPointF &position);
	void changeParent(const Id &parentId, const Id &childId);
	const qrRepo::LogicalRepoApi &api() const;
	qrRepo::LogicalRepoApi &mutableApi() const;
	virtual bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());
	virtual qReal::details::ModelsAssistInterface* modelAssistInterface() const;
	LogicalModelAssistApi &logicalModelAssistApi() const;
	virtual void stackBefore(const QModelIndex &element, const QModelIndex &sibling);

signals:
	void parentChanged(IdList const &idList);
	void nameChanged(Id const &id);
	void addedElementToModel(Id const &id);
	void propertyChanged(Id const &id);

private:
	virtual void init();
	void loadSubtreeFromClient(modelsImplementation::LogicalModelItem * const parent);
	modelsImplementation::LogicalModelItem *loadElement(modelsImplementation::LogicalModelItem *parentItem
			, const Id &id);
	void addInsufficientProperties(const Id &id, const QString &name = QString());

	virtual modelsImplementation::AbstractModelItem *createModelItem(const Id &id
			, modelsImplementation::AbstractModelItem *parentItem) const;
	void initializeElement(const Id &id, modelsImplementation::AbstractModelItem *parentItem
			, modelsImplementation::AbstractModelItem *item, const QString &name, const QPointF &position);
	QString pathToItem(const modelsImplementation::AbstractModelItem * const item) const;
	virtual void removeModelItemFromApi(details::modelsImplementation::AbstractModelItem *const root
			, details::modelsImplementation::AbstractModelItem *child);

	GraphicalModelView mGraphicalModelView;
	qrRepo::LogicalRepoApi &mApi;
	LogicalModelAssistApi *mLogicalAssistApi;
};
}

}

}
