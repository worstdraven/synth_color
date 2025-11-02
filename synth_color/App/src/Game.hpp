# pragma once
# include "Common.hpp"

using Scene::Width;
using Scene::Height;

// ゲームシーン
class Game : public App::Scene
{
public:

	Game(const InitData& init);

	~Game();

	void update() override;

	void draw() const override;

	void updateFadeIn(double t) override;

	void drawFadeIn(double t) const override;

	void updateFadeOut(double t) override;

	void drawFadeOut(double t) const override;

private:
	bool checkPuzzleClear() const;

	// ドラッグ中の図形のインデックス
	int m_selectedPieceIndex = -1;

	// 図形の配列
	Array<Piece>& m_pieces;

	Rect m_puzzleViewport{
		static_cast<int>(Scene::Width() * 0.45), static_cast<int>(Scene::Height() * 0.15),
		static_cast<int>(Scene::Width() * 0.53), static_cast<int>(Scene::Height() * 0.83)
	};
	Vec2 m_puzzleViewportDest{ m_puzzleViewport.center()};

	Circle m_answerViewport{ getData().gridSize * 6.0, getData().gridSize * 10.0 , getData().gridSize * 5.5 };
	Vec2 m_answerViewportDest{ m_answerViewport.center };
	Vec2 m_answerViewportOrig{ - m_answerViewport.r, Height() + m_answerViewport.r};

	Transition m_clearTransition{ 1.0s, 0.0s };

	double m_deltaT = 0;
};

