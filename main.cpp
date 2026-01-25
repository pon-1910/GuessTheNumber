#include <iostream>
#include <cstdlib> // rand, srand用
#include <ctime> // time用

using namespace std;

int main() {
	// 乱数の種を初期化
	std::srand(static_cast<unsigned int>(std::time(nullptr))); 

	int answer = std::rand() % 101; // 0から100までの乱数を生成
	int guess;
	int count = 0; // 試行回数をカウントする変数
	int max_attempts = 10; // 最大試行回数

	std::cout << " --- 数当てゲーム(0 ～ 100) --- " << std::endl;
	std::cout << " 私が考えた数字を当てて見てね！ " << std::endl;

	// メインループ
	while (true) {
		std::cout << "数字を入力してください。(0 ～ 100)" << std::endl;
		std::cout << "> ";
		std::cin >> guess;

		// 入力の範囲チェック
		if (guess < 0 || guess > 100) {
			std::cout << "エラー：0から100の間で入力してください！" << std::endl;
			continue;
		}

		count++; // 試行回数を増やす

		// 判定
		if (guess < answer) {
			std::cout << "もっと大きい数字です。" << std::endl;
		} else if (guess > answer) {
			std::cout << "もっと小さい数字です。" << std::endl;
		} else {
			std::cout << "正解です！おめでとうございます！" << std::endl;
			std::cout << "あなたは " << count << " 回で正解しました。" << std::endl;
			break; // 正解したらループを抜ける
		}

		// 最大試行回数のチェック
		if (count >= max_attempts) {
			std::cout << "残念！最大試行回数を超えました。正解は " << answer << " でした。" << std::endl;
			break;
		}

		// ヒントの表示
		if (count == 5) {
			std::cout << "\n--- ヒント! ---" << std::endl;
			if (answer < 10) {
				std::cout << "この数字は1桁です（10の位は0です）。" << std::endl;
			} else if (answer == 100) {
				std::cout << "10の位と1の位は0です。" << std::endl;
			} else {
				std::cout << "10の位は" << (answer / 10) << "です。" << std::endl;
			}
			std::cout << "---------------\n" << std::endl;
		}
	}

	return 0;
}