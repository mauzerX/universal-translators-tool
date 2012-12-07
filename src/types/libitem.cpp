/***************************************************************
 * Name:      libitem.h
 * Purpose:   implementation of LibItem class
 * Author:    Pavlovets Ilia (ilia.pavlovets@gmail.com)
 * Created:   2012-12-06
 * Copyright: Pavlovets Ilia
 * License:
 **************************************************************/
 
#include "pch.h"
#include "libitem.h"

const LibItemId LIBITEM_BADID = 0xFFFFFFFF;
const LibItemId LIBITEM_ROOTID = 0;


LibItem::LibItem():
	mID(LIBITEM_BADID),
	mText(wxEmptyString),
	mIsOk(false),
	mParent(NULL),
	mChildren()
{
}



LibItem::LibItem(LibItem* parent, LibItemId id):
	mID(id),
	mText(wxEmptyString),
	mIsOk(parent != NULL),
	mParent(parent),
	mChildren()
{
}



LibItem::LibItem( const LibItem& item ):
	mID(item.mID),
	mText(item.mText),
	mIsOk(item.mIsOk),
	mParent(item.mParent),
	mChildren(item.mChildren)
{
}



/*virtual */ LibItem::~LibItem()
{
	for (LibItemArray::iterator it = mChildren.begin(); it != mChildren.end(); ++it)
	{
		delete *it;
	}

	mChildren.clear();
}



inline void LibItem::ParseId( LibItemId id, int &level, int& n )
{
	level = id >> 20;
	n = id & 0x000FFFFF;
}



inline LibItemId LibItem::PackId( int level, int n )
{
	wxASSERT( level <= 0xFFF && n <= 0xFFFFF );

	LibItemId result = level;
	result <<= 20;
	result |= n;
	return result;
}



inline LibItemId LibItem::CreateNewId()	// for children
{
	int level, n;
	ParseId( mID, level, n );
	return PackId( level + 1, mChildren.size() );
}



LibItem* LibItem::AddChild()
{
	LibItem* newItem = new LibItem( this, CreateNewId() );
	return newItem;
}
