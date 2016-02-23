#ifndef __REGEXP_H__
#define __REGEXP_H__

#ifdef __GNUC__
#define __T(x)      x
#define _T(x)      x

typedef char TCHAR, CHAR;
typedef unsigned char TBYTE ;
typedef unsigned char (* PTBYTE);
typedef CHAR (*PCHAR),(*PTCHAR), (*LPCH), (*PCH);
typedef const CHAR (*LPCCH), (*PCCH);

typedef  CHAR (*NPSTR), (*LPSTR), (*PSTR);
typedef  PSTR (*PZPSTR);
typedef  const PSTR (*PCZPSTR);
typedef  const CHAR (*LPCSTR),  (*PCSTR);
typedef  const PCSTR  (*PZPCSTR);
typedef unsigned char   _TUCHAR;

typedef LPSTR LPTCH, PTCH;
typedef LPSTR PTSTR, LPTSTR, PUTSTR, LPUTSTR;
typedef LPCSTR PCTSTR, LPCTSTR, PCUTSTR, LPCUTSTR;


#define _tcscat     strcat
#define _tcscat_s   strcat_s
#define _tcscpy     strcpy
#define _tcscpy_s   strcpy_s
#define _tcsdup     _strdup
#define _tcslen     strlen
#define _tcsnlen    strnlen
#define _tcsxfrm    strxfrm
#define _tcsxfrm_l    _strxfrm_l
#define _tcserror   strerror
#define _tcserror_s   strerror_s
#define __tcserror  _strerror
#define __tcserror_s  _strerror_s

#define _istalpha isalpha
#define _tcsstr strstr 
#define _tcschr strchr
#define _tcsncmp strncmp
#define _tcsspn strspn
#define _tcscspn strcspn
#define _alloca  malloc
#define _tcsncpy strncpy
#else
#include <tchar.h>
#endif

typedef std::basic_string<TCHAR>	CString;

class regexp;

class Regexp
{
public:
	enum { NSUBEXP = 10 };

	Regexp();
	Regexp( LPCTSTR exp, BOOL iCase = 0 );
	Regexp( const Regexp &r );
	~Regexp();
	const Regexp & operator=( const Regexp & r );

	bool Match( const TCHAR * s );
	int SubStrings() const;
	
	const CString operator[]( unsigned int i ) const;
	int SubStart( unsigned int i ) const;
	int SubLength( unsigned int i ) const;

	CString GetReplaceString( LPCTSTR source ) const;

	CString GetErrorString() const;
	bool CompiledOK() const;

#if defined( _RE_DEBUG )
	void Dump();
#endif
private:
	regexp * rc;
	mutable CString m_szError;
	const TCHAR * string;	/* used to return substring offsets only */

	void ClearErrorString() const;
	int safeIndex( unsigned int i ) const;

};

#endif

