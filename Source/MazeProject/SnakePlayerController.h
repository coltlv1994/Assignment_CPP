// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "enum.h"
#include "SnakePawn.h"
#include "SnakeWorld.h"
#include "Apple.h"
#include "GameFramework/PlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "SnakePlayerController.generated.h"

class UInputMappingContext;

class PathNode
{
public:
	float m_distance = 0.0f;
	float m_heuristic = 0.0f;
	int m_parent = -1;
	int m_x = 0;
	int m_y = 0;
	int m_index = 0;
	std::map<int, float> m_neighbors;

	PathNode(int p_x, int p_y, int p_index);

	void AddNeighbor(const int XTILES, const int YTILES, int p_nodeIndex, std::vector<int>& p_unoccupiedIndex);
};

class A_Asterik_Algorithm
{
public:
	using IndexDistanceTuple = std::tuple<int, float>;
	std::vector<PathNode> m_gameMap;
	std::vector<int> m_unoccupiedTiles;
	std::vector<int> m_foundPath; // it could be in reverse order

	void GameMapInit(int XTILES, int YTILES, std::map<int, TileProperty>& p_tileMap);

	void CalculateDistance(int ENDX, int ENDY);

	void FindPath(int XTILES, int YTILES, int STARTX, int STARTY, int ENDX, int ENDY, std::map<int, TileProperty>* p_tileMapPointer);

	void UpdatePriorityQueue(std::vector<IndexDistanceTuple>& priorityQueue);
};

/**
 * 
 */
UCLASS()
class MAZEPROJECT_API ASnakePlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

private:
	std::array<int, 2> m_gridSize;

public:
	ASnakePlayerController();
	~ASnakePlayerController();

	// Called every frame
	//virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	AApple* appleOnMap;

	UPROPERTY(BlueprintReadWrite)
	ASnakeWorld* snakeWorld;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<ASnakePawn> m_snakePawn1;

	UPROPERTY(BlueprintReadWrite)
	bool isAIControlled = false;

	// Can either be a human controller or AI-controlled.
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<ASnakePawn> m_snakePawn2;

	UFUNCTION(BlueprintCallable)
	void SetDirection(Direction p_direction);

	UFUNCTION(BlueprintCallable)
	void SetDirectionPlayer2(Direction p_direction);

	UFUNCTION(BlueprintCallable)
	void SetPlayer2AIControlled(bool p_isAIControlled);

	void NewAppleRoute(int xTile, int yTile);

	void GetTileMapFromWorld(void);

	void GetGridSize(void);

	std::map<int, TileProperty>* m_tileMapPointer;

	A_Asterik_Algorithm* m_aAsterikAlgorithm = nullptr;

	bool isPlayer2Initialized = false;

	void ResetPawns();
};
