#pragma once

// ��ũ�� ��� ����: ����Ű �ƽ�Ű �ڵ� ��.
#define LEFT   75      // ��������Ű.
#define RIGHT  77      // ��������Ű.
#define UP     72      // ���ʹ���Ű.
#define DOWN   80      // �Ʒ�����Ű.
#define ENTER  13      // ����Ű.

// ������ ������ ����: ���� ��.
enum FontColor {
	BLACK, BLUE, GREEN, JADE, RED, PURPLE, YELLOW, WHITE, GRAY,
	LIGHT_BLUE, LIGHT_GREEN, LIGHT_JADE, LIGHT_RED, LIGHT_PURPLE, LIGHT_YELLOW, LIGHT_WHITE
};

// ��ǥ x, y�� ������ POSITION �̸��� ����ü ����.
typedef struct _POSITION {
	short x;
	short y;
}POSITION;

// ���� �Լ� ������Ÿ�� ����.
POSITION posAdd( POSITION originPos, POSITION dirPos );
drawBoard( );
drawExplain( );
drawLevel( );
drawMenu( int key );
drawManual( );
drawResult( int isTrueEnding, char* cause );
drawStatus( );
checkIfGameOver( int isTrueEnding );
clearScreen( );
goMenu( int menu );
initGame( );
isKeyDown( );
movePlayer( int key );
moveEnemy( );
moveObject( char what, POSITION originPos, POSITION dirPos, int index );
setupBoard( );
setupStation( );
setupObject( char what, int count );
setupPlayer( );
startMenu( );
startGame( );
updateTime( int time );