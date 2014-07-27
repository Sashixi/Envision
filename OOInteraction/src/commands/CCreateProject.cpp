/***********************************************************************************************************************
 **
 ** Copyright (c) 2011, 2014 ETH Zurich
 ** All rights reserved.
 **
 ** Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 ** following conditions are met:
 **
 **    * Redistributions of source code must retain the above copyright notice, this list of conditions and the
 **      following disclaimer.
 **    * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 **      following disclaimer in the documentation and/or other materials provided with the distribution.
 **    * Neither the name of the ETH Zurich nor the names of its contributors may be used to endorse or promote products
 **      derived from this software without specific prior written permission.
 **
 **
 ** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 ** INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 ** DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 ** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 ** SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 ** WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 ** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **
 **********************************************************************************************************************/

#include "CCreateProject.h"

#include "OOModel/src/declarations/Project.h"
#include "OOModel/src/declarations/NameImport.h"
#include "VisualizationBase/src/items/RootItem.h"
#include "VisualizationBase/src/cursor/LayoutCursor.h"
#include "VisualizationBase/src/declarative/GridLayouter.h"
#include "InteractionBase/src/events/SetCursorEvent.h"

namespace OOInteraction {

CCreateProject::CCreateProject() : CreateNamedObjectWithAttributes("project", {})
{
}

Interaction::CommandResult* CCreateProject::executeNamed(Visualization::Item* /*source*/, Visualization::Item* target,
	const std::unique_ptr<Visualization::Cursor>& cursor, const QString& name, const QStringList& /*attributes*/)
{
	auto parent = dynamic_cast<OOModel::Project*> (target->node());

	auto project = new OOModel::Project();
	if (!name.isEmpty()) project->setName(name);

	bool newManager = false;
	if (parent)
	{
		parent->beginModification("create project");
		if (auto layc = dynamic_cast<Visualization::LayoutCursor*>(cursor.get()))
				{
					Visualization::GridLayouter::setPositionInGrid(
							parent->projects()->nodes() + parent->modules()->nodes() +
							parent->classes()->nodes() + parent->methods()->nodes(), layc->x(), layc->y(),
							project, Visualization::GridLayouter::ColumnMajor);
				}
		parent->projects()->append(project);
		parent->endModification();
	}
	else
	{
		// If Java is loaded as a library then use it in this project
		for (auto manager : Model::AllTreeManagers::instance().loadedManagers())
		{
			if (manager->isPartiallyLoaded() && manager->name().contains("java", Qt::CaseInsensitive))
			{
				project->libraries()->append(new Model::UsedLibrary(manager->name()));
				auto import = new OOModel::NameImport( new OOModel::ReferenceExpression("lang",
																		new OOModel::ReferenceExpression("java")));
				import->setImportAll(true);
				project->subDeclarations()->append(import);
				break;
			}
		}

		newManager = true;
		auto manager = new Model::TreeManager();
		manager->setRoot(project);

		auto vis = new Visualization::RootItem(project);
		vis->setPos(target->pos());
		target->scene()->addTopLevelItem( vis );
		target->scene()->listenToTreeManager(manager);
	}

	target->setUpdateNeeded(Visualization::Item::StandardUpdate);
	if (newManager) target->scene()->addPostEventAction(new Interaction::SetCursorEvent(target->scene(), project,
			Interaction::SetCursorEvent::CursorDefault, true));
	else target->scene()->addPostEventAction(new Interaction::SetCursorEvent(target, project,
			Interaction::SetCursorEvent::CursorDefault, true));

	return new Interaction::CommandResult();
}

} /* namespace OOInteraction */
