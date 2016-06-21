/**************/
/*  4차 과제  */
/* 2016722080 */
/*   허상훈   */
/**************/
#include <windows.h>

#include "prototype.h"

main( ) {
	// 출력 콘솔창의 크기를 가로 30, 세로 50 line 으로 설정한다.
	system( "mode con:cols=50 lines=30" );

	// 출력 콘솔창의 제목을 설정한다.
	system( "title 지각맨~Late Man~" );

	// GameManager.c 의 startMenu() 함수 호출.
	startMenu( );
}