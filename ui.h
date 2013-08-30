#ifndef __CHESS_UI_H__
#define __CHESS_UI_H__

#include "gobang.h"

#ifdef __cplusplus
extern "C" {
#endif


void print_board( char a[15][15] );

//从键盘获得用户输入的棋步
extern STEP get_input_step( char board[15][15], char player);

extern STEP read_step( const char * path, char player );
extern void write_step(const char * path, STEP s );


#ifdef __cplusplus
}
#endif

#endif
