/*
 * ItemContainer.cpp
 *
 *  Created on:	Jan 10, 2018
 *      Author:	HL65536
 *     Version:	1.0
 */

#include "ItemContainer.h"

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
void ItemContainer::draw(Timestamp t, Frustum* viewFrustum, ChunkManager* cm,DrawServiceProvider* dsp)
{
	dsp->transformViewToGUI(0.75f);
	subsection inventoryBounds=dsp->g->generateSubsection(0,0,2,2,SNAP_W);
//	std::cout<<"test here1"<<std::endl;
	TRANSPARENT_SECTION_DO_LATER(0.75f)
	{
//		std::cout<<"test here2"<<std::endl;
//		dsp->g->setSubsection(&inventoryBounds);
//		glColor4f(1,1,1,0.5f);
//		glBegin(GL_TRIANGLE_FAN);
//		glVertex3f(-1, -1, 1);
//		glVertex3f(-1, 1, 1);
//		glVertex3f(1, 1, 1);
//		glVertex3f(1, -1, 1);
//		glEnd();
//		dsp->g->resetLastSubsection();
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

