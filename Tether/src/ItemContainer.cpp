/*
 * ItemContainer.cpp
 *
 *  Created on:	Jan 10, 2018
 *      Author:	HL65536
 *     Version:	1.0
 */

#include "ItemContainer.h"

Graphics2D * ItemContainer::g=0;

ItemContainer::ItemContainer():
items()
{

	// TODO Auto-generated constructor stub

}

u32 ItemContainer::maximumAdd(Item* item)
{
	return (u32)-1;//TODO
}

#include "myAssert.h"
Item* ItemContainer::addItem(Item* item)
{
	assert(item->amount!=0);
	u32 toAdd=maximumAdd(item);
	if(toAdd>=item->amount)
	{
		insertR(item);
		return 0;
	}
	else if(toAdd>0)
	{
		Item * other=item->newClone();
		other->amount=toAdd;
		insertR(other);
		item->amount-=toAdd;
		return item;
	}
	return item;
}

Item* ItemContainer::removeNextItem(u32 maxCount)
{
	if(maxCount==0) return 0;
	if(items.size()>0)
	{
		Item * curItem=items.back();
		items.pop_back();
		assert(curItem->amount!=0);
		if(maxCount>=curItem->amount)
		{
			return curItem;
		}
		else
		{
			Item * other=curItem->newClone();
			other->amount-=maxCount;
			insertR(other);

			curItem->amount=maxCount;
			return curItem;
		}
	}
	return 0;
}

Item * ItemContainer::newClone()
{
	return 0;//containers cannot be cloned
}

void ItemContainer::insertR(Item* it)
{
	Item * last=items.back();
	if(last->ID==it->ID)
	{
		last->amount+=it->amount;
	}
	else
	{
		items.push_back(it);
	}
}


void ItemContainer::draw(Timestamp t, Frustum* viewFrustum, ChunkManager* cm,DrawServiceProvider* dsp)
{
	if((!g) || dsp->reinitGraphics()) g=new Graphics2D(6,dsp->getAspectRatio(),dsp->suggestFont());
	dsp->transformViewToGUI(0.75f);
	subsection inventoryBounds=g->generateSubsection(0,0,2,2,SNAP_W);
	TRANSPARENT_SECTION_DO_LATER(0.75f)
	{
		g->setSubsection(&inventoryBounds);
		glColor4f(1,1,1,0.5f);
		glBegin(GL_TRIANGLE_FAN);
		glVertex3f(-1, -1, 1);
		glVertex3f(-1, 1, 1);
		glVertex3f(1, 1, 1);
		glVertex3f(1, -1, 1);
		glEnd();
		g->resetLastSubsection();
	}

	dsp->revertView();
}

ItemContainer::~ItemContainer()
{
	u32 size=items.size();
	for(u32 i=0;i<size;i++)
	{
		delete items[i];
	}
}

