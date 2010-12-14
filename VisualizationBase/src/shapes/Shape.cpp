/***********************************************************************************************************************
 * Shape.cpp
 *
 *  Created on: Dec 13, 2010
 *      Author: Dimitar Asenov
 **********************************************************************************************************************/

#include "shapes/Shape.h"

namespace Visualization {

Shape::Shape(Item* parent_, ShapeStyle *style) :
	parent(parent_), style_(style), width_(0), height_(0), xOffset_(0), yOffset_(0)
{
}

Shape::~Shape()
{
}

void Shape::setItemSize(int width, int height)
{
	parent->size.setWidth(width);
	parent->size.setHeight(height);
	setParentNeedsUpdate();
}

void Shape::setItemBoundingRect(int x, int y, int width, int height)
{
	parent->bounding_rect.setRect(x, y, width, height);
	setParentNeedsUpdate();
}

void Shape::setParentNeedsUpdate()
{
	parent->needsUpdate_ = true;
}

void Shape::setInnerWidth(int width)
{
	width_ = width;
	if ( sizeToUse == OutterSize ) height_ = 0;
	sizeToUse = InnerSize;
	update();
	setParentNeedsUpdate();
}

void Shape::setInnerHeight(int height)
{
	height_ = height;
	if ( sizeToUse == OutterSize ) width_ = 0;
	sizeToUse = InnerSize;
	update();
	setParentNeedsUpdate();
}

void Shape::setInnerSize(int width, int height)
{
	width_ = width;
	height_ = height;
	sizeToUse = InnerSize;
	update();
	setParentNeedsUpdate();
}

void Shape::setOutterWidth(int width)
{
	width_ = width;
	if ( sizeToUse == InnerSize ) height_ = 0;
	sizeToUse = OutterSize;
	update();
	setParentNeedsUpdate();
}
void Shape::setOutterHeight(int height)
{
	height_ = height;
	if ( sizeToUse == InnerSize ) width_ = 0;
	sizeToUse = OutterSize;
	update();
	setParentNeedsUpdate();
}

void Shape::setOutterSize(int width, int height)
{
	width_ = width;
	height_ = height;
	sizeToUse = OutterSize;
	update();
	setParentNeedsUpdate();
}

void Shape::setOffset(int x, int y)
{
	xOffset_ = x;
	yOffset_ = y;
	update();
	setParentNeedsUpdate();
}

void Shape::setStyle(ShapeStyle *style)
{
	style_ = style;
	setParentNeedsUpdate();
}

QPoint Shape::contentPosition()
{
	return QPoint(xOffset_, yOffset_);
}

}