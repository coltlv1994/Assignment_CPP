// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "enum.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GameStateSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnGameStateChanged, SnakeGameState, oldGameState, SnakeGameState, newGameState);

/**
 * 
 */
UCLASS()
class MAZEPROJECT_API UGameStateSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

private:
	UPROPERTY()
	SnakeGameState m_gameState = SnakeGameState::None;
	int m_noOfPlayers = 0; // actual number of players will be one more than this.
	SnakePlayMode m_playMode = SnakePlayMode::PvE;

public:
	UFUNCTION(BlueprintCallable)
	SnakeGameState GetGameState()
	{
		return m_gameState;
	}

	UFUNCTION(BlueprintCallable)
	void ChangeGameState(SnakeGameState p_gameState);

	UFUNCTION(BlueprintCallable)
	void RotatePlayerNumber();

	UFUNCTION(BlueprintCallable)
	int GetPlayerNumber();

	UPROPERTY(BlueprintAssignable)
	FOnGameStateChanged GameStateChanged;

	UFUNCTION(BlueprintCallable)
	void SetPVPGame();

	UFUNCTION(BlueprintCallable)
	void SetPVEGame();

	UFUNCTION(BlueprintCallable)
	SnakePlayMode GetGameplayMode();

	void SetGridMap(GridMap* p_gridMap);
	GridMap* GetGridMap(void);

	GridMap* m_gridMapPointer = nullptr;

	int nextLevel = 1;

	UFUNCTION(BlueprintCallable)
	void GoToNextLevel();
	
};
