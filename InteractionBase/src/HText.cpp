/***********************************************************************************************************************
 * HText.cpp
 *
 *  Created on: Jan 11, 2011
 *      Author: Dimitar Asenov
 **********************************************************************************************************************/

#include "HText.h"
#include "VisualizationBase/headers/Scene.h"
#include "VisualizationBase/headers/items/TextRenderer.h"
#include "VisualizationBase/headers/items/ModelItem.h"

namespace Interaction {

HText::HText()
{
}

HText* HText::instance()
{
	static HText h;
	return &h;
}

void HText::mousePressEvent(Visualization::Item *target, QGraphicsSceneMouseEvent *event)
{
	if ( event->button() == Qt::LeftButton )
	{
		Visualization::TextRenderer < Visualization::Item > *ti = dynamic_cast<Visualization::TextRenderer<Visualization::Item>*> (target);
		if ( ti ) ti->setSelected(event->pos().x(), event->pos().x());
		else
		{
			Visualization::TextRenderer < Visualization::ModelItem > *tmi
					= dynamic_cast<Visualization::TextRenderer<Visualization::ModelItem>*> (target);
			if ( tmi ) tmi->setSelected(event->pos().x(), event->pos().x());
		}

		target->scene()->updateTopLevelItems();
	}
	else InteractionHandler::mousePressEvent(target, event);
}

void HText::mouseMoveEvent(Visualization::Item *target, QGraphicsSceneMouseEvent *event)
{
	Visualization::TextRenderer < Visualization::Item > *ti = dynamic_cast<Visualization::TextRenderer<Visualization::Item>*> (target);
	if ( ti ) ti->setSelected(event->buttonDownPos(Qt::LeftButton).x(), event->pos().x());
	else
	{
		Visualization::TextRenderer < Visualization::ModelItem > *tmi = dynamic_cast<Visualization::TextRenderer<Visualization::ModelItem>*> (target);
		if ( tmi ) tmi->setSelected(event->buttonDownPos(Qt::LeftButton).x(), event->pos().x());
	}

	target->scene()->updateTopLevelItems();
	InteractionHandler::mouseMoveEvent(target, event);
}

void HText::focusOutEvent(Visualization::Item *target, QFocusEvent *)
{
	Visualization::TextRenderer < Visualization::Item > *ti = dynamic_cast<Visualization::TextRenderer<Visualization::Item>*> (target);
		if ( ti ) ti->resetSelected();
		else
		{
			Visualization::TextRenderer < Visualization::ModelItem > *tmi = dynamic_cast<Visualization::TextRenderer<Visualization::ModelItem>*> (target);
			if ( tmi ) tmi->resetSelected();
		}
}

}