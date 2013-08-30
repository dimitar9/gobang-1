#include <stdio.h>
#include "gobang.h"
#include <unistd.h>
#include "ui.h"


//黑棋程序，从white.pipe读取白方棋步，黑方下棋后将自己的棋步写入到black.pipe
int main()
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
		next = get_computer_step(board,BLACK); //计算或等待输入棋步
		board[next.row][next.col] = next.player;
		write_step("black.pipe", next); //写入文件通知对方
		print_board( board );   //显示
		printf("上一步：黑方( %c%d )\n", next.col+'a', next.row+1);
		if( is_winner(board,next) )
		{
			printf("棋局结束，黑方胜\n");
			return 0;
		}

		if( 225 == count_piece(board) )
		{
			printf("棋盘已满，棋局结束\n");
			return 0;
		}
		//白方走子
		//next = get_computer_step( board, WHITE); 
		next = read_step("white.pipe", WHITE);
		board[next.row][next.col] = next.player;
		print_board( board );
		printf("上一步：白方( %c%d )\n", next.col+'a', next.row+1);
		if( is_winner(board,next) )
		{
			printf("棋局结束，白方胜\n");
			return 0;
		}
	}

	return 0;
}
