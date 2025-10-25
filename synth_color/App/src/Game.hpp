# pragma once
# include "Common.hpp"

class BasePiece {
public:
	BasePiece() = default;

	explicit BasePiece(const Color& color)
		: m_color(color) {
	}

	virtual ~BasePiece() = default;

	void update();

	inline void draw() const {
		m_polygon.draw(m_color);
	}

	inline Polygon getPolygon() const {
		return m_polygon;
	}

protected:
	Color m_color{ 255, 255, 255 };
	Polygon m_polygon;
};

class TrianglePiece : public BasePiece {
public:
	inline TrianglePiece(const Vec2& p1, const Vec2& p2, const Vec2& p3, const Color& color)
		: BasePiece{ color } {
		m_polygon = Triangle{ p1, p2, p3 }.asPolygon();
	};
};

class RectanglePiece : public BasePiece {
public:
	inline RectanglePiece(const Vec2& pos, const Size& size, const Color& color)
		: BasePiece{ color } {
		m_polygon = RectF{ pos, size }.asPolygon();
	};
};

class CirclePiece : public BasePiece {
public:
	inline CirclePiece(const Vec2& center, double radius, const Color& color)
		: BasePiece{ color } {
		m_polygon = Circle{ center, radius }.asPolygon(30);
	};
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

