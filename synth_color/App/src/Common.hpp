# pragma once
# include <Siv3D.hpp>
# include "Piece.hpp"

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

	int16 gridSize = Scene::Width() / 25;

	void drawGrid() const {
		Scene::SetBackground(Palette::White);
		for (int i = 0; i < Scene::Width(); i += gridSize) {
			for (int j = 0; j < Scene::Height(); j += gridSize) {
				Circle{ i, j, 1 }.draw(Palette::Black);
			}
		}
	}
};

static constexpr ColorF SubtractiveCyan = ColorF{ 1.0, 0.0, 0.0 };
static constexpr ColorF SubtractiveMagenta = ColorF{ 0.0, 1.0, 0.0 };
static constexpr ColorF SubtractiveYellow = ColorF{ 0.0, 0.0, 1.0 };

using App = SceneManager<State, GameData>;
