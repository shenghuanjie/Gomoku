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
#define CROSSRU 0xbf /*���Ͻǵ�*/ 
#define CROSSLU 0xda /*���Ͻǵ�*/ 
#define CROSSLD 0xc0 /*���½ǵ�*/ 
#define CROSSRD 0xd9 /*���½ǵ�*/ 
#define CROSSL 0xc3 /*���*/ 
#define CROSSR 0xb4 /*�ұ�*/ 
#define CROSSU 0xc2 /*�ϱ�*/ 
#define CROSSD 0xc1 /*�±�*/ 
#define CROSS 0xc5 /*ʮ�ֽ����*/ 

/*�����������Ͻǵ�����Ļ�ϵ�λ��*/ 
#define MAPXOFT 5 
#define MAPYOFT 2 

/*����1����ҵĲ���������*/ 
#define PLAY1UP 0x1157/*����--'W'*/ 
#define PLAY1DOWN 0x1f53/*����--'S'*/ 
#define PLAY1LEFT 0x1e41/*����--'A'*/ 
#define PLAY1RIGHT 0x2044/*����--'D'*/ 
#define PLAY1DO 0x3920/*����--�ո��*/ 

/*����2����ҵĲ���������*/ 
#define PLAY2UP 0x4800/*����--�����up*/ 
#define PLAY2DOWN 0x5000/*����--�����down*/ 
#define PLAY2LEFT 0x4b00/*����--�����left*/ 
#define PLAY2RIGHT 0x4d00/*����--�����right*/ 
#define PLAY2DO 0x1c0d/*����--�س���Enter*/ 

/*��������Ϸ��;�˳�, �ɰ� Esc ��*/ 
#define ESCAPE 0x011b 

/*���������Ͻ�����״̬, ���õ��������� */ 
/*��������, ��Ӧ��ָ�����ĸ���ҵ����� */ 
#define CHESSNULL 0 /*û������*/ 
#define CHESS1 'O'/*һ����ҵ�����*/ 
#define CHESS2 'X'/*������ҵ�����*/ 

/*���尴�����*/ 
#define KEYEXIT 0/*�˳���*/ 
#define KEYFALLCHESS 1/*���Ӽ�*/ 
#define KEYMOVECURSOR 2/*����ƶ���*/ 
#define KEYINVALID 3/*��Ч��*/ 

/*������ų���: ��, �� --- ��Ϊ1, ��Ϊ0 */ 
#define TRUE 1 
#define FALSE 0 

/**********************************************************/ 
/* �������ݽṹ */ 

/*���̽������������ݽṹ*/ 
struct point 
{ 
int x,y; 
}; 

/**********************************************************/ 
/*�Զ��庯��ԭ��˵�� */ 
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
/* ����ȫ�ֱ��� */ 
int gPlayOrder; /*ָʾ��ǰ���巽 */ 
struct point gCursor; /*����������ϵ�λ�� */ 
char gChessBoard[19][19];/*���ڼ�¼�����ϸ����״̬*/ 
/**********************************************************/ 

/**********************************************************/ 
/*������*/ 
void main() 
{ 
int press; 
int bOutWhile=FALSE;/*�˳�ѭ����־*/ 

Init();/*��ʼ��ͼ������*/ 

while(1) 
{ 
press=GetKey();/*��ȡ�û��İ���ֵ*/ 
switch(CheckKey(press))/*�жϰ������*/ 
{ 
/*���˳���*/ 
case KEYEXIT: 
clrscr();/*����*/ 
bOutWhile = TRUE; 
break; 

/*�����Ӽ�*/ 
case KEYFALLCHESS: 
if(ChessGo(gPlayOrder,gCursor)==FALSE)/*����*/ 
DoError();/*���Ӵ���*/ 
else 
{ 
DoOK();/*������ȷ*/ 

/*�����ǰ���巽Ӯ��*/ 
if(JudgeWin(gPlayOrder,gCursor)==TRUE) 
{ 
DoWin(gPlayOrder); 
bOutWhile = TRUE;/*�˳�ѭ����־��Ϊ��*/ 
} 
/*����*/ 
else 
/*�������巽*/ 
ChangeOrder(); 
ShowOrderMsg(gPlayOrder); 
} 
break; 

/*�ǹ���ƶ���*/ 
case KEYMOVECURSOR: 
MoveCursor(gPlayOrder,press); 
break; 

/*����Ч��*/ 
case KEYINVALID: 
break; 
} 

if(bOutWhile==TRUE) 
break; 
} 

/*��Ϸ����*/ 
EndGame(); 
} 
/**********************************************************/ 

/*�����ʼ�������ݳ�ʼ��*/ 
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

/* ���ַ�Ϊ1����� */ 
gPlayOrder = CHESS1; 
/* ������������, �������ϸ��㿪ʼ��ʱ��û������ */ 
for(i=0;i<19;i++) 
for(j=0;j<19;j++) 
gChessBoard[i][j]=CHESSNULL; 
/*����ʼλ��*/ 
gCursor.x=gCursor.y=0; 

/*������*/ 
textmode(C40); 
DrawMap(); 

/*��ʾ������˵��*/ 
i=0; 
textcolor(BROWN); 
while(Msg[i]!=NULL) 
{ 
gotoxy(25,3+i); 
cputs(Msg[i]); 
i++; 
} 

/*��ʾ��ǰ���巽*/ 
ShowOrderMsg(gPlayOrder); 
/*����������̵����Ͻǵ㴦*/ 
gotoxy(gCursor.x+MAPXOFT,gCursor.y+MAPYOFT); 
} 

/*������*/ 
void DrawMap(void) 
{ 
int i,j; 

clrscr(); 

for(i=0;i<19;i++) 
for(j=0;j<19;j++) 
DrawCross(i,j); 

} 

/*�������ϵĽ����*/ 
void DrawCross(int x,int y) 
{ 
gotoxy(x+MAPXOFT,y+MAPYOFT); 
/*���������һ����ҵ�����*/ 
if(gChessBoard[x][y]==CHESS1) 
{ 
textcolor(LIGHTBLUE); 
putch(CHESS1); 
return; 
} 
/*��������Ƕ�����ҵ�����*/ 
if(gChessBoard[x][y]==CHESS2) 
{ 
textcolor(LIGHTRED); 
putch(CHESS2); 
return; 
} 

textcolor(GREEN); 

/*���Ͻǽ����*/ 
if(x==0&&y==0) 
{ 
putch(CROSSLU); 
return; 
} 

/*���½ǽ����*/ 
if(x==0&&y==18) 
{ 
putch(CROSSLD); 
return; 
} 

/*���Ͻǽ����*/ 
if(x==18&&y==0) 
{ 
putch(CROSSRU); 
return; 
} 

/*���½ǽ����*/ 
if(x==18&&y==18) 
{ 
putch(CROSSRD); 
return; 
} 

/*��߽罻���*/ 
if(x==0) 
{ 
putch(CROSSL); 
return; 
} 

/*�ұ߽罻���*/ 
if(x==18) 
{ 
putch(CROSSR); 
return; 
} 

/*�ϱ߽罻���*/ 
if(y==0) 
{ 
putch(CROSSU); 
return; 
} 

/*�±߽罻���*/ 
if(y==18) 
{ 
putch(CROSSD); 
return; 
} 

/*�����м�Ľ����*/ 
putch(CROSS); 
} 

/*�������巽*/ 
int ChangeOrder(void) 
{ 
if(gPlayOrder==CHESS1) 
gPlayOrder=CHESS2; 
else 
gPlayOrder=CHESS1; 

return(gPlayOrder); 
} 

/*��ȡ����ֵ*/ 
int GetKey(void) 
{ 
char lowbyte; 
int press; 

while (bioskey(1) == 0) 
;/*����û�û�а�������ѭ��*/ 

press=bioskey(0); 
lowbyte=press&0xff; 
press=press&0xff00 + toupper(lowbyte); 
return(press); 
} 

/*���Ӵ�����*/ 
void DoError(void) 
{ 
sound(1200); 
delay(50); 
nosound(); 
} 

/*Ӯ�崦��*/ 
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

/*����*/ 
int ChessGo(int Order,struct point Cursor) 
{ 
/*�жϽ��������������*/ 
if(gChessBoard[Cursor.x][Cursor.y]==CHESSNULL) 
{ 
/*��û������, ���������*/ 
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

/*�жϵ�ǰ���巽���Ӻ��Ƿ�Ӯ��*/ 
int JudgeWin(int Order,struct point Cursor) 
{ 
int i; 
for(i=0;i<4;i++) 
/*�ж���ָ���������Ƿ�������5�����巽������*/ 
if(JudgeWinLine(Order,Cursor,i)) 
return TRUE; 
return FALSE; 
} 

/*�ж���ָ���������Ƿ�������5�����巽������*/ 
int JudgeWinLine(int Order,struct point Cursor,int direction) 
{ 
int i; 
struct point pos,dpos; 
const int testnum = 5; 
int count; 

switch(direction) 
{ 
case 0:/*��ˮƽ����*/ 
pos.x=Cursor.x-(testnum-1); 
pos.y=Cursor.y; 
dpos.x=1; 
dpos.y=0; 
break; 
case 1:/*�ڴ�ֱ����*/ 
pos.x=Cursor.x; 
pos.y=Cursor.y-(testnum-1); 
dpos.x=0; 
dpos.y=1; 
break; 
case 2:/*�����������ϵ�б����*/ 
pos.x=Cursor.x-(testnum-1); 
pos.y=Cursor.y+(testnum-1); 
dpos.x=1; 
dpos.y=-1; 
break; 
case 3:/*�����������µ�б����*/ 
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

/*�ƶ����*/ 
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

/*��Ϸ��������*/ 
void EndGame(void) 
{ 
textmode(C80); 
} 

/*��ʾ��ǰ���巽*/ 
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

/*������ȷ����*/ 
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

/*����û��İ������*/ 
int CheckKey(int press) 
{ 
if(press==ESCAPE) 
return KEYEXIT;/*���˳���*/ 

else 
if 
( ( press==PLAY1DO && gPlayOrder==CHESS1) || 
( press==PLAY2DO && gPlayOrder==CHESS2) 
) 
return KEYFALLCHESS;/*�����Ӽ�*/ 

else 
if 
( press==PLAY1UP || press==PLAY1DOWN || 
press==PLAY1LEFT || press==PLAY1RIGHT || 
press==PLAY2UP || press==PLAY2DOWN || 
press==PLAY2LEFT || press==PLAY2RIGHT 
) 
return KEYMOVECURSOR;/*�ǹ���ƶ���*/ 

else 
return KEYINVALID;/*������Ч*/ 
} 