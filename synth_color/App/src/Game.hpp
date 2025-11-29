# pragma once
# include "Common.hpp"

class LightBloom {
public:
	const RenderTexture gaussianA1, gaussianB1;
	const RenderTexture gaussianA4, gaussianB4;
	const RenderTexture gaussianA8, gaussianB8;
	const Size sceneSize;

	LightBloom(const Size& size = Scene::Size()) :sceneSize{ size },
		gaussianA1{ size }, gaussianB1{ size },
		gaussianA4{ size / 4 }, gaussianB4{ size / 4 },
		gaussianA8{ size / 8 }, gaussianB8{ size / 8 } {
	}

	void draw(const Vec2& pos = { 0,0 })const {
		Shader::GaussianBlur(gaussianA1, gaussianB1, gaussianA1);
		Shader::Downsample(gaussianA1, gaussianA4);
		Shader::GaussianBlur(gaussianA4, gaussianB4, gaussianA4);
		Shader::Downsample(gaussianA4, gaussianA8);
		Shader::GaussianBlur(gaussianA8, gaussianB8, gaussianA8);
		const ScopedRenderStates2D blend{ BlendState::Additive };
		gaussianA1.resized(sceneSize).draw(pos, ColorF{ 0.1 });
		gaussianA4.resized(sceneSize).draw(pos, ColorF{ 0.4 });
		gaussianA8.resized(sceneSize).draw(pos, ColorF{ 0.8 });
	}
};

class ScopedLightBloom {
public:
	ScopedLightBloom(const LightBloom& lightBloom) :target{ lightBloom.gaussianA1.clear(ColorF{ 0.0 }) } {}
	const ScopedRenderTarget2D target;
	const ScopedRenderStates2D blend{ BlendState::Additive };
};

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

	const Audio m_clearAudio{ U"audio/clear.mp3" };

	double m_deltaT = 0;

	LightBloom m_lightBloom;
};
