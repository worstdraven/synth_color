# pragma once
# include "Common.hpp"


// ゲームシーン
class Game : public App::Scene
{
public:

	Game(const InitData& init);

	~Game();

	void update() override;

	void draw() const override;

	void updateFadeOut(double t) override;

	void drawFadeOut(double t) const override;

private:
	bool checkPuzzleClear() const;

	// ドラッグ中の図形のインデックス
	int m_selectedPieceIndex = -1;

	// 図形の配列
	Array<Piece>& m_pieces;

	//Point m_puzzleViewportTopLeft{ static_cast<int>(Scene::Width() * 0.45), static_cast<int>(Scene::Height() * 0.15) };
	Rect m_puzzleViewportRect{
		static_cast<int>(Scene::Width() * 0.45), static_cast<int>(Scene::Height() * 0.15),
		static_cast<int>(Scene::Width() * 0.5), static_cast<int>(Scene::Height() * 0.80)
	};
};

