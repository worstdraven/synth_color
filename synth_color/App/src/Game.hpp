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

	void updateFadeIn(double t) override;

	void drawFadeIn(double t) const override {
		draw();
	};

	void updateFadeOut(double t) override;

	void drawFadeOut(double t) const override {
		draw();
	};

private:
	bool checkPuzzleClear() const;

	// ドラッグ中の図形のインデックス
	int m_selectedPieceIndex = -1;

	// 図形の配列
	Array<Piece>& m_pieces;

	// パズルビューポート
	RectF m_puzzleViewport{
		ratioVec(0.45, 0.15),
		ratioVec(0.53, 0.83)
	};
	Vec2 m_puzzleViewportDest{ m_puzzleViewport.center() };

	// お手本ビューポート
	Circle m_answerViewport{ ratioVec(0.24, 0.54) , getData().gridSize * 5.5 };
	Vec2 m_answerViewportDest{ m_answerViewport.center };
	Vec2 m_answerViewportOrig{ -m_answerViewport.r, Height() + m_answerViewport.r };

	Transition m_clearTransition{ ChangeSceneDuration / 2.0 * 0.8, 0.0s };

	Transition m_changeSceneTransition{ ChangeSceneDuration / 2.0, ChangeSceneDuration / 2.0 };

	double m_deltaT = 0;
};

