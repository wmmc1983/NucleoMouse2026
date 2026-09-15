/*
 * search.h
 *
 *  Created on: 2026/06/17
 *      Author: Karasawa
 */

#ifndef INC_SEARCH_H_
#define INC_SEARCH_H_
#include "global.h"
#include "maze.h"

typedef struct {
	bool second; //2回目の探索かどうか
	int16_t x; //座標X
	int16_t y; //座標Y
	SearchDirection dir; //方向
	Section section; //迷路のセクション情報
	uint8_t r_cnt; //経路カウンタ
} MouseData;

extern MouseData mouse;

void SearchInit(void);

void SearchA(void);

void SearchB(void);

void SearchC(void);

void TestSearch(void);

#endif /* INC_SEARCH_H_ */
