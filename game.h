#pragma once

//ゲーム全体のヘッダファイル

//ヘッダーファイルの読み込み
#include"DxLib.h"

// プログラムは WinMain から始まります

#define GAME_TITLE "ゲームタイトル" //ゲームタイトル
#define GAME_WIDTH 1280             //ゲームの画面の幅
#define GAME_HEIGHT 720             //ゲームの画面の高さ
#define GAME_COLOR 32               //ゲームの色域
#define GAME_ICON_ID 333			//ゲームのICONのID
#define GAME_WINDOW_BAR 0			//ウィンドウバーの種類
#define GAME_DEBUG TRUE			//デバッグモード

//列挙型
enum GAME_SCENE {
	GAME_SCENE_TITLE,
	GAME_SCENE_PLAY,
	GAME_SCENE_END,
	GAME_SCENE_CHANGE
};	//ゲームシーン