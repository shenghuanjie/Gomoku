#include<iostream>
#include<string>
using namespace std;
short chess[17][17]={};
short score[17][17]={};
short x,y,x1,y1,step=1;
struct 
{
	short xx[3];
	short yy[3];
	bool done;
}regret;
void backward()
{
	chess[regret.xx[0]][regret.yy[0]]=0;
	chess[regret.xx[(step+1)%2+1]][regret.yy[(step+1)%2+1]]=0;
	regret.done=true;
}
bool judge(short color)//�ж�ʤ��
{
	bool tell=false;
	short tempx,tempy,count=0,hang,lie;
	for(hang=1;hang<=15;hang++)
	{
		for(lie=1;lie<=15;lie++)
		{
			if(chess[hang][lie]==color)
			{
				tempx=hang;tempy=lie;count=1;
				while(chess[tempx+1][tempy]==color)//����
				{
					tempx++;count++;
						if(count==5)
						{
							tell=true;
							goto end;
						}
				}
				tempx=hang;tempy=lie;count=1;
				while(chess[tempx][tempy+1]==color)//����
				{
					tempy++;count++;
						if(count==5)
						{
							tell=true;
							goto end;
						}
				}
				tempx=hang;tempy=lie;count=1;
				while(chess[tempx+1][tempy+1]==color)//��б
				{
					tempx++;tempy++;count++;
						if(count==5)
						{
							tell=true;
							goto end;
						}
				}
				tempx=hang;tempy=lie;count=1;
				while(chess[tempx+1][tempy-1]==color)//��б
				{
					tempx++;tempy--;count++;
						if(count==5)
						{
							tell=true;
							goto end;
						}
				}
			}
		}
	}
end:return tell;
}
bool secure(short dot)
{
	if(dot<=15&&dot>=1)
		return true;
	else
		return false;
}
short AIjump(short color,short hang,short lie)
{
	short tempx,tempy,movex,movey,tell=0,use;
	short example[4]={color,color,0,color};
	for(movex=-1;movex<=1;movex++)
	{
		for(movey=-1;movey<=1;movey++)
		{
			if(movex==0&&movey==0)continue;
			if(secure(hang+movex)&&secure(hang-movex*2)&&secure(lie+movey)&&secure(lie-movey*2))
			{
			tempx=hang+movex;tempy=lie+movey;
			for(use=0;use<=3;use++)
			{
				if(chess[tempx][tempy]!=example[use])goto next1;
				tempx-=movex;tempy-=movey;
			}
			tell+=2;
			if(chess[hang+movex*2][lie+movey*2]==(3-color)||chess[hang+movex*2][lie+movey*2]==-1)tell--;
			if(chess[hang-movex*3][lie+movey*3]==(3-color)||chess[hang-movex*3][lie+movey*3]==-1)tell--;
			}
next1:      
			if(secure(hang-movex*3)&&secure(lie-movey*3))
			{
			tempx=hang;tempy=lie;
			for(use=0;use<=3;use++)
			{
				if(chess[tempx][tempy]!=example[use])goto next2;
				tempx-=movex;tempy-=movey;
			}
			tell+=2;
			if(chess[hang+movex*1][lie+movey*1]==(3-color)||chess[hang+movex*1][lie+movey*1]==-1)tell--;
			if(chess[hang-movex*4][lie+movey*4]==(3-color)||chess[hang-movex*4][lie+movey*4]==-1)tell--;
			}
next2:
			if(secure(hang+movex*3)&&secure(lie+movey*3))
			{
				tempx=hang+movex*3;tempy=lie+movey*3;
				for(use=0;use<=3;use++)
			{
				if(chess[tempx][tempy]!=example[use])goto end;
				tempx-=movex;tempy-=movey;
			}
			tell+=2;
			if(chess[hang+movex*4][lie+movey*4]==(3-color)||chess[hang+movex*4][lie+movey*4]==-1)tell--;
			if(chess[hang-movex][lie+movey]==(3-color)||chess[hang-movex][lie+movey]==-1)tell--;
			}
		}
	}
end:return tell;
}
short AIblank2(short color,short hang,short lie)
{
	short tell=0;
	if(chess[hang+1][lie]==0&&chess[hang+2][lie]==color)//����
	{
		tell++;

	}
	if(chess[hang-1][lie]==0&&chess[hang-2][lie]==color)//����
	{
		tell++;
	}
	if(chess[hang][lie-1]==0&&chess[hang][lie-2]==color)//����
	{
		tell++;
	}
	if(chess[hang][lie+1]==0&&chess[hang][lie+2]==color)//����
	{
		tell++;
	}
	if(chess[hang-1][lie-1]==0&&chess[hang-2][lie-2]==color)//����
	{
		tell++;
	}
	if(chess[hang+1][lie+1]==0&&chess[hang+2][lie+2]==color)//����
	{
		tell++;
	}
	if(chess[hang+1][lie-1]==0&&chess[hang+2][lie-2]==color)//����
	{
		tell++;
	}
	if(chess[hang-1][lie+1]==0&&chess[hang-2][lie+2]==color)//����
	{
		tell++;
	}
	return tell;
}
short AIn(short color,short hang,short lie,short n1)
{
	bool temp=false;
	short tempx,tempy,count=0,tell=0,x;
	short n=1-n1;
	for(x=n;x<=0;x++)
	{
		tempx=hang+x;tempy=lie;count=0;
		temp=(tempx>=1&&tempx<=15&&chess[tempx-1][tempy]!=color);//���ŵ�
		while(temp&&chess[tempx][tempy]==color)
		{
			count++;
			tempx++;
		}
		if(count==n1)
		{
			tell+=2;
			if(tempx>=0&&tempx<=16&&(chess[tempx][tempy]==(3-color)||chess[tempx][tempy]==-1))tell--;
			if((tempx>=(1+n1))&&(chess[tempx-1-n1][tempy]==(3-color)||chess[tempx-1-n1][tempy]==-1))tell--;
			if(n1==4&&tell==2)tell*=15;
			x=1;break;
		}
	}

	for(x=n;x<=0;x++)
	{
		tempx=hang;tempy=lie+x;count=0;
		temp=(tempy>=1&&tempy<=15&&chess[tempx][tempy-1]!=color);//���ŵ�
		while(temp&&chess[tempx][tempy]==color)
		{
			count++;
			tempy++;
		}
		if(count==n1)
		{
			tell+=2;
			if(tempy>=0&&tempy<=16&&(chess[tempx][tempy]==(3-color)||chess[tempx][tempy]==-1))tell--;
			if((tempy>=(1+n1))&&(chess[tempx][tempy-1-n1]==(3-color)||chess[tempx][tempy-1-n1]==-1))tell--;
			if(n1==4&&tell==2)tell*=15;
			x=1;break;
		}
	}

	for(x=n;x<=0;x++)
	{
		tempx=hang+x;tempy=lie+x;count=0;
		temp=(tempx>=1&&tempx<=15&&tempy>=1&&tempy<=15&&chess[tempx-1][tempy-1]!=color);//��������
		while(temp&&chess[tempx][tempy]==color)
		{
			count++;
			tempx++;tempy++;
		}
		if(count==n1)
		{
			tell+=2;
			if(tempx>=0&&tempx<=16&&tempy>=0&&tempy<=16&&(chess[tempx][tempy]==(3-color)||chess[tempx][tempy]==-1))tell--;
			if((tempx>=(1+n1))&&(tempy>=(1+n1))&&(chess[tempx-1-n1][tempy-1-n1]==(3-color)||chess[tempx-1-n1][tempy-1-n1]==-1))tell--;
			if(n1==4&&tell==2)tell*=15;
			x=1;break;
		}
	}

	for(x=n;x<=0;x++)
	{
		tempx=hang+x;tempy=lie-x;count=0;
		temp=(tempx>=1&&tempx<=15&&tempy>=1&&tempy<=15&&chess[tempx-1][tempy+1]!=color);//��������
		while(temp&&chess[tempx][tempy]==color)
		{
			count++;
			tempx++;tempy--;
		}
		if(count==n1)
		{
			tell+=2;
			if(tempx>=0&&tempx<=16&&tempy>=0&&tempy<=16&&(chess[tempx][tempy]==(3-color)||chess[tempx][tempy]==-1))tell--;
			if((tempx>=(1+n1))&&(tempy<=(15-n1))&&(chess[tempx-1-n1][tempy+1+n1]==(3-color)||chess[tempx-1-n1][tempy+1+n1]==-1))tell--;
			if(n1==4&&tell==2)tell*=15;
			x=1;break;
		}
	}
	return tell;
}
void AIanalyze(short color)
{
	short hang,lie;
	for(hang=1;hang<=15;hang++)
	{
		for(lie=1;lie<=15;lie++)
		{
				if(chess[hang][lie]==0)
				{
					chess[hang][lie]=color;
					if(judge(color)){score[hang][lie]=10000;chess[hang][lie]=0;return;}
					score[hang][lie]+=AIn(color,hang,lie,4)*120;
					score[hang][lie]+=AIn(color,hang,lie,3)*120;
					score[hang][lie]+=AIn(color,hang,lie,2)*9;
					score[hang][lie]+=AIjump(hang,lie,color)*40;
					score[hang][lie]+=AIblank2(color,hang,lie)*3;
					color=3-color;
                    score[hang][lie]+=AIn(color,hang,lie,4)*100;
					score[hang][lie]+=AIn(color,hang,lie,3)*101;
					score[hang][lie]+=AIn(color,hang,lie,2)*6;
					score[hang][lie]+=AIjump(hang,lie,color)*35;
					chess[hang][lie]=0;
				}
				else
					score[hang][lie]=-1;
		}
	}
}
void AI(short choose)
{
	short maxa=0,gotox[20],gotoy[20],count=0;
	AIanalyze(choose);AIanalyze(3-choose);
	for(int i=1;i<=15;i++)
	{
		for(int j=1;j<=15;j++)
		{
			if(score[i][j]> maxa)
			{
				maxa=score[i][j];
			}
		}
	}
	for(int i=1;i<=15;i++)
	{
		for(int j=1;j<=15;j++)
		{
			if(score[i][j]==maxa)
			{
				gotox[count]=i;
				gotoy[count]=j;
				count++;
			}
		}
	}
	short temp=rand()%count;
	x=gotox[temp];y=gotoy[temp];
	regret.xx[step%2+1]=x;
	regret.yy[step%2+1]=y;
	chess[x][y]=choose;
}
short startmenu()//��ʼ�˵�
{
	short menu;
	cout<<"O(��_��)O~��ӭ����������С��Ϸ"<<endl;
	cout<<endl;
	cout<<"�˻���ս������1"<<endl;
	cout<<"˫�˶�ս������2"<<endl;
	cout<<"�˳�����0"<<endl;
	cout<<"��ѡ��";
	cin>>menu;
	return (menu);
}
void printchessboard(bool admit)//��ӡ����
{
	if(admit)cout<<"O(��_��)O����~��������0 0"<<endl;
	short i=1;
	cout<<"  ";
	while(i<=9)
	{cout<<i++<<" ";}
	while(i<=15)
	{cout<<i++;}
		cout<<endl;
	i=1;
	for(short hang=1;hang<=15;hang++)
	{
		for(short lie=1;lie<=15;lie++)
		{
	        if(chess[hang][lie]==0)
	        {
			if(hang==1)
			{
				if(lie==1)  cout<<i++<<" ��";
				if((lie>1)&&(lie<15)) cout<<"��";
				if(lie==15) cout<<"��"<<endl;
	         }
	         if((hang>1)&&(hang<15))
	         {
				 if(lie==1) {if(i<=9)cout<<i++<<" ��";else cout<<i++<<"��";}
				 if((lie>1)&&(lie<15)) cout<<"��";
				 if(lie==15) cout<<"��"<<endl;
	          }
			 if(hang==15)
			 {
				 if(lie==1) cout<<i++<<"��";
				 if((lie>1)&&(lie<15)) cout<<"��";
				 if(lie==15) cout<<"��"<<endl;
			
			 }
			}
			if(chess[hang][lie]==1)
				if(lie==1)
				{if(i<=9)cout<<i++<<" ��";else cout<<i++<<"��";}
				else if(lie==15)
					cout<<"��"<<endl;
				else
					cout<<"��";
			if(chess[hang][lie]==2)
				if(lie==1)
				{if(i<=9)cout<<i++<<" ��";else cout<<i++<<"��";}
				else if(lie==15)
					cout<<"��"<<endl;
			    else 
					cout<<"��";

		}
	}
}
void player()//��ս����
{
	if(judge((step-1)%2+1))
	{system("cls");cout<<"���"<<step%2+1<<"��ʤ"<<endl;return;}
	if(step==225)
	{system("cls");cout<<"����"<<endl;return;}
		system("cls");
		printchessboard(0);
		if(step!=1)cout<<"��һ�����"<<step%2+1<<"�ڣ�"<<x<<"��"<<y<<"������"<<endl;
		cout<<"��"<<step++<<"�������"<<step%2+1<<"�����ӣ�";
		x1=x;y1=y;
		while(cin>>x>>y)
		{
			if(x>15||x<1||y>15||y<1)//�ж��Ƿ�Խ��
			{   system("cls");
				printchessboard(0);
				if(step>2)cout<<"��һ�����"<<(step-1)%2+1<<"�ڣ�"<<x1<<"��"<<y1<<"������"<<endl;
				cout<<"��Ҫ���������������ӣ����"<<step%2+1<<"���������ӣ�";
				continue;
			}
			else if(chess[x][y]==0)
				break;
			else//��������
			{
				system("cls");
				printchessboard(0);
				cout<<"��һ�����"<<(step-1)%2+1<<"�ڣ�"<<x1<<"��"<<y1<<"������"<<endl;
				cout<<"�õ��Ѿ������ӣ����"<<step%2+1<<"���������ӣ�";
			}
		}
		chess[x][y]=(step-1)%2+1;
		player();
}
void computer(short choose)
{
	if(judge(3-choose))
	{system("cls");cout<<"���Ի�ʤ"<<endl;return;}
	if(step==225)
	{system("cls");cout<<"����"<<endl;return;}
		system("cls");
		printchessboard(1);
		if(step!=1||choose==1){cout<<"��һ�������ڣ�"<<x<<"��"<<y<<"������"<<endl;}
		    cout<<"��"<<step++<<"�غϣ���������ӣ�";
			//���ã���ʾ����
			//cout<<endl;
			//for(int i=1;i<=15;i++)
			//{for(int j=1;j<=15;j++)
			//{if(score[i][j]==-1)cout<<score[i][j]<<" ";else cout<<score[i][j]<<"  ";;}cout<<endl;}
			x1=x;y1=y;
		while(cin>>x>>y)
		{
			if(x==0&&y==0)
			{
				if(regret.done)
				{
				system("cls");
				printchessboard(1);
				if(step>2){cout<<"��һ�������ڣ�"<<regret.xx[(step+1)%2+1]<<"��"<<regret.yy[(step+1)%2+1]<<"������"<<endl;}
				cout<<"���������������壬���������ӣ�";
				}
				else
				{
					if(step<=2)
					{
						cout<<"��ʲô�أ���û��ʼ���أ�"<<endl;
					cout<<"��"<<step-1<<"�غϣ���������ӣ�";
					}
					else
					{
				backward();
				system("cls");
				printchessboard(1);
				cout<<"���׳ɹ�"<<endl;
				step-=2;
				if(step>=2)cout<<"��һ�������ڣ�"<<regret.xx[step%2+1]<<"��"<<regret.yy[step%2+1]<<"������"<<endl;
				cout<<"��"<<step++<<"�غϣ���������ӣ�";
					}
				}
				continue;
			}
			else if(x>15||x<1||y>15||y<1)
			{   system("cls");
				printchessboard(1);
				if(step>2)cout<<"��һ�������ڣ�"<<x1<<"��"<<y1<<"������"<<endl;
				cout<<"��Ҫ���������������ӣ����������ӣ�";
				continue;
			}
			else if(chess[x][y]==0)
			{
				regret.xx[0]=x;
				regret.yy[0]=y;
				regret.done=false;
				break;
			}
			else
			{
				system("cls");
				printchessboard(1);
				cout<<"��һ�������ڣ�"<<x1<<"��"<<y1<<"������"<<endl;
				cout<<"�õ��Ѿ������ӣ����������ӣ�";
			}
		}
		chess[x][y]=choose;
		if(judge(choose))
	    {system("cls");cout<<"��һ�ʤ"<<endl;return;}  
		memset(score,0,sizeof(score));
		AI(3-choose);
		computer(choose);
}
int main()//������
{
	begin:system("cls");
	memset(chess,0,sizeof(chess));
	regret.done=false;
	short menu;
	short choose;
	for(int o=0;o<=16;o++)
	{
		chess[0][o]=-1;
		chess[16][o]=-1;
		chess[o][0]=-1;
		chess[o][16]=-1;
	}
	step=1;
    menu=startmenu();
	if(menu==0)
	{
		char assurance;
		cout<<"Are you sure��Y/N"<<endl;
		cin>>assurance;
		if(assurance=='y'||assurance=='Y'){
			system("cls");cout<<"O(��_��)O~Bye bye"<<endl;return 0;}
		else
			goto begin;
	}
	else if(menu==1)
	{
		cout<<endl;
		cout<<"���ֺ�������1"<<endl;
		cout<<"���ְ�������2"<<endl;
		cout<<"��ѡ��";
		while(cin>>choose)
		{
			if(choose!=1&&choose!=2)
				;
			else
				break;
		}
		if(choose==2)
		{
		x=rand()%3+6,y=rand()%3+6;
		chess[x][y]=choose;
		printchessboard(1);
		computer(3-choose);
		}
		else
		{computer(3-choose);}
	}
	else if(menu==2){player();}
	else
	{
		cout<<"��ˣ�ң��������ˣ�"<<endl;
		return 0;
	}
	/*cout<<endl;
			for(int i=1;i<=15;i++)
			{for(int j=1;j<=15;j++)
			{if(score[i][j]==-1)cout<<chess[i][j]<<" ";else cout<<chess[i][j]<<"  ";;}cout<<endl;}*/
		cout<<"����һ�� Y or N"<<endl;
	cin.get();
	char u;
	cin>>u;
	if(u=='y'||u=='Y')
	{system("cls");goto begin;}
	return 0;
}