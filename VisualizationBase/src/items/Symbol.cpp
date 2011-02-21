/***********************************************************************************************************************
 * Symbol.cpp
 *
 *  Created on: Feb 11, 2011
 *      Author: Dimitar Asenov
 **********************************************************************************************************************/

#include "items/Symbol.h"

#include "Styles.h"

namespace Visualization {
ITEM_COMMON_DEFINITIONS(Symbol)

Symbol::Symbol(Item* parent, const StyleType *style) :
	TextRenderer(parent, style, style->symbol())
{
	setEditable(false);
}

bool Symbol::setText(const QString&)
{
	return false;
}

QString Symbol::currentText()
{
	return style()->symbol();
}

}
