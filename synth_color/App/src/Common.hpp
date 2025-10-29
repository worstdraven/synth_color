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
	Array<std::unique_ptr<BasePiece>> pieces;
};

using App = SceneManager<State, GameData>;
