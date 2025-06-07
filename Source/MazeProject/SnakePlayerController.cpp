// Fill out your copyright notice in the Description page of Project Settings.


#include "SnakePlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "Kismet/GameplayStatics.h"

#include <algorithm>
#include <queue>
#include <set>

ASnakePlayerController::ASnakePlayerController()
{
	//PrimaryActorTick.bCanEverTick = true;
}

ASnakePlayerController::~ASnakePlayerController()
{
	delete m_aAsterikAlgorithm;
}

void ASnakePlayerController::BeginPlay()
{
	Super::BeginPlay();
}

void ASnakePlayerController::SetDirection(Direction p_direction)
{
	if (IsValid(m_snakePawn1))
	{
		m_snakePawn1->SetDirection(p_direction);
	}

	if (isPlayer2Initialized == false)
	{
		isPlayer2Initialized = true;
		m_snakePawn1->SetWorldActor(snakeWorld);
		m_snakePawn2->SetWorldActor(snakeWorld);
		m_snakePawn1->SetActorTickEnabled(true);
		m_snakePawn2->SetActorTickEnabled(true);

		if (m_snakePawn2->isAIControlled == true)
		{
			// acquire tile map
			m_aAsterikAlgorithm = new A_Asterik_Algorithm();
			GetTileMapFromWorld();
			int appleTile = (int)(snakeWorld->GetAppleTile());
			GetGridSize();
			m_aAsterikAlgorithm->GameMapInit(m_gridSize[0], m_gridSize[1], *m_tileMapPointer);
			// at this moment, player 2 is absolutely valid
			NewAppleRoute(appleTile % m_gridSize[1], appleTile / m_gridSize[1]);
			// bind
			snakeWorld->controllerRef = this;
		}
	}
}

void ASnakePlayerController::SetDirectionPlayer2(Direction p_direction)
{
	m_snakePawn2->SetDirection(p_direction);
}

void ASnakePlayerController::SetPlayer2AIControlled(bool p_isAIControlled)
{
	m_snakePawn2->SetAIControlled(p_isAIControlled);
}

void ASnakePlayerController::GetTileMapFromWorld(void)
{
	m_tileMapPointer = snakeWorld->GetTileMapPointer();
}

void ASnakePlayerController::GetGridSize(void)
{
	m_gridSize[0] = (*snakeWorld->GetGridSize())[0];
	m_gridSize[1] = (*snakeWorld->GetGridSize())[1];
}

void ASnakePlayerController::NewAppleRoute(int xTile, int yTile)
{
	if (IsValid(m_snakePawn2))
	{
		// null pointer will return
		// This function might be called before player2 binding is completed.

		// set destination
		m_aAsterikAlgorithm->CalculateDistance(xTile, yTile);

		// get pawn current tile
		int currentTileX, currentTileY;
		m_snakePawn2->GetPawnTile(currentTileX, currentTileY);

		m_aAsterikAlgorithm->FindPath(m_gridSize[0], m_gridSize[1], currentTileX, currentTileY, xTile, yTile, m_tileMapPointer);

		// read the route and convert to Tile*
		std::vector<Tile*> pathToPawn;
		snakeWorld->TranslateTileIndexToTiles(m_aAsterikAlgorithm->m_foundPath, pathToPawn);

		// set to pawn 2
		m_snakePawn2->SetDestinationTileList(pathToPawn);
	}
}

void ASnakePlayerController::ResetPawns()
{
	if (IsValid(m_snakePawn1))
	{
		// reset score and transform, clean destination list, remove direction,
		// pawn 1 is always human-controlled.
		m_snakePawn1->SetActorLocation(FVector(100.0f, 100.0f, 0.0f)); // spawn location
		m_snakePawn1->m_direction = Direction::None;
		m_snakePawn1->m_nextDirection = Direction::None;
		m_snakePawn1->currentScore = 0;
	}

	if (IsValid(m_snakePawn2))
	{
		// same as snake pawn 2
		m_snakePawn2->SetActorLocation(FVector(100.0f, 100.0f, 0.0f)); // spawn location
		m_snakePawn2->currentScore = 0;
		m_snakePawn2->m_direction = Direction::None;

		if (m_snakePawn2->isAIControlled)
		{
			m_snakePawn2->m_destinationTileList.clear();

			//additionally if AI controller, re - trigger apple event
			int appleTile = (int)(snakeWorld->GetAppleTile());
			GetGridSize();
			
			// clean old game map and force a reconstruct
			m_aAsterikAlgorithm->m_gameMap.clear();
			m_aAsterikAlgorithm->GameMapInit(m_gridSize[0], m_gridSize[1], *m_tileMapPointer);

			// at this moment, player 2 is absolutely valid
			NewAppleRoute(appleTile % m_gridSize[1], appleTile / m_gridSize[1]);
		}
		else
		{
			m_snakePawn2->m_nextDirection = Direction::None;
		}


	}

}

PathNode::PathNode(int p_x, int p_y, int p_index)
{
	m_x = p_x;
	m_y = p_y;
	m_index = p_index;
}

void PathNode::AddNeighbor(const int XTILES, const int YTILES, int p_nodeIndex, std::vector<int>& p_unoccupiedIndex)
{
	if (m_neighbors.size() > 0)
	{
		return;
	}

	m_neighbors[p_nodeIndex - XTILES] = 1.0f;
	m_neighbors[p_nodeIndex + XTILES] = 1.0f;
	m_neighbors[p_nodeIndex - 1] = 1.0f;
	m_neighbors[p_nodeIndex + 1] = 1.0f;

	if (p_nodeIndex % XTILES == 0)
	{
		m_neighbors.erase(p_nodeIndex - 1);
	}

	if (p_nodeIndex % XTILES == XTILES - 1)
	{
		m_neighbors.erase(p_nodeIndex + 1);
	}

	if (p_nodeIndex <= XTILES)
	{
		m_neighbors.erase(p_nodeIndex - XTILES);
	}

	if (p_nodeIndex >= (XTILES * YTILES - XTILES))
	{
		m_neighbors.erase(p_nodeIndex + XTILES);
	}

	for (const auto& kv : m_neighbors)
	{
		if (std::find(p_unoccupiedIndex.begin(), p_unoccupiedIndex.end(), kv.first) == p_unoccupiedIndex.end())
		{
			m_neighbors.erase(kv.first);
		}
	}
}

void A_Asterik_Algorithm::GameMapInit(int XTILES, int YTILES, std::map<int, TileProperty>& p_tileMap)
{
	if (m_gameMap.size() != p_tileMap.size())
	{
		m_gameMap.clear();
		m_unoccupiedTiles.clear();

		int noOfTiles = p_tileMap.size();

		for (int i = 0; i < noOfTiles; i++)
		{
			if (p_tileMap[i] != TileProperty::Obstacle)
			{
				m_unoccupiedTiles.push_back(i);
			}

			// gameMap is an std::vector and it will keep the order
			// don't change it since it requires exactly the same mappint between index and pathnode's index
			m_gameMap.push_back(PathNode(i / XTILES, i % XTILES, i));
		}

		for (int i = 0; i < noOfTiles; i++)
		{
			m_gameMap[i].AddNeighbor(XTILES, YTILES, i, m_unoccupiedTiles);
		}

		// distance will be called at separate function
	}
}

void A_Asterik_Algorithm::CalculateDistance(int ENDX, int ENDY)
{
	// this function resets game map
	for (auto& node : m_gameMap)
	{
		node.m_parent = -1;
		// Euclidean distance as heuristic
		node.m_heuristic = sqrtf(powf(ENDX - node.m_x, 2.0) + powf(ENDY - node.m_y, 2.0f));
		node.m_distance = INFINITY;
	}
}

void A_Asterik_Algorithm::UpdatePriorityQueue(std::vector<IndexDistanceTuple>& priorityQueue)
{
	// This function will update the priority queue with information from m_gameMap and resort it
	for (IndexDistanceTuple& valuePair : priorityQueue)
	{
		int index = std::get<0>(valuePair);
		std::get<1>(valuePair) = m_gameMap[index].m_distance + m_gameMap[index].m_heuristic;
	}

	// use lambda expression
	std::sort(
		priorityQueue.begin(),
		priorityQueue.end(),
		[](const IndexDistanceTuple& lhs, const IndexDistanceTuple& rhs)
		{
			return std::get<1>(lhs) > std::get<1>(rhs); // descending order
		});
}

void A_Asterik_Algorithm::FindPath(int XTILES, int YTILES, int STARTX, int STARTY, int ENDX, int ENDY, std::map<int, TileProperty>* p_tileMapPointer)
{
	using IndexDistanceTuple = std::tuple<int, float>;
	std::map<int, TileProperty>& tileMap = *p_tileMapPointer;
	const int endIndex = ENDX + XTILES * ENDY;

	UE_LOG(LogTemp, Warning, TEXT("Pathfinding: to %d, %d, start: %d, %d"), ENDX, ENDY, STARTX, STARTY);

	GameMapInit(XTILES, YTILES, tileMap);

	CalculateDistance(ENDX, ENDY);

	// set start point; it has shortest distance
	m_gameMap[STARTX + STARTY * XTILES].m_distance = 0.0f;

	// constructing priority queue
	// it only contains unoccupied tiles
	// for convenience and performance, it is actually descending since we always remove the smallest "distance"
	std::vector<IndexDistanceTuple> priorityQueue;

	for (const PathNode& node : m_gameMap)
	{
		if (std::find(m_unoccupiedTiles.begin(), m_unoccupiedTiles.end(), node.m_index) != m_unoccupiedTiles.end())
		{
			priorityQueue.push_back(IndexDistanceTuple(node.m_index, 0.0f));
		}
	}

	std::set<int> visitedNodes;

	while (priorityQueue.size() != 0)
	{
		// update the priority queue
		UpdatePriorityQueue(priorityQueue);

		// this value cannot change
		const PathNode& currentNode = m_gameMap[std::get<0>(priorityQueue.back())]; // get node reference with smallest distance

		if (currentNode.m_index == endIndex)
		{
			// arrive at the end
			break;
		}

		for (const auto& neighbor : currentNode.m_neighbors)
		{
			// neightbor: index, relative distance to this node
			// this value can be changed
			PathNode& neighborNode = m_gameMap[neighbor.first];
			if (visitedNodes.find(neighbor.first) == visitedNodes.end() && (currentNode.m_distance + neighbor.second < neighborNode.m_distance))
			{
				// this node is not visited, and current node has smaller value
				neighborNode.m_parent = currentNode.m_index;
				neighborNode.m_distance = currentNode.m_distance + neighbor.second;
			}
		}

		visitedNodes.insert(currentNode.m_index);
		priorityQueue.pop_back();
	}

	// clear historical path
	m_foundPath.clear();
	int tailIndex = endIndex;

	while (tailIndex != -1)
	{
		m_foundPath.push_back(tailIndex);
		tailIndex = m_gameMap[tailIndex].m_parent;
	}

	// push current node to destination
	// floating point numbers have precision issue and it may cause unwanted bugs
	m_foundPath.push_back(STARTX + STARTY * XTILES);

	// A* algorithm needs to reverse the path.
	// But we store the path in a vector, performance-wise we should read and remove from the last
	// it looks like: end -> waynode_n -> waynode_(n-1) -> ... -> start -> (an additional start to make sure this does not bug)
	//std::reverse(m_foundPath.begin(), m_foundPath.end());
}