
#pragma once

#ifndef _WINDEF_

#pragma pack(1)

// -------------------------------------------------------------------------

#ifdef __cplusplus
#	undef  NULL
#	define NULL    0
#else
#	ifndef NULL
#	define NULL    ((void *)0)
#	endif
#endif

#ifndef FALSE
#define FALSE	0
#endif

#ifndef TRUE
#define TRUE	1
#endif

#undef far
#undef near

#define far
#define near

#ifndef CDECL
#define CDECL	__cdecl
#endif

#ifndef PASCAL
#define PASCAL	__stdcall
#endif

#undef FAR
#undef NEAR
#define FAR                 far
#define NEAR                near

#ifndef CONST
#define CONST               const
#endif

#ifndef MAX_PATH
#define MAX_PATH			1024
#endif

#ifndef IN
#define IN
#endif

#ifndef OUT
#define OUT
#endif

// -------------------------------------------------------------------------

#ifndef BASETYPES
#define BASETYPES
typedef unsigned long ULONG;
typedef unsigned long SIZE_T;
typedef ULONG *PULONG;
typedef unsigned short USHORT;
typedef USHORT *PUSHORT;
typedef unsigned char UCHAR;
typedef UCHAR *PUCHAR;
typedef char *PSZ;
#endif  /* !BASETYPES */

#ifndef VOID
#define VOID void
typedef char CHAR;
typedef short SHORT;
typedef long LONG;
typedef LONG *PLONG;
#endif

typedef void* PVOID;
typedef unsigned short VARTYPE;
typedef short VARIANT_BOOL;
typedef long SCODE;


typedef unsigned int        DWORD;
typedef int                 BOOL;
typedef unsigned char       BYTE;
typedef unsigned short      WORD;
typedef float               FLOAT;
typedef double				DOUBLE;
typedef FLOAT               *PFLOAT;
typedef BOOL near           *PBOOL;
typedef BOOL far            *LPBOOL;
typedef BYTE near           *PBYTE;
typedef BYTE far            *LPBYTE;
typedef int near            *PINT;
typedef int far             *LPINT;
typedef WORD near           *PWORD;
typedef WORD far            *LPWORD;
typedef long far            *LPLONG;
typedef DWORD near          *PDWORD;
typedef DWORD far           *LPDWORD;
typedef void far            *LPVOID;
typedef CONST void far      *LPCVOID;

typedef int   		        INT;
typedef int					INT32;
typedef unsigned int        UINT;
typedef unsigned int        UINT32;
typedef unsigned int        *PUINT;
typedef unsigned long long	ULONGLONG;
typedef unsigned long long	UINT64;
typedef long long			LONGLONG;
typedef DWORD				LCID;

typedef unsigned long long	UINT_PTR;
typedef long long			INT_PTR;

typedef DWORD   COLORREF;
typedef DWORD*	LPCOLORREF;

#ifndef _HRESULT_DEFINED
#define _HRESULT_DEFINED
typedef LONG HRESULT;
#endif // !_HRESULT_DEFINED

#define MAKEWORD(a, b)      ((WORD)(((BYTE)(a)) | ((WORD)((BYTE)(b))) << 8))
#define MAKELONG(a, b)      ((LONG)(((WORD)(a)) | ((DWORD)((WORD)(b))) << 16))
#define LOWORD(l)           ((WORD)(l))
#define HIWORD(l)           ((WORD)(((DWORD)(l) >> 16) & 0xFFFF))
#define LOBYTE(w)           ((BYTE)(w))
#define HIBYTE(w)           ((BYTE)(((WORD)(w) >> 8) & 0xFF))

// -------------------------------------------------------------------------
#include <wchar.h>

typedef wchar_t WCHAR;

typedef WCHAR*	PWCHAR;
typedef WCHAR*	LPWCH, *PWCH;
typedef WCHAR*	LPWSTR, *PWSTR;
typedef CONST WCHAR* LPCWCH, *PCWCH;
typedef CONST WCHAR* LPCWSTR, *PCWSTR;

typedef CHAR *PCHAR;
typedef CHAR *LPCH, *PCH;

typedef CONST CHAR *LPCCH, *PCCH;
typedef CHAR *NPSTR;
typedef CHAR *LPSTR, *PSTR;
typedef CONST CHAR *LPCSTR, *PCSTR;

// --> Neutral ANSI/UNICODE types and macros
#ifdef  UNICODE

#ifndef _TCHAR_DEFINED
#define _TCHAR_DEFINED
typedef WCHAR TCHAR, *PTCHAR;
typedef WCHAR TBYTE, *PTBYTE;
#endif

typedef LPWSTR LPTCH, PTCH;
typedef LPWSTR PTSTR, LPTSTR;
typedef LPCWSTR LPCTSTR;
typedef LPWSTR LP;
#define __TEXT(quote) L##quote

#else   /* UNICODE */

#ifndef _TCHAR_DEFINED
#define _TCHAR_DEFINED
typedef char TCHAR, *PTCHAR;
typedef unsigned char TBYTE, *PTBYTE ;
#endif /* !_TCHAR_DEFINED */

typedef LPSTR LPTCH, PTCH;
typedef LPSTR PTSTR, LPTSTR;
typedef LPCSTR LPCTSTR;
#define __TEXT(quote) quote

#endif /* UNICODE */

#define TEXT(quote) __TEXT(quote)

// LLG_
#define strcmpi strcasecmp
#define stricmp strcasecmp

#define GUID_DEFINED

typedef struct _GUID {          // size is 16
    DWORD Data1;
    WORD   Data2;
    WORD   Data3;
    BYTE  Data4[8];
} GUID;

// -------------------------------------------------------------------------

typedef struct tagRECT
{
    LONG    left;
    LONG    top;
    LONG    right;
    LONG    bottom;
} RECT, *PRECT, NEAR *NPRECT, FAR *LPRECT;

typedef const RECT FAR* LPCRECT;

typedef struct tagPOINT
{
    LONG  x;
    LONG  y;
} POINT, *PPOINT, NEAR *NPPOINT, FAR *LPPOINT;

typedef struct tagSIZE
{
    LONG        cx;
    LONG        cy;
} SIZE, *PSIZE, *LPSIZE;

typedef union _LARGE_INTEGER
{
	struct {
		DWORD LowPart; 
		LONG  HighPart; 
	};
	LONGLONG QuadPart;
} LARGE_INTEGER, *PLARGE_INTEGER; 

typedef union _ULARGE_INTEGER
{
	struct {
		DWORD LowPart; 
		DWORD HighPart;
	};
	ULONGLONG QuadPart;
} ULARGE_INTEGER, *PULARGE_INTEGER;

/* real definition that makes the C++ compiler happy */
typedef struct tagDEC {
    USHORT wReserved;
    union {
        struct {
            BYTE scale;
            BYTE sign;
        };
        USHORT signscale;
    };
    ULONG Hi32;
    union {
        struct {
#ifdef _MAC
            ULONG Mid32;
            ULONG Lo32;
#else
            ULONG Lo32;
            ULONG Mid32;
#endif
        };
        ULONGLONG Lo64;
    };
} DECIMAL;

// -------------------------------------------------------------------------

#ifndef DECLARE_HANDLE
#ifdef STRICT
	typedef void* HANDLE;
	#define DECLARE_HANDLE(name) struct name##__ { int unused; }; typedef struct name##__ *name
#else
	typedef void* HANDLE;
	#define DECLARE_HANDLE(name) typedef HANDLE name
#endif
	typedef HANDLE* PHANDLE;
#endif

#if !defined(__HMODULE)
#define __HMODULE
	DECLARE_HANDLE(HINSTANCE);
	typedef HINSTANCE HMODULE;	/* HMODULEs can be used in place of HINSTANCEs */
#endif

#ifndef HWND
	#define HWND unsigned long
#endif

#endif

//#define VIETNAM
#ifdef VIETNAM
#define LANG_001  "Nhận được %d  %s"
#define LANG_002  "Không thể ngồi thiền!"
#define LANG_003  "Không thể ngồi thiền!"
#define LANG_004  "Đang ngồi thiền!"
#define LANG_005  "Không thể ngồi thiền"
#define LANG_006  "Túi đầy!"
#define LANG_007  "Không thể nhận thêm vàng"
#define LANG_008  "VIP%d mới được dùng tính năng này"
#define LANG_009  "Hãy tăng đến cấp 51 đã"
#define LANG_010  "%s (Hạng %d)"
#define LANG_011  "[Đế Thiên Thần Hoàng]%s[Tộc:%s]"
#define LANG_012  "<color=green>%s hiến cho tộc "
#define LANG_013  "%d trân châu,"
#define LANG_014  "%d phỉ thúy,"
#define LANG_015  "%d mã não,"
#define LANG_016  "%d kim cương"
#define LANG_017  ", nhận %d độ cống hiến!"
#define LANG_018  "Trong ngày rời tộc không thể gia nhập tộc mới"
#define LANG_019  "Túi không đủ chỗ"
#define LANG_020  "KNB không đủ"
#define LANG_021  "Bạc không đủ"
#define LANG_022  "Chiến hồn không đủ"
#define LANG_023  "Cống hiến không đủ"
#define LANG_024  "Danh vọng không đủ"
#define LANG_025  "Phong Thần"
#define LANG_026  "Phách không đủ"
#define LANG_027  "Chân khí không đủ"
#define LANG_028  "Bạn đã mua %d %s"
#define LANG_029  "Thiết bị không thể được bán！"
#define LANG_030  "bạn bị <color=231,209,21>\\b%s\\b@<color=235,123,49> đánh bại, xuống hạng %d, <color=green>\\bNhấp vào đấu trường\\b*"
#define LANG_031  "Tộc trưởng"
#define LANG_032  "T.Anh"
#define LANG_033  "Tộc nhân"
#define LANG_034  "Hạng đã thay đổi, khiêu chiến không hiệu lực!"
#define LANG_035  "Bản đồ này chưa mở!"

#define LANG_036  "Hãy chọn đối tượng chat"
#define LANG_037  "[%s] đã rời mạng hoặc không tồn tại"
#define LANG_038  "Không thể chat với chính mình"
#define LANG_039  "Bản đồ chưa mở, xin chờ"
#define LANG_040  "Bạn đã bị cấm chat"
#define LANG_041  "Cấp 10 mới mở kênh chat thế giới"
#define LANG_042  "Chúc mừng đã hoàn thành tu tiên, xin hãy nhận thưởng đính kèm!"
#define LANG_043  "%s是精英，不能踢出"
#define LANG_044  "系统"
#define LANG_045  "当前地图不能进行组队操作!"
#define LANG_046  "无家族"

#else
#define LANG_001  "你获得了 %d 个 %s"
#define LANG_002  "战斗地图中不可打坐!"
#define LANG_003  "泳池中不可打坐!"
#define LANG_004  "当前已在打坐中!"
#define LANG_005  "当前状态不可打坐，请先停止操作玩家后再打坐!"
#define LANG_006  "背包已满, 无法获得物品！"
#define LANG_007  "无法获得更多金钱：数额超出限制！"
#define LANG_008  "VIP%d才能使用此功能"
#define LANG_009  "请先升到51级方可进入"
#define LANG_010  "%s(第%d名)"
#define LANG_011  "[帝天神皇]%s[家族:%s]"
#define LANG_012  "<Color=green>%s 为本族捐献了"
#define LANG_013  "%d珍珠,"
#define LANG_014  "%d翡翠,"
#define LANG_015  "%d玛瑙,"
#define LANG_016  "%d钻石"
#define LANG_017  ", 获得了%d贡献度, 大家鼓掌！"
#define LANG_018  "退出家族当天不能加入新的家族"
#define LANG_019  "主背包空间不够,请清理背包"
#define LANG_020  "元宝不够"
#define LANG_021  "银两不够"
#define LANG_022  "战魂不够"
#define LANG_023  "贡献度不够"
#define LANG_024  "声望不够"
#define LANG_025  "封神之竞"
#define LANG_026  "精魄不够"
#define LANG_027  "真气不够"
#define LANG_028  "你购买了 %d 个 %s"
#define LANG_029  "装备不能出售！"
#define LANG_030  "你在竞技场中被<color=231,209,21>\\b%s\\b@<color=235,123,49>击败, 排名降低%d名, <color=green>\\b点击进入竞技场\\b*"
#define LANG_031  "族长"
#define LANG_032  "精英"
#define LANG_033  "族人"
#define LANG_034  "该角色排名已产生变化！此次挑战无效！"
#define LANG_035  "该地图尚未开放，敬请期待!"

#define LANG_036  "请指定私聊对象"
#define LANG_037  "玩家[%s]已下线或者不存在"
#define LANG_038  "不能与自己私聊"
#define LANG_039  "该地图尚未开放，敬请期待!"
#define LANG_040  "你被禁言了，不能发言"
#define LANG_041  "10级开启世界频道发言"
#define LANG_042  "恭喜您完成了修仙！附件为修仙奖励，请查收!"
#define LANG_043  "%s是精英，不能踢出"
#define LANG_044  "系统"
#define LANG_045  "当前地图不能进行组队操作!"
#define LANG_046  "无家族"

#endif
#pragma pack()
