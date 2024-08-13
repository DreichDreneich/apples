#pragma once

using namespace std;

namespace ApplesGame
{
	enum class ActorType
	{
		NONE,
		APPLE,
		STONE,
		BONUS,
	};

	struct GameEl
	{
		int idx;
		ActorType type;
	};

	class GameField
	{
	public:
		vector<vector<GameEl>> grid;
		static Vector2f FieldToScreenPosition(Vector2i);
		void Draw();
	};
}