#include <iostream>
#include <cstdlib> // rand, srand用
#include <ctime>   // time用
#include <limits>  // for numeric_limits
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>

using namespace std;

// スコアを扱うための構造体
struct GameRecord {
	string name;
	int score;
};

class NumberGuessGame {
private:
	// ゲームの内部データ
	int answer;        // 正解の数字
	int max_range;     // 数字の範囲上限
	int max_attempts;  // 最大試行回数
	int hint_timings;  // ヒント表示のタイミング
	int difficulty;    // 難易度レベル
	int count;         // 試行回数
	string playerName; // プレイヤー名

	// 内部だけで使うヒント表示処理
	void showHint() {
		int hint;
		cout << "\n--- ヒント! ---\n";

		switch (difficulty) {
		case 1:
			cout << "\nどのヒントが欲しいですか？\n1: 偶数か奇数か\n2: 前半か後半か\n3: 10の位の数字\n>";
			cin >> hint;

			if (hint == 1) {
				cout << "ヒント: 答えは " << (answer % 2 == 0 ? "偶数" : "奇数") << " です。\n";
			} else if (hint == 2) {
				cout << "ヒント: 答えは " << (answer < (max_range / 2) ? "前半" : "後半") << " の範囲にあります。\n";
			} else if (hint == 3) {
				if (answer < 10) {
					cout << "この数字は1桁です（10の位は0です）。\n";
				} else {
					cout << "ヒント: 答えの10の位の数字は " << (answer / 10) % 10 << " です。\n";
				}
			} else {
				cout << "無効な選択です。ヒントをスキップします。\n";
			}
			break;

		case 2:
			cout << "\nどのヒントが欲しいですか？\n1: 前半か後半か\n2: 10の位の数字\n>";
			cin >> hint;

			if (hint == 1) {
				cout << "ヒント: 答えは " << (answer < (max_range / 2) ? "前半" : "後半") << " の範囲にあります。\n";
			} else if (hint == 2) {
				if (answer < 10) {
					cout << "この数字は1桁です（10の位は0です）。\n";
				} else if (answer == 100) {
					cout << "10の位と1の位は0です。\n";
				} else {
					cout << "ヒント: 答えの10の位の数字は " << (answer / 10) % 10 << " です。\n";
				}
			} else {
				cout << "無効な選択です。ヒントをスキップします。\n";
			}
			break;

		case 3:
			cout << "\n何桁目ヒントが欲しいですか？\n1: 1の位\n2: 10の位\n3: 100の位\n>";
			cin >> hint;
			
			if (hint == 1) {
				cout << "1の位は" << (answer % 10) << "です。\n";
			} else if (hint == 2) {
				cout << "10の位は" << (answer / 10) % 10 << "です。\n";
			} else if (hint == 3) {
				cout << "100の位は" << (answer / 100) << "です。\n";
			} else {
				cout << "無効な選択です。ヒントをスキップします。\n";
			}
			break;
		}
		cout << "---------------\n";
	}

public:
	// コンストラクタ（初期化）
	NumberGuessGame() {
		std::srand(static_cast<unsigned int>(std::time(nullptr)));
		count = 0;
	}

	// 難易度設定と初期化
	void setup() {
		cout << "あなたの名前を入力してください\n>";
		cin >> playerName;

		cout << "難易度を選んでください（1:初級, 2:中級, 3:上級）\n>";
		if (!(std::cin >> difficulty)) {
			cin.clear();
			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			difficulty = 2; // デフォルト
		}

		switch (difficulty) {
		case 1: // 初級
			max_range = 50;
			max_attempts = 15;
			hint_timings = 3;		   // 3回目にヒント
			break;
		case 2: // 中級
			max_range = 100;
			max_attempts = 10;
			hint_timings = 5;		  // 5回目にヒント
			break;
		case 3: // 上級
			max_range = 200;
			max_attempts = 5;
			hint_timings = 4;        // 4回目にヒント（最終局面）
			break;
		default:
			max_range = 100;
			max_attempts = 10;
			hint_timings = 5;		  // 5回目にヒント
			// 整合性のため difficulty を中級に揃える
			difficulty = 2;
			break;
		}
		answer = std::rand() % (max_range + 1); // 0からmax_rangeまでの乱数を生成
		cout << " --- 数当てゲーム開始！ --- \n";
	}

	// ゲームのメインループ
	void play() {
		int guess;
		while (true) {
			cout << "数字を入力してください。(0 ～ " << max_range << ")\n>";
			cin >> guess;

			// 入力の範囲チェック
			if (guess < 0 || guess > max_range) {

				cout << "エラー：0から" << max_range << "の間で入力してください！\n";
				continue;
			}

			count++; // 試行回数を増やす

			// 判定
			if (guess < answer) {
				cout << "もっと大きい数字です。\n";
			}
			else if (guess > answer) {
				cout << "もっと小さい数字です。\n";
			}
			else {
				cout << "正解です！あなたは " << count << " 回で正解しました。\n";
				saveScore(count); // スコア保存
				break; // 勝利！
			}

			// 最大試行回数のチェック
			if (count >= max_attempts) {
				cout << "残念！終了です。正解は " << answer << " でした。\n";
				break; // 敗北
			}

			// ヒントの表示
			if (count == hint_timings) {
				showHint();
			}
		}
	}

	// ランキング表示
	void showRanking() {
		ifstream ifs("ranking.txt");

		if (!ifs) {
			// ファイルがない（一度も遊んでいない）場合
			cerr << "--- まだ記録がありません。あなたが最初のプレイヤーです！ ---\n";
			return;
		}

		vector<GameRecord> records;
		string line;
		
		// ファイルデータを読み込み、構造体にして vector に入れる
		while (getline(ifs, line)) {
			// "Player: 5回"という文字列から数字を抽出する処理
			size_t colonPos = line.find(": ");
			size_t kaiPos = line.find("回");

			if (colonPos != string::npos && kaiPos != string::npos) {
				// 0文字目からコロンの前までが「名前」
				string name = line.substr(0, colonPos);

				// コロンの2文字後から「回」の前までが「スコア」
				int score = stoi(line.substr(colonPos + 2, kaiPos - (colonPos + 2)));
				records.push_back({ name, score });
			}
		}
		ifs.close();

		// スコアでソート（昇順）
		sort(records.begin(), records.end(), [](const GameRecord& a, const GameRecord& b) {
			return a.score < b.score;
			});

		// 上位5件を表示
		cout << "\n=== ランキング ===\n";
		int displayCount = min((int)records.size(), 5);
		for (int i = 0; i < displayCount; ++i) {
			cout << i + 1 << "位: " << records[i].name << " - " << records[i].score << " 回\n";
		}
		cout << "==================\n\n";
		
	}

	void saveScore(int score) {
		// ranking.txt というファイルを「追記モード(app)」で開く
		ofstream ofs("ranking.txt", ios::app);

		if (!ofs) {
			cerr << "ファイルを開けませんでした。\n";
			return;
		}

		// プレイヤー名とスコアに書き込む
		ofs << playerName << ": " << score << " 回\n";

		cout << "スコアを保存しました！\n";
		ofs.close();
	}
};

int main() {
	// クラスを実態化(インスタンス化)して実行
	NumberGuessGame game;
	game.showRanking();
	game.setup();
	game.play();

	return 0;
}