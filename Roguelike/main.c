/**************/
/*  4�� ����  */
/* 2016722080 */
/*   �����   */
/**************/
#include <windows.h>

#include "prototype.h"

main( ) {
	// ��� �ܼ�â�� ũ�⸦ ���� 30, ���� 50 line ���� �����Ѵ�.
	system( "mode con:cols=50 lines=30" );

	// ��� �ܼ�â�� ������ �����Ѵ�.
	system( "title ������~Late Man~" );

	// GameManager.c �� startMenu() �Լ� ȣ��.
	startMenu( );
}