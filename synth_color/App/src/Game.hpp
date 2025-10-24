﻿# pragma once
# include "Common.hpp"

class BasePiece {
public:
	BasePiece() = default;

	explicit BasePiece(const ColorF& color)
		: m_color(color) {
	}

	virtual ~BasePiece() = default;

	void update();

	virtual void draw() const = 0;

	Polygon getPolygon() const {
		return m_polygon;
	}

protected:
	ColorF m_color{ 0.00, 0.00, 0.00 };
	Polygon m_polygon;
};

class TrianglePiece : public BasePiece {
public:
	TrianglePiece(const Vec2& p1, const Vec2& p2, const Vec2& p3, const ColorF& color);

	void draw() const override;
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

	Array<std::unique_ptr<BasePiece>> m_pieces;
};

