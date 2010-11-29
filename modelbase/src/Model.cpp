/***********************************************************************************************************************
 * Model.cpp
 *
 *  Created on: Nov 8, 2010
 *      Author: Dimitar Asenov
 **********************************************************************************************************************/

#include "Model.h"
#include "UndoCommand.h"
#include "ModelException.h"
#include "commands/SetModificationTarget.h"

namespace Model {

QList<Model*> Model::loadedModels;

Model::Model() :
	root(NULL), currentModificationTarget(NULL), currentModificationLock(NULL), pushedNewCommandsOnTheStack(false), modificationInProgress(false), nextId(0)
{
	commands.setUndoLimit(100);
	loadedModels.append(this);
}

Model::~Model()
{
	loadedModels.removeAll(this);
	// TODO Make sure to persist and destroy the tree in a nice way.
}

void Model::beginModification(Node* modificationTarget, const QString &text)
{
	exclusiveAccess.lock();
	modificationInProgress = true;
	modificationText = text;

	if (modificationTarget) changeModificationTarget(modificationTarget);
}

void Model::changeModificationTarget(Node* modificationTarget)
{
	if ( !modificationInProgress ) throw ModelException("Switching modification targets without calling Model.beginModification() first");

	pushCommandOnUndoStack( new SetModificationTarget(currentModificationTarget, currentModificationLock, currentModificationTarget, modificationTarget));
}

void Model::endModification()
{
	if ( pushedNewCommandsOnTheStack )
	{
		pushedNewCommandsOnTheStack = false;
		commands.endMacro();
	}

	modificationInProgress = false;
	exclusiveAccess.unlock();
}

void Model::beginExclusiveRead()
{
	exclusiveAccess.lock();
}

void Model::endExclusiveRead()
{
	exclusiveAccess.unlock();
}

NodeReadWriteLock* Model::getRootLock()
{
	return &rootLock;
}

bool Model::isInCurrentAccessUnit(const Node* node) const
{
	return node->getAccessLock() == currentModificationLock;
}

Node* Model::getRoot()
{
	return root;
}

NodeIdType Model::generateNextId()
{
	if ( root != NULL && !modificationInProgress ) throw ModelException("Creating a new node without calling Model.beginModification() first");

	return nextId++;
}

void Model::pushCommandOnUndoStack(UndoCommand* command)
{
	if ( !modificationInProgress ) throw ModelException("Changing the application tree without calling Model.beginModification() first");

	if ( pushedNewCommandsOnTheStack == false )
	{
		pushedNewCommandsOnTheStack = true;
		commands.beginMacro(modificationText);
	}

	commands.push(command);
}

void Model::undo()
{
	if ( !modificationInProgress ) throw ModelException("Requesting an Undo without calling Model.beginModification() first");
	if ( pushedNewCommandsOnTheStack ) throw ModelException("Requesting an undo in the middle of a modification after inserting new commands.");

	commands.undo();
}

void Model::redo()
{
	if ( !modificationInProgress ) throw ModelException("Requesting a Redo without calling Model.beginModification() first");
	if ( pushedNewCommandsOnTheStack ) throw ModelException("Requesting a Redo in the middle of a modification after inserting new commands.");

	commands.redo();
}

void Model::save(PersistentStore& store)
{
	if ( root ) store.saveNode(root, "root", false);
}

Model* Model::getModel(Node* root)
{
	for (QList<Model*>::iterator model = loadedModels.begin(); model != loadedModels.end(); model++)
	{
		if ( (*model)->getRoot() == root ) return *model;
	}

	return NULL;
}

Node* Model::createRoot(const QString &typeName)
{
	if ( root == NULL )
	{
		commands.clear();
		root = Node::createNewNode(typeName, NULL, this);
	}

	return root;
}

}