# pragma once

struct Piece {

	// ポリゴンをそのまま渡すピース
	inline Piece(const Polygon& poly, const Color& color)
		: poly{ poly }
		, color{ color } {
	}

	// 多角形ピース
	inline Piece(const Array<Vec2>& pos, const Color& color)
		: poly{ pos }
		, color{ color } {
	}

	// 円形ピース
	inline Piece(const Vec2& center, double radius, const Color& color)
		: poly{ Circle{center, radius}.asPolygon(30) }
		, color{ color } {
	}

	inline void draw() const {
		poly.draw(color);
	}

	inline Vec2 getPrimaryPos() const {
		return poly.vertices()[0];
	};

	Polygon poly;
	Color color;
	Vec2 origPos, destPos;
};
