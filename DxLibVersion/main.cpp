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

enum GameScene {
	SCENE_NAME_INPUT, // プレイヤー名入力シーン
	SCENE_GAME_MAIN   // ゲームメインシーン
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
	GameScene currentScene = SCENE_NAME_INPUT; // 最初のシーンはプレイヤー名入力

	char inputName[64] = ""; // プレイヤー名入力用バッファ
	int inputHandle = -1;    // 入力ボックスの完了番号

	// 共通色
	unsigned int white = GetColor(255, 255, 255); // 白色
	unsigned int yellow = GetColor(255, 255, 0);  // 黄色

	SetFontSize(20); // フォントサイズ設定

	while (ProcessMessage() == 0 && ClearDrawScreen() == 0 ) {
		// シーンごとに切り替え
		if (currentScene == SCENE_NAME_INPUT) {
			// プレイヤー名入力シーン
			char askMsg[] = "プレイヤー名を入力してください(Enterで決定)";
			int askWidth = GetDrawStringWidth(askMsg, -1);
			DrawString((640 - askWidth) / 2, 100, askMsg, white);

			// 初めてこのシーンに来た時に入力ボックスを作る
			if (inputHandle == -1) {
				// x, y, 長さ,キャンセル不可
				inputHandle = MakeKeyInput(64, FALSE, FALSE, FALSE);
				SetActiveKeyInput(inputHandle); // 入力可能状態にする
			}

			// 入力ボックスを描画
			DrawKeyInputString(100, 140, inputHandle);

			// 入力が完了（Enterが押された）したかチェック
			if (CheckKeyInput(inputHandle) != 0) {
				GetKeyInputString(inputName, inputHandle); // 入力内容をコピー
				game.playerName = inputName;               // クラスにプレイヤー名をセット
				DeleteKeyInput(inputHandle);               // 入力ボックスを削除
				inputHandle = -1;                         // ハンドルをリセット
				currentScene = SCENE_GAME_MAIN;            // ゲームメインシーンに切り替え

				while (CheckHitKey(KEY_INPUT_RETURN) != 0) {
					// Enterキーが押されている間は待機（これでEnterの二重入力を防止）
					ProcessMessage();
				}
			}
		} else if (currentScene == SCENE_GAME_MAIN) {
			DrawBox(50, 50, 590, 430, GetColor(50, 50, 50), TRUE); // 背景枠
			
			// タイトル中心寄せ（画面幅 640 想定）
			char title[] = "=== 数当てゲーム ===";
			int titleWidth = GetDrawStringWidth(title, -1);
			int titleX = (640 - titleWidth) / 2;
			DrawString(titleX, 80, title, yellow);

			// プレイヤー名/試行回数/ヒント
			DrawFormatString(100, 110, GetColor(255, 255, 0), "プレイヤー: %s さん", game.playerName.c_str());
			DrawFormatString(100, 150, white, "現在の試行回数: %d 回", game.count);
			if (game.count == 0) {
				DrawString(100, 190, game.message.c_str(), GetColor(100, 255, 100));
			}
			else {
				DrawFormatString(100, 170, GetColor(100, 255, 100), "ヒント: %s", game.message.c_str());
			}

			// 入力案内/入力処理
			if (!game.isGameOver) {
				DrawString(100, 250, "[Enter]キーを押して数字を入力してください", white);

				// 入力処理
				if (CheckHitKey(KEY_INPUT_RETURN)) {
					ScreenFlip(); // 入力前に画面反映
					int myInput = KeyInputNumber(100, 280, 200, 0, FALSE);
					game.checkGuess(myInput); // クラスのロジックを呼び出す

					while (CheckHitKey(KEY_INPUT_RETURN) != 0) {
						// Enterキーが押されている間は待機（これでEnterの二重入力を防止）
						ProcessMessage();
					}
				}
			}
			else {
				// 500ミリ秒ごとに色を切り替える
				if ((GetNowCount() / 500) % 2 == 0) {
					DrawFormatString(150, 300, yellow, "＼( ^o^)／ 正解！おめでとう！ ＼(^o^ )／");
				}
				DrawString(100, 350, "Escキーで終了、Rキーでもう一度遊ぶ", white);

				if (CheckHitKey(KEY_INPUT_R)) {
					// プレイヤー名入力画面へ戻す準備
					game = NumberGuessGame(); // ゲームオブジェクトをリセット
					inputName[0] = '\0';  // 入力バッファをリセット
					currentScene = SCENE_NAME_INPUT; // シーンを切り替え
				}
			}

		}

		// 共通終了キー
		if (CheckHitKey(KEY_INPUT_ESCAPE)) {
			break;
		}

		ScreenFlip(); // 裏画面と表画面を入れ替える
	}

	// 入力ハンドルが残っていたら削除
	if (inputHandle != -1) {
		DeleteKeyInput(inputHandle);
	}

	// DXライブラリの終了処理
	DxLib_End();
	return 0;
}