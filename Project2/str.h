//  MyString.H
//    Class definition for MyString.C

#ifndef _MyString_h
#define _MyString_h

#include <iostream>
using std::istream;
using std::ostream;


class MyString {
  private:
    char  BOUNDS_ERR;			// Used for out of bounds error
    int   m_size;			// Maximum length of MyString
    int   size;				// Length of MyString
    char *str;				// Character array for MyString

  public:
    MyString();				// Constructor
    MyString(				// Constructor (copy constructor)
      const MyString& s			//  MyString object to construct from
    );
    MyString(				// Constructor (copy constructor)
      const char *s			//  Character array to construct from
    );
    MyString(				// Constructor (copy constructor)
      const char c			//  Character to construct from
    );
   ~MyString();				// Destructor

    int      is_int();			// MyString is integer test
    int      is_float();		// MyString is float test
    int      len() const;		// Length of MyString
    int      strstr(			// Index of first occurance of substr
               const MyString& sub	//  SubMyString to search for
             );
    MyString   substr(			// Return subMyString of MyString
               int a,			//  Beginning of subMyString
               int b			//  End of subMyString
             );
    int      token(			// Tokenize MyString
               MyString buf[],		//  Array to store tokens
               int n,			//  Size of array
               char *sep = " \t",	//  Separator character(s)
               char *quote = "\"",	//  Quote character(s)
               int eat = 1		//  Eat consecutive separators flag
             );

    int      operator==(		// Equality operator
               const MyString& s		//  MyString to compare with
             );
    void     operator+=(		// Concatenate operator
               const MyString& s		//  MyString to concatenate
             );
    int      operator!=(		// Inequality operator
               const MyString& s		//  MyString to compare with
             );
    int      operator<=(		// Less than or equal operator
               const MyString& s		//  MyString to compare with
             );
    int      operator>=(		// Greater than or equal operator
               const MyString& s		//  MyString to compare with
             );
    int      operator<(			// Less than operator
               const MyString& s		//  MyString to compare with
             );
    int      operator>(			// Greater than operator
               const MyString& s		//  MyString to compare with
             );
    char&    operator[](		// Index operator
               int i			//  Index to access
             );
    MyString&  operator=(			// Assignment operator
               const MyString& s		//  MyString to compare with
             );
    MyString   operator+(			// Append MyString operator
               const MyString& s		//  MyString to compare with
             );
    MyString   operator+(			// Append character operator
               const char& c		//  MyString to compare with
             );

    int      operator==( const char *s ) { return operator==((MyString) s ); }
    void     operator+=( const char *s ) { return operator+=((MyString) s ); }
    int      operator!=( const char *s ) { return operator!=((MyString) s ); }
    int      operator<=( const char *s ) { return operator<=((MyString) s ); }
    int      operator>=( const char *s ) { return operator>=((MyString) s ); }
    int      operator>( const char *s ) { return operator>((MyString) s ); }
    int      operator<( const char *s ) { return operator<((MyString) s ); }
    MyString&  operator=( const char *s ) { return operator=((MyString) s ); }
    MyString   operator+( const char *s ) { return operator+((MyString) s ); }

    operator char*() const;		// Cast to character MyString
    operator unsigned char*() const;	// Cast to character MyString
    operator float() const;		// Cast to float
    operator int() const;		// Cast to integer

    friend ostream&  operator<<(	// Redirect MyString
               ostream& stream,		//  Output stream to redirect to
               const MyString& s		//  MyString to redirect
             );
    friend istream&  operator>>(	// Redirect into MyString
               istream& stream,		//  Input stream to redirect into
               MyString& s		//  MyString to redirect
             );
};

#endif