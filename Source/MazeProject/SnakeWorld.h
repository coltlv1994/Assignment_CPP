// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <map>
#include <array>
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SnakePawn.h"
#include "Apple.h"	
#include "enum.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GameStateSubsystem.h"
#include "Components/BoxComponent.h"
#include "SnakeWorld.generated.h"

class ASnakePlayerController;

UCLASS()
class MAZEPROJECT_API ASnakeWorld : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties 
	ASnakeWorld();

	UPROPERTY(BlueprintReadWrite)
	USceneComponent* m_sceneComponent;

	UPROPERTY(BlueprintReadWrite, EditAnywhere) 
	UInstancedStaticMeshComponent* m_instancedWalls_p;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UBoxComponent* m_collisionBox;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	AApple* appleOnMap;

	//UPROPERTY(BlueprintReadWrite, EditAnywhere)
	//ASnakePawn* player2;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void NewAppleLocation(int& p_AppleX, int& p_AppleY);

	std::map<int, TileProperty>* GetTileMapPointer(void);

	UFUNCTION()
	void HandleAppleEatenEvent(AApple* appleActor);

	UFUNCTION(BlueprintCallable)
	void ConstructWalls();

	UFUNCTION(BlueprintCallable)
	void GoToNextLevel();

	uint16_t GetAppleTile(void);

	std::array<int, 2>* GetGridSize(void);

	void TranslateTileIndexToTiles(std::vector<int>& tileIndexVector, std::vector<Tile*>& targetTiles);

	ASnakePlayerController* controllerRef;

	int nextLevel = 0; // default start from 1

private:
	uint16_t m_appleTile = 0;
	std::map<int, TileProperty> m_tileMap;
	std::array<int, 2> m_gridSize;
	GridMap* m_gridMapPointer = nullptr;
};
