/*
 * maze.h
 *
 *  Created on: 2026/06/17
 *      Author: Karasawa
 */

#ifndef INC_MAZE_H_
#define INC_MAZE_H_

typedef enum {//enumは整数型として扱われるため、0～3の範囲で表現できる
	KNorth,   //0
	KEast,    //1
	KSouth,   //2
	KWest,    //3
} SearchDirection;

typedef enum {//enmuは整数型として扱われるため、0～3の範囲で表現できる
	KForward, //0
	KTurnL90, //1
	KTurnR90, //2
	KTurn180, //3
} RouteMotion;

typedef struct {
	bool north; //北方向の壁の有無
	bool east;  //東方向の壁の有無
	bool south; //南方向の壁の有無
	bool west;  //西方向の壁の有無
	bool visited; //訪問済みかどうか
} Section;

typedef struct {
	Section map[MAZE_SIZE][MAZE_SIZE]; //迷路マップ格納配列
	uint8_t step_map[MAZE_SIZE][MAZE_SIZE];        //歩数マップ格納配列
	uint8_t goal_x, goal_y;      //ゴール座標
	RouteMotion route[MAX_ROUTE];          //最短経路格納配列
	uint8_t r_cnt;               //経路カウンタ
} MazeData;

extern MazeData maze;

/*============================================================
	関数プロトタイプ宣言
============================================================*/
//====迷路系====

void MazeInit(MazeData* p_maze);

void MazeMakeStepMap(MazeData* p_maze, uint8_t start_x, uint8_t start_y, bool is_second);

void MazeMakeRoute(MazeData* p_maze, uint8_t start_x, uint8_t start_y, SearchDirection start_dir);

void MazeMakeRouteLeftHand(MazeData* p_maze, uint8_t start_x, uint8_t start_y, SearchDirection start_dir);

void MazeWriteWallMap(MazeData* p_maze, uint8_t x, uint8_t y, Section section);

void MazeWriteFlsh(MazeData* p_maze);

void MazeReadFlsh(MazeData* p_maze);

#endif /* INC_MAZE_H_ */
