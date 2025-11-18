# pragma once
# include <Siv3D.hpp>
# include "Piece.hpp"

using Scene::Width;
using Scene::Height;

// シーンのステート
enum class State
{
	Title,
	Level,
	Game,
	Ranking,
};

// 共有するデータ
struct GameData
{
	// 直前のゲームのスコア
	int32 lastScore = 0;

	// ハイスコア
	Array<int32> highScores = { 10, 8, 6, 4, 2 };

	// ピースの配列
	Array<Piece> pieces;

	Array<Point> correctPositions;
	Point correctCenter;

	int16 gridSize = Width() / 25;

	int16 fetchGridSize() {
		gridSize = Width() / 25;
		return gridSize;
	}

	void drawGrid() const {
		Scene::SetBackground(Palette::White);
		for (int i = 0; i < Width(); i += gridSize) {
			for (int j = 0; j < Height(); j += gridSize) {
				Circle{ i, j, 1 }.draw(Palette::Black);
			}
		}
	}

	int currentLevel = -1;

	Array<bool> isCleared;
};

// 画面遷移時間
static constexpr Duration TitleFadeInDuration{ 0.4s };
static constexpr Duration TitleToLevelDuration{ 2.0s };
static constexpr Duration LevelToGameDuration{ 3.5s };
static constexpr Duration GameToGameDuration{ 3.0s };
static constexpr Duration GameToLevelDuration{ 3.0s };

// 減法混色の三原色
static constexpr ColorF SubtractiveCyan = ColorF{ 1.0, 0.0, 0.0 };
static constexpr ColorF SubtractiveMagenta = ColorF{ 0.0, 1.0, 0.0 };
static constexpr ColorF SubtractiveYellow = ColorF{ 0.0, 0.0, 1.0 };

using App = SceneManager<State, GameData>;
