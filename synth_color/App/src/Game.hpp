# pragma once
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

	Array<std::unique_ptr<BasePiece>> m_pieces;
};

