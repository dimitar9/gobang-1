#include <stdio.h>
#include "gobang.h"
#include <unistd.h>
#include "ui.h"

//人机对战 或 机器对战
int game()
{
	char board[15][15] = {0};
	STEP next;

	print_board( board );
	
	while(1)
	{
		if( 225 == count_piece(board) )
		{
			printf("棋盘已满，棋局结束\n");
			return 0;
		}
		//黑方走子	
		next = get_input_step(board,BLACK); //计算或等待输入棋步
		board[next.row][next.col] = next.player;
		print_board( board );   //显示
		printf("上一步：黑方( %c%d )\n", next.col+'A', next.row+1);
		if( is_winner(board,next) )
		{
			printf("棋局结束，黑方胜\n");
			return 0;
		}
		//sleep(1);

		if( 225 == count_piece(board) )
		{
			printf("棋盘已满，棋局结束\n");
			return 0;
		}
		//白方走子
		next = get_computer_step( board, WHITE); 
		board[next.row][next.col] = next.player;
		print_board( board );
		printf("上一步：白方( %c%d )\n", next.col+'A', next.row+1);
		if( is_winner(board,next) )
		{
			printf("棋局结束，白方胜\n");
			return 0;
		}
		//sleep(1);
	}

	return 0;
}

int main()
{
	int cmd;
	while(1)
	{
		game();
		puts("再来一局( y/n ) ?");
		cmd = getchar();
		while( getchar() != '\n');
		if( cmd != 'y' )
			break;
	}
	return 0;
}
