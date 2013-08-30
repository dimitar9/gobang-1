#include <stdio.h>
#include "gobang.h"
#include <unistd.h>
#include "ui.h"


//白棋程序，从black.pipe读取黑方棋步，白方下棋后将自己的棋步写入到white.pipe
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
		next = read_step("black.pipe", BLACK); //读取黑方棋步
		set_piece(board, next);
		print_board( board );

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
		next = get_computer_step(board,WHITE);
		write_step("white.pipe", next); //写入文件通知对方
		set_piece(board, next); //落子
		print_board( board );   //显示
		printf("上一步：白方( %c%d )\n", next.col+'a', next.row+1);
		if( is_winner(board,next) )
		{
			printf("棋局结束，白方胜\n");
			return 0;
		}
	}

	return 0;
}
