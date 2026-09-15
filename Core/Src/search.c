/*
 * search.c
 *
 *  Created on: 2026/06/17
 *      Author: Karasawa
 */
#include "global.h"
#include "search.h"
#include "sensor.h"
//#include "Core/Inc/search.h"
#include "drive.h"

MouseData mouse = { 0 }; //マウス情報を格納する構造体

//+++++++++++++++++++++++++++++++++++++++++++++++
// 関数名：SearchInit
// 内容：マウスの初期状態（座標、向き、探索フラグ、壁情報、経路カウンタ）の初期化を行う
// 引数：なし
// 戻り値：なし
//+++++++++++++++++++++++++++++++++++++++++++++++
void SearchInit(void) {
	mouse.second = false; //2回目の探索かどうかを初期化
	mouse.x = 0; //マウスのX座標を初期化
	mouse.y = 0; //マウスのY座標を初期化
	mouse.dir = KNorth; //マウスの方向を初期化
	mouse.section.north = false; //北方向の壁の有無を初期化
	mouse.section.east = false;  //東方向の壁の有無を初期化
	mouse.section.south = false; //南方向の壁の有無を初期化
	mouse.section.west = false;  //西方向の壁の有無を初期化
	mouse.section.visited = false; //訪問済みかどうかを初期化
	mouse.r_cnt = 0; //経路カウンタを初期化
	MazeInit(&maze); //迷路情報を初期化
}

//前方宣言
void GetSectionInfo(void);
void AdvancePosition(void);
void ConfirmRoute(void);
void OutputDebugMap(MazeData* p_maze);
void OutputDebugRoute(MazeData* p_maze);
//+++++++++++++++++++++++++++++++++++++++++++++++
// 関数名：SearchA
// 内容：左手法（左壁沿いアルゴリズム）を用いた探索走行を行う
// 引数：なし
// 戻り値：なし
//+++++++++++++++++++++++++++++++++++++++++++++++
void SearchA(void) {
	if (mouse.second) {//二次走行の場合は
		MazeReadFlsh(&maze);//フラッシュメモリから迷路情報を読み込む
	}
	//====スタート位置壁情報取得====
	GetSectionInfo(); //スタート位置の壁情報を取得
	mouse.section.north = false; //スタート位置の北側は壁がない
	MazeWriteWallMap(&maze, mouse.x, mouse.y, mouse.section); //スタート位置の壁情報を迷路マップに書き込む
	//====前に壁が無い想定で問答無用で前進====
	/*半区画前進関数*/
	DriveHalfSectionAccel(); //半区画加速走行
	AdvancePosition(); //マウスの座標を進行方向に1区画進める
	GetSectionInfo(); //進んだ先の壁情報を取得
	MazeWriteWallMap(&maze, mouse.x, mouse.y, mouse.section); //進んだ先の壁情報を迷路マップに書き込む
	//====経路作成====
	MazeMakeRouteLeftHand(&maze, mouse.x, mouse.y, mouse.dir); //左手法で次の1手を決定
	mouse.r_cnt = 0; //経路カウンタを初期化
	//====探索走行====
	while (mouse.x != maze.goal_x || mouse.y != maze.goal_y) {
		//----進行----
		switch (maze.route[mouse.r_cnt]) {
			//----前進----
		case KForward:
			DriveOneSection(); //半区画前進関数
			break;
			//----左90度回転----
		case KTurnL90:
			DriveTurnL90(); //左90度回転関数
			mouse.dir = (mouse.dir + 3) % 4; //左90度回転
			break;
			//----右90度回転----
		case KTurnR90:
			DriveTurnR90(); //右90度回転関数
			mouse.dir = (mouse.dir + 1) % 4; //右90度回転
			break;
			//----180度回転----
		case KTurn180:
			GetWallSensorInfo(); //センサーの壁情報を取得
			/*半区画減速走行をここに書く*/
			DriveHalfSectionBreak();
			if (sensor.wall_f && sensor.wall_l) {
				/*右旋回*/
				DriveRotateR90();
				/*位置合わせ*/
				DriveSetPosition();
				/*右旋回*/
				DriveRotateR90();
				/*位置合わせ*/
				DriveSetPosition();
			}
			else if (sensor.wall_f && sensor.wall_r) {
				/*左旋回*/
				DriveRotateL90();
				/*位置合わせ*/
				DriveSetPosition();
				/*左旋回*/
				DriveRotateL90();
				/*位置合わせ*/
				DriveSetPosition();
			}
			else {
				/*180度旋回*/
				DriveRotateR180();
			}
			/*半区画加速走行をここに書く*/
			DriveHalfSectionAccel();
			mouse.dir = (mouse.dir + 2) % 4; //180度回転
			break;
		default:
			break;
		}
		mouse.r_cnt++; //経路カウンタをインクリメント
		AdvancePosition(); //マウスの座標を進行方向に1区画進める
		GetSectionInfo(); //進んだ先の壁情報を取得
		MazeWriteWallMap(&maze, mouse.x, mouse.y, mouse.section); //進んだ先の壁情報を迷路マップに書き込む
		MazeMakeRouteLeftHand(&maze, mouse.x, mouse.y, mouse.dir); //左手法で次の1手を決定
		mouse.r_cnt = 0; //経路カウンタを初期化
	}
	////====ゴール到達====
	//ゴール到達時の処理
	DriveHalfSectionBreak();
	/*180度旋回をする*/
	DriveRotateR180();
	mouse.dir = (mouse.dir + 2) % 4; //180度回転
	if (!mouse.second) {
		//フラッシュメモリに迷路情報を書き込む
		MazeWriteFlsh(&maze);
	}
}

//+++++++++++++++++++++++++++++++++++++++++++++++
// 関数名：SearchB
// 内容：幅優先探索（足立法）を用いた迷路の探索・最短走行を行う
// 引数：なし
// 戻り値：なし
//+++++++++++++++++++++++++++++++++++++++++++++++
void SearchB(void) {
	//幅優先探索を用いた探索アルゴリズムBの実装
	if (mouse.second) {//二次走行の場合は
		MazeReadFlsh(&maze);//フラッシュメモリから迷路情報を読み込む
	}
	//====スタート位置壁情報取得====
	GetSectionInfo(); //スタート位置の壁情報を取得
	mouse.section.north = false; //スタート位置の北側は壁がない
	MazeWriteWallMap(&maze, mouse.x, mouse.y, mouse.section); //スタート位置の壁情報を迷路マップに書き込む
	//====前に壁が無い想定で問答無用で前進====
	/*半区画前進関数*/
	DriveHalfSectionAccel(); //半区画加速走行
	AdvancePosition(); //マウスの座標を進行方向に1区画進める
	GetSectionInfo(); //進んだ先の壁情報を取得
	MazeWriteWallMap(&maze, mouse.x, mouse.y, mouse.section); //進んだ先の壁情報を迷路マップに書き込む
	//====経路作成====
	MazeMakeStepMap(&maze, mouse.x, mouse.y, mouse.second); //歩数マップを作成
	MazeMakeRoute(&maze, mouse.x, mouse.y, mouse.dir); //歩数マップから経路を作成
	mouse.r_cnt = 0; //経路カウンタを初期化
	//====探索走行====
	while (mouse.x != maze.goal_x || mouse.y != maze.goal_y) {
		//----進行----
		switch (maze.route[mouse.r_cnt]) {
			//----前進----
		case KForward:
			DriveOneSection(); //半区画前進関数
			break;
			//----左90度回転----
		case KTurnL90:
			DriveTurnL90(); //左90度回転関数
			mouse.dir = (mouse.dir + 3) % 4; //左90度回転
			break;
			//----右90度回転----
		case KTurnR90:
			DriveTurnR90(); //右90度回転関数
			mouse.dir = (mouse.dir + 1) % 4; //右90度回転
			break;
			//----180度回転----
		case KTurn180:
			GetWallSensorInfo(); //センサーの壁情報を取得
			/*半区画減速走行をここに書く*/
			DriveHalfSectionBreak();
			if (sensor.wall_f && sensor.wall_l) {
				/*右旋回*/
				DriveRotateR90();
				/*位置合わせ*/
				DriveSetPosition();
				/*右旋回*/
				DriveRotateR90();
				/*位置合わせ*/
				DriveSetPosition();
			}
			else if (sensor.wall_f && sensor.wall_r) {
				/*左旋回*/
				DriveRotateL90();
				/*位置合わせ*/
				DriveSetPosition();
				/*左旋回*/
				DriveRotateL90();
				/*位置合わせ*/
				DriveSetPosition();
			}
			else {
				DriveRotateR180();
				/*180度旋回*/
			}
			/*半区画加速走行をここに書く*/
			DriveHalfSectionAccel();
			mouse.dir = (mouse.dir + 2) % 4; //180度回転
			break;
		default:
			break;
		}
		mouse.r_cnt++; //経路カウンタをインクリメント
		AdvancePosition(); //マウスの座標を進行方向に1区画進める
		GetSectionInfo(); //進んだ先の壁情報を取得
		MazeWriteWallMap(&maze, mouse.x, mouse.y, mouse.section); //進んだ先の壁情報を迷路マップに書き込む
		ConfirmRoute(); //進行方向に壁がある場合は、経路を修正する
	}
	////====ゴール到達====
	//ゴール到達時の処理
	DriveHalfSectionBreak();
	/*180度旋回をする*/
	DriveRotateR180();
	mouse.dir = (mouse.dir + 2) % 4; //180度回転
	if (!mouse.second) {
		//フラッシュメモリに迷路情報を書き込む
		MazeWriteFlsh(&maze);
	}
}

//+++++++++++++++++++++++++++++++++++++++++++++++
// 関数名：SearchC
// 内容：探索アルゴリズムCを実行する（拡張用）
// 引数：なし
// 戻り値：なし
//+++++++++++++++++++++++++++++++++++++++++++++++
void SearchC(void) {
	//探索アルゴリズムCの実装
	//ここに探索アルゴリズムCのコードを追加する
}

//+++++++++++++++++++++++++++++++++++++++++++++++
// 関数名：GetSectionInfo
// 内容：壁センサーから相対的な壁情報を取得し、現在のマウスの向きに応じた絶対方角の壁情報に変換・更新する
// 引数：なし
// 戻り値：なし
//+++++++++++++++++++++++++++++++++++++++++++++++
void GetSectionInfo(void) {
	//センサーの相対的な壁情報を取得
	GetWallSensorInfo(); //センサーの壁情報を取得
	//絶対的な方角の壁情報に変換する
	switch (mouse.dir) {
	case KNorth:
		mouse.section.north = sensor.wall_f;
		mouse.section.east = sensor.wall_r;
		mouse.section.south = false; //後ろの壁はセンサーで検知できないため、falseに設定
		mouse.section.west = sensor.wall_l;
		break;
	case KEast:
		mouse.section.north = sensor.wall_l;
		mouse.section.east = sensor.wall_f;
		mouse.section.south = sensor.wall_r;
		mouse.section.west = false; //後ろの壁はセンサーで検知できないため、falseに設定
		break;
	case KSouth:
		mouse.section.north = false; //後ろの壁はセンサーで検知できないため、falseに設定
		mouse.section.east = sensor.wall_l;
		mouse.section.south = sensor.wall_f;
		mouse.section.west = sensor.wall_r;
		break;
	case KWest:
		mouse.section.north = sensor.wall_r;
		mouse.section.east = false; //後ろの壁はセンサーで検知できないため、falseに設定
		mouse.section.south = sensor.wall_l;
		mouse.section.west = sensor.wall_f;
		break;
	default:
		break;
	}
	mouse.section.visited = true; //訪問済みに設定
}

//+++++++++++++++++++++++++++++++++++++++++++++++
// 関数名：AdvancePosition
// 内容：現在のマウスの向きに基づき、自己座標（x, y）を進行方向に1区画更新する
// 引数：なし
// 戻り値：なし
//+++++++++++++++++++++++++++++++++++++++++++++++
void AdvancePosition(void) {
	//マウスの座標を進行方向に1区画進める
	switch (mouse.dir) {
	case KNorth:
		mouse.y += 1;
		break;
	case KEast:
		mouse.x += 1;
		break;
	case KSouth:
		mouse.y -= 1;
		break;
	case KWest:
		mouse.x -= 1;
		break;
	default:
		break;
	}
}

//+++++++++++++++++++++++++++++++++++++++++++++++
// 関数名：ConfirmRoute
// 内容：一次走行時において、次の予定動作の進行方向に壁が存在するか判定し、壁で遮られている場合は歩数マップと経路の再計算を行う
// 引数：なし
// 戻り値：なし
//+++++++++++++++++++++++++++++++++++++++++++++++
void ConfirmRoute(void) {
	if (!mouse.second) {
		//進行方向に壁がある場合は、経路を再計算する
		bool is_blocked = false;
		switch (maze.route[mouse.r_cnt]) {
		case KForward:
			if (sensor.wall_f) is_blocked = true;
			break;
		case KTurnL90:
			if (sensor.wall_l) is_blocked = true;
			break;
		case KTurnR90:
			if (sensor.wall_r) is_blocked = true;
			break;
		case KTurn180:
			//180度ターンの場合は壁がない
			break;
		default:
			//不正な値の場合は壁があるとみなす
			is_blocked = true;
			break;
		}
		if (is_blocked) {
			//経路を再計算する
			MazeMakeStepMap(&maze, mouse.x, mouse.y, mouse.second); //迷路情報から歩数マップを作成
			MazeMakeRoute(&maze, mouse.x, mouse.y, mouse.dir); //歩数マップから経路を作成
			mouse.r_cnt = 0; //経路カウンタを初期化
		}
	}
}
//以下デバッグ用の関数群
void TestSearch(void) {
	int mode = 0;
	while (1) 
	{
		mode = select_mode(mode);
		switch (mode) {
			case 0:
				printf("Mode 0 selected\n");
				//迷路データの初期化
				MazeInit(&maze);
				break;
			case 1:
				printf("Mode 1 selected\n");
				//探索データの初期化
				SearchInit();
				break;
			case 2:
				printf("Mode 2 selected\n");
				//フラッシュから迷路データを読み込む
				MazeReadFlsh(&maze);
				break;
			case 3:
				printf("Mode 3 selected\n");
				//迷路データの表示
				OutputDebugMap(&maze);
				break;
			case 4:
				printf("Mode 4 selected\n");
				//経路データの表示
				OutputDebugRoute(&maze);
				break;
			case 5:
				printf("Mode 5 selected\n");
				//左手法でルートを作成
				MazeMakeRouteLeftHand(&maze, mouse.x, mouse.y, mouse.dir);
				break;
			case 6:
				printf("Mode 6 selected\n");
				//幅優先探索でルートを作成
				MazeMakeStepMap(&maze, mouse.x, mouse.y, mouse.second);
				MazeMakeRoute(&maze, mouse.x, mouse.y, mouse.dir);
				break;
		}
	}
}
//+++++++++++++++++++++++++++++++++++++++++++++++
// OutputDebugMap
// 現在の迷路マップ情報とマウス・ゴールの位置をコンソールへ出力する
// 引数：MazeData* p_maze - 迷路データへのポインタ
// 戻り値：なし
//+++++++++++++++++++++++++++++++++++++++++++++++
void OutputDebugMap(MazeData* p_maze) {
	printf("map:\n");

	for (int y = MAZE_SIZE - 1; y >= 0; y--) {
		//----[1] セル上部（北壁）----
		for (int x = 0; x < MAZE_SIZE; x++) {
			printf(p_maze->map[x][y].north ? "+---+" : "+   +");
		}
		printf("\n");

		//----[2] セル中央（西壁 / マウス・ゴール / 東壁）----
		for (int x = 0; x < MAZE_SIZE; x++) {
			// 左右の壁文字を決定
			char west = p_maze->map[x][y].west ? '|' : ' ';
			char east = p_maze->map[x][y].east ? '|' : ' ';

			// セル内部の文字列を決定（デフォルトは空白）
			const char* body = "   ";
			if (x == mouse.x && y == mouse.y) {
				body = "\x1b[42mMSE\x1b[49m"; // マウス位置（緑）
			}
			else if (x == p_maze->goal_x && y == p_maze->goal_y) {
				body = "\x1b[41mGOL\x1b[49m"; // ゴール位置（赤）
			}

			printf("%c%s%c", west, body, east);
		}
		printf("\n");

		//----[3] セル下部（南壁）----
		for (int x = 0; x < MAZE_SIZE; x++) {
			printf(p_maze->map[x][y].south ? "+---+" : "+   +");
		}
		printf("\n");
	}
}
//+++++++++++++++++++++++++++++++++++++++++++++++
// 関数名：OutputDebugRoute
// 内容：現在の迷路経路データをコンソールへ出力する
// 引数：MazeData* p_maze - 迷路データへのポインタ
// 戻り値：なし
//+++++++++++++++++++++++++++++++++++++++++++++++
void OutputDebugRoute(MazeData* p_maze) {
	printf("Route Data (count: %d):\n", p_maze->r_cnt);

	// maze.r_cnt に経路の総手数が格納されている前提で出力します
	// ※必要に応じてループ回数は MAX_ROUTE に変更してください
	for (int i = 0; i < p_maze->r_cnt; i++) {
		switch (p_maze->route[i]) {
		case KForward:
			printf("F "); // 直進
			break;
		case KTurnL90:
			printf("L "); // 左90度ターン
			break;
		case KTurnR90:
			printf("R "); // 右90度ターン
			break;
		case KTurn180:
			printf("U "); // Uターン(180度ターン)
			break;
		default:
			printf("- ");
			break;
		}
	}
	printf("\n");
}
