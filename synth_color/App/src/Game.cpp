﻿# include "Game.hpp"

Game::Game(const InitData& init)
	: IScene{ init }
{
	m_pieces << std::make_unique<TrianglePiece>(Vec2{ 100, 500 }, Vec2{ 200, 600 }, Vec2{ 100, 600 }, ColorF{ 1.0, 1.0, 0.0, 0.2 });
	m_pieces << std::make_unique<TrianglePiece>(Vec2{ 300, 500 }, Vec2{ 400, 600 }, Vec2{ 300, 600 }, ColorF{ 0.0, 1.0, 1.0, 0.2 });
	m_pieces << std::make_unique<TrianglePiece>(Vec2{ 500, 500 }, Vec2{ 600, 600 }, Vec2{ 500, 600 }, ColorF{ 1.0, 0.0, 1.0, 0.2 });
	m_pieces << std::make_unique<TrianglePiece>(Vec2{ 700, 500 }, Vec2{ 800, 600 }, Vec2{ 700, 600 }, ColorF{ 1.0, 1.0, 0.0, 0.2 });
}

void Game::update()
{
	for (auto& poly : m_pieces) {
		poly->update();
	}

	const Array<Polygon> intersection_polygon = Geometry2D::And(m_pieces[0]->getPolygon(), m_pieces[1]->getPolygon());
	Print << m_pieces[0]->getPolygon().centroid();
	for (const auto& polygon : intersection_polygon)
	{
		polygon.draw(Palette::Black);
		polygon.drawFrame(2, Palette::Red);
		Print << m_pieces[0]->getPolygon().centroid();
	}
}

void Game::draw() const
{
	Scene::SetBackground(ColorF{ 0.2 });

	for (const auto& poly : m_pieces) {
		poly->draw();
	}
}

void BasePiece::update() {
	if (m_polygon.leftPressed()) {
		m_polygon.moveBy(Cursor::DeltaF());
	}
}

TrianglePiece::TrianglePiece(const Vec2& p1, const Vec2& p2, const Vec2& p3, const ColorF& color)
	: BasePiece{ color } {
	m_polygon = Triangle{ p1, p2, p3 }.asPolygon();
}

void TrianglePiece::draw() const {
	m_polygon.draw(m_color);
}
