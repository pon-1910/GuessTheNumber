#include <iostream>
#include <cstdlib> // rand, srand用
#include <ctime>   // time用
#include <limits>  // for numeric_limits

using namespace std;

int main() {
	// 乱数の種を初期化
	std::srand(static_cast<unsigned int>(std::time(nullptr))); 

	int difficulty;	  // 難易度選択用変数
	int max_range;	  // 数字の範囲上限
	int max_attempts; // 最大試行回数
	int hint_timings; // ヒント表示のタイミング
	int hint;		  // ヒントの内容

	std::cout << "難易度を選んでください（1:初級, 2:中級, 3:上級）\n" ;
	if (!(std::cin >> difficulty)) {

		cin.clear();
		cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		cout << "入力が無効です。中級モードで開始します。\n";
		difficulty = 2;

	}
	
	// 難易度ごとのヒント表示タイミングを定義
	switch (difficulty) {
		case 1: // 初級
			max_range = 50;
			max_attempts = 15;
			hint_timings = 3;		   // 3回目にヒント
			std::cout << " --- 数当てゲーム(初級) --- \n";			
			break;
		case 2: // 中級
			max_range = 100;
			max_attempts = 10;
			hint_timings = 5;		  // 5回目にヒント
			std::cout << " --- 数当てゲーム(中級) --- \n";
			break;
		case 3: // 上級
			max_range = 200;
			max_attempts = 5;
			hint_timings = 4;        // 4回目にヒント（最終局面）
			std::cout << " --- 数当てゲーム(上級) --- \n";
			break;
		default:
			cout << "無効な選択です。中級モードで開始します。\n";
			max_range = 100;
			max_attempts = 10;
			hint_timings = 5;		  // 5回目にヒント
			std::cout << " --- 数当てゲーム(中級) --- \n";
			// 整合性のため difficulty を中級に揃える
			difficulty = 2;
			break;
	}

	int answer = std::rand() % (max_range + 1); // 0からmax_rangeまでの乱数を生成
	int guess;
	int count = 0; // 試行回数をカウントする変数

	std::cout << " 私が考えた数字を当てて見てね！ \n";

	// メインループ
	while (true) {
		std::cout << "数字を入力してください。(0 ～ " << max_range << ")\n";
		std::cout << "> ";
		std::cin >> guess;

		// 入力の範囲チェック
		if (guess < 0 || guess > max_range) {

			std::cout << "エラー：0から" << max_range << "の間で入力してください！\n";
			continue;

		}

		count++; // 試行回数を増やす

		// 判定
		if (guess < answer) {

			std::cout << "もっと大きい数字です。\n";

		} else if (guess > answer) {
			
			std::cout << "もっと小さい数字です。\n";

		} else {
			
			std::cout << "正解です！おめでとうございます！\n";
			std::cout << "あなたは " << count << " 回で正解しました。\n";
			break; // 正解したらループを抜ける

		}

		// 最大試行回数のチェック
		if (count >= max_attempts) {

			std::cout << "残念！最大試行回数を超えました。正解は " << answer << " でした。\n";
			break;
		}

		// ヒントの表示
		if (count == hint_timings) {

			std::cout << "\n--- ヒント! ---\n";

			switch (difficulty) {
			case 1:
				std::cout << "\nどのヒントが欲しいですか？\n";
				std::cout << "1: 偶数か奇数か\n";
				std::cout << "2: 前半か後半か\n";
				std::cout << "3: 10の位の数字\n";
				std::cin >> hint;

				if (hint == 1) {
					
					std::cout << "ヒント: 答えは " << (answer % 2 == 0 ? "偶数" : "奇数") << " です。\n";

				} else if (hint == 2) {

					std::cout << "ヒント: 答えは " << (answer < (max_range / 2) ? "前半" : "後半") << " の範囲にあります。\n";

				} else if (hint == 3) {
					if (answer < 10) {

						std::cout << "この数字は1桁です（10の位は0です）。\n";

					} else {

						std::cout << "ヒント: 答えの10の位の数字は " << (answer / 10) % 10 << " です。\n";

					}
				} else {

					std::cout << "無効な選択です。ヒントをスキップします。\n";

				}
				break;
				
			case 2:
				std::cout << "\nどのヒントが欲しいですか？\n";
				std::cout << "1: 前半か後半か\n";
				std::cout << "2: 10の位の数字\n";
				std::cin >> hint;

				if (hint == 1) {

					std::cout << "ヒント: 答えは " << (answer < (max_range / 2) ? "前半" : "後半") << " の範囲にあります。\n";

				} else if (hint == 2) {
					if (answer < 10) {

						std::cout << "この数字は1桁です（10の位は0です）。\n";

					}
					else if (answer == 100) {

						std::cout << "10の位と1の位は0です。\n";

					}else {

						std::cout << "ヒント: 答えの10の位の数字は " << (answer / 10) % 10 << " です。\n";

					}
				} else {

					std::cout << "無効な選択です。ヒントをスキップします。\n";

				}
				break;

			case 3:
				std::cout << "\n何桁目ヒントが欲しいですか？\n";
				std::cout << "1: 1の位\n";
				std::cout << "2: 10の位\n";
				std::cout << "3: 100の位\n";

				std::cin >> hint;

				if (hint == 1) {

					std::cout << "1の位は" << (answer % 10) << "です。\n";

				} else if (hint == 2) {
					
					std::cout << "10の位は" << (answer / 10) % 10 << "です。\n";

				} else if (hint == 3) {
					
					std::cout << "10の位は" << (answer / 100) << "です。\n";

				} else {
					
					std::cout << "無効な選択です。ヒントをスキップします。\n";

				}
				break;
			}

			std::cout << "---------------\n";
		}
	}
	return 0;
}