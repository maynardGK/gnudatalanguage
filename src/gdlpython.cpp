/***************************************************************************
                          gdlpython.cpp  -  python embedded in GDL
                             -------------------
    begin                : July 22 2002
    copyright            : (C) 2002 by Marc Schellens
    email                : m_schellens@users.sourceforge.net
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifdef HAVE_CONFIG_H
#include <config.h>
#else
#define USE_PYTHON 1
#endif

#ifdef USE_PYTHON

#include <deque>

#include "datatypes.hpp"
#include "envt.hpp"
#include "gdlpython.hpp"
#include "objects.hpp"

// already included from basegdl.hpp
//#include <Python.h>

#include <numarray/libnumarray.h>
//#include <numarray/numarray.h>

using namespace std;

void PythonInit()
{
  if( Py_IsInitialized()) return;
  Py_Initialize();
  
  static int argc = 1;
  static char* arg0 = "./py/python.exe";
  static char* argv[] = {arg0};
  PySys_SetArgv(argc, argv);

  import_libnumarray();
}

// don't use, as numarray cannot be restarted
void PythonEnd()
{
  if( !Py_IsInitialized()) return;
  Py_Finalize();
}

template< typename T>
T* NewFromPyArrayObject( const dimension& dim, PyArrayObject *array)
{
  T* res = new T( dim, BaseGDL::NOZERO);
  SizeT nEl = res->N_Elements();
  typename T::Ty* dPtr = 
    reinterpret_cast<typename T::Ty*>( NA_OFFSETDATA(array));
  for( SizeT i=0; i<nEl; ++i) (*res)[i] = dPtr[i];
  Py_DECREF(array); // must be decremented
  return res;
}

BaseGDL* FromPython( PyObject* pyObj)
{
  int isArray = NA_NumArrayCheck( pyObj);
  if( !isArray)
    {
      if( PyString_Check( pyObj))
	{
	  return new DStringGDL( PyString_AsString( pyObj));
	}
      if( PyInt_Check( pyObj))
	{
	  return new DLongGDL( PyInt_AsLong( pyObj));
	}
      if( PyLong_Check( pyObj))
	{
	  return new DLongGDL( PyLong_AsLong( pyObj));
	}
      if( PyFloat_Check( pyObj))
	{
	  return new DDoubleGDL( PyFloat_AsDouble( pyObj));
	}
      if( PyComplex_Check( pyObj))
	{
	  DDouble re,im;
	  re = PyComplex_RealAsDouble( pyObj);
	  im = PyComplex_ImagAsDouble( pyObj);
	  return new DComplexDblGDL( DComplexDbl( re, im));
	}
      throw GDLException( "PYTHON: Cannot convert python scalar.") ;
    }

  PyArrayObject* array = reinterpret_cast< PyArrayObject*>( pyObj); 
  NumarrayType item_type = static_cast< NumarrayType>( array->descr->type_num);

  // make array contiguous
  array = NA_InputArray( pyObj, item_type, C_ARRAY);
  if( array == NULL)
    throw GDLException( "PYTHON: Error getting python array.") ;
  
  int nDim = array->nd;
  SizeT dimArr[ MAXRANK];
  if( nDim > MAXRANK)
    {
    Warning( "PYTHON: Array has more than "+MAXRANK_STR+
	     " dimensions. Extending last one."); 
    SizeT lastDim = array->dimensions[ MAXRANK-1];
    for( SizeT i=MAXRANK; i<nDim; ++i) lastDim *= array->dimensions[ i];
    for( SizeT i=0; i<MAXRANK-1; ++i)
      dimArr[ i] = array->dimensions[ i];
    dimArr[ MAXRANK-1] = lastDim;
    nDim = MAXRANK;
    }
  else
    {
      for( SizeT i=0; i<nDim; ++i)
	dimArr[ i] = array->dimensions[ i];
    }
  dimension dim( dimArr, nDim);

  switch( item_type) 
    {
      // case tAny:  //UNDEF***
    case tUInt8:   //BYTE
      return NewFromPyArrayObject< DByteGDL>( dim, array);
    case tInt16:   //INT
      return NewFromPyArrayObject< DIntGDL>( dim, array);
    case tInt32:     //LONG	
      return NewFromPyArrayObject< DLongGDL>( dim, array);
    case tFloat32:   //FLOAT	
      return NewFromPyArrayObject< DFloatGDL>( dim, array);
    case tFloat64:  //DOUBLE	
      return NewFromPyArrayObject< DDoubleGDL>( dim, array);
    case tComplex32:  //COMPLEX	
      return NewFromPyArrayObject< DComplexGDL>( dim, array);
      // case tAny:  //STRING***	
      // case tAny:  //STRUCT***	
    case tComplex64: //COMPLEXDBL	
      return NewFromPyArrayObject< DComplexDblGDL>( dim, array);
      // case tAny:  //PTR***		
      // case tAny:  //OBJECT***
    case tUInt16:         //UINT*
      return NewFromPyArrayObject< DUIntGDL>( dim, array);
    case tUInt32:         //ULONG*
      return NewFromPyArrayObject< DULongGDL>( dim, array);
//     case tLong64:          //LONG64*
//       return NewFromPyArrayObject< DLong64GDL>( dim, array);
//     case tULong64:         //ULONG64*
//       return NewFromPyArrayObject< DULong64GDL>( dim, array);
    default:
      Py_DECREF(array); // must be decremented
      throw GDLException( "FromPython: Unknown array type.") ;
    }
  
  return NULL; // compiler shut-up
}

namespace lib {

  using namespace std;

  BaseGDL* gdlpython( EnvT* e, int kIx)
  {
    PythonInit();

    SizeT nParam = e->NParam();

    static int argvIx =  e->KeywordIx( "ARGV");
    BaseGDL* argv1 = e->GetKW( argvIx);
    if( argv1 != NULL)
      {
	DStringGDL* argvS = dynamic_cast<DStringGDL*>( argv1);
	if( argvS == NULL)
	  e->Throw( "ARGV keyword must be of type STRING.");
	
	int argc = argvS->N_Elements();
	char** argv = new char*[ argc];

	// pyhton copies the value -> threats it as const
	for( int i=0; i<argc; ++i)
	  argv[i] = const_cast<char*>((*argvS)[ i].c_str()); 

	PySys_SetArgv(argc, argv);
	delete[] argv;
      }

    if( nParam < 2 && kIx != -1)
      e->Throw( "Function must have at least 2 parameters.");
    
    if( nParam == 0)
      return NULL; // ok, just keywords

    DString module;
    e->AssureScalarPar<DStringGDL>( 0, module);
    
//     PyObject* pName = PyString_FromString( module.c_str());
//     // Error checking of pName left out
//     PyObject* pModule = PyImport_Import(pName);
//     Py_DECREF(pName);
    PyObject* pModule = 
      PyImport_ImportModule(const_cast<char*>(module.c_str()));
    if (pModule == NULL)
      {
	PyErr_Print();
	e->Throw( "Failed to load module: "+module);
      }

    if( nParam == 1)
      {
	Py_DECREF(pModule);
	return NULL; // ok, only load module
      }
    
    DString function;
    e->AssureScalarPar<DStringGDL>( 1, function);

    PyObject* pDict = PyModule_GetDict(pModule);
    /* pDict is a borrowed reference */

    PyObject* pFunc = PyDict_GetItemString(pDict, function.c_str());
    /* pFunc: Borrowed reference */

    if( !(pFunc && PyCallable_Check(pFunc)))
      {
	if (PyErr_Occurred()) PyErr_Print();
	e->Throw( "Cannot find function: "+function);
      }

    PyObject* pArgs = PyTuple_New( nParam-2);
    for( SizeT i = 2; i<nParam; ++i) 
      {
	BaseGDL* actPar = e->GetParDefined( i);
	PyObject* pValue = actPar->ToPython();
	if (!pValue) 
	  {
	    Py_DECREF(pArgs);
	    Py_DECREF(pModule);
	    
	    e->Throw( "Cannot convert value: "+
		      e->GetParString( i));
	  }
	/* pValue reference stolen here: */
	PyTuple_SetItem(pArgs, i-2, pValue);
      }

    PyObject* pResult = PyObject_CallObject(pFunc, pArgs);
    Py_DECREF(pArgs);
    Py_DECREF(pModule);
    // pDict and pFunc are borrowed and must not be Py_DECREF-ed
    if( pResult == NULL) 
      {
	PyErr_Print();
	e->Throw( "Call failed: "+module+"."+function);
      }

    if( kIx == -1) // called as procedures
      {
	Py_DECREF(pResult);
	return NULL;
      }

    // as function
    BaseGDL* res;
    if( pResult == Py_None)
      {
	Py_DECREF(pResult);
	BaseGDL* defRet = e->GetKW( kIx);
	if( defRet == NULL)
	  e->Throw( "PYTHON: Function returned 'None' "
		    "and DEFAULTRETURN not defined.");
	res = defRet->Dup();
      }
    else
      {
	try {
	  res = FromPython( pResult);
	  Py_DECREF(pResult);
	}
	catch(...)
	  {
	    Py_DECREF(pResult);
	    throw;
	  }
      }
    
    return res;
  }

  // GDL PYTHON procedure
  // PYTHON,module,function[,args]
  void gdlpython_pro( EnvT* e)
  {
    gdlpython( e, -1);
  }

  // GDL PYTHON function
  BaseGDL* gdlpython_fun( EnvT* e)
  {
    static int kIx = e->KeywordIx( "DEFAULTRETURNVALUE");
    return gdlpython( e, kIx);
  }


} // namespace

#endif // USE_PYTHON
