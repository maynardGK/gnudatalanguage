/***************************************************************************
                          assocdata.cpp  -  GDL ASSOC datatype
                             -------------------
    begin                : July 22 2002
    copyright            : (C) 2002 by Marc Schellens
    email                : m_schellens@users.sf.net
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "assocdata.hpp"
#include "arrayindex.hpp"
#include "objects.hpp"
#include "io.hpp"

using namespace std;

template< class Parent_>
Assoc_< Parent_>::Assoc_( int lun_, Parent_* assoc_, SizeT fileOffset_): 
    Parent_( assoc_->Dim(), BaseGDL::NOZERO),
    lun( lun_-1), fileOffset( fileOffset_), sliceSize( assoc_->NBytes())
{}

template<>
Assoc_< DStructGDL>::Assoc_( int lun_, DStructGDL* assoc_, SizeT fileOffset_): 
    DStructGDL( assoc_->Desc(), assoc_->Dim(), BaseGDL::NOZERO),
    lun( lun_-1), fileOffset( fileOffset_), sliceSize( assoc_->NBytes())
{}

// writing 1
// assigns srcIn to this at ixList, if ixList is NULL does linear copy
// assumes: ixList has this already set as variable
// ("offset" is offset into srcIn)
template<class Parent_>
void Assoc_<Parent_>::AssignAt( BaseGDL* srcIn, 
				ArrayIndexListT* ixList, 
				SizeT offset)
{
  if( offset != 0)
    throw GDLException("Internal error: Assoc::AssignAt: offset != 0");

  SizeT recordNum;
  bool ixEmpty = ixList->ToAssocIndex( recordNum);
  
  if( !ixEmpty)
    {
      // throw GDLException("File expression cannot be subindexed for output.");
      SizeT seekPos = fileOffset + recordNum * sliceSize;

      if( fileUnits[ lun].Size() > seekPos)
	{
	  fstream& fs = fileUnits[ lun].IStream();
	  fileUnits[ lun].Seek( seekPos);
	  Parent_::Read( fs, fileUnits[ lun].SwapEndian());
	}
      else
	{
	  Parent_::Clear(); // zero fields
	}

      Parent_::AssignAt( srcIn, ixList, offset);

      fstream& fs = fileUnits[ lun].OStream();
      fileUnits[ lun].SeekPad( seekPos);
      Parent_::Write( fs, fileUnits[ lun].SwapEndian());
    }
  else
    {
      // ix empty -> write direct
      fstream& ofs = fileUnits[ lun].OStream();
      fileUnits[ lun].SeekPad( fileOffset + recordNum * sliceSize);
      srcIn->Write( ofs, fileUnits[ lun].SwapEndian());
    }
}

// (writing 2)
// should not be called at all
// only from AccessdescT::Resolve()
// and this only for dot expressions (which are not allowed for assoc vars)
template<class Parent_>
void Assoc_<Parent_>::InsertAt( SizeT offset, 
				BaseGDL* srcIn, 
				ArrayIndexListT* ixList)
{
  throw GDLException("Internal error: Assoc::InsertAt: called.");

  SizeT recordNum;
  bool ixEmpty = ixList->ToAssocIndex( recordNum);

  if( ixEmpty)
    {
      // potential problem if offset != 0
      Parent_::InsertAt( offset, srcIn, NULL);

      fstream& ofs = fileUnits[ lun].OStream();
      fileUnits[ lun].SeekPad( fileOffset + recordNum * sliceSize);
      Parent_::Write( ofs, fileUnits[ lun].SwapEndian());
    }
  else
    {
      // throw GDLException("File expression cannot be subindexed for output.");
      fstream& fs = fileUnits[ lun].IStream();
      fileUnits[ lun].Seek( fileOffset + recordNum * sliceSize);
      Parent_::Read( fs, fileUnits[ lun].SwapEndian());
      
      Parent_::InsertAt( offset, srcIn, ixList);

      fileUnits[ lun].SeekPad( fileOffset + recordNum * sliceSize);
      Parent_::Write( fs, fileUnits[ lun].SwapEndian());
      return;
    }
}

// reading OK
// returns (*this)[ ixList]
template<class Parent_>
Parent_* Assoc_<Parent_>::Index( ArrayIndexListT* ixList)
{
  SizeT recordNum;
  bool ixEmpty = ixList->ToAssocIndex( recordNum);

  fstream& fs = fileUnits[ lun].IStream();
  fileUnits[ lun].Seek( fileOffset + recordNum * sliceSize);
  Parent_::Read( fs, fileUnits[ lun].SwapEndian());

  if( ixEmpty)
    {
      return Parent_::Dup();
    }
  else
    {
      return Parent_::Index( ixList);
    }
}


// explicit instantiation requests

template class Assoc_< DByteGDL>;
template class Assoc_< DIntGDL>;
template class Assoc_< DUIntGDL>;
template class Assoc_< DLongGDL>;
template class Assoc_< DULongGDL>;
template class Assoc_< DLong64GDL>;
template class Assoc_< DULong64GDL>;
template class Assoc_< DPtrGDL>;
template class Assoc_< DStructGDL>;
template class Assoc_< DFloatGDL>;
template class Assoc_< DDoubleGDL>;
template class Assoc_< DStringGDL>;
template class Assoc_< DObjGDL>;
template class Assoc_< DComplexGDL>;
template class Assoc_< DComplexDblGDL>;
