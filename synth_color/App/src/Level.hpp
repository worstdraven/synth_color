# pragma once
# include "Common.hpp"

class LevelInfo {
private:
	String m_levelTitle;
};

class Level : public App::Scene
{
public:

	Level(const InitData& init);

	void update() override;

	void draw() const override;

private:

	Array<LevelInfo> m_levelInfos;
};
