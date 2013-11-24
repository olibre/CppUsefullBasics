//---------------------------------------------------------------------
// Copyleft olibre@Lmap.org
// This C header file is licenced under two licences, 
// choose the one you prefer:
// - MPL v2 or later
// - GPL v3 or later 
//---------------------------------------------------------------------

#ifndef __BASICS_PRINT_H__
#define __BASICS_PRINT_H__

// --------------------------------------------------------------------------------------------------------------------------------------
inline void writeSeconds (char a[], int sec)
{
  assert( a[0] == '8' );
  assert( a[1] == '6' );
  assert( a[2] == '4' );
  assert( a[3] == '0' );
  assert( a[4] == '0' );

  assert( sec >= 0     ); 
  assert( sec <= 99999 ); 

  int r = sec;  a[0] = '0' + r/10000;  assert(r/10000 >= 0);  assert(r/10000 <= 9);
  r %= 10000;   a[1] = '0' + r/1000;   assert(r/1000  >= 0);  assert(r/1000  <= 9);
  r %= 1000;    a[2] = '0' + r/100;    assert(r/100   >= 0);  assert(r/100   <= 9);
  r %= 100;     a[3] = '0' + r/10;     assert(r/10    >= 0);  assert(r/10    <= 9);
  r %= 10;      a[4] = '0' + r/1;      assert(r/1     >= 0);  assert(r/1     <= 9);  
}

// --------------------------------------------------------------------------------------------------------------------------------------
inline void printSec (std::ofstream& errors_, int sec)
{
  assert( sec >= 0     ); 
  assert( sec <= 99999 ); 

  int r = sec;  errors_.rdbuf()->sputc ('0' + r/10000);  assert(r/10000 >= 0);  assert(r/10000 <= 9);
  r %= 10000;   errors_.rdbuf()->sputc ('0' + r/1000 );  assert(r/1000  >= 0);  assert(r/1000  <= 9);
  r %= 1000;    errors_.rdbuf()->sputc ('0' + r/100  );  assert(r/100   >= 0);  assert(r/100   <= 9);
  r %= 100;     errors_.rdbuf()->sputc ('0' + r/10   );  assert(r/10    >= 0);  assert(r/10    <= 9);
  r %= 10;      errors_.rdbuf()->sputc ('0' + r/1    );  assert(r/1     >= 0);  assert(r/1     <= 9);  
}

// --------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
inline void printAny (std::ofstream& errors_, T any)
{
  FID_UTILITIES::StrStream strstr;
  strstr << any; //convert to string
  errors_.rdbuf()->sputn (strstr.c_str(), strstr.length());
}

// --------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
inline void printAnyJson (std::ofstream& json, T any)
{
  printAny (json, any);
}

// --------------------------------------------------------------------------------------------------------------------------------------
inline void printAnyJson (std::ofstream& json, const std::string str)
{
  json.rdbuf()->sputc ('"');
  json.rdbuf()->sputn (str.c_str(), str.length());
  json.rdbuf()->sputc ('"');
}

// --------------------------------------------------------------------------------------------------------------------------------------
template <size_t LENGTH>
inline void printStr (std::ofstream& errors_, const char (&str) [LENGTH] )
{
  errors_.rdbuf()->sputn (str, LENGTH-1);
}

// --------------------------------------------------------------------------------------------------------------------------------------
template <size_t LENGTH>
inline void printStr (std::ofstream* f1, std::ofstream* f2, const char (&str) [LENGTH] )
{
  if (f1) f1->rdbuf()->sputn (str, LENGTH-1);
  if (f2) f2->rdbuf()->sputn (str, LENGTH-1);
}

// --------------------------------------------------------------------------------------------------------------------------------------
//                0  0  1            2
//            012345678901234 56 78901       
//char a[] = "{ /*hh:mm:ss*/ \"t\":86400, ..blabla..";
inline void writeHHMMSS (char a[], int sec)
{
  assert( a[0] == 'h' );
  assert( a[1] == 'h' );
  assert( a[2] == ':' );
  assert( a[3] == 'm' );
  assert( a[4] == 'm' );
  assert( a[5] == ':' );
  assert( a[6] == 's' );
  assert( a[7] == 's' );

  int hh = sec / 3600;
  int mm = sec / 60;
  int ss = sec - 60*mm;
  mm -= 60*hh;

  int h = hh/10;  a[0]='0'+h;  a[1]='0'+hh%10;  assert(h>=0); assert(h<=9); assert(hh%10>=0); assert(hh%10<=9);
  int m = mm/10;  a[3]='0'+m;  a[4]='0'+mm%10;  assert(m>=0); assert(m<=9); assert(mm%10>=0); assert(mm%10<=9);
  int s = ss/10;  a[6]='0'+s;  a[7]='0'+ss%10;  assert(s>=0); assert(s<=9); assert(ss%10>=0); assert(ss%10<=9);
}

// --------------------------------------------------------------------------------------------------------------------------------------
//            012345678901234556789901
//char a[] = "{ /*hh:mm:ss*/ \"t\":86400, ";
inline void writeTimestamps( char a[], int sec )
{
  writeHHMMSS(  a+ 4, sec );
  writeSeconds( a+19, sec );
}

// --------------------------------------------------------------------------------------------------------------------------------------
// str must be a string, not a pointer
// e.g.: sputnUnsigned( ofstream, "ascii=", 10 );
#define SPUTN_UNSIGNED( f, str, value )                                \
do {                                                                   \
  if (value <= 0)                                                      \
  {                                                                    \
    f->rdbuf()->sputn (str "0", sizeof(str));                          \
    if (value < 0)                                                     \
    {                                                                  \
      std::cerr <<"ERROR unexpected negative number: " str << value <<'\n';\
      breakpoint();                                                    \
    }                                                                  \
  }                                                                    \
  else                                                                 \
  {                                                                    \
    size_t pos = sizeof(str)-1;                                        \
    char   buf[] = str "........"; /*concatenation at compile time*/   \
    size_t n = MAG::Tools::convert_unsigned_base10 (value, buf+pos);   \
    assert( pos+n < sizeof(buf) );                                     \
    f->rdbuf()->sputn( buf, pos+n );                                   \
  }                                                                    \
} while(0)

// ----------------------------------------------------------------------------
inline void printSecUsec (std::ofstream& errors_, long long usec)
{
  long long rest = usec;  
  errors_.rdbuf()->sputc ('0' + rest/1000/1000/10000);  rest %= 1000*1000*10000;
  errors_.rdbuf()->sputc ('0' + rest/1000/1000/1000 );  rest %= 1000*1000*1000;
  errors_.rdbuf()->sputc ('0' + rest/1000/1000/100  );  rest %= 1000*1000*100;
  errors_.rdbuf()->sputc ('0' + rest/1000/1000/10   );  rest %= 1000*1000*10;
  errors_.rdbuf()->sputc ('0' + rest/1000/1000/1    );  rest %= 1000*1000*1;
  errors_.rdbuf()->sputc ('.');
  errors_.rdbuf()->sputc ('0' + rest/1000/100       );  rest %= 1000*100;
  errors_.rdbuf()->sputc ('0' + rest/1000/10        );  rest %= 1000*10;
  errors_.rdbuf()->sputc ('0' + rest/1000/1         );  rest %= 1000*1;
  errors_.rdbuf()->sputc ('0' + rest/100            );  rest %= 100;
  errors_.rdbuf()->sputc ('0' + rest/10             );  rest %= 10;
  errors_.rdbuf()->sputc ('0' + rest/1              );
}

// ----------------------------------------------------------------------------
inline void printXX (std::ofstream& errors_, int xx)
{
  assert (0 < xx);
  assert (    xx < 100);

  if (xx > 9)
  {
    int d = xx / 10;
    errors_.rdbuf()->sputc ('0' + d);
    errors_.rdbuf()->sputc ('0' + xx - 10*d);
  }
  else
  { 
    errors_.rdbuf()->sputc ('0' + xx);
  }
}

#endif
