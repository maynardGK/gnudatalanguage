
/***************************************************************************
                          str.cpp  -  basic string manipulation functions
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

#include "includefirst.hpp"

#include <cstdlib>

// that's enough with Cygwin >= 1.7.1 
#ifdef __CYGWIN__
extern "C" {
#endif

// quoting http://permalink.gmane.org/gmane.os.openbsd.tech/19860 :
// 'wordexp() will never be in OpenBSD's libc' :)
// (TODO: perhaps better to implement it using HAVE_WORDEXP_H? + once more below in WordExp())
#if (!defined(__OpenBSD__) && !defined(_WIN32)) || defined(__CYGWIN__)
#  include <wordexp.h>
#endif

#ifdef __CYGWIN__
}
#endif

#include "str.hpp"
#include "gdlexception.hpp"
#include "initsysvar.hpp" // GDLPath();

using namespace std;

string GetEnvString(const char* env)
{
    char* c=getenv(env);
    if( !c) return string("");
    return string(c);
}

DLong StrPos(const string& s, const string& searchStr, long pos, 
	      bool reverseOffset, bool reverseSearch)
{
  if( s == "") return -1;

  long strLen = s.length();
  if( pos == string::npos)
    {
      if( reverseSearch || reverseOffset)
	pos = strLen - 1;
      else
	pos = 0;
    }
  else
    {
      if( pos < 0) pos = 0;
      if( reverseOffset)
	pos = strLen - pos - 1;
    }

  if( searchStr == "")
    {
      if( pos >= strLen)
	return strLen - 1; 
      else 
	{
	  if( reverseSearch && pos < 0) return 0;
	  return pos;
	}
    }
  if( pos < 0) return -1;

  string::size_type res;
  if( reverseSearch)
    {
      res = s.rfind( searchStr, pos);
      if( res == string::npos) return -1;
    }
  else
    {
      res = s.find( searchStr, pos);
      if( res == string::npos) return -1;
    }

  return res;
}
//#define STRMID_DEBUG 
string StrMid(const string& s, long first, long len, bool reverse)
{
#ifdef STRMID_DEBUG
cout << "DebugInfo: StrMid(\"" << s << "\"," << first <<","<<len<<","<<reverse<<") = ";//<<endl
#endif

  // (long)string::npos == -1
  if( len != string::npos && len <= 0)
  {
#ifdef STRMID_DEBUG
	cout << "." << endl;
#endif
	return string("");
  }
  long strLen = s.length();
  if( reverse)
    {
      if( first < 0) return string("");
      first = strLen - first -1;
    }

  if( first >= strLen) return string("");
  if( first < 0) first = 0; 

#ifdef STRMID_DEBUG
	cout << s.substr( first, len)<<"." << endl;
#endif
  return s.substr( first, len);
}

string StrCompress(const string& s, bool removeAll)
{
  SizeT strLen = s.length();
  if( strLen == 0) return string("");

  string res;

  if( removeAll)
    {
      for( SizeT i=0; i<strLen; ++i)
	if( s[i] != ' ' && s[i] != '\t') res += s[i];
      return res;
    }

  SizeT actPos = 0;
  while( actPos < strLen)
    {
      SizeT first = s.find_first_not_of(" \t",actPos);
      if( first >= strLen)
	{
	  res += " ";
	  break;
	}
      if( first != actPos) res += " ";

      SizeT last  = s.find_first_of(" \t",first);
      if( last >= strLen) last = strLen;
      actPos = last;

      res += s.substr( first, last-first);
    }

  return res;
}

void StrPut(std::string& s1, const std::string& s2, DLong pos)
{
  unsigned len1=s1.length();
  unsigned len2=s2.length();
  if( pos >= len1) return;
  unsigned n = (len1 > pos+len2) ? len2 : len1-pos;
  s1.replace( pos, n, s2, 0, n);
}

string StrUpCase(const string& s)
{
  unsigned len=s.length();
  char const *sCStr=s.c_str();
  char* r = new char[len+1];
  ArrayGuard<char> guard( r);
  r[len]=0;
  for(unsigned i=0;i<len;i++)
    r[i]=toupper(sCStr[i]);
  return string(r);
}
void StrUpCaseInplace( string& s)
{
  unsigned len=s.length();
//   char const *sCStr=s.c_str();
//   char* r = new char[len+1];
//   ArrayGuard<char> guard( r);
//   r[len]=0;
  for(unsigned i=0;i<len;i++)
    s[i]=toupper(s[i]);
//   return string(r);
}

string StrLowCase(const string& s)
{
  unsigned len=s.length();
  char const *sCStr=s.c_str();
  char* r = new char[len+1];
  ArrayGuard<char> guard( r);
  r[len]=0;
  for(unsigned i=0;i<len;i++)
    r[i]=tolower(sCStr[i]);
  return string(r);
}
void StrLowCaseInplace(string& s)
{
  unsigned len=s.length();
//   char const *sCStr=s.c_str();
  for(unsigned i=0;i<len;i++)
    s[i]=tolower(s[i]);
//     s[i]=tolower(sCStr[i]);
}

// replacement for library routine 
// double strtod( const char* cStart, char** cEnd);
// to hanlde d/D instead of e/E (e. g. 1.2D5)
// this is done very slow by copying the string and replacing the d/D with e/E
// however, it is done only, if strtod stops at a 'd' or 'D' character 
double StrToD( const char* cStart, char** cEnd)
{
  double ret = strtod( cStart, cEnd);
  if( cEnd != NULL && (**cEnd == 'd' || **cEnd == 'D'))
    {
      int dPos = *cEnd - cStart;      

      // copy the string here. This is very slow.
      // but the glibc implementation is hidden
      // I have not investigated further, but I assume this is because processor specific
      // optimizations are used. So it might be ok to copy the string here as in the regular
      // case the optimzed strtod function will make up for the loss.
      string cStr( cStart);

      // replace d by e and D by E
      cStr[dPos] = (**cEnd == 'd')? 'e':'E';

      char* cEndD;
      const char* cStrc_str = cStr.c_str();
      
      double retD = strtod( cStrc_str, &cEndD);      
      
      // set end as if orignal string had the d/D replaced
      *cEnd = const_cast<char*>(cStart) + (cEndD - cStrc_str);      
      // return replaced result
      return retD;
    }
  return ret;
}
double Str2D( const char* cStart)
{
  char* cEnd;
  double ret = strtod( cStart, &cEnd);
  if( cEnd == cStart)
    {
      Warning("Type conversion error: "
	      "Unable to convert given STRING: '"+string(cStart)+"' to DOUBLE.");
    }
  return ret;
}
long int Str2L( const char* cStart, int base)
{
  char* cEnd;
  long int ret = strtol( cStart, &cEnd, base);
  if( cEnd == cStart)
    {
      Warning("Type conversion error: "
	      "Unable to convert given STRING: '"+string(cStart)+"' to LONG.");
    }
  return ret;
}
unsigned long int Str2UL( const char* cStart, int base)
{
  char* cEnd;
  unsigned long int ret = strtoul( cStart, &cEnd, base);
  if( cEnd == cStart)
    {
      Warning("Type conversion error: "
	      "Unable to convert given STRING: '"+string(cStart)+"' to ULONG.");
    }
  return ret;
}

double Str2D( const string& s)
{
  const char* cStart = s.c_str();
  return Str2D( cStart);
}
long int Str2L( const string& s, int base)
{
  const char* cStart = s.c_str();
  return Str2L( cStart, base);
}
unsigned long int Str2UL( const string& s, int base)
{
  const char* cStart = s.c_str();
  return Str2UL( cStart, base);
}

void WordExp( string& s)
{
//cout << "WordExp  in: " << s << endl;
#if (!defined(__OpenBSD__) && !defined(_WIN32)) || defined(__CYGWIN__)
// esc whitespace
// which is not already escaped
//   string sEsc;
//   for( int i=0; i<s.length(); ++i)
//   {
//     if( s[i] == ' ')
// 	sEsc += "\\ ";
//     else if( s[i] == '\\')
//       {
// 	if( (i+1)<s.length())
// 	{
// 	  if( s[i+1] == ' ')
// 	  {
// 	    sEsc += "\\ ";
// 	    ++i;
// 	  }
// 	}	  
//       }    
//     else
// 	sEsc += s[i];
//   }
//cout << "WordExp esc: " << sEsc << endl;
 wordexp_t p;
 int ok0 = wordexp( s.c_str(), &p, 0);
//  int ok0 = wordexp( sEsc.c_str(), &p, 0);
 if( ok0 == 0)
   {
     //        cout<< p.we_wordc<<"word count\n";
     if( p.we_wordc > 0)
       {
	 //	 s="";
	 string ss= p.we_wordv[0];
	 for(int i=1,ind=s.find(" "); i<p.we_wordc; i++)
	    {
	      while(s[ind++]==' ') ss+=" ";
	      //s+=" ";
	      ss+= p.we_wordv[i];
	      ind=s.find(" ",ind);
	      //	      cout<<"in for\n";
	    }
	  // s=p.we_wordv[0];
	 s=ss;
       }
     //     cout<<s<<"--result\n";
#  if defined(__APPLE__)
     p.we_offs = 0;
#  endif
     wordfree( &p);
   }
#endif
//cout << "WordExp out: " << s << endl;
}

string FullPathFileName(string in_file)
{
  
  string AbsolutePath;

  char *symlinkpath =const_cast<char*> (in_file.c_str());
  char actualpath [PATH_MAX+1];
  char *ptr;

  ptr = realpath(symlinkpath, actualpath);
  if( ptr != NULL ){
    AbsolutePath =string(ptr);
  }else {
    AbsolutePath = in_file;
  }
 
  int debug=0;
  if (debug) {
    cout << in_file << endl;
    cout << AbsolutePath << endl;
  }

  return AbsolutePath;

}

// Tries to find file "fn" along GDLPATH.
// If found, sets fn to the full pathname.
// and returns true, else false
// 
// this line is no more true : "If fn starts with '/' or ".." or "./", just checks if it is readable."
// new since AC 11-Sept-2014: we return the absolute path, this is needed
// for outputs in various procedures:
// GDL> HELP, /source  ou HELP, /traceback
// GDL> print, ROUTINE_INFO('dist',/function,/source)


bool CompleteFileName(string& fn)
{
  WordExp( fn);

  // try actual directory (or given path)
  FILE *fp = fopen(fn.c_str(),"r");
  if(fp)
    {
      fclose(fp);
      fn=FullPathFileName(fn);
      return true;
    }

  if( PathGiven(fn)) return false;

  StrArr path=SysVar::GDLPath();
  if( path.size() == 0)
    {
      string act="./pro/"; // default path if no path is given
	
#ifdef GDL_DEBUG
      cout << "Looking in:" << endl;
      cout << act << endl;
#endif

      act=act+fn;
      fp = fopen(act.c_str(),"r");
      if(fp) {
	fclose(fp);
	fn=act;
	fn=FullPathFileName(fn);
	return true;
      }
    }
  else
    for(unsigned p=0; p<path.size(); p++)
      {
	string act=path[p];
	
	AppendIfNeeded(act,"/");
	
#ifdef GDL_DEBUG
	if( p == 0) cout << "Looking in:" << endl;
	cout << act << endl;
#endif

	act=act+fn;
	fp = fopen(act.c_str(),"r");
	if(fp) {
	  fclose(fp);
	  fn=act;
	  fn=FullPathFileName(fn);
	  return true;
	}
      }
  return false;
}
