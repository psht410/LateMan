#pragma once

// 매크로 상수 선언: 방향키 아스키 코드 값.
#define LEFT   75      // 좌측방향키.
#define RIGHT  77      // 우측방향키.
#define UP     72      // 위쪽방향키.
#define DOWN   80      // 아래방향키.
#define ENTER  13      // 엔터키.

// 열거형 데이터 선언: 색상 값.
enum FontColor {
	BLACK, BLUE, GREEN, JADE, RED, PURPLE, YELLOW, WHITE, GRAY,
	LIGHT_BLUE, LIGHT_GREEN, LIGHT_JADE, LIGHT_RED, LIGHT_PURPLE, LIGHT_YELLOW, LIGHT_WHITE
};

// 좌표 x, y를 저장할 POSITION 이름의 구조체 선언.
typedef struct _POSITION {
	short x;
	short y;
}POSITION;

// 이하 함수 프로토타입 선언.
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