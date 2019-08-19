/*
 * ItemContainer.cpp
 *
 *  Created on:	Jan 10, 2018
 *      Author:	HL65536
 *     Version:	1.0
 */

#include "ItemContainer.h"
#include "TickServiceProvider.h"
#include "DrawServiceProvider.h"
#include "EventMapper.h"
#include "Graphics2D.h"

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

/*
#include <iostream>
#include <string>



#include <filesystem>
namespace fs = std::filesystem;
#define FS_SUPPORT "native"

    //#include <experimental/filesystem>
    //namespace fs = std::experimental::filesystem;
    //#define FS_SUPPORT "experimental"

    //#define FS_SUPPORT "not supported"

using std::cout;
using std::string;

int fuck (int argc , char** argv)
{
    cout << "Filesystem library support status : " << FS_SUPPORT << '\n';

    if(FS_SUPPORT == "not_supported") return 1;

    fs::path p0;
    p0 = "/home/meph/Documents";

    cout << p0    << " "
                << ((fs::exists(p0))?"exists": "does not exists")
                << " and is " << (fs::is_directory(p0)? "a directory":"not a")
                << " directory!" << "\n\nIt has following content:\nFlat:\n";
    for(auto& e : fs::directory_iterator(p0))
    {
        cout << e << '\n';
    }
//    cout << "Recursive:\n";
//    for(auto& e : fs::recursive_directory_iterator(p0))
//    {
//        cout << e << '\n';
//    }
    return 0;
}
*/
#include "ShortNames.h"
#include "WarnErrReporter.h"
#include <iostream>

void ItemContainer::draw(Timestamp t, Frustum* viewFrustum, ChunkManager* cm,DrawServiceProvider* dsp)
{
	std::cout<<"selected: "<<selected<<std::endl;
	dsp->transformViewToGUI(0.75f);
	subsection inventoryBounds=dsp->g->generateSubsection(0,0,1.8f,1.8f,SNAP_MID);
	TRANSPARENT_SECTION_DO_LATER(0.75f)
	{
		dsp->g->setSubsection(&inventoryBounds);
		glColor4f(0,0,0,0.75f);
		glBegin(GL_TRIANGLE_FAN);
		glVertex3f(-1, -1, 1);
		glVertex3f(-1, 1, 1);
		glVertex3f(1, 1, 1);
		glVertex3f(1, -1, 1);
		glEnd();

		dsp->g->resetLastSubsection();
		dsp->revertView();


		dsp->transformViewToGUI(0.751f);
		dsp->g->setSubsection(&inventoryBounds);
		int selectionOffset=selected-firstInList;
		subsection selection=dsp->g->generateSubsection(0,-(selectionOffset+0.5f)*2.0f/maxListLen+1,2.0f,2.0f,SNAP_N);
		dsp->g->setSubsection(&selection);

		glColor4f(1,1,0.5f,0.2f);
		glBegin(GL_TRIANGLE_FAN);
		glVertex3f(-1, -1.0f/maxListLen, 1);
		glVertex3f(-1, 1.0f/maxListLen, 1);
		glVertex3f(1, 1.0f/maxListLen, 1);
		glVertex3f(1, -1.0f/maxListLen, 1);
		glEnd();

		dsp->g->resetAllSubsections();
		dsp->revertView();
	}
	OTHER_SECTION
	{
		dsp->transformViewToGUI(0.752f);
		u32 last=items.size();//actually last item index + 1
		u32 maxListIndx=firstInList+maxListLen;//actually last item index + 1
		maxListIndx=maxListIndx<last?maxListIndx:last;
		float firstLine=1;
		float lineIncrement=-2.0f/maxListLen;
		float curLine=firstLine;
		glColor3f(1,1,1);
		dsp->g->setSubsection(&inventoryBounds);
		for(u32 i=firstInList;i<maxListIndx;i++)
		{
			Item * item=items[i];
			if(!item) WarnErrReporter::notInitializedErr("Found item that is null despite item slot being occupied");
			else
			{
				curLine+=lineIncrement;
				dsp->g->drawString(item->name,-0.95f,curLine,-lineIncrement);
			}
		}
		dsp->g->resetLastSubsection();
	}

	dsp->revertView();
}
#include "EventDefines.h"
void ItemContainer::tick(Timestamp t, TickServiceProvider* tsp)
{
	EventMapper * eMap=tsp->eMap;
	i64 selectAdd=eMap->getStatusAndReset(STATUS_ID_SELECTION_DOWN);
	selectAdd-=eMap->getStatusAndReset(STATUS_ID_SELECTION_UP);
	if(items.size()<1) return;
	while(selectAdd+selected<0) selectAdd+=items.size();
	while(selectAdd+selected>=(i64)items.size()) selectAdd-=items.size();
	selected=(u32)(selectAdd+selected);
	if(selected<firstInList) firstInList=selected;
	if(selected>=firstInList+maxListLen) firstInList=selected-maxListLen+1;

}

ItemContainer::~ItemContainer()
{
	u32 size=items.size();
	for(u32 i=0;i<size;i++)
	{
		delete items[i];
	}
}

