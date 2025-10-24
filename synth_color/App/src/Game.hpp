# pragma once
# include "Common.hpp"

class BasePoly {
public:
	BasePoly() = default;

	explicit BasePoly(const ColorF& color)
		: m_color(color) {
	}

	virtual ~BasePoly() = default;

	virtual void update() = 0;

	virtual void draw() const = 0;

protected:
	ColorF m_color{ 0.00, 0.00, 0.00 };
};

class TrianglePoly : public BasePoly {
public:
	TrianglePoly(const Vec2& p1, const Vec2& p2, const Vec2& p3, const ColorF& color);

	void update() override;

	void draw() const override;
private:
	Vec2 m_p1, m_p2, m_p3;
};


// ゲームシーン
class Game : public App::Scene
{
public:

	Game(const InitData& init);

	void update() override;

	void draw() const override;

private:

	// ブロックのサイズ
	static constexpr Size BrickSize{ 40, 20 };

	// ボールの速さ
	static constexpr double BallSpeed = 480.0;

	// ボールの速度
	Vec2 m_ballVelocity{ 0, -BallSpeed };

	// ボール
	Circle m_ball{ 400, 400, 8 };

	// ブロックの配列
	Array<Rect> m_bricks;

	// 現在のゲームのスコア
	int32 m_score = 0;

	// ブロックを壊したときの効果音
	Audio m_brickSound{ GMInstrument::Woodblock, PianoKey::C5, 0.2s, 0.1s };

	Rect getPaddle() const;

	Array<std::unique_ptr<BasePoly>> m_polygons;
};

