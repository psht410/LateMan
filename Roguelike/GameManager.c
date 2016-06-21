#include <conio.h>
#include <stdio.h>
#include <stdlib.h>		// abs, rand, malloc(free), exit, system 등에 필요.
#include <time.h>		// 난수 시드 값 조정에 필요.
#include <windows.h>	// 콘솔 폰트 설정, Sleep 함수에 필요.

// 본 스크립트에서 사용할 매크로 상수, enumerator, 구조체 및
// 함수의 프로토타입을 선언한 헤더 파일.
#include "prototype.h"

// 플레이어 객체, 적 객체의 좌표를 담을 구조체 인스턴스 선언.
POSITION playerPos = { 1, 1 };
POSITION enemyPos[10];

// 맵의 가로-세로 선언.
const int columns = 15;	// 세로 길이 (y axis).
const int rows = 15;	// 가로 길이 (x axis).

// 맵 타일을 저장할 2 차원 배열 선언.
char **board;

// PC, NPC 및 타일 등을 표시할 문자 선언.
const char chPlayer = '@';
const char chEnemy = 'e';
const char chItem = '$';
const char chNext = 'N';
const char chFloor = '.';
const char chTrap = 'X';
const char chOuterWall = '#';

// 선택된 메뉴임을 표시해주는 문자 선언.
const char isSelected = '>';

// 메인화면 드로잉과 각 레벨 구간의 역 이름을 배열 선언.
const char titleArt[10][50] = {
	"   __    ___  ______ ____  __  ___ ___    _  __",
	"  / /   / _ |/_  __// __/ /  |/  // _ |  / |/ /",
	" / /__ / __ | / /  / _/  / /|_/ // __ | /    / ",
	"/____//_/ |_|/_/  /___/ /_/  /_//_/ |_|/_/|_/  "
};
const char listMenu[3][20] = { "1. 게임시작", "2. 게임설명", "3. 게임종료" };
const char listStation[22][10] = {
	"신도림", "영등포", "신길", "대방", "노량진", "용산", "남영", "서울역",
	"시청", "종각",	"종로3가", "종로5가", "동대문", "동묘앞", "신설동",
	"제기동", "청량리", "회기", "외대앞", "신이문", "석계", "광운대"
};

// 인게임, 발생하는 충돌 이벤트에서 처리할 값을 선언.
const int timeEnemy = -10;
const int timeItem = 25;
const int timeWalk = -1;
const int timeTrap = -50;

// 마지막 Level 값을 가지고 있음.
const int maxStation = 22;

// 인게임, 정보를 계산하기 위해 선언.
int bfrTime;		// 충돌 이벤트가 일어나기 1프레임 직전의 '시간' 값.
int curTime;		// 현재 가진 '시간' 값.
int curStation;		// 현재 구간 레벨.
int curScore;		// 현재 점수.

int cntEnemy;		// 적 타일의 개수.
int cntItem;		// 아이템 타일의 개수.
int cntTrap;		// 함정 타일의 개수.

// 콘솔 폰트 색상 값을 담는 변수.
int colorStatus = LIGHT_WHITE;

// 현재 선택된 메뉴 값을 담는 변수.
int selectedMenu = 0;

// 두 구조체 객체 parameter 의 값을 합한 구조체 객체를 반환하는 함수.
POSITION posAdd( POSITION originPos, POSITION dirPos ) {
	POSITION newPos;

	newPos.x = originPos.x + dirPos.x;
	newPos.y = originPos.y + dirPos.y;

	return newPos;
}

// 메인메뉴 호출
startMenu( ) {
	clearScreen( );
	// 시작할 때 메뉴를 한번 그려줌.
	drawMenu( 0 );

	// 키보드 입력이 있을 때만 실행되며 조건에 맞춰 drawMenu 함수를 호출함.
	while ( 1 ) {
		if ( _kbhit( ) ) {
			clearScreen( );
			char key = _getch( );

			if ( key == -32 || key == 0xe0 || key == 0 || key == ENTER) 
				if(key != ENTER )
					key = _getch( );

			drawMenu( key );
		}
	}
}

// 메인메뉴 드로잉
drawMenu(int key ) {
	// 입력받은 키에 따라 메뉴의 상태를 바꾸거나 호출한다.
	switch ( key ) {
	case UP:
		selectedMenu--;
		if ( selectedMenu < 0 )
			selectedMenu = 2;
		break;
	case DOWN:
		selectedMenu++;
		if ( selectedMenu > 2 )
			selectedMenu = 0;
		break;
	case ENTER:
		// 선택한 메뉴를 호출함.
		goMenu( selectedMenu );
		break;
	}

	// 메인 타이틀 로고 드로잉 시작.
	SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), BACKGROUND_GREEN );
	printf( "%50s                     ", " " );
	SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), LIGHT_GREEN );
	printf( "~지각맨~" );
	SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), BACKGROUND_GREEN );
	printf( "                     %50s", " " );
	for ( int i = 0; i < 5; i++ ) {
		SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), LIGHT_GREEN );
		printf( " %s ", titleArt[i]);
		SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), BACKGROUND_GREEN );
		printf( "\n");
	}
	printf( "%50s                     ", " " );
	SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), LIGHT_GREEN );
	printf( "~지각맨~" );
	SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), BACKGROUND_GREEN );
	printf("                     %50s", " " );
	// 메인 타이틀 로고 드로잉 종료.
	
	// 메뉴 항목 드로잉 시작.
	for ( int i = 0; i < 3; i++ ) {
		int color = 0;
		switch ( i ) {
			case 0:
				color = LIGHT_JADE;
				break;
			case 1:
				color = LIGHT_YELLOW;
				break;
			case 2:
				color = LIGHT_RED;
				break;
		}
		SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), color );
		if ( selectedMenu == i )
			printf( "\n\n%c%30s\n", isSelected, listMenu[i] );
		else
			printf( "\n\n%30s\n", listMenu[i] );
	}
	// 메뉴 항목 드로잉 종료.
}

// 메뉴 이동.
goMenu( int menu ) {
	switch ( menu ) {
	case 0:
		startGame( );	// 게임 시작.
		break;
	case 1:
		drawManual( );	// 게임 설명.
		break;
	case 2:
		exit( 0 );		// 게임 종료.
		break;
	}
}

// 게임 설명 드로잉.
drawManual( ) {
	SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), LIGHT_WHITE );
	printf( "\n\n   당신은 상습적으로 지각을 하는 악질이다" );
	printf( "\n\n   지각 횟수가 쌓이고 쌓여....." );
	printf( "\n\n   한번 더 지각을 하면 기적의 " );

	SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), LIGHT_RED );
	printf( "F" );

	SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), LIGHT_WHITE );
	printf(" 학점을 받게된다. " );
	printf( "\n\n   때문에 오늘만큼은 " );

	SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), YELLOW );
	printf("절대 지각을 해선 안된다." );


	SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), LIGHT_YELLOW );
	printf( "\n\n\n\n   게임목표 : 시간을 수집하며 뺏기지 않고" );
	printf( "\n\n              제 시간까지 학교에 도착한다." );


	SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), LIGHT_GREEN );
	printf( "\n\n\n   메뉴 조작키 : 상/하 방향키, 엔터 키" );


	SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), LIGHT_JADE );
	printf( "\n\n\n   인게임 조작키 : 상하좌우 방향키, 종료 Q 키" );


	SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), LIGHT_RED );
	printf( "\n\n\n\n\n   아무 키나 누르면 메인화면으로 돌아갑니다...\n" );

	// 아무 키나 누르면 다음 line 으로 넘어감.
	_getch( );

	// 메인화면을 드로잉하기 전에 설명글을 지움.
	clearScreen( );
}

// 게임 시작.
startGame( ) {
	// 난수 시드값 교정.
	srand( time( NULL ) );	rand( );

	// 게임 초기화 작업.
	initGame( );

	// 키보드를 누를 때만 아래 블럭이 실행됨.
	while ( 1 ) {
		if ( _kbhit( ) ) {
			isKeyDown( );	// 누른 키를 검사 후 각 이벤트 호출.
			clearScreen( );	// 화면 지우기.

			// 맵, 스테이터스, 설명 순서대로 드로잉.
			drawBoard( );
			drawStatus( );
			drawExplain( );
		}
	}
}

// 게임 초기화.
initGame( ) {
	// 세로 길이 만큼 char 형 크기로 할당.
	board = ( char** )malloc( sizeof( char* ) * columns );

	// 가로 길이 만큼 char 형 크기로 각 세로 칸 마다 할당.
	for ( int i = 0; i < columns; i++ )
		board[i] = ( char* )malloc( sizeof( char ) * rows );

	// 인게임에서 사용될 값을 초기화.
	curTime = 100;
	curStation = 0;
	curScore = 0;
	bfrTime = curTime;

	colorStatus = LIGHT_WHITE;

	// 현재 레벨 구간의 맵을 생성.
	setupStation( );

	// 그려준다.
	drawBoard( );
	drawStatus( );
	drawExplain( );
}

// 현재 레벨 구간의 맵을 생성.
setupStation( ) {
	// 현재 레벨이 최대 레벨보다 높으면.
	if ( ++curStation > maxStation )
		// 다 깬거니까 트루엔딩 처리를 한다.
		checkIfGameOver( 1 );

	// 플레이어 ('@') 위치 초기화.
	playerPos.x = 1;
	playerPos.y = 1;

	// NPC 타일의 갯수를 레벨 디자인 값에 따라 초기화한다.
	cntEnemy = ( int )( curStation * .6 );
	cntTrap = ( int )( curStation * 1.0 );
	cntItem = ( int )( curStation * .5 );

	// 1차 레이어 생성 ( 땅, 벽 ).
	setupBoard( );

	// 2차 레이어 생성 ( NPC ).
	setupObject( chEnemy, cntEnemy );
	setupObject( chItem, cntItem );
	setupObject( chTrap, cntTrap );

	// 3차 레이어 생성 ( PC ).
	setupPlayer( );

	// 현재 구간의 레벨 값을 알려줌.
	drawLevel( );
}

// 1차 맵 레이어를 생성하며 땅, 벽, 탈출구 생성.
setupBoard( ) {
	// 가로 세로 모든 칸에 걸쳐 루프를 돈다.
	for ( int x = 0; x < columns; x++ ) {
		for ( int y = 0; y < rows; y++ ) {
			// 일단 땅('*')을 넣는다.
			board[x][y] = chFloor;

			// 만약 테두리면?
			if ( x == 0 || x == columns - 1 || y == 0 || y == rows - 1 )
				// 바깥 벽('#')을 넣는다.
				board[x][y] = chOuterWall;
		}
	}

	// 오른쪽 구석은 항상 다음 칸으로 넘어가는 타일.
	board[columns - 2][rows - 2] = chNext;
}

// 넘겨받은 오브젝트와 횟수만큼 랜덤하게 생성.
setupObject( char what, int count ) {
	int r = 0;	// 가로.
	int c = 0;	// 세로.

	for ( int i = 0; i < count; i++ ) {
		do {
			// 테두리에서 한 칸 떨어진 곳 이외에만 값이 잡힘.
			r = ( rand( ) % ( rows - 1 ) ) + 1;
			c = ( rand( ) % ( columns - 1 ) ) + 1;

			// 생성될 자리에 다른 오브젝트가 있으면 다시 난수를 생성.
		} while ( board[c][r] != '.' );

		// 생성할 오브젝트가 '적'이라면 각각의 구조체 객체에 좌표를 저장.
		if ( what == chEnemy ) {
			enemyPos[i].x = r;
			enemyPos[i].y = c;
		}

		// 맵에 삽입.
		board[c][r] = what;
	}
}

// 플레이어의 위치를 세팅.
setupPlayer( ) {
	board[playerPos.y][playerPos.x] = chPlayer;
}

// 맵을 드로잉.
drawBoard( ) {
	for ( int i = 0; i < columns; i++ ) {
		printf( "\n\t" );
		for ( int j = 0; j < rows; j++ ) {
			// 현재 그릴 맵 타일에 따라 폰트 색상을 다르게 함.
			switch ( board[i][j] ) {
			case '@':
				SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), LIGHT_GREEN );
				break;
			case 'e':
				SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), LIGHT_RED );
				break;
			case '$':
				SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), LIGHT_YELLOW );
				break;
			case 'X':
				SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), LIGHT_PURPLE );
				break;
			case '.':
				SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), LIGHT_WHITE );
				break;
			case 'N':
				SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), BACKGROUND_GREEN );
				break;
			case '#':
				SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), LIGHT_JADE );
				break;
			}
			printf( "%c ", board[i][j] );
		}
	}
}

// 현재 스테이터스를 드로잉.
drawStatus( ) {
	// 충돌 이벤트에 따라 색상이 달라짐.
	SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), colorStatus );

	// '시간'이 증감되었다면?
	if ( colorStatus != LIGHT_WHITE )
		// 변동 값을 함께 출력.
		printf( "\n\n\t남은 시간 : %d -> %d 분 ( %d 분 ) ", bfrTime, curTime, curTime - bfrTime );
	else
		printf( "\n\n\t남은 시간 : %d 분 ", curTime );

	// 현재 구간의 역 이름과 남은 레벨 및 점수 등의 정보를 표시.
	printf( "\n\n\t현재 구간 : %s, %d 정거장 남음", listStation[curStation-1], maxStation - curStation );
	printf( "\n\n\t현재 점수 : %d 점", curScore );
}

// 게임 정보를 드로잉.
drawExplain( ) {
	SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), LIGHT_GREEN );
	printf( "\n\n\n\t플레이어: %c, 다음 역: %c, 종료: 'Q'키", chPlayer, chNext );
	SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), LIGHT_RED );
	printf( "\n\n\t잡 상인: %c, %d 분", chEnemy, timeEnemy );
	SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), LIGHT_YELLOW );
	printf( "\n\t시   간: %c, +%d 분", chItem, timeItem );
	SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), LIGHT_PURPLE );
	printf( "\n\t남의 땀: %c, %d 분\n", chTrap, timeTrap );
	SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), LIGHT_WHITE );
}

// 구간 넘어갈 때마다 호출되며, 현재 스테이터스를 드로잉함.
drawLevel( ) {
	clearScreen( );

	// 현재 스테이터스를 드로잉.
	drawStatus( );
	// 1.5 초간 대기.
	Sleep( 1500 );

	clearScreen( );
}

// 플레이어의 이동 방향을 지시.
movePlayer( int key ) {
	POSITION dir = { 0, 0 };

	// 입력된 방향키에 따라 축 값을 증감함.
	switch ( key ) {
	case LEFT:
		dir.x = -1;
		break;
	case RIGHT:
		dir.x = 1;
		break;
	case UP:
		dir.y = -1;
		break;
	case DOWN:
		dir.y = 1;
		break;
	}

	// 플레이어 객체를, 현재 위치와 갈 방향을 함께 moveObject 함수에 넘겨 호출함.
	// 맨 끝의 parameter 값은 인덱스 값이므로 배열 객체를 넘길 때만 사용함.
	moveObject( chPlayer, playerPos, dir, 0);
}

// 적의 이동 방향을 지시.
moveEnemy( ) {
	// 맵에 존재하는 모든 적의 개수만큼 반복.
	for ( int i = 0; i < cntEnemy; i++ ) {
		POSITION dir = { 0, 0 };

		// x, y 축의 절대값을 비교한다.
		if ( abs( playerPos.x - enemyPos[i].x ) > abs( playerPos.y - enemyPos[i].y ) )
			// 가로로 이동해야함 ( x 차이의 절대값이 더 크므로 가로로 더 멂 ).
			dir.x = ( playerPos.x < enemyPos[i].x ) ? -1 : 1;
		else
			// 세로로 이동해야함 ( y 차이의 절대값이 더 크므로 세로로 더 멂 ).
			dir.y = ( playerPos.y < enemyPos[i].y ) ? -1 : 1;

		// 움직일 확률은 현재 레벨(역 구간) * 3 % 확률.
		if ( rand( ) % 100 < curStation*3 )
			// 적 객체를, 현재 위치와 갈 방향 및 현재 적의 인덱스를 함께 moveObject 함수에 넘겨 호출함.
			moveObject( chEnemy, enemyPos[i], dir, i );
	}
}

// parameter 값으로 받은 오브젝트의 정보를 처리하여 이동하는 함수.
moveObject( char what, POSITION originPos, POSITION dirPos, int index ) {
	// 원래 위치와 방향을 더해 새 위치 값을 만듦.
	POSITION newPos = posAdd(originPos, dirPos);
	
	// 이동 가능 불가능 플래그.
	int canMove = 0;

	// 새 위치의 맵 타일을 검사함.
	switch ( board[newPos.y][newPos.x] ) {
		case '.':	// 땅.
			canMove = 1;
			// 플레이어가 땅과 충돌하면 '시간' 갱신.
			if ( what == chPlayer )
				updateTime( timeWalk );
			break;
		case '#':	// 벽.
			canMove = 0;
			// 플레이어가 벽과 충돌하면 '시간' 유지.
			if ( what == chPlayer )
				updateTime( 0 );
			break;
		case 'X':	// 함정.
			canMove = 0;
			// 플레이어가 함정에 충돌하면 '시간' 갱신.
			if ( what == chPlayer ) {
				// 플레이어는 함정 타일을 통과 가능.
				canMove = 1;
				updateTime( timeTrap );
			}
			break;
		case 'e':	// 적.
			canMove = 0;
			// 플레이어가 적에게 충돌하면 '시간' 갱신.
			if ( what == chPlayer )
				updateTime( timeEnemy );
			break;
		case '$':	// 아이템.
			canMove = 1;
			// 플레이어가 아이템에 충돌하면 '시간' 갱신.
			if ( what == chPlayer )
				updateTime( timeItem );
			break;
		case 'N':	// 다음 레벨.
			// 다음 레벨 구간을 로딩함.
			setupStation( );
			break;
		case '@':	// 플레이어.
			// 적이 플레이어에게 충돌하면 '시간' 갱신.
			if ( what == chEnemy )
				updateTime( timeEnemy );
			break;
	}
	
	// 움직일 수 있는 타일이라면, 새 위치로 이동하며 원래 타일은 '땅'이 된다.
	if ( canMove ) {
		board[newPos.y][newPos.x] = what;
		board[originPos.y][originPos.x] = chFloor;
	}

	// 플레이어 또는 적이 움직였다면 위치 변수 값을 갱신한다.
	if ( what == chPlayer && canMove)
		playerPos = newPos;
	if ( what == chEnemy && canMove ) 
		enemyPos[index] = newPos;
}

// 넘겨받은 값 만큼 '시간'을 갱신함.
updateTime(int time ) {
	// 갱신되기 전 '시간' 값을 저장.
	bfrTime = curTime;
	// '시간' 값을 갱신.
	curTime += time;
	// 넘겨받은 값의 절대값을 점수로 쌓음.
	curScore += abs( time );

	// 적이나 함정과 충돌했다면.
	if ( time == timeEnemy || time == timeTrap )
		colorStatus = LIGHT_RED;
	// 아이템을 먹은 경우.
	else if ( time == timeItem )
		colorStatus = LIGHT_GREEN;
	// 그냥 걸어감.
	else
		colorStatus = LIGHT_WHITE;

	// 배드엔딩인지 검사함.
	checkIfGameOver( 0 );
}

// 게임오버인지 검사함.
checkIfGameOver( int isTrueEnding ) {
	// '시간'을 다 써 배드엔딩이거나, 트루엔딩일 때 실행.
	if ( curTime <= 0 || isTrueEnding ) {
		// 사인을 저장할 문자열 공간 선언.
		char *cause = NULL;

		// 변동값이 함정의 '시간'값 이상이라면?
		if ( bfrTime >= curTime + abs( timeTrap ) )
			cause = "누군가의 땀을 밟고 실족하여\n\n\t\t병원에 갔다.";
			//strcpy( cause, "누군가의 땀을 밟고 실족하여\n\n\t\t병원에 갔다." );
		// 아니면, 변동값이 적의 '시간'값 이상이라면?
		else if ( bfrTime >= curTime + abs( timeEnemy ) )
			cause = "잡상인한테 걸렸다.";
			//strcpy( cause, "잡상인한테 걸렸다." );
		// 그냥 걸어가다 죽음.
		else
			cause = "늦장부려서";
			//strcpy( cause, "늦장부려서" );

		// 사인을 넘기며 결과를 드로잉.
		drawResult( isTrueEnding, cause );

		// 트루엔딩은 이 분기로 오지 않음 ( 바로 startMenu() 로 덤프됨 ).
		char key = _getch( );

		// 입력한 키가 '재시도 키'면 게임을 다시 시작.
		if ( key == 'r' || key == 'R' ) {
			clearScreen( );
			startGame( );
		}
		// 아니면 2 초후 메인화면으로 돌아감.
		else {
			printf( "\n\n\t재시도 키 이외 다른 키가 입력되어" );
			printf( "\n\n\t메인화면으로 돌아갑니다.\n\t" );

			Sleep( 2000 );

			startMenu( );
		}
	}
}

drawResult( int isTrueEnding, char* cause ) {
	// 게임을 클리어했다면 트루엔딩 값이 1 임.
	if ( isTrueEnding ) {
		SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), LIGHT_JADE );
		puts( "\n\n\n==================================================" );
		puts( "   축하합니다! 당신은 간신히 낙제를 면했습니다.   " );
		puts( "==================================================" );
		SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), LIGHT_WHITE );
		printf( "\n\t점수 : %d\n", curScore );

		printf( "\n\n\t아무 키나 누르면 메인화면으로 돌아갑니다.\n\t" );

		_getch( );

		startMenu( );
	}
	// 아니면 배드엔딩.
	else {
		SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), LIGHT_RED );
		puts( "\n\n\n==================================================" );
		puts( "  결국 F 를 받았다. 재시도는 'R' 을 입력해주세요.   " );
		puts( "==================================================" );
		SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), LIGHT_WHITE );
		printf( "\n\t늦은 이유 : %s\n", cause );
		printf( "\n\t마지막 정차 : %s, %d / %d\n", listStation[curStation - 1], curStation, maxStation );
		printf( "\n\t마지막 점수 : %d\n", curScore );
	}
}

// 인게임에서 사용될 입력 루틴.
isKeyDown( ) {
	char key = _getch( );

	// 입력한 키가 방향키라면 특수키이므로 이를 검사함.
	if ( key == -32 || key == 0xe0 || key == 0 ) {
		// 실제 방향키 아스키코드 값이 들어가는 부분.
		key = _getch( );

		// 플레이어도 움직이고.
		movePlayer( key );
		// 적도 움직인다.
		moveEnemy( );
	}

	// 만약 종료키인 'q'('Q') 를 눌렀다면 게임 종료.
	if ( key == 'q' || key == 'Q' )
		exit( 0 );
}

// 콘솔 화면 지우는 함수.
clearScreen( ) {
	system( "cls" );
}