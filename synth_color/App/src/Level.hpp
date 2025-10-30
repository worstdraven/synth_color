# pragma once
# include "Common.hpp"

class Level : public App::Scene
{
public:

	Level(const InitData& init);

	void update() override;

	void draw() const override;

private:
	void setLevelDesign(int level);
};
