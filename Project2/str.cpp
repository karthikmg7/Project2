#define _CRT_SECURE_NO_DEPRECATE	// Supress strncpy dep warns

#include <string.h>
#include <assert.h>
#include "str.h"

/*--------------------------------------------------------------------------*/
/*  STRING.C								    */
/*    This class implements strings in C++, which can be created, assigned  */
/*    compared, and manipulated in an intuitive manner			    */
/*									    */
/*- Modification History ---------------------------------------------------*/
/*  When:	Who:			Comments:			    */
/*									    */
/*  05-Aug-93	Christopher G. Healey	Initial implementation		    */
/*              George Tsiknis						    */
/*  01-Jan-01	Christopher G. Healey	Moved token() from filereader to    */
/*					string class			    */
/*  01-Feb-06	Christopher G. Healey	Added support for NAMESPACEs	    */
/*  17-Apr-06	Christopher G. Healey	Added ability in token() for caller */
/*					to specify quote character(s),	    */
/*					rather than assuming double-quote   */
/*  19-Apr-06	Christopher G. Healey	Added ability in token() to specify */
/*					whether to ignore consecutive	    */
/*					separators or not		    */
/*  14-Jul-08	Christopher G. Healey	Added [] to array delete statements */
/*  29-Jul-08	Christopher G. Healey	Fixed some signed/unsigned warnings */
/*--------------------------------------------------------------------------*/

using std::cerr;			// std namespace defaults
using std::istream;
using std::ostream;


int MyString::is_int()

   //  This method returns TRUE if the MyString represents a valid integer,
   //  FALSE otherwise
{
   //  Ensure the characters in the MyString are only digits

   if ( size != (int) strspn( str, "0123456789+-" ) ) {
      return 0;

   } else {
      return 1;
   }
}					// End method is_int


int MyString::is_float()

   //  This method returns TRUE if the MyString represents a valid float,
   //  FALSE otherwise
{
   //  Ensure the characters in the MyString are only digits or decimal place

   if ( size != (int) strspn( str, "0123456789.+-" ) ) {
      return 0;

   } else {
      return 1;
   }
}					// End method is_float


int MyString::len() const

   //  This method returns the length of the given MyString
{
   return size;
}					// End method len


int MyString::strstr( const MyString& sub )

   //  This method returns the index of the first occurrance of the
   //  given subMyString, or -1 if no such subMyString exists
   //
   //  sub:  SubMyString to search for
{
   char *pos;				// Position of first subMyString


   if ( ( pos = ::strstr( str, sub.str ) ) == NULL ) {
      return -1;
   } else {
      return int( pos - str );
   }
}					// End method strstr


MyString MyString::substr( int a, int b )

   //  This method returns the subMyString in the range str[ a..b ]
   //
   //  a:  Start index of subMyString
   //  b:  End index of subMyString
{
   MyString  ret_str;			// MyString object to return


   if ( a < 0 || a >= size || b < 0 || b >= size || a > b )
      return ret_str;

   //  Ensure ret_str.str large enough to hold ( b - a + 1 ) chars plus
   //  one char for end-of-MyString marker

   if ( ( b - a + 1 ) > ret_str.m_size ) {
      do {
         ret_str.m_size *= 2;
      }
      while( ( b - a + 1 ) > ret_str.m_size );

      delete [] ret_str.str;
      ret_str.str = new char[ ret_str.m_size + 1 ];
      assert( ret_str.str != NULL );
   }

   ret_str.size = b - a + 1;
   strncpy( ret_str.str, &str[ a ], b - a + 1 );
   ret_str.str[ ret_str.size ] = '\0';

   return ret_str;
}					// End method substr


int MyString::token( MyString buf[], int n, char *sep, char *quo, int eat )

   //  This method parses the MyString into individual words, and stores
   //  those words in the given array
   //
   //  buf:  Buffer to use to store words
   //  n:    Maximum size of buffer
   //  sep:  Separator character(s) (default=space, tab)
   //  quo:  Quote character(s) (default=double-quote)
   //  eat:  Eat consecutive separators flag (default=TRUE)
{
   char  def_quo[] = "\"";		// Default quote (double-quote)
   char  def_sep[] = " \t";		// Default separator (space, tab)
   int   elem = 0;			// Current buffer element
   int   i;				// Current line element
   int   j;				// Loop counter
   int   s;				// Start of subMyString position


   if ( sep == NULL )			// If caller passes explicit NULLs..
      sep = def_sep;			// ..switch to proper default values
   if ( quo == NULL )
      quo = def_quo;

   i = 0;
   while( i != size && elem < n ) {

   //  Walk through MyString searching for the first non-separator char;
   //  record empty tokens if we're not eating consecutive separators

      for( s = i; i != size && strchr( sep, str[ i ] ) != NULL; i++ ) {
      }

      if ( i - s > 1 && !eat ) {	// Don't eat consecutive separators?
        if ( s == 0 && elem < n )	// Account for missing first token
           buf[ elem++ ] = "";

         for( j = s + 1; j < i && elem < n; j++ )
            buf[ elem++ ] = "";
      }

      //  If line ended in separator, account for missing final token

      if ( i == size && !eat && elem < n )
        buf[ elem++ ] = "";

      if ( i == size || elem >= n )	// Line ended in sep, or out of tokens?
         return elem;

      //  Process quoted text?

      if ( strchr( quo, str[ i ] ) != NULL ) {
         i++;				// Ignore quote character
         s = i;				// Store start subMyString position

         while( i != size && strchr( quo, str[ i ] ) == NULL )
            i++;

         if ( s == i ) {		// Empty MyString within quotes?
           buf[ elem ] = "";
         } else {
           buf[ elem ] = substr( s, i - 1 );
         }

         if ( i == size ) {
            cerr << "MyString::token(), mismatched quotes\n";
            return elem;
         }

         i++;				// Ignore closing quote
         elem++;			// Increment token count

   //  Otherwise, if not end of line, process normal token

      } else {
         s = i;				// Store start subMyString position

         while( i != size && strchr( sep, str[ i ] ) == NULL )
            i++;

         buf[ elem ] = substr( s, i - 1 );
         elem++;
      }
   }

   return elem;				// Return token count
}					// End method token


int MyString::operator==( const MyString& s )

   //  Compare operator for MyStrings, returns TRUE if strings are equal,
   //  FALSE otherwise
   //
   //  s:  String to compare against
{
   return !strcmp( str, s.str );
}					// End operator ==


void MyString::operator+=( const MyString& s )

  //  Concatenate operator, performs in-place on current object
  //
  //  s:  MyString to concatenate from
{
  char *prev_str;			// Old MyString array


  if ( size + s.size > m_size ) {	// Expand to fit new result?
    do {
      m_size *= 2;
    }
    while( size + s.size > m_size );

    prev_str = str;			// Save old MyString

    str = new char[ m_size + 1 ];	// Create new, larger MyString
    assert( str != NULL );

    strcpy( str, prev_str );		// Copy old MyString, delete it
    delete [] prev_str;
  }

  strcat( str, s.str );			// Concatenate new MyString
  size += s.size;
}					// End method operator+=


int MyString::operator!=( const MyString& s )

   //  Compare operator for MyStrings, return TRUE if MyStrings not equal,
   //  FALSE otherwise
   //
   //  s:  MyString to compare against
{
   return strcmp( str, s.str ) != 0;
}					// End operator !=


int MyString::operator<=( const MyString& s )

   //  Compare operator for MyStrings, returns TRUE if MyString <= s,
   //  FALSE otherwise
   //
   //  s:  MyString to compare against
{
   return strcmp( str, s.str ) <= 0;
}					// End operator <=


int MyString::operator>=( const MyString& s )

   //  Compare operator for MyStrings, returns TRUE if MyString >= s,
   //  FALSE otherwise
   //
   //  s:  MyString to compare against
{
   return strcmp( str, s.str ) >= 0;
}					// End operator >=


int MyString::operator<( const MyString& s )

   //  Compare operator for MyStrings, returns TRUE if MyString < s,
   //  FALSE otherwise
   //
   //  s:  MyString to compare against
{
   return strcmp( str, s.str ) < 0;
}					// End operator <


int MyString::operator>( const MyString& s )

   //  Compare operator for MyStrings, returns TRUE if MyString > s,
   //  FALSE otherwise
   //
   //  s:  MyString to compare against
{
   return strcmp( str, s.str ) > 0;
}					// End operator >


char& MyString::operator[]( int i )

   //  Indexing operator for MyStrings, return reference to given element
   //
   //  i:  Index into MyString
{
   if ( i < 0 || i >= size ) {
      cerr << "MyString::operator[], out of bounds\n";

      BOUNDS_ERR = 0;			// Reference to error character
      return BOUNDS_ERR;
   }

   return str[ i ];
}					// End operator []


MyString& MyString::operator=( const MyString& s )

   //  Assignment operator for MyStrings, copy from already existing object
   //
   //  s:  MyString object to copy from
{
   if ( s.size > m_size ) {		// Not enough space for old MyString?
      do {
         m_size *= 2;
      }
      while( s.size > m_size );

      delete [] str;
      str = new char[ m_size + 1 ];
      assert( str != NULL );
   }

   size = s.size;			// Copy instance variable values
   strcpy( str, s.str );

   return *this;
}					// End operator =


MyString MyString::operator+( const MyString& s )

   //  This method concatenates the given MyString onto our MyString
   //
   //  s:  MyString to concatenate
{
   MyString  ret_str;			// MyString object to return


   if ( size + s.size > ret_str.m_size ) {
      do {
         ret_str.m_size *= 2;
      }
      while( size + s.size > ret_str.m_size );

      delete [] ret_str.str;
      ret_str.str = new char[ ret_str.m_size + 1 ];
      assert( ret_str.str != NULL );
   }

   ret_str.str[ 0 ] = '\0';

   if ( size > 0 ) {
      strcpy( ret_str.str, str );	// Copy into MyString object
   }

   strcat( ret_str.str, s.str );
   ret_str.size = size + s.size;	// Set new MyString's length

   return ret_str;			// Return MyString object
}					// End operator +


MyString MyString::operator+( const char& c )

   //  This method concatenates the given character onto our MyString
   //
   //  c:  Character to concatenate
{
   char    arr[ 2 ];			// Array to hold char to concatenate
   MyString  ret_str;			// MyString object to return


   arr[ 0 ] = c;			// Create array with 1 char in it
   arr[ 1 ] = '\0';

   if ( size + 1 > ret_str.m_size ) {
      do {
         ret_str.m_size *= 2;
      }
      while( size + 1 > ret_str.m_size );

      delete [] ret_str.str;
      ret_str.str = new char[ ret_str.m_size + 1 ];
      assert( ret_str.str != NULL );
   }

   ret_str.str[ 0 ] = '\0';

   if ( size > 0 ) {
      strcpy( ret_str.str, str );	// Copy into MyString object
   }

   strcat( ret_str.str, arr );

   ret_str.size = size + 1;		// Set new MyString's length
   return ret_str;			// Return MyString object
}					// End operator +


MyString::operator char*() const

   //  This method provides automatic conversion from MyString to char *
{
   return str;
}					// End MyString to char * conversion


MyString::operator unsigned char*() const

   //  This method provides automatic conversion from MyString to
   //  unsigned char *
{
  return (unsigned char *) str;
}					// End MyString to unsigned char *


MyString::operator float() const

   //  This method provides automatic conversion from MyString to float
{
   return (float) atof( str );
}					// End MyString to float conversion


MyString::operator int() const

   //  This method provides automatic conversion from MyString to integer
{
   return atoi( str );
}					// End MyString to int conversion


MyString::MyString()

   //  Constructor for MyStrings, initialize instance variables to given size
{
   m_size = 32;

   str = new char[ m_size + 1 ];
   assert( str != NULL );

   str[ 0 ] = '\0';
   size = 0;
}					// End constructor method


MyString::MyString( const char *s )

   //  Constructor for MyStrings, cast from a character array to a MyString
   //
   //  s:  Character array to cast from
{
   m_size = 32;

   if ( !s ) {				// Assigning from NULL pointer
      str = new char[ m_size + 1 ];
      assert( str != NULL );

      str[ 0 ] = '\0';
      size = 0;

   } else {
      while( (int) strlen( s ) > m_size ) {
         m_size *= 2;
      }

      str = new char[ m_size + 1 ];
      assert( str != NULL );

      size = strlen( s );		// Copy instance variable values
      strcpy( str, s );
   }
}					// End constructor method


MyString::MyString( const char c )

   //  Constructor for MyStrings, cast from a character to a MyString
   //
   //  c:  Character to cast from
{
   m_size = 32;

   str = new char[ m_size + 1 ];	// Allocate memory for the MyString
   assert( str != NULL );

   str[ 0 ] = c;
   str[ 1 ] = '\0';

   size = 1;
}					// End constructor method


MyString::MyString( const MyString& s )

   //  Constructor for MyStrings, assign from already existing object
   //
   //  s:  MyString object to copy from
{
   m_size = s.m_size;

   str = new char[ m_size + 1 ];	// Allocate memory for the MyString
   assert( str != NULL );

   size = s.size;			// Copy instance variable values
   strcpy( str, s.str );
}					// End copy constructor


MyString::~MyString()

   //  Destructor for MyStrings, free any memory which may be allocated
{
   if ( str ) {				// Free memory, if it exists
      delete [] str;
   }
}					// End destructor method


ostream& operator<<( ostream& stream, const MyString& s )

   //  This function allows MyStrings to be used as part of an output stream
   //  redirection chain
   //
   //  stream:  Output stream to direct MyString
   //  s:       MyString to output
{
   return stream << s.str;
}					// End operator <<


istream& operator>>( istream& stream, MyString& s )

   //  This function allows MyStrings to be used as part of an input stream
   //  redirection chain
   //
   //  stream:  Input stream to direct into MyString
   //  s:       MyString to hold input
{
   char  tmp_str[ 1024 ];		// Temporary character array


   stream.getline( tmp_str, 1024 );	// Read one line of input

   s = tmp_str;
   return stream;
}					// End operator >>