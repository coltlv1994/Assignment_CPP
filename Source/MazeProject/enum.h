#pragma once
#include <map>
#include <vector>

UENUM(BlueprintType)
enum class Direction : uint8
{
	None = 0,
	Up = 1,
	Down = 2,
	Left = 3,
	Right = 4,
};

UENUM(BlueprintType)
enum class TileProperty : uint8
{
	Space = 0,
	Apple = 1,
	Obstacle = 2,
};

UENUM(BlueprintType)
enum class SnakeGameState : uint8
{
	None,
	MainMenu,
	Play,
	Outro
};

UENUM(BlueprintType)
enum class SnakeMenuAction : uint8
{
	None,
	ChangeMenu,
	ChangeNumberOfPlayers,
	StartPvPGame,
	StartPvEGame,
	QuitGame,
};

UENUM(BlueprintType)
enum class SnakePlayMode : uint8
{
	PvE,
	PvP
};

UENUM(BlueprintType)
enum class SnakePlayerCategory : uint8
{
	Player1,
	Player2,
	AI
};

class Tile
{
public:
	Tile()
	{
		m_x = 0;
		m_y = 0;
	}

	Tile(int p_x, int p_y, TileProperty p_tileProperty)
	{
		m_x = p_x;
		m_y = p_y;
		m_tileProperty = p_tileProperty;
	}

	void GetTileLocation(int& x, int& y);
	void SetTile(TileProperty tp);
	void SetTile(int x, int y, TileProperty tp);
	TileProperty GetTileProperty();

private:
	int m_x;
	int m_y;
	TileProperty m_tileProperty = TileProperty::Space;
};

class GridMap
{
public:
	GridMap(int p_xTiles, int p_yTiles, const std::map<int, TileProperty>& p_mapInDict);
	void ConstructGridMap(int p_xTiles, int p_yTiles, const std::map<int, TileProperty>& p_mapInDict);
	void SetTileProperty(int p_x, int p_y, TileProperty p_tileProperty);
	Tile** GetGridMap(void);
	void ConstructPath(std::vector<int>& tileIndexVector, std::vector<Tile*>& targetTiles);
	void Clear(void);
	~GridMap();

private:
	int m_xTiles;
	int m_yTiles;

	// m_gridMap[y][x]
	// this notion keeps tiles on the same row (x-axis) in the same Tile* m_gridMap[y]
	Tile** m_gridMap = nullptr;
};