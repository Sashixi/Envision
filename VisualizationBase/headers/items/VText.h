/***********************************************************************************************************************
 * VText.h
 *
 *  Created on: Dec 14, 2010
 *      Author: Dimitar Asenov
 **********************************************************************************************************************/

#ifndef VTEXT_H_
#define VTEXT_H_

#include "visualizationbase_api.h"

#include "TextStyle.h"
#include "../ModelItem.h"
#include "ModelBase/headers/nodes/Text.h"

#include <QtGui/QStaticText>

namespace Visualization {

class VISUALIZATIONBASE_API VText : public ModelItem
{
	public:
		typedef TextStyle StyleType;

	private:
		QStaticText text;
		TextStyle* style;

		qreal xOffset;
		qreal yOffset;

	public:
		VText(Item* parent, Model::Text *text, TextStyle *style = TextStyle::getDefault());
		static const QString& className();

		void setStyle(TextStyle *style);

		virtual void determineChildren();
		virtual void updateState();

		virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
};

inline const QString& VText::className() {static QString name("VText"); return name;}
inline void VText::setStyle(TextStyle *style_)  { style = style_; setUpdateNeeded(); };

}

#endif /* VTEXT_H_ */
