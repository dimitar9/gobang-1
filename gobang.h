#ifndef __GOBANG_H__
#define __GOBANG_H__

#include <stdlib.h>
#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * 棋盘上只放0，1，2
 */
#define NOTHING 0
#define BLACK   1
#define WHITE   2

//棋步，包含黑方/白方，行，列
typedef struct step
{
	char player;
	char row;
	char col;
}STEP;

//用行偏移和列偏移描述方向，怎么在一个方向上前进一步
typedef struct offset
{
	char row_offset; //行偏移
	char col_offset; //列偏移
}OFFSET;

//描述四个方向前进行的列偏移量,其它四个方向对应负数即可
extern const OFFSET  direction[4] ;


//让计算机为player一方计算出合适的一步棋
extern STEP get_computer_step( char board[15][15], char player);



//落子，把棋步s里的信息,设置到棋盘中
extern inline void set_piece(char board[15][15], STEP s);

//计算盘上棋子数量
extern int count_piece(char board[15][15] );

//对于刚下的棋子，以其为中心检测是否赢了
int is_winner(char board[15][15], STEP last);


#ifdef __cplusplus
}
#endif

#endif
