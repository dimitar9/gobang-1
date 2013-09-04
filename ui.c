#include <stdio.h>
#include <string.h>
#include "ui.h"
#include "gobang.h"

//表示界面上黑白子的字符,相关宏,下标与字符有对应关系 
//NOTHING 0   '.'
//BLACK   1   'X'
//WHITE   2   'O'
static char piece[4] = { '.', 'X', 'O'};

//打印列名, A B C ....
static void print_col_title()
{
	int col;

	printf("   "); //3个空格
	for( col=0; col<15; col++ )
	{	
		printf(" %c", 'A'+col); 
	}
	putchar('\n');
}

//打印棋盘及棋子( . O X )
void print_board( char a[15][15] )
{
	int row, col;

	print_col_title(); //打印列标题

	//打印各行
	for( row = 0; row<15; row++ )
	{
		printf("%3d", row+1); //行号

		for( col=0; col<15; col++ )
		{
			if( 7==col && 7==row &&  0 == a[row][col] )
			{
				printf(" +");  // + 标记棋盘中心
				continue;
			}

			printf(" %c", piece[ a[row][col] ]);
		}

		printf("%3d\n", row+1); //行号
	}

	print_col_title(); //最后一行，打印列标题
	putchar('\n');
}


//从键盘获得用户输入的棋步
extern STEP get_input_step( char board[15][15], char player)
{
	STEP s ;
	int row, col;
	char cmd[16] = {0};

INPUT:
	do
	{
		printf("请 %s 走子(例:d4) ", BLACK==player ? "黑棋" : "白棋" );
		fgets(cmd, 16, stdin);

		if( 0 == strcmp(cmd,"quit\n") || 0 == strcmp(cmd, "exit\n") )
			exit(0); //退出程序 

		if( cmd[0] < 'a' )
			col = cmd[0] - 'A' + 1;   //解析列,应该在1～15
		else
			col = cmd[0] - 'a' + 1;  

		row = atoi(cmd+1);

		if( row<1 || row>15 || col<1 || col>15 )
			goto INPUT;

		row--; //输入的是n行，对应数组中的第n-1行
		col--;
	}while( board[row][col] > 0 );

	s.player = player;
	s.row = row;
	s.col = col;

	return s;
}


//从管道文件中读一个棋步（以便两个程序通过管道传递棋步）
STEP read_step( const char * filename, char player )
{
	FILE*fp;
	STEP s ;
	char buf[16] = {0};

	fp = fopen(filename, "r");
	fgets(buf,16,fp);
	
	s.player = player;
	s.col = buf[0] - 'A';
	s.row = atoi(buf+1) - 1;

	fclose(fp);
	return s;
}

//将一个棋步写入到管道文件中（以便两个程序通过管道传递棋步）
void write_step(const char * filename, STEP s )
{	
	FILE * fp = fopen(filename, "a");
	fprintf(fp ,"%c%d\n", s.col+65, s.row+1); //行转字符串存入buf 
	fclose(fp);
}

