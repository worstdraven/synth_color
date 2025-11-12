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

	inline void draw(double alpha = 1.0) const {
		poly.draw(color.withA(alpha * 255)).drawFrame(2, Palette::Dimgray.withA(alpha * 255));
	}

	inline Vec2 getPrimaryPos() const {
		return poly.vertices()[0];
	};

	Polygon poly;
	Color color;
	Vec2 origPos{ 0.0, 0.0 }, destPos{ 0.0, 0.0 };
};
