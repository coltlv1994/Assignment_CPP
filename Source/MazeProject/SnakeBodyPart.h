// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "enum.h"
#include "SnakeBodyPart.generated.h"

UCLASS()
class MAZEPROJECT_API ASnakeBodyPart : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASnakeBodyPart();
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* SceneComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USphereComponent* CollisionComponent;

	ASnakeBodyPart* nextBodyPart = nullptr;

	Direction m_direction = Direction::None;
	Direction m_nextDirection = Direction::None;
	float m_speed = 0.0f; // AI and player has different speed so this must be changed accordingly
	float m_gridSize = 100.0f;
	float m_movedTileDistance = 0.0f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetInitialStatus(float p_movementSpeed, float movedTileDistance, Direction p_initDirection); // sync parent's status

	void AddNextBodyPart(ASnakeBodyPart* newBodyPart);

	void SetDirection(Direction p_direction);

	void GetNextSpawnLocation(FVector& p_SpawnLocation);
};
