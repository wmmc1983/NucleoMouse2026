/*
 * maze.c
 *
 *  Created on: 2026/06/17
 *      Author: Karasawa
 */
#include "global.h"
#include "maze.h"
#include "flash.h"
MazeData maze = { 0 }; //迷路情報を格納する構造体

//+++++++++++++++++++++++++++++++++++++++++++++++
// 関数名：MazeInit
// 内容：迷路情報の初期化を行う
// 引数：MazeData* maze - 迷路情報構造体へのポインタ
// 戻り値：なし
//+++++++++++++++++++++++++++++++++++++++++++++++
void MazeInit(MazeData* p_maze) {
	int i, j;
	for (i = 0; i < MAZE_SIZE; i++) {
		for (j = 0; j < MAZE_SIZE; j++) {
			p_maze->map[i][j].north = false;
			p_maze->map[i][j].east = false;
			p_maze->map[i][j].south = false;
			p_maze->map[i][j].west = false;
			p_maze->map[i][j].visited = false;
			p_maze->step_map[i][j] = 0;
		}
	}
	p_maze->goal_x = GOAL_X; //ゴール座標の初期化
	p_maze->goal_y = GOAL_Y; //ゴール座標の初期化
	p_maze->r_cnt = 0; //経路カウンタの初期化
}

//+++++++++++++++++++++++++++++++++++++++++++++++
// 関数名：MazeMakeStepMap
// 内容：幅優先探索を使用し、ゴールセルから各マスへの歩数マップを作成する
// 引数：MazeData* maze - 迷路情報構造体へのポインタ
//       uint8_t start_x - 現在の自機座標X
//       uint8_t start_y - 現在の自機座標Y
//       bool is_second - 2回目の探索かどうか
// 戻り値：なし
//+++++++++++++++++++++++++++++++++++++++++++++++
void MazeMakeStepMap(MazeData* p_maze, uint8_t start_x, uint8_t start_y, bool is_second) {
	//幅優先探索を使用して、迷路の歩数マップを作成する処理
	//--全てのセルの歩数を初期化--
	for (int i = 0; i < MAZE_SIZE; i++) {
		for (int j = 0; j < MAZE_SIZE; j++) {
			p_maze->step_map[i][j] = MAX_ROUTE-1; //255は最大値として使用（未訪問を示す）
		}
	}
	//--ゴールセルの歩数を0に設定--
	uint8_t m_step = 0; //ゴールセルの歩数を0に設定
	p_maze->step_map[p_maze->goal_x][p_maze->goal_y] = m_step;
	//--自機座標にたどり着くまでの歩数を計算--
	while (p_maze->step_map[start_x][start_y] == MAX_ROUTE-1) {
		for (int i = 0; i < MAZE_SIZE; i++) {
			for (int j = 0; j < MAZE_SIZE; j++) {
				if (p_maze->step_map[i][j] == m_step) {
					//北方向のセルが存在し、壁がない場合
					if (j < MAZE_SIZE - 1 && !p_maze->map[i][j].north && p_maze->step_map[i][j + 1] == MAX_ROUTE-1) {
						if (!is_second || p_maze->map[i][j + 1].visited) {
							p_maze->step_map[i][j + 1] = m_step + 1;
						}
					}
					//東方向のセルが存在し、壁がない場合

					//+++++++++++++++++++++++++++++++++++++++++++++++
					// TODO: ここに東側の場合の処理を書こう。
					//+++++++++++++++++++++++++++++++++++++++++++++++

					//南方向のセルが存在し、壁がない場合

					//+++++++++++++++++++++++++++++++++++++++++++++++
					// TODO: ここに南側の場合の処理を書こう。
					//+++++++++++++++++++++++++++++++++++++++++++++++

					//西方向のセルが存在し、壁がない場合

					//+++++++++++++++++++++++++++++++++++++++++++++++
					// TODO: ここに西側の場合の処理を書こう。
					//+++++++++++++++++++++++++++++++++++++++++++++++

					
				}
			}
		}
		m_step++;
	}
}

//+++++++++++++++++++++++++++++++++++++++++++++++
// 関数名：MazeMakeRoute
// 内容：歩数マップを基に、現在の自機座標からゴールまでの最短経路を作成する
// 引数：MazeData* maze - 迷路情報構造体へのポインタ
//       uint8_t start_x - 現在の自機座標X
//       uint8_t start_y - 現在の自機座標Y
//       SearchDirection start_dir - 現在の自機の向き
// 戻り値：なし
//+++++++++++++++++++++++++++++++++++++++++++++++
void MazeMakeRoute(MazeData* p_maze, uint8_t start_x, uint8_t start_y, SearchDirection start_dir) {
	//経路を作成する処理
	//--経路カウンタを初期化--
	p_maze->r_cnt = 0;
	//--自機座標からゴールまでの経路を作成--
	int x = start_x;
	int y = start_y;
	SearchDirection dir = start_dir; // 現在向いている方向

	// ゴール(歩数0)に到達するまで繰り返す
	while (p_maze->step_map[x][y] > 0) {
		uint8_t current_step = p_maze->step_map[x][y];
		uint8_t next_step = current_step - 1;
		SearchDirection next_dir = dir; // 次に行く方向

		// 周囲4マスの歩数を確認して、進むべき方向（壁がなく、歩数が1少ない方向）を探す
		if (y < MAZE_SIZE - 1 && !p_maze->map[x][y].north && p_maze->step_map[x][y + 1] == next_step) {
			next_dir = KNorth;
		}

		//+++++++++++++++++++++++++++++++++++++++++++++++
		// TODO: ここに東側の場合の処理を書こう。
		//+++++++++++++++++++++++++++++++++++++++++++++++

		//+++++++++++++++++++++++++++++++++++++++++++++++
		// TODO: ここに南側の場合の処理を書こう。
		//+++++++++++++++++++++++++++++++++++++++++++++++

		//+++++++++++++++++++++++++++++++++++++++++++++++
		// TODO: ここに西側の場合の処理を書こう。
		//+++++++++++++++++++++++++++++++++++++++++++++++

		else {
			// 経路が見つからない場合(エラー)は終了
			break;
		}

		// 現在の進行方向(dir)と次の方向(next_dir)の相対角度を求める
		// KNorth(0), KEast(1), KSouth(2), KWest(3)
		int diff = (next_dir - dir + 4) % 4; //enumはint型なので、diffは0～3の範囲になる

		if (diff == 0) {
			p_maze->route[p_maze->r_cnt] = KForward;
		} else if (diff == 1) {
			p_maze->route[p_maze->r_cnt] = KTurnR90;
		} else if (diff == 2) {
			p_maze->route[p_maze->r_cnt] = KTurn180;
		} else if (diff == 3) {
			p_maze->route[p_maze->r_cnt] = KTurnL90;
		}
		
		p_maze->r_cnt++;

		// 機体の座標と向きを更新
		dir = next_dir;
		if (dir == KNorth) y += 1;
		else if (dir == KEast) x += 1;
		else if (dir == KSouth) y -= 1;
		else if (dir == KWest) x -= 1;
	}
}

//+++++++++++++++++++++++++++++++++++++++++++++++
// 関数名：MazeMakeRouteLeftHand
// 内容：左手法により次の1手のみを決定する
// 引数：MazeData* maze - 迷路情報構造体へのポインタ
//       uint8_t start_x - 現在の自機座標X
//       uint8_t start_y - 現在の自機座標Y
//       SearchDirection start_dir - 現在の自機の向き
// 戻り値：なし
//+++++++++++++++++++++++++++++++++++++++++++++++
void MazeMakeRouteLeftHand(MazeData* p_maze, uint8_t start_x, uint8_t start_y, SearchDirection start_dir) {
	// 左手法で次の1手だけを決定する処理
	
	// -- 経路カウンタを初期化 --
	p_maze->r_cnt = 0;

	// -- 現在の座標と向きを取得 --
	int x = start_x;
	int y = start_y;
	SearchDirection dir = start_dir; // 現在向いている方向

	bool wall_left  = false; // 左の壁
	bool wall_front = false; // 前の壁
	bool wall_right = false; // 右の壁

	// 自分が向いている方向(dir)を基準にして、前後左右の壁の有無を調べる
	if (dir == KNorth) {
		wall_left  = p_maze->map[x][y].west;
		wall_front = p_maze->map[x][y].north;
		wall_right = p_maze->map[x][y].east;
	} else if (dir == KEast) {
		wall_left  = p_maze->map[x][y].north;
		wall_front = p_maze->map[x][y].east;
		wall_right = p_maze->map[x][y].south;
	} else if (dir == KSouth) {
		wall_left  = p_maze->map[x][y].east;
		wall_front = p_maze->map[x][y].south;
		wall_right = p_maze->map[x][y].west;
	} else if (dir == KWest) {
		wall_left  = p_maze->map[x][y].south;
		wall_front = p_maze->map[x][y].west;
		wall_right = p_maze->map[x][y].north;
	}

	//左手法により次の手を決定する
	// 優先順位：左 ・ 前 ・ 右 ・ 180度ターン
	if (!wall_left) {
		// 左に壁がない場合は、左に曲がる
		p_maze->route[p_maze->r_cnt] = KTurnL90;
	} 
	else if (!wall_front) {
		// 左に壁があり、前方には壁がない場合は、直進する
		p_maze->route[p_maze->r_cnt] = KForward;
	} 
	else if (!wall_right) {
		// 左も前も壁があり、右には壁がない場合は、右に曲がる
		p_maze->route[p_maze->r_cnt] = KTurnR90;
	} 
	else {
		// 左も前も右も壁（行き止まり）の場合は、180度ターンする
		p_maze->route[p_maze->r_cnt] = KTurn180;
	}
	
	// 1手分のみ記録したのでカウンタを1にする
	p_maze->r_cnt = 1;
}
//+++++++++++++++++++++++++++++++++++++++++++++++
// 関数名：MazeWriteWallMap
// 内容：現在のマウスの座標に、取得済みの絶対方向の壁情報を書き込み、訪問済みにする
// 引数：MazeData* maze - 迷路情報構造体へのポインタ
//       uint8_t x - 書き込むX座標
//       uint8_t y - 書き込むY座標
//       Section section - 書き込む壁情報
// 戻り値：なし
//+++++++++++++++++++++++++++++++++++++++++++++++
void MazeWriteWallMap(MazeData* p_maze, uint8_t x, uint8_t y, Section section) {

	if (x < 0 || x >= MAZE_SIZE || y < 0 || y >= MAZE_SIZE) {
		// 座標が迷路の範囲外の場合は何もしない
		return;
	}

	p_maze->map[x][y] = section; //現在のマウスの座標に壁情報を書き込む
	//周辺のマスの壁情報も更新する
	if (y < MAZE_SIZE - 1) {
		p_maze->map[x][y + 1].south = section.north;
	}
	if (x < MAZE_SIZE - 1) {
		p_maze->map[x + 1][y].west = section.east;
	}
	if (y > 0) {
		p_maze->map[x][y - 1].north = section.south;
	}
	if (x > 0) {
		p_maze->map[x - 1][y].east = section.west;
	}
}

//+++++++++++++++++++++++++++++++++++++++++++++++
// 関数名：MazeWriteFlsh
// 内容：現在の迷路情報を圧縮し、フラッシュメモリにバックアップとして書き込む
// 引数：MazeData* maze - 迷路情報構造体へのポインタ
// 戻り値：なし
//+++++++++++++++++++++++++++++++++++++++++++++++
void MazeWriteFlsh(MazeData* p_maze) {
	// 迷路情報をフラッシュメモリに書き込む処理
	FlashEnableWrite();
	
	uint32_t address = 0;
	// 1マスあたり5bitなので、16bit(1 Halfword)に2マス分のデータを圧縮して書き込む
	for (int i = 0; i < MAZE_SIZE; i++) {
		for (int j = 0; j < MAZE_SIZE; j += 2) {
			uint16_t data = 0;
			
			// 1つめのマス (下位8ビットに格納)
			if (p_maze->map[i][j].north)   data |= (1 << 0);
			if (p_maze->map[i][j].east)    data |= (1 << 1);
			if (p_maze->map[i][j].south)   data |= (1 << 2);
			if (p_maze->map[i][j].west)    data |= (1 << 3);
			if (p_maze->map[i][j].visited) data |= (1 << 4);

			// 2つめのマス (上位8ビットに格納)
			if (p_maze->map[i][j + 1].north)   data |= (1 << 8);
			if (p_maze->map[i][j + 1].east)    data |= (1 << 9);
			if (p_maze->map[i][j + 1].south)   data |= (1 << 10);
			if (p_maze->map[i][j + 1].west)    data |= (1 << 11);
			if (p_maze->map[i][j + 1].visited) data |= (1 << 12);

			FlashWriteHalfword(address, data);
			address++;
		}
	}
	
	FlashDisableWrite();
}

//+++++++++++++++++++++++++++++++++++++++++++++++
// 関数名：MazeReadFlsh
// 内容：フラッシュメモリから保存されたデータを読み込み、迷路情報として復元する
// 引数：MazeData* maze - 迷路情報構造体へのポインタ
// 戻り値：なし
//+++++++++++++++++++++++++++++++++++++++++++++++
void MazeReadFlsh(MazeData* p_maze) {
	// フラッシュメモリから迷路情報を読み込む処理
	uint32_t address = 0;
	
	for (int i = 0; i < MAZE_SIZE; i++) {
		for (int j = 0; j < MAZE_SIZE; j += 2) {
			uint16_t data = FlashReadHalfword(address);
			
			// 1つめのマスの復元
			p_maze->map[i][j].north   = (data & (1 << 0)) != 0;
			p_maze->map[i][j].east    = (data & (1 << 1)) != 0;
			p_maze->map[i][j].south   = (data & (1 << 2)) != 0;
			p_maze->map[i][j].west    = (data & (1 << 3)) != 0;
			p_maze->map[i][j].visited = (data & (1 << 4)) != 0;

			// 2つめのマスの復元
			p_maze->map[i][j + 1].north   = (data & (1 << 8)) != 0;
			p_maze->map[i][j + 1].east    = (data & (1 << 9)) != 0;
			p_maze->map[i][j + 1].south   = (data & (1 << 10)) != 0;
			p_maze->map[i][j + 1].west    = (data & (1 << 11)) != 0;
			p_maze->map[i][j + 1].visited = (data & (1 << 12)) != 0;
			
			address++;
		}
	}
}