//---------------------------------------------------------------------
// Copyleft olibre@Lmap.org
// This C header file is licenced under two licences, 
// choose the one you prefer:
// - MPL v2 or later
// - GPL v3 or later 
//---------------------------------------------------------------------

#ifndef __STRING_OPERATIONS_H_INCLUDED__
#define __STRING_OPERATIONS_H_INCLUDED__

#include "basics/verify.h"

// inline functions

//---------------------------------------------------------------------
template <size_t LENGTH> inline
bool isSame( const char (&buffer1)[LENGTH], const char *buffer2 )
{
  return ! memcmp( buffer1, buffer2, LENGTH );
}

//---------------------------------------------------------------------
template <size_t LENGTH> inline
size_t copy( char (&dst)[LENGTH], const char *src )
{
  memcpy( dst, src, LENGTH );
  return LENGTH;
}

//---------------------------------------------------------------------
inline size_t copy( char &dst, const char src )
{
  dst = src;
  return 1;
}

//---------------------------------------------------------------------
#define COPY( dst, src ) ({ VERIFY(sizeof(dst)==sizeof(src)); memcpy( &dst, src, sizeof(src) ); })

//---------------------------------------------------------------------
template <size_t LEN> inline
void fill( char (&buffer)[LEN], char filler )
{
  memset( buffer, filler, LEN );
}

//---------------------------------------------------------------------
template <int LENGTH> inline
std::string getString( const char (&buffer)[LENGTH] )
{
  return std::string( buffer, LENGTH );
}

//---------------------------------------------------------------------
template <int LENGTH> inline
std::string trim( const char (&buffer)[LENGTH] )
{
  const char* ptr = &buffer[LENGTH-1];
  for( int len=LENGTH, i=len-1; i>=0; len=i--, --ptr )
  {
    if( *ptr != ' ' && *ptr != 0 )  //trim
      return std::string( buffer, len );
  }
  return std::string(); //LENGTH=0 or buffer filled solely with spaces and/or null chars
}

//---------------------------------------------------------------------
// Write Char Array
template <size_t LENGTH> inline
void wca( std::ostream& log, const char (&char_array)[LENGTH] )
{
  log.rdbuf()->sputn( char_array, LENGTH );
}

//---------------------------------------------------------------------
// Write a struct composed solely of Char Arrays
#define WCA( log, structure )  log.rdbuf()->sputn( (const char*)&(structure), sizeof(structure) )

//---------------------------------------------------------------------
inline
void setAndBlank( char *dst, size_t LENDST, const char *src, size_t LENSRC )
{
  //do not copy null terminated char
  size_t lensrc = src[LENSRC-1] ? LENSRC : LENSRC-1;

  if (lensrc < LENDST)
  {
    memcpy( dst, src, lensrc );
    memset( dst+lensrc, ' ', LENDST - lensrc );
  }
  else
  {
    memcpy( dst, src, LENDST );
  }
}

//---------------------------------------------------------------------
inline
void setAndBlank( char *dst, size_t LENDST, const char* src )
{
  setAndBlank( dst, LENDST, src, strlen(src) );
}

//---------------------------------------------------------------------
template <size_t LENSRC> inline
void setAndBlank( char *dst, size_t LENDST, const char (&src)[LENSRC] )
{
  setAndBlank( dst, LENDST, src, LENSRC );
}

//---------------------------------------------------------------------
template <size_t LENDST> inline
void setAndBlank( char (&dst)[LENDST], const char *src, size_t LENSRC )
{
  setAndBlank( dst, LENDST, src, LENSRC );
}

//---------------------------------------------------------------------
template <size_t LENDST, size_t LENSRC> inline
void setAndBlank( char (&dst)[LENDST], const char (&src)[LENSRC] )
{
  setAndBlank( dst, LENDST, src, LENSRC );
}

//---------------------------------------------------------------------
template <size_t LEN> inline
void blank( char (&buffer)[LEN] )
{
  memset( buffer, ' ', LEN );
}

//---------------------------------------------------------------------
inline
void blank( void *buffer, size_t LEN )
{
  memset( buffer, ' ', LEN );
}

//---------------------------------------------------------------------
#define BLANK(structure) blank( &(structure), sizeof(structure) )

//----------------------------------------------------------------------------------------------
inline
int asciiToUnsigned( const char *ascii, const int LENGTH )
{
  int out = 0;
  const char* ptr = ascii + LENGTH-1;
  int count = LENGTH-1;
  for (int mult = 1; ; mult *= 10)
  {
    VERIFY( *ptr - '0' >= 0 );
    VERIFY( *ptr - '0' <= 9 );
    out += (*ptr-- - '0') * mult;
    if( count-- == 0 ) return out;
  }
}

//----------------------------------------------------------------------------------------------
template <int LENGTH> inline
int asciiToUnsigned( const char (&ascii)[LENGTH] )
{
  return asciiToUnsigned( ascii, LENGTH );
}

/*----------------------------------------------------------------------------------------------
template<int LENGTH> inline
int asciiToSigned( const char (&ascii)[LENGTH] )
{
  if ( ascii[0] == '+' )
    return asciiToUnsigned( &ascii[1], LENGTH-1 );
  
  VERIFY( ascii[0] == '-' );
  int out = 0;
  const char* ptr = ascii + LENGTH-1;
  int count = LENGTH-1;
  for (int mult = 1; ; mult *= 10)
  {      
    VERIFY( *ptr - '0' >= 0 );
    VERIFY( *ptr - '0' <= 9 );
    out -= (*ptr - '0') * mult;
    if (--count <= 0) return out;
    --ptr;      
  }
}*/

//----------------------------------------------------------------------------------------------
template <int LENGTH> inline
int asciiToSigned( const char (&ascii)[LENGTH], int firstSignificativeDigit = 1 )
{
  for( int i = 1; i < firstSignificativeDigit; ++i )
    VERIFY( ascii[i] == '0' );

  if ( ascii[0] == '+' )
    return asciiToUnsigned( &ascii[firstSignificativeDigit], LENGTH - firstSignificativeDigit );
  else
    VERIFY( ascii[0] == '-' );

  int out = 0;
  const char* ptr = ascii + LENGTH-1; //start from last char
  int count = LENGTH-1 - firstSignificativeDigit;
  for( int mult = 1; ; mult *= 10 )
  {      
    VERIFY( *ptr - '0' >= 0 );
    VERIFY( *ptr - '0' <= 9 );
    out -= (*ptr-- - '0') * mult;
    if( count-- == 0 ) return out;
  }
}
#define LEN_PRICE     9
#define MAX_PRICE    +999999999
#define MIN_PRICE    -999999999

#define LEN_QUANTITY  7
#define MAX_QUANTITY +9999999
#define MIN_QUANTITY -9999999

//----------------------------------------------------------------------------------------------
// Price can be negative (this is the default)
inline
int asciiToPrc( const char (&ascii)[LEN_PRICE] )
{
  VERIFY( MAX_PRICE == - MIN_PRICE );
  VERIFY( MAX_PRICE == 999999999 ); // 9 significative digits
  int firstSignificativeDigit = LEN_PRICE - 9;
  int prc = asciiToSigned( ascii, firstSignificativeDigit );
  VERIFY( prc == asciiToSigned( ascii ));
  return prc;
}

//----------------------------------------------------------------------------------------------
// Quantity can be negative (S = Signed)
inline
int asciiToQtyS( const char (&ascii)[QUANTITY_LEN], const int MARGIN = 0 )
{
  VERIFY( MAX_QUANTITY == - MIN_QUANTITY );
  VERIFY( MAX_QUANTITY == 9999999 ); // 7 significative digits
  const int firstSignificativeDigit = LEN_QUANTITY - 7 - MARGIN; //-1 => security margin
  const int qty = asciiToSigned( ascii, firstSignificativeDigit );
  VERIFY( qty == asciiToSigned( ascii ));
  return qty;
}

//----------------------------------------------------------------------------------------------
// Price must be positive (U = Unsigned)
inline
int asciiToPrcU( const char (&ascii)[LEN_PRICE] )
{
  VERIFY( MAX_PRICE_LIM == - MIN_PRICE );
  VERIFY( MAX_PRICE_LIM == 999999999 ); // 9 significative digits
  const int significativeDigits = 9;
  const int firstDigit          = LEN_PRICE - significativeDigits;
  VERIFY( firstDigit > 0 );
  VERIFY( ascii[0] == '+' );
  const int prc = asciiToUnsigned( &ascii[firstDigit], significativeDigits );
  VERIFY( prc == asciiToSigned( ascii ));
  return prc;
}

//----------------------------------------------------------------------------------------------
// Quantity must be positive (this is the default)
inline
int asciiToQty( const char (&ascii)[LEN_QUANTITY], const int MARGIN = 0 )
{
  VERIFY( MAX_QUANTITY == - MIN_QUANTITY );
  VERIFY( MAX_QUANTITY == 9999999 ); // 7 significative digits
  const int significativeDigits = 7 + MARGIN; //+1 => security margin
  const int firstDigit          = LEN_QUANTITY- significativeDigits;
  VERIFY( firstDigit > 0 );
  VERIFY( ascii[0] == '+' );
  const int qty = asciiToUnsigned( &ascii[firstDigit], significativeDigits );
  VERIFY( qty == asciiToSigned( ascii ));
  return qty;
}

//----------------------------------------------------------------------------------------------
template <int LENGTH> inline
void resetSigned( char (&ascii)[LENGTH] )
{
  ascii[0] = '+';
  int count = LENGTH-2;
  char* ptr = &ascii[1]; //start from last char
  for(;;)
  {      
    *ptr++ = '0';
    if( count-- == 0 ) return;
  }
}

//----------------------------------------------------------------------------------------------
template <int LENGTH> inline
void resetUnsigned( char (&ascii)[LENGTH] )
{
  int count = LENGTH-1;
  char* ptr = ascii; //start from last char
  for(;;)
  {      
    *ptr++ = '0';
    if( count-- == 0 ) return;
  }
}

//----------------------------------------------------------------------------------------------
inline
double decimalPosition( const char pos[2] )
{
  VERIFY( pos[0] == '0' );
  switch( pos[1] )
  {
  case '0': return         1.0;
  case '1': return         0.1;
  case '2': return        0.01;
  case '3': return       0.001;
  case '4': return      0.0001;
  case '5': return     0.00001;
  default:  VERIFY(0);
  case '6': return    0.000001;
//case '7': return   0.0000001;
//case '8': return  0.00000001;
//case '9': return 0.000000001;
  }
}

#endif
