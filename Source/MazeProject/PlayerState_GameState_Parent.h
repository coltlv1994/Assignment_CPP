// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "enum.h"
#include "GameFramework/GameState.h"
#include "PlayerState_GameState_Parent.generated.h"

/**
 * 
 */
UCLASS()
class MAZEPROJECT_API APlayerState_GameState_Parent : public AGameState
{
	GENERATED_BODY()

private:
	UPROPERTY()
	SnakePlayMode m_snakePlayMode;

public:
	UFUNCTION(BlueprintCallable)
	SnakePlayMode GetSnakePlayMode();

	UFUNCTION(BlueprintCallable)
	void  SetSnakePlayMode(SnakePlayMode p_snakePlayMode);
};
