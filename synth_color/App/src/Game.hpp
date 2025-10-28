# pragma once
# include "Common.hpp"

class BasePiece {
public:
	BasePiece() = default;

	explicit BasePiece(const Color& color)
		: m_color(color) {
	}

	virtual ~BasePiece() = default;


	inline void draw() const {
		m_polygon.draw(m_color);
	}

	inline Polygon getPolygon() const {
		return m_polygon;
	}

	inline Vec2 getPrimaryPos() const {
		return m_polygon.vertices()[0];
		//return m_primary_pos;
	};

	inline void moveBy(const Vec2& delta) {
		m_polygon.moveBy(delta);
	}

	inline bool isPolygonPressed() const {
		return m_polygon.leftClicked();
	}

protected:
	Color m_color{ 255, 255, 255 };
	Polygon m_polygon;
	//Vec2 m_primary_pos;
};

class TrianglePiece : public BasePiece {
public:
	inline TrianglePiece(const Vec2& p1, const Vec2& p2, const Vec2& p3, const Color& color)
		: BasePiece{ color } {
		m_polygon = Triangle{ p1, p2, p3 }.asPolygon();
		//m_primary_pos = p1;
	};
};

class RectanglePiece : public BasePiece {
public:
	inline RectanglePiece(const Vec2& pos, const Size& size, const Color& color)
		: BasePiece{ color } {
		m_polygon = RectF{ pos, size }.asPolygon();
		//m_primary_pos = pos;
	};
};

class CirclePiece : public BasePiece {
public:
	inline CirclePiece(const Vec2& center, double radius, const Color& color)
		: BasePiece{ color } {
		m_polygon = Circle{ center, radius }.asPolygon(30);
		//m_primary_pos = center;
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

	// ドラッグ中の図形のインデックス
	int m_selectedPieceIndex = -1;

	// 図形の配列
	Array<std::unique_ptr<BasePiece>> m_pieces;

	// グリッドサイズ
	const int m_gridSize = 30;

};

