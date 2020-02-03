#include "stdafx.h"
#include "detours/detours.h"

#ifdef _X64
#pragma comment(lib,"detours/lib.X64/detours.lib")
#else
#pragma comment(lib,"detours/lib.X86/detours.lib")
#endif 

typedef DWORD( WINAPI *fnMessageBox )( DWORD , TCHAR* , TCHAR* , DWORD );

fnMessageBox g_pSrcMessageBoxW ;

DWORD WINAPI MyMessageBox( HWND hWnd , TCHAR* pText , TCHAR* pTitle , DWORD type ) 
{
	g_pSrcMessageBoxW( 0 , L"�ڵ����MessageBox�е����˿�" , L"��ʾ" , 0 );
	return 0;
}

LPVOID hookFunction( LPVOID pSrcFcuntion , LPVOID pDesFcuntion ) {

	// 
	DetourTransactionBegin( );
	DetourUpdateThread( GetCurrentThread( ) );

	DetourAttach( &pSrcFcuntion , pDesFcuntion );

	if( DetourTransactionCommit( ) == NO_ERROR )
		return pSrcFcuntion;
	return NULL;
}



int _tmain(int argc, _TCHAR* argv[])
{
	MessageBoxW( 0 , L"���溯��" , L"��ʾ" , 0 );


	g_pSrcMessageBoxW = 
		(fnMessageBox)hookFunction( &MessageBoxW , &MyMessageBox );

	MessageBoxW( 0 , L"���溯��" , L"��ʾ" , 0 );


	DetourTransactionBegin( );
	DetourUpdateThread( GetCurrentThread( ) );

	DetourDetach( (LPVOID*)&g_pSrcMessageBoxW , &MyMessageBox );
	DetourTransactionCommit( );
	
	MessageBoxW( 0 , L"���溯��" , L"��ʾ" , 0 );


	return 0;
}

