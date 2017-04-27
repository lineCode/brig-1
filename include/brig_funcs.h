/*
 * Brig - a ligthweight C++ GUI framework
 *
 *
 * Copyright 2017 Alexander S.Kresin <alex@kresin.ru>
 * www - http://www.kresin.ru
 */

#ifndef BRIG_FUNCS_H_
#define BRIG_FUNCS_H_

#include "brig_defs.h"

int brig_Main( int argc, char *argv[] );

extern BRIG_HANDLE brig_InitMainWindow( PBRIG_CHAR lpAppName, PBRIG_CHAR lpTitle,
      int x, int y, int width, int height, long int lStyle,
      PBRIG_ICON hIcon, PBRIG_BRUSH hBrush );
extern void brig_ActivateMainWindow( int bShow, HACCEL hAcceler, int iMaxMin );
extern BRIG_HANDLE brig_InitDialog( PBRIG_CHAR lpTitle,
      int x, int y, int width, int height, long int lStyle );
extern void brig_ActivateDialog( BRIG_HANDLE handle, bool bModal );
extern void brig_CloseWindow( BRIG_HANDLE handle );

extern bool brig_CheckBtnGet( BRIG_HANDLE handle );
extern void brig_CheckBtnSet( BRIG_HANDLE handle, bool bValue );
extern bool brig_RadioBtnGet( BRIG_HANDLE handle );
extern void brig_RadioBtnSet( BRIG_HANDLE handle, bool bValue );

extern void brig_MsgInfo( PBRIG_CHAR sText, PBRIG_CHAR sTitle );
extern void brig_MsgStop( PBRIG_CHAR sText, PBRIG_CHAR sTitle );
extern int brig_MsgOkCancel( PBRIG_CHAR sText, PBRIG_CHAR sTitle );
extern int brig_MsgYesNo( PBRIG_CHAR sText, PBRIG_CHAR sTitle );

extern void brig_writelog( const char * sFile, const char * sTraceMsg, ... );

extern void brig_SetWindowText( BRIG_HANDLE handle, PBRIG_CHAR lpTitle );
extern PBRIG_CHAR brig_GetWindowText( BRIG_HANDLE handle );
extern void brig_MoveWindow( BRIG_HANDLE handle, int iLeft, int iTop, int iWidth, int iHeight );
extern void brig_EnableWindow( BRIG_HANDLE handle, bool bEnable );
extern void brig_ShowWindow( BRIG_HANDLE handle, bool bShow );
extern BRIG_HANDLE brig_GetActiveWindow( void );
extern BRIG_HANDLE brig_SetFocus( BRIG_HANDLE handle );
extern BRIG_HANDLE brig_GetFocus( void );
extern void brig_SetTopmost( BRIG_HANDLE handle );
extern void brig_RemoveTopmost( BRIG_HANDLE handle );

extern void brig_RedrawWindow( BRIG_HANDLE handle );
extern void brig_DeleteObject( PBRIG_BRUSH pBrush );
extern void brig_DeleteObject( PBRIG_PEN pPen );
extern void brig_DeleteObject( PBRIG_FONT pFont );
extern PBRIG_BRUSH brig_SelectObject( PBRIG_DC hDC, PBRIG_BRUSH pBrush );
extern PBRIG_PEN brig_SelectObject( PBRIG_DC hDC, PBRIG_PEN pPen );
extern PBRIG_FONT brig_SelectObject( PBRIG_DC hDC, PBRIG_FONT pFont );

extern PBRIG_BRUSH brig_CreateBrush( long int lColor );
extern bool brig_CompareBrush( PBRIG_BRUSH pBrush, long int lColor );
extern PBRIG_PEN brig_CreatePen( int iWidth = 1, long int lColor = 0, int iStyle = 0 );
extern bool brig_ComparePen( PBRIG_PEN pPen, int iWidth, long int lColor, int iStyle );
extern PBRIG_FONT brig_CreateFont( PBRIG_CHAR fontName, int fnHeight, int fnWeight,
               DWORD fdwCharSet, DWORD fdwItalic, DWORD fdwUnderline, DWORD fdwStrikeOut  );
extern long int brig_ChooseColor( long int lDefColor = -1 );

extern PBRIG_PPS brig_BeginPaint( BRIG_HANDLE handle );
extern void brig_EndPaint( BRIG_HANDLE handle, PBRIG_PPS pps );
extern void brig_GetClientRect( BRIG_HANDLE handle, RECT *prc );
extern void brig_FillRect( PBRIG_DC hDC, int iLeft, int iTop, int iRight, int iBottom, PBRIG_BRUSH hBrush );
extern void brig_DrawRect( PBRIG_DC hDC, int iLeft, int iTop, int iRight, int iBottom );
extern void brig_moveto( PBRIG_DC hDC, int iLeft, int iTop );
extern void brig_lineto( PBRIG_DC hDC, int iLeft, int iTop );
extern void brig_Ellipse( PBRIG_DC hDC, int iLeft, int iTop, int iRight, int iBottom );

extern void brig_SetFont( BRIG_HANDLE handle, PBRIG_FONT pFont );
extern long int brig_SetTextColor( PBRIG_DC hDC, long int lColor );
extern long int brig_SetBkColor( PBRIG_DC hDC, long int lColor );
extern void brig_SetTransparentMode( PBRIG_DC hDC, bool bTransp );
extern int brig_DrawText( PBRIG_DC hDC, PBRIG_CHAR lpText, int x1, int y1, int x2, int y2, unsigned int uiFormat );

extern void brig_DrawGradient( PBRIG_DC hDC, int x1, int y1, int x2, int y2, int type,
      vector<long> *pArrColor = NULL, vector<double> *pArrStop = NULL, vector<int> *pArrRadius = NULL );

extern BRIG_HMENU brig_CreateMenu( void );
extern BRIG_HMENU brig_CreatePopupMenu( void );
extern int brig_SetMenu( BRIG_HANDLE hWnd, BRIG_HMENU hMenu );

#if defined( BRIG_OS_WIN )
extern wchar_t * brig_strToWC( UINT cp, char * szText );
extern char * brig_WCTostr( UINT cp, wchar_t * wcText, int uilen );
#endif

#if defined( UNICODE )
#define brig_str2WC( szText )             brig_strToWC( CP_UTF8, szText )
#define brig_WC2str( szText, uiLen )      brig_WCTostr( CP_UTF8, szText, uiLen )
#else
extern PBRIG_WCHAR brig_str2WC( PBRIG_CHAR szText );
extern PBRIG_CHAR brig_WC2str( PBRIG_WCHAR wcText, int uilen );
#endif
extern void brig_free( PBRIG_WCHAR wcString );

#endif  // BRIG_FUNCS_H_
