/***********************************************************************************************************************
 **
 ** Copyright (c) 2011, 2012 ETH Zurich
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

/*
 * CCreateMethod.cpp
 *
 *  Created on: Mar 1, 2012
 *      Author: Dimitar Asenov
 */

#include "commands/CCreateMethod.h"

#include "OOModel/src/top_level/Class.h"
#include "OOModel/src/top_level/Method.h"

#include "InteractionBase/src/events/SetCursorEvent.h"

namespace OOInteraction {

CCreateMethod::CCreateMethod() : CreateNamedObjectWithAttributes("method",
		{{"public", "private","protected"}, {"static"}})
{
}

Interaction::CommandResult* CCreateMethod::create(Visualization::Item* /*source*/, Visualization::Item* target,
	const QString& name, const QStringList& attributes)
{
	auto cl = dynamic_cast<OOModel::Class*> (target->node());
	Q_ASSERT(cl);

	auto m = new OOModel::Method();
	if (!name.isEmpty()) m->setName(name);

	// Set visibility
	if (attributes.first() == "private" ) m->setVisibility(OOModel::Visibility::PRIVATE);
	else if (attributes.first() == "protected" ) m->setVisibility(OOModel::Visibility::PROTECTED);
	else if (attributes.first() == "public" ) m->setVisibility(OOModel::Visibility::PUBLIC);
	else m->setVisibility(OOModel::Visibility::DEFAULT);

	// Set scope
	if (attributes.last() == "static") m->setStorageSpecifier(OOModel::StorageSpecifier::CLASS_VARIABLE);
	else m->setStorageSpecifier(OOModel::StorageSpecifier::INSTANCE_VARIABLE);

	cl->beginModification("create class");
	cl->methods()->append(m);
	cl->endModification();

	target->scene()->addPostEventAction(new Interaction::SetCursorEvent(target, m));

	return new Interaction::CommandResult();
}

} /* namespace OOInteraction */
