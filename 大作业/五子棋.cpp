/*#include<iostream>
using namespace std;
void board()
{
	int i,j;
	char a[37][38];
	for(i=0;i<37;i++)
	for(j=0;j<37;j+=2)
	a[i][j]=197;
	for(i=0;i<37;i++)
	for(j=1;j<36;j+=2)
		a[i][j]=196;
	for(i=0;i<37;i++)
	a[i][37]=0;
	for(i=0;i<37;i++)
	{
		for(j=0;j<38;j++)
	cout<<a[i][j];cout<<endl;
	}
}
void s()
{
	char s[2];
	s[0]=157;
	s[1]=0;
	cout<<s<<endl;
}
int main()
{
    s();
    return 0;
}*/

#include <stdio.h> 
#include"bios.h"
#include <ctype.h> 
#include <conio.h> 
#include <dos.h> 
#define CROSSRU 0xbf /*右上角点*/ 
#define CROSSLU 0xda /*左上角点*/ 
#define CROSSLD 0xc0 /*左下角点*/ 
#define CROSSRD 0xd9 /*右下角点*/ 
#define CROSSL 0xc3 /*左边*/ 
#define CROSSR 0xb4 /*右边*/ 
#define CROSSU 0xc2 /*上边*/ 
#define CROSSD 0xc1 /*下边*/ 
#define CROSS 0xc5 /*十字交叉点*/ 

/*定义棋盘左上角点在屏幕上的位置*/ 
#define MAPXOFT 5 
#define MAPYOFT 2 

/*定义1号玩家的操作键键码*/ 
#define PLAY1UP 0x1157/*上移--'W'*/ 
#define PLAY1DOWN 0x1f53/*下移--'S'*/ 
#define PLAY1LEFT 0x1e41/*左移--'A'*/ 
#define PLAY1RIGHT 0x2044/*右移--'D'*/ 
#define PLAY1DO 0x3920/*落子--空格键*/ 

/*定义2号玩家的操作键键码*/ 
#define PLAY2UP 0x4800/*上移--方向键up*/ 
#define PLAY2DOWN 0x5000/*下移--方向键down*/ 
#define PLAY2LEFT 0x4b00/*左移--方向键left*/ 
#define PLAY2RIGHT 0x4d00/*右移--方向键right*/ 
#define PLAY2DO 0x1c0d/*落子--回车键Enter*/ 

/*若想在游戏中途退出, 可按 Esc 键*/ 
#define ESCAPE 0x011b 

/*定义棋盘上交叉点的状态, 即该点有无棋子 */ 
/*若有棋子, 还应能指出是哪个玩家的棋子 */ 
#define CHESSNULL 0 /*没有棋子*/ 
#define CHESS1 'O'/*一号玩家的棋子*/ 
#define CHESS2 'X'/*二号玩家的棋子*/ 

/*定义按键类别*/ 
#define KEYEXIT 0/*退出键*/ 
#define KEYFALLCHESS 1/*落子键*/ 
#define KEYMOVECURSOR 2/*光标移动键*/ 
#define KEYINVALID 3/*无效键*/ 

/*定义符号常量: 真, 假 --- 真为1, 假为0 */ 
#define TRUE 1 
#define FALSE 0 

/**********************************************************/ 
/* 定义数据结构 */ 

/*棋盘交叉点坐标的数据结构*/ 
struct point 
{ 
int x,y; 
}; 

/**********************************************************/ 
/*自定义函数原型说明 */ 
void Init(void); 
int GetKey(void); 
int CheckKey(int press); 
int ChangeOrder(void); 
int ChessGo(int Order,struct point Cursor); 
void DoError(void); 
void DoOK(void); 
void DoWin(int Order); 
void MoveCursor(int Order,int press); 
void DrawCross(int x,int y); 
void DrawMap(void); 
int JudgeWin(int Order,struct point Cursor); 
int JudgeWinLine(int Order,struct point Cursor,int direction); 
void ShowOrderMsg(int Order); 
void EndGame(void); 
/**********************************************************/ 

/**********************************************************/ 
/* 定义全局变量 */ 
int gPlayOrder; /*指示当前行棋方 */ 
struct point gCursor; /*光标在棋盘上的位置 */ 
char gChessBoard[19][19];/*用于记录棋盘上各点的状态*/ 
/**********************************************************/ 

/**********************************************************/ 
/*主函数*/ 
void main() 
{ 
int press; 
int bOutWhile=FALSE;/*退出循环标志*/ 

Init();/*初始化图象，数据*/ 

while(1) 
{ 
press=GetKey();/*获取用户的按键值*/ 
switch(CheckKey(press))/*判断按键类别*/ 
{ 
/*是退出键*/ 
case KEYEXIT: 
clrscr();/*清屏*/ 
bOutWhile = TRUE; 
break; 

/*是落子键*/ 
case KEYFALLCHESS: 
if(ChessGo(gPlayOrder,gCursor)==FALSE)/*走棋*/ 
DoError();/*落子错误*/ 
else 
{ 
DoOK();/*落子正确*/ 

/*如果当前行棋方赢棋*/ 
if(JudgeWin(gPlayOrder,gCursor)==TRUE) 
{ 
DoWin(gPlayOrder); 
bOutWhile = TRUE;/*退出循环标志置为真*/ 
} 
/*否则*/ 
else 
/*交换行棋方*/ 
ChangeOrder(); 
ShowOrderMsg(gPlayOrder); 
} 
break; 

/*是光标移动键*/ 
case KEYMOVECURSOR: 
MoveCursor(gPlayOrder,press); 
break; 

/*是无效键*/ 
case KEYINVALID: 
break; 
} 

if(bOutWhile==TRUE) 
break; 
} 

/*游戏结束*/ 
EndGame(); 
} 
/**********************************************************/ 

/*界面初始化，数据初始化*/ 
void Init(void) 
{ 
int i,j; 
char *Msg[]= 
{ 
"Player1 key:", 
" UP----w", 
" DOWN--s", 
" LEFT--a", 
" RIGHT-d", 
" DO----space", 
"", 
"Player2 key:", 
" UP----up", 
" DOWN--down", 
" LEFT--left", 
" RIGHT-right", 
" DO----ENTER", 
"", 
"exit game:", 
" ESC", 
NULL, 
}; 

/* 先手方为1号玩家 */ 
gPlayOrder = CHESS1; 
/* 棋盘数据清零, 即棋盘上各点开始的时候都没有棋子 */ 
for(i=0;i<19;i++) 
for(j=0;j<19;j++) 
gChessBoard[i][j]=CHESSNULL; 
/*光标初始位置*/ 
gCursor.x=gCursor.y=0; 

/*画棋盘*/ 
textmode(C40); 
DrawMap(); 

/*显示操作键说明*/ 
i=0; 
textcolor(BROWN); 
while(Msg[i]!=NULL) 
{ 
gotoxy(25,3+i); 
cputs(Msg[i]); 
i++; 
} 

/*显示当前行棋方*/ 
ShowOrderMsg(gPlayOrder); 
/*光标移至棋盘的左上角点处*/ 
gotoxy(gCursor.x+MAPXOFT,gCursor.y+MAPYOFT); 
} 

/*画棋盘*/ 
void DrawMap(void) 
{ 
int i,j; 

clrscr(); 

for(i=0;i<19;i++) 
for(j=0;j<19;j++) 
DrawCross(i,j); 

} 

/*画棋盘上的交叉点*/ 
void DrawCross(int x,int y) 
{ 
gotoxy(x+MAPXOFT,y+MAPYOFT); 
/*交叉点上是一号玩家的棋子*/ 
if(gChessBoard[x][y]==CHESS1) 
{ 
textcolor(LIGHTBLUE); 
putch(CHESS1); 
return; 
} 
/*交叉点上是二号玩家的棋子*/ 
if(gChessBoard[x][y]==CHESS2) 
{ 
textcolor(LIGHTRED); 
putch(CHESS2); 
return; 
} 

textcolor(GREEN); 

/*左上角交叉点*/ 
if(x==0&&y==0) 
{ 
putch(CROSSLU); 
return; 
} 

/*左下角交叉点*/ 
if(x==0&&y==18) 
{ 
putch(CROSSLD); 
return; 
} 

/*右上角交叉点*/ 
if(x==18&&y==0) 
{ 
putch(CROSSRU); 
return; 
} 

/*右下角交叉点*/ 
if(x==18&&y==18) 
{ 
putch(CROSSRD); 
return; 
} 

/*左边界交叉点*/ 
if(x==0) 
{ 
putch(CROSSL); 
return; 
} 

/*右边界交叉点*/ 
if(x==18) 
{ 
putch(CROSSR); 
return; 
} 

/*上边界交叉点*/ 
if(y==0) 
{ 
putch(CROSSU); 
return; 
} 

/*下边界交叉点*/ 
if(y==18) 
{ 
putch(CROSSD); 
return; 
} 

/*棋盘中间的交叉点*/ 
putch(CROSS); 
} 

/*交换行棋方*/ 
int ChangeOrder(void) 
{ 
if(gPlayOrder==CHESS1) 
gPlayOrder=CHESS2; 
else 
gPlayOrder=CHESS1; 

return(gPlayOrder); 
} 

/*获取按键值*/ 
int GetKey(void) 
{ 
char lowbyte; 
int press; 

while (bioskey(1) == 0) 
;/*如果用户没有按键，空循环*/ 

press=bioskey(0); 
lowbyte=press&0xff; 
press=press&0xff00 + toupper(lowbyte); 
return(press); 
} 

/*落子错误处理*/ 
void DoError(void) 
{ 
sound(1200); 
delay(50); 
nosound(); 
} 

/*赢棋处理*/ 
void DoWin(int Order) 
{ 
sound(1500);delay(100); 
sound(0); delay(50); 
sound(800); delay(100); 
sound(0); delay(50); 
sound(1500);delay(100); 
sound(0); delay(50); 
sound(800); delay(100); 
sound(0); delay(50); 
nosound(); 

textcolor(RED+BLINK); 
gotoxy(25,20); 
if(Order==CHESS1) 
cputs("PLAYER1 WIN!"); 
else 
cputs("PLAYER2 WIN!"); 
gotoxy(25,21); 
cputs("\n"); 
getch(); 
} 

/*走棋*/ 
int ChessGo(int Order,struct point Cursor) 
{ 
/*判断交叉点上有无棋子*/ 
if(gChessBoard[Cursor.x][Cursor.y]==CHESSNULL) 
{ 
/*若没有棋子, 则可以落子*/ 
gotoxy(Cursor.x+MAPXOFT,Cursor.y+MAPYOFT); 
textcolor(LIGHTBLUE); 
putch(Order); 
gotoxy(Cursor.x+MAPXOFT,Cursor.y+MAPYOFT); 
gChessBoard[Cursor.x][Cursor.y]=Order; 
return TRUE; 
} 
else 
return FALSE; 
} 

/*判断当前行棋方落子后是否赢棋*/ 
int JudgeWin(int Order,struct point Cursor) 
{ 
int i; 
for(i=0;i<4;i++) 
/*判断在指定方向上是否有连续5个行棋方的棋子*/ 
if(JudgeWinLine(Order,Cursor,i)) 
return TRUE; 
return FALSE; 
} 

/*判断在指定方向上是否有连续5个行棋方的棋子*/ 
int JudgeWinLine(int Order,struct point Cursor,int direction) 
{ 
int i; 
struct point pos,dpos; 
const int testnum = 5; 
int count; 

switch(direction) 
{ 
case 0:/*在水平方向*/ 
pos.x=Cursor.x-(testnum-1); 
pos.y=Cursor.y; 
dpos.x=1; 
dpos.y=0; 
break; 
case 1:/*在垂直方向*/ 
pos.x=Cursor.x; 
pos.y=Cursor.y-(testnum-1); 
dpos.x=0; 
dpos.y=1; 
break; 
case 2:/*在左下至右上的斜方向*/ 
pos.x=Cursor.x-(testnum-1); 
pos.y=Cursor.y+(testnum-1); 
dpos.x=1; 
dpos.y=-1; 
break; 
case 3:/*在左上至右下的斜方向*/ 
pos.x=Cursor.x-(testnum-1); 
pos.y=Cursor.y-(testnum-1); 
dpos.x=1; 
dpos.y=1; 
break; 
} 

count=0; 
for(i=0;i<testnum*2+1;i++)/*????????i<testnum*2-1*/ 
{ 
if(pos.x>=0&&pos.x<=18&&pos.y>=0&&pos.y<=18) 
{ 
if(gChessBoard[pos.x][pos.y]==Order) 
{ 
count++; 
if(count>=testnum) 
return TRUE; 
} 
else 
count=0; 
} 
pos.x+=dpos.x; 
pos.y+=dpos.y; 
} 

return FALSE; 
} 

/*移动光标*/ 
void MoveCursor(int Order,int press) 
{ 
switch(press) 
{ 
case PLAY1UP: 
if(Order==CHESS1&&gCursor.y>0) 
gCursor.y--; 
break; 
case PLAY1DOWN: 
if(Order==CHESS1&&gCursor.y<18) 
gCursor.y++; 
break; 
case PLAY1LEFT: 
if(Order==CHESS1&&gCursor.x>0) 
gCursor.x--; 
break; 
case PLAY1RIGHT: 
if(Order==CHESS1&&gCursor.x<18) 
gCursor.x++; 
break; 

case PLAY2UP: 
if(Order==CHESS2&&gCursor.y>0) 
gCursor.y--; 
break; 
case PLAY2DOWN: 
if(Order==CHESS2&&gCursor.y<18) 
gCursor.y++; 
break; 
case PLAY2LEFT: 
if(Order==CHESS2&&gCursor.x>0) 
gCursor.x--; 
break; 
case PLAY2RIGHT: 
if(Order==CHESS2&&gCursor.x<18) 
gCursor.x++; 
break; 
} 

gotoxy(gCursor.x+MAPXOFT,gCursor.y+MAPYOFT); 
} 

/*游戏结束处理*/ 
void EndGame(void) 
{ 
textmode(C80); 
} 

/*显示当前行棋方*/ 
void ShowOrderMsg(int Order) 
{ 
gotoxy(6,MAPYOFT+20); 
textcolor(LIGHTRED); 
if(Order==CHESS1) 
cputs("Player1 go!"); 
else 
cputs("Player2 go!"); 

gotoxy(gCursor.x+MAPXOFT,gCursor.y+MAPYOFT); 
} 

/*落子正确处理*/ 
void DoOK(void) 
{ 
sound(500); 
delay(70); 
sound(600); 
delay(50); 
sound(1000); 
delay(100); 
nosound(); 
} 

/*检查用户的按键类别*/ 
int CheckKey(int press) 
{ 
if(press==ESCAPE) 
return KEYEXIT;/*是退出键*/ 

else 
if 
( ( press==PLAY1DO && gPlayOrder==CHESS1) || 
( press==PLAY2DO && gPlayOrder==CHESS2) 
) 
return KEYFALLCHESS;/*是落子键*/ 

else 
if 
( press==PLAY1UP || press==PLAY1DOWN || 
press==PLAY1LEFT || press==PLAY1RIGHT || 
press==PLAY2UP || press==PLAY2DOWN || 
press==PLAY2LEFT || press==PLAY2RIGHT 
) 
return KEYMOVECURSOR;/*是光标移动键*/ 

else 
return KEYINVALID;/*按键无效*/ 
} 