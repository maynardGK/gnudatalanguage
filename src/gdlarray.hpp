/***************************************************************************
                          gdlarray.hpp  -  basic typedefs
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

#ifndef GDLARRAY_HPP_
#define GDLARRAY_HPP_

// #define GDLARRAY_CACHE
#undef GDLARRAY_CACHE

//#define GDLARRAY_DEBUG
#undef GDLARRAY_DEBUG

// for complex (of POD)
const bool TreatPODComplexAsPOD = true;

template <typename T, bool IsPOD>
class GDLArray
{
private:
	enum GDLArrayConstants
	{
		smallArraySize = 27,
		maxCache = 1000 * 1000 // ComplexDbl is 16 bytes
	};
		
	typedef T Ty;

#ifdef USE_EIGEN  
  EIGEN_ALIGN16 char scalarBuf[ smallArraySize * sizeof(Ty)];
#else
  char scalarBuf[ smallArraySize * sizeof(Ty)];
#endif
  
  Ty* Scalar()
  {
    assert( sz <= smallArraySize);
    if( IsPOD)
    {
      return reinterpret_cast<Ty*>(scalarBuf);
    }
    else
    {
      Ty* b = reinterpret_cast<Ty*>(scalarBuf); 
      for( int i = 0; i<sz; ++i) 
	new (&(b[ i])) Ty();
      return b;
    }
  }

#ifdef GDLARRAY_CACHE
#error "GDLARRAY_CACHE defined"
	static SizeT cacheSize;
	static Ty* cache;
	static Ty* Cached( SizeT newSize);
#endif
		
	Ty*    buf;
	SizeT sz;

public:
  	GDLArray() throw() : buf( NULL), sz( 0) {}
  
#ifndef GDLARRAY_CACHE

  Ty* New( SizeT s)
  {
// better align all data, also POD    
//     if( IsPOD)
//     {
#ifdef USE_EIGEN 
    return Eigen::internal::aligned_new<Ty>( s);
#else
    return new Ty[ s];
#endif
//     }
//     else // non-pod
//       return new Ty[ s];
  }
    
  ~GDLArray() throw()
  {
  if( IsPOD)
    {
#ifdef USE_EIGEN  
    if( buf != Scalar()) 
	Eigen::internal::aligned_delete( buf, sz);
#else
    if( buf != Scalar()) 
	delete[] buf; // buf == NULL also possible
#endif
    // no cleanup of "buf" here
    }
  else
    {
    if( buf != reinterpret_cast<Ty*>(scalarBuf)) 
      delete[] buf; // buf == NULL also possible
    else
      for( int i = 0; i<sz; ++i) 
	buf[i].~Ty();
    }
  }

  GDLArray( const GDLArray& cp) : sz( cp.size())
  {
    if( IsPOD)
    {
    try {
	buf = (cp.size() > smallArraySize) ? New(cp.size()) /*New T[ cp.size()]*/ : Scalar();
    } catch (std::bad_alloc&) { ThrowGDLException("Array requires more memory than available"); }

    std::memcpy(buf,cp.buf,sz*sizeof(T));
    }
    else
    {
      try {
	buf = (cp.size() > smallArraySize) ? New(cp.size()) /*new Ty[ cp.size()]*/ : Scalar();
      } catch (std::bad_alloc&) { ThrowGDLException("Array requires more memory than available"); }
     for( SizeT i=0; i<sz; ++i)
       buf[ i] = cp.buf[ i];
    }
  }

  GDLArray( SizeT s, bool dummy) : sz( s)
  {
    try {
      buf = (s > smallArraySize) ? New(s) /*T[ s]*/ : Scalar();
    } catch (std::bad_alloc&) { ThrowGDLException("Array requires more memory than available"); }
  }
  
  GDLArray( T val, SizeT s) : sz( s)
  {
    try {
	    buf = (s > smallArraySize) ? New(s) /*T[ s]*/ : Scalar();
    } catch (std::bad_alloc&) { ThrowGDLException("Array requires more memory than available"); }

    for( SizeT i=0; i<sz; ++i)
      buf[ i] = val;
  }
  
  GDLArray( const T* arr, SizeT s) : sz( s)
  {
    if( IsPOD)
    {
      try
      {
	      buf = ( s > smallArraySize ) ? New(s) /*T[ s]*/: Scalar();
      }
      catch ( std::bad_alloc& ) { ThrowGDLException ( "Array requires more memory than available" ); }

      std::memcpy(buf,arr,sz*sizeof(T));
    }
    else
    {    
      try {
	buf = (s > smallArraySize) ? New(s) /*new Ty[ s]*/: Scalar();
      } catch (std::bad_alloc&) { ThrowGDLException("Array requires more memory than available"); }
      for( SizeT i=0; i<sz; ++i)
	buf[ i] = arr[ i];
      }
  }

#else // GDLARRAY_CACHE

  // use definition in datatypes.cpp
  GDLArray( const GDLArray& cp) ;
  GDLArray( SizeT s, bool b) ;
  GDLArray( T val, SizeT s) ;
  GDLArray( const T* arr, SizeT s) ;
  ~GDLArray() throw();

#endif // GDLARRAY_CACHE
  


  explicit GDLArray( const T& s) throw() : /*scalar( s),*/ sz( 1)
  { 
    buf = Scalar();
    buf[0] = s;    
  }

  T& operator[]( SizeT ix) throw()
  {
    // if( ix >= sz) 
    assert( ix < sz);
    return buf[ ix];
  }
  const T& operator[]( SizeT ix) const throw()
  {
//     if( ix >= sz) // debug 
      assert( ix < sz);
    return buf[ ix];
  }

// private: // disable
// only used (indirect) by DStructGDL::DStructGDL(const DStructGDL& d_)
void InitFrom( const GDLArray& right )
{
  if( IsPOD)
  {
    assert( &right != this);
    assert ( sz == right.size() );
    std::memcpy(buf,right.buf,sz*sizeof(T));
  }
  else
  {
    assert( &right != this);
    assert ( sz == right.size() );
    for ( SizeT i=0; i<sz; ++i )
	buf[ i] = right.buf[ i];
  }    
}

GDLArray& operator= ( const GDLArray& right )
{
    assert( this != &right);
    assert( sz == right.size());
    for ( SizeT i=0; i<sz; ++i )
	    buf[ i] = right.buf[ i];
    return *this;
}

//   GDLArray& operator+=( const GDLArray& right) throw()
//   {
//     for( SizeT i=0; i<sz; ++i)
//       buf[ i] += right.buf[ i];
//     return *this;
//   }
//   GDLArray& operator-=( const GDLArray& right) throw()
//   {
//     for( SizeT i=0; i<sz; ++i)
//       buf[ i] -= right.buf[ i];
//     return *this;
//   }
  GDLArray& operator+=( const T& right) throw()
  {
    for( SizeT i=0; i<sz; ++i)
      buf[ i] += right;
    return *this;
  }
  GDLArray& operator-=( const T& right) throw()
  {
    for( SizeT i=0; i<sz; ++i)
      buf[ i] -= right;
    return *this;
  }

  void SetBuffer( T* b) throw()
  {
    buf = b;
  }
  T* GetBuffer() throw()
  {
    return buf;
  }
  void SetBufferSize( SizeT s) throw()
  {
    sz = s;
  }

  SizeT size() const throw()
  {
    return sz;
  }

void SetSize( SizeT newSz ) // only used in DStructGDL::DStructGDL( const string& name_) (dstructgdl.cpp)
{
	assert ( sz == 0);
	if ( newSz > smallArraySize )
	{
		try
		{
			buf = New(newSz) /*new T[ newSz]*/;
		}
		catch ( std::bad_alloc& )
		{
			ThrowGDLException ( "Array requires more memory than available" );
		}
	}
	else
	{
		// default constructed instances have buf == NULL and size == 0
		// make sure buf is set corectly if such instances are resized
		buf = Scalar();
	}
	sz = newSz;
}

  
}; // GDLArray

#endif
