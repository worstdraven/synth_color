# pragma once
# include <Siv3D.hpp>
# include "Piece.hpp"

using Scene::Width;
using Scene::Height;

// 画面遷移時間
static constexpr Duration TitleFadeInDuration{ 0.4s };
static constexpr Duration TitleToLevelDuration{ 2.0s };
static constexpr Duration LevelToGameDuration{ 3.5s };
static constexpr Duration GameToGameDuration{ 3.0s };
static constexpr Duration GameToLevelDuration{ 3.0s };

// 減法混色の三原色
static constexpr ColorF SubtractiveCyan = ColorF{ 1.0, 0.0, 0.0 };
static constexpr ColorF SubtractiveMagenta = ColorF{ 0.0, 1.0, 0.0 };
static constexpr ColorF SubtractiveYellow = ColorF{ 0.0, 0.0, 1.0 };

static ColorF getSubtractiveColorF(String color) {
	if (color == U"cyan") {
		return SubtractiveCyan;
	}
	else if (color == U"magenta") {
		return SubtractiveMagenta;
	}
	else if (color == U"yellow") {
		return SubtractiveYellow;
	}
	else {
		throw Error{ U"invalid subtractive color input" };
	}
}

// シーンのステート
enum class State
{
	Title,
	Level,
	Game
};

// 再帰的に JSON の要素を表示
static void ShowObject(const JSON& value)
{
	switch (value.getType())
	{
	case JSONValueType::Empty:
		Console << U"empty";
		break;
	case JSONValueType::Null:
		Console << U"null";
		break;
	case JSONValueType::Object:
		for (const auto& object : value)
		{
			Console << U"[{}]"_fmt(object.key);
			ShowObject(object.value);
		}
		break;
	case JSONValueType::Array:
		for (auto&& [index, object] : value)
		{
			ShowObject(object);
		}
		break;
	case JSONValueType::String:
		Console << value.getString();
		break;
	case JSONValueType::Number:
		Console << value.get<double>();
		break;
	case JSONValueType::Bool:
		Console << value.get<bool>();
		break;
	}
}

// 共有するデータ
struct GameData
{
	// ピースの配列
	Array<Piece> pieces;

	Array<Point> correctPositions;
	Point correctCenter;

	int16 gridSize = Width() / 25;

	int16 fetchGridSize() {
		gridSize = Width() / 25;
		return gridSize;
	}

	void drawGrid() const {
		Scene::SetBackground(Palette::White);
		for (int i = 0; i < Width(); i += gridSize) {
			for (int j = 0; j < Height(); j += gridSize) {
				Circle{ i, j, 1 }.draw(Palette::Black);
			}
		}
	}

	Array<bool> isCleared;

	int currentLevel = -1;

	void loadLevelDesign() {
		const JSON levelJson = JSON::Load(U"text/level_design.json");
		fetchGridSize();

		if (not levelJson) {
			throw Error{ U"Failed to load 'level_design.json'" };
		}

		JSON selectedLevel = NULL;

		// get selected level json
		for (auto&& [index, object] : levelJson[U"levels"]) {
			if (object[U"levelId"] == currentLevel) {
				selectedLevel = object;
				break;
			}
		}
		if (selectedLevel == NULL) {
			throw Error{ U"Invalid level select" };
		}

		pieces.clear();
		correctPositions.clear();
		for (auto&& [index, object] : selectedLevel[U"pieces"]) {
			correctPositions << Point{
				gridSize * object[U"correctPosition"][0].get<int>(),
				gridSize * object[U"correctPosition"][1].get<int>()
			};
			if (object[U"shape"] == U"circle") {
				pieces << Piece{
					Vec2{0, 0},
					gridSize * object[U"radius"].get<double>(),
					getSubtractiveColorF(object[U"color"].getString())
				};
			}
			else if (object[U"shape"] == U"rectangle" || object[U"shape"] == U"triangle") {
				Array<Vec2> vertices;
				for (auto&& [vIndex, vObject] : object[U"vertices"]) {
					vertices << Vec2{
						gridSize * vObject[0].get<double>(),
						gridSize * vObject[1].get<double>()
					};
				}
				pieces << Piece{
					vertices,
					getSubtractiveColorF(object[U"color"].getString())
				};
			}
			else {
				throw Error{ U"not supported piece shape" };
			}
		}

		correctCenter = Point{
			gridSize * selectedLevel[U"correctCenter"][0].get<int>(),
			gridSize * selectedLevel[U"correctCenter"][1].get<int>()
		};
	};
};

static constexpr Vec2 ratioVec(double x, double y) {
	// 位置を0から1の割合で指定して、画面内の位置を返す。
	return Vec2{ Width() * x, Height() * y };
}

using App = SceneManager<State, GameData>;
