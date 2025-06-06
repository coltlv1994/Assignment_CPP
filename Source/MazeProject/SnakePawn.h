// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Apple.h"
#include "GameFramework/Pawn.h"
#include "Components/SphereComponent.h"
#include "enum.h"
#include <vector>
#include "SnakePawn.generated.h"

class ASnakeWorld;

UCLASS()
class MAZEPROJECT_API ASnakePawn : public APawn
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* SceneComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USphereComponent* CollisionComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	Direction m_direction = Direction::None;

	// Sets default values for this pawn's properties
	ASnakePawn();

private:
	float m_velocity_z = 0.0f;

	float m_speed = 500.0f;

	const float m_gridSize = 100.0f;

	float m_movedTileDistance = 0.0f;

	ASnakeWorld* m_snakeWorld;

	void HandleGravity(FVector& p_position, float p_deltaTime);

	void HandleGround(FVector& p_position, float p_deltaTime);

	int lastCurrentX, lastCurrentY, lastDestX, lastDestY;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	void Jump();

	UFUNCTION(BlueprintCallable)
	void SetDirection(Direction p_direction);

	std::vector<Tile*> m_destinationTileList;

	void SetDestinationTileList(std::vector<Tile*>& p_route);

	bool isAIControlled = false;

	Direction m_nextDirection = Direction::None;

	void GetPawnTile(int &x, int &y);

	void SetAIControlled(bool p_isAIControlled);

	UFUNCTION(BlueprintCallable)
	void Score();

	void SetWorldActor(ASnakeWorld* p_snakeWorld);

	int currentScore = 0;
	
	const int maxScore = 3;

};
