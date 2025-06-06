#include "enum.h"

void Tile::GetTileLocation(int& x, int& y)
{
	x = m_x;
	y = m_y;
}

TileProperty Tile::GetTileProperty()
{
	return m_tileProperty;
}

void Tile::SetTile(int x, int y, TileProperty tp)
{
	m_x = x;
	m_y = y;
	m_tileProperty = tp;
}

void Tile::SetTile(TileProperty tp)
{
	m_tileProperty = tp;
}

GridMap::GridMap(int p_xTiles, int p_yTiles, const std::map<int, TileProperty>& p_mapInDict)
{
	ConstructGridMap(p_xTiles, p_yTiles, p_mapInDict);
}

void GridMap::ConstructGridMap(int p_xTiles, int p_yTiles, const std::map<int, TileProperty>& p_mapInDict)
{
	// make sure no memory leak
	//Clear();

	m_xTiles = p_xTiles;
	m_yTiles = p_yTiles;

	int totalTiles = m_xTiles * m_yTiles;

	m_gridMap = new Tile* [m_yTiles];
	for (int i = 0; i < m_yTiles; ++i)
	{
		m_gridMap[i] = new Tile[m_xTiles];
	}

	for (auto const& item : p_mapInDict)
	{
		int tx = item.first % m_xTiles;
		int ty = item.first / m_xTiles;
		m_gridMap[ty][tx].SetTile(tx, ty, item.second);
	}
}

void GridMap::Clear(void)
{
	for (int i = 0; i < m_yTiles; ++i)
	{
		delete[] m_gridMap[i];
	}

	delete[] m_gridMap;

	m_xTiles = 0;
	m_yTiles = 0;
	m_gridMap = nullptr;
}

Tile** GridMap::GetGridMap(void)
{
	return m_gridMap;
}

GridMap::~GridMap()
{
	Clear();
}

void GridMap::SetTileProperty(int p_x, int p_y, TileProperty p_tileProperty)
{
	m_gridMap[p_y][p_x].SetTile(p_tileProperty);
}

void GridMap::ConstructPath(std::vector<int>& tileIndexVector, std::vector<Tile*>& targetTiles)
{
	for (const int& index : tileIndexVector)
	{
		int tx = index % m_xTiles;
		int ty = index / m_xTiles;
		targetTiles.push_back(&m_gridMap[ty][tx]);
	}

	// remove the last
	//targetTiles.pop_back();
}
