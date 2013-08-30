#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "gobang.h"


//落子，把棋步s里的信息,设置到棋盘中
inline void set_piece(char board[15][15], STEP s)
{
	board[s.row][s.col] = s.player;
}

//判断棋盘是否已满
int count_piece(char board[15][15] )
{
	int row,col ;
	int count = 0;

	for(row=0; row<15; row++)
	{
		for( col=0; col<15; col++)
		{
			if( board[row][col] > 0 )
				count++;
		}
	} 

	return count;  //棋盘已满
}

//检测一个行列坐标是否落在棋盘上
static inline int is_valid(int row,int col)
{
	return (row>=0 && row<15 && col>=0 && col<15) ;
}

//描述四个方向前进行的列偏移量,其它四个方向对应负数即可
// 向上,右上,向右,右下; 
const OFFSET  direction[4] = { {-1,0}, {-1,1}, {0,1}, {1,1} };

//以刚下的棋子为中心检测它是否赢了
int is_winner(char board[15][15], STEP last)
{
	int count = 0;
	int row,col,i,j;

	//检测四条线(横、竖、斜上、斜下),
	//一条线黑子(白子)连续相连达到5个即赢
	//对于一条线上来说，向左至多搜索4个，向右至多搜索4个
	for(i=0; i<4; i++ )
	{
		count = 1; //刚下的棋子last本身算一个
		
		row = last.row;	
		col = last.col;
		for( j=1; j<=4; j++ )
		{
			row = row + direction[i].row_offset; //行坐标＋方向偏移量
			col = col + direction[i].col_offset; //列坐标＋方向偏移量
			if( is_valid(row,col) && board[row][col] == last.player )
				count++;
			else 
				break; //碰到无子，对方棋子或出界则停止统计
		}

		row = last.row;	
		col = last.col;
		for( j=1; j<=4; j++ )
		{
			row = row - direction[i].row_offset; //行坐标＋反方向偏移量
			col = col - direction[i].col_offset; //列坐标＋反方向偏移量

			if( is_valid(row,col) && board[row][col] == last.player )
				count++;
			else 
				break; 
		} //一条线检测完毕

		if( count >= 5 )
			return 1;   //赢了

	} //四条线检测完毕
	
	return 0; //未胜利
}

/*
 * 对于要下的step这步棋,判断在某方向上是否是死胡同,
 * 例XOOOOX, X__O_X
 * step:假设要落子的位置和玩家
 * dir_index: 方向索引，范围0~3, 用于OFFSET direction[4]
 * 						0竖直，1右上斜，2水平，3右下斜
 * 返回：死路返回1，否则返回0
 */
int is_blind_alley(char board[15][15], STEP s, int dir_index) 
{
	int count = 1; 
	int row,col;
	int i;

	row = s.row;
	col = s.col;
	for( i=1; i<=4; i++ )
	{
		row = row + direction[dir_index].row_offset; 
		col = col + direction[dir_index].col_offset;

		if( is_valid(row,col) && board[row][col] != 3-s.player )
			count++;
		else
			break; //遇到边界或对手的棋子,停止在此方向上的统计
	}
	
	row = s.row;
	col = s.col;
	for( i=1; i<=4; i++ )
	{
		row = row - direction[dir_index].row_offset; //某方向反偏移一步
		col = col - direction[dir_index].col_offset; //某方向反偏移一步

		if( is_valid(row,col) && board[row][col] != 3-s.player )
			count++; 
		else
			break; //遇到边界或对手的棋子,停止在此方向上的统计
	}

	if( count < 5 )
		return 1;  //死路
	else
		return 0; //不是死胡同
}

/* 
 * 对于棋步s，算一算在一条线上的得分
 * dir_index方向索引，范围0～3
 */
int get_line_score(char board[15][15], STEP s, int dir_index) 
{
	const static int score[6] = {0,10,20,40,90,1000};

	int score_left=0, score_right=0; // O_+_OO, 往左连2，往右连3,分别计分
	int block_left=0, block_right=0; //记录左右是否有堵塞
	int gap_left=0,   gap_right=0; //记录左右是否有间隔
	int left=0, mid=1, right=0;      //分3块统计本方子数,例O_O+O_O,1_3_1
	int row, col, i;	

	if( board[s.row][s.col] > 0 )
<<<<<<< HEAD
		return 0; //此处有子，0分
=======
		return 0;     //此处有子, 0分
>>>>>>> refactor
	if( is_blind_alley(board, s, dir_index) )
		return 0;     //死胡同，两头被夹，XOOOX, 0分

	//向"右"统计, 除了count[2]统计中间棋子，其它相关数组使用下标0
	row = s.row; 
	col = s.col;
	for( i=0; i<5; i++) //向右探测5步,可检测出O_OOOX的堵塞情况
	{
		row = row + direction[dir_index].row_offset; //某方向偏移一步
		col = col + direction[dir_index].col_offset; //某方向偏移一步

		//遇到出界或遇到对方棋子
		if( ! is_valid(row,col)  ||  board[row][col] == 3-s.player )
		{
			if( 1 == gap_right  &&  0 == right )
				break;        // O+O_X, 中间有空位，不算阻塞
			block_right = 1;  //O+_OX,一边阻塞
			break;
		}
		else if( NOTHING == board[row][col] ) //遇到空位,无子
		{
			if( 0 == gap_right )
				gap_right = 1; //首次遇到空位,继续前进
			else
				break;        //O+_ _,O+_O_,遇到第二个空位,停止
		}
		else    //遇到本方棋子
		{
			if( 0 == gap_right )
				mid++;
			else
				right++;
		}
	}

	//向"左"统计, 除了count[2]统计中间棋子，其它相关数组使用下标1
	row = s.row;
	col = s.col;
	for( i=0; i<5; i++)
	{
		row = row - direction[dir_index].row_offset; //某方向反偏移一步
		col = col - direction[dir_index].col_offset; //某方向反偏移一步

		//遇到出界或遇到对方棋子,有逻辑短路
		if( ! is_valid(row,col)  ||  board[row][col] == 3-s.player )
		{
			if( 1==gap_left  &&  0==left )
				break;     //...O+_X, 间有空位，不算阻塞，但停止前进和统计
			block_left = 1;  //O+OX, 记为一边阻塞
			break;
		}
		else if( NOTHING == board[row][col] ) //遇到空位,无子
		{
			if( 0 == gap_left )
				gap_left = 1; //首次遇到空位,OO_
			else
				break;        //遇到第二个空位,OO_O_, OO_ _
		}
		else  //遇到本方棋子 
		{
			if( 0 == gap_left )
				mid++;    
			else
				left++;
		}
	} //左右统计完毕

	if( mid>=5 )
		return score[5]; //能直连5个，直接返回1000
	
	//分 左中,中右 两部分,分开算分
	//右中结合
	i = mid + right;
	score_right = score[i]; 
	if( block_right  ||  block_left && 0==left )
		score_right -= (score[i] - score[i-1])/2; //有堵塞，扣分

	if( right > 0 && gap_right > 0 )   //带空位OO_O, 比OOO分数要低点
		score_right -= (score[i]-score[i-1]) *2/3; 
	
	//左中结合
	i = mid + left;
	score_left = score[i] ;
	if( block_left  ||  block_right && 0==right )
		score_left -= (score[i] - score[i-1])/2; //有堵塞，扣分

	if(left > 0  &&  gap_left > 0 )
		score_left -= (score[i]-score[i-1]) *2/3; //有间隔，扣分

	if( score_left > score_right )
		return score_left;
	else
		return score_right;
}

/*
 * 计算<row,col>的放黑子或白子的得分
 * 四条线分别计算得分，累加
 * 返回得分
 */
int get_point_score(char board[15][15], STEP s) 
{
	int i;                //方向索引，0~3代表四条线
	int score = 0;
	
	if( board[s.row][s.col] > 0 ) 
		return 0; //此处有子

	for( i=0; i<4; i++)
	{
		score += get_line_score(board, s, i);		
	} 
	return score;	
}

/*
 * 根据指定的player，计算最佳棋步
 * 找到最大得分的点，
 */
STEP get_computer_step( char board[15][15], char player)
{
	STEP s;
	STEP black = {BLACK,-1,-1};  //记录黑子得分最大值的位置
	STEP white = {WHITE,-1,-1};  //记录白子得分最大值的位置
	int bmax = -1;              //黑子最大得分
	int wmax = -1;              //白子最大得分
	int row, col;
	int score;

	//盘上无子,下中心
	if( 0 == count_piece(board) )
	{
		s.player = player;
		s.row = 7;
		s.col = 7;
		return s;  
	}

	for( row=0; row<15; row++ )
	{
		for( col=0; col<15; col++ )
		{
			if( board[row][col] > 0 )
				continue;

			s.player = BLACK;
			s.row = row;
			s.col = col;
			score = get_point_score(board, s);
			if( score > bmax )
			{
				bmax = score; //记录当前黑方最大分数
				black = s;    //记录当前黑方最大分数的位置
			}

			s.player = WHITE;
			score = get_point_score(board, s);
			if( score > wmax )
			{
				wmax = score; //记录当前白方最大分数
				white = s;    //记录当前白方最大分数的位置
			}
		}
	}	

	black.player = player;
	white.player = player;

	//如果本方和对方都能成5，以本方先成5为最佳
	if( bmax >= 900 && wmax>=900 )
	{
		if( BLACK==player)
			return black;
		else if( WHITE==player)
			return white;
	}

	//取最大分值的点
	if( bmax > wmax )
		return black;
	else if( bmax < wmax )
		return white;
	else if ( BLACK == player )
		return black; //分数相同时，以本方为主
	else 
		return white;
}

