#include "DxLib.h"
#include <string>
#include <vector>
#include <algorithm>
#include <ctime>

using namespace std;

class NumberGuessGame {
public:
	// ゲームの内部データ
	int answer;        // 正解の数字
	int max_range;     // 数字の範囲上限
	int max_attempts;  // 最大試行回数
	int hint_timings;  // ヒント表示のタイミング
	int difficulty;    // 難易度レベル
	int count;         // 試行回数
	string playerName; // プレイヤー名
	string message;    // ゲームメッセージ
	bool isGameOver;  // ゲームオーバーフラグ

	NumberGuessGame() {
		srand(static_cast<unsigned int>(time(nullptr)));
		max_range = 100;
		answer = rand() % max_range + 1;
		count = 0;
		message = "数字当てゲームへようこそ！\n1から100の間の数字を予想してね！";
		isGameOver = false;
	}

	void checkGuess(int guess) {
		count++;
		if (guess < answer) {
			message = "もっと大きい数字だよ！";
		}
		else if (guess > answer) {
			message = "もっと小さい数字だよ！";
		}
		else {
			message = "正解！おめでとう！";
			isGameOver = true;
		}
	}

};

// メイン関数
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	ChangeWindowMode(TRUE); // ウィンドウモード

	// DXライブラリの初期化
	if (DxLib_Init() == -1) {
		return -1; // エラーが起きたら直ちに終了
	}

	SetDrawScreen(DX_SCREEN_BACK); // 裏画面を描画対象にする

	NumberGuessGame game; // ゲームオブジェクトの作成
	int myInput = 0;      // プレイヤーの入力
	unsigned int white = GetColor(255, 255, 255); // 白色
	unsigned int yellow = GetColor(255, 255, 0); // 黄色
	SetFontSize(20); // フォントサイズ設定
	
	// タイトル文字
	char title[] = "=== 数当てゲーム ===";
	int titleWidth = GetDrawStringWidth(title, -1);
	int titleX = (640 - titleWidth) / 2; // 640はデフォルトの画面幅

	// メインループ
	while (ProcessMessage() == 0 && ClearDrawScreen() == 0 ) {
		
		// 描画処理
		DrawBox(50, 50, 590, 430, GetColor(50, 50, 50), TRUE); // 背景枠
		DrawString(titleX, 80, title, yellow);
		DrawFormatString(100, 130, white, "現在の試行回数: %d 回", game.count);

		if (game.count == 0) {
			DrawFormatString(100, 170, GetColor(100, 255, 100), "%s", game.message.c_str());
		} else {
			DrawFormatString(100, 170, GetColor(100, 255, 100), "ヒント: %s", game.message.c_str());
		}

		if (!game.isGameOver) {
			DrawString(100, 250, "[Enter]キーを押して数字を入力してください", white);

			// 入力処理
			if (CheckHitKey(KEY_INPUT_RETURN)) {
				// 一旦画面をリフレッシュして入力を促す
				ScreenFlip();
				myInput = KeyInputNumber(100, 280, 200, 0, FALSE);

				// クラスのロジックを呼び出す
				game.checkGuess(myInput);
			}
		} else {
			// 500ミリ秒ごとに色を切り替える
			if ((GetNowCount() / 500) % 2 == 0) {
				DrawFormatString(150, 300, yellow, "＼( ^o^)／ 正解！おめでとう！ ＼(^o^ )／");
			}
			DrawString(100, 350, "Escキーで終了、Rキーでもう一度遊ぶ", white);

			if (CheckHitKey(KEY_INPUT_R)) {
				game = NumberGuessGame(); // コンストラクタを呼び出してリセット
			}
		}

		if (CheckHitKey(KEY_INPUT_ESCAPE)) {
			break;
		}

		ScreenFlip(); // 裏画面と表画面を入れ替える

	}

	// DXライブラリの終了処理
	DxLib_End();

	return 0;
}