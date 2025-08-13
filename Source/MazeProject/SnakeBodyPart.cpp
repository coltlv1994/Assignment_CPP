// Fill out your copyright notice in the Description page of Project Settings.


#include "SnakeBodyPart.h"

// Sets default values
ASnakeBodyPart::ASnakeBodyPart()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));

	RootComponent = SceneComponent;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));

	CollisionComponent->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void ASnakeBodyPart::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASnakeBodyPart::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (m_direction == Direction::None && m_nextDirection == Direction::None)
	{
		return;
	}

	FVector rightVector = GetActorRightVector();
	FVector forwardVector = GetActorForwardVector();
	FVector Position = GetActorLocation();

	float totalMoveDistance = m_speed * DeltaTime;

	float moveDistance = totalMoveDistance;

	if (m_movedTileDistance + moveDistance >= m_gridSize)
	{
		moveDistance = m_gridSize - m_movedTileDistance;
	}

	m_movedTileDistance += moveDistance;

	switch (m_direction)
	{
	case Direction::Right:
		Position += rightVector * moveDistance;
		break;
	case Direction::Up:
		Position += forwardVector * moveDistance;
		break;
	case Direction::Left:
		Position += rightVector * moveDistance * -1.0f;
		break;
	case Direction::Down:
		Position += forwardVector * moveDistance * -1.0f;
	default:
		break;
	}

	if (m_movedTileDistance >= m_gridSize)
	{
		m_movedTileDistance = 0.0f;
	}

	SetActorLocation(Position);
}

void ASnakeBodyPart::SetInitialStatus(float p_movementSpeed, float p_movedTileDistance, Direction p_initDirection)
{
	m_speed = p_movementSpeed;
	m_movedTileDistance = p_movedTileDistance;
	m_direction = p_initDirection;
}

void ASnakeBodyPart::AddNextBodyPart(ASnakeBodyPart* p_newBodyPart)
{
	if (IsValid(nextBodyPart))
	{
		nextBodyPart->AddNextBodyPart(p_newBodyPart);
	}
	else
	{
		nextBodyPart = p_newBodyPart;
		nextBodyPart->m_speed = m_speed;
		nextBodyPart->m_movedTileDistance = m_movedTileDistance;
		nextBodyPart->m_direction = m_direction;
	}
}

void ASnakeBodyPart::SetDirection(Direction p_direction)
{
	if (IsValid(nextBodyPart))
	{
		//UE_LOG(LogTemp, Warning, TEXT("%s, m_direction: %d, p_direction: %d"), *GetName(), m_direction, p_direction);
		nextBodyPart->SetDirection(m_direction);
		m_direction = p_direction;
	}
	else
	{
		m_direction = p_direction;
	}
}

void ASnakeBodyPart::GetNextSpawnLocation(FVector& p_SpawnLocation)
{
	if (IsValid(nextBodyPart))
	{
		nextBodyPart->GetNextSpawnLocation(p_SpawnLocation);
	}
	else
	{
		// it is the last body part
		FVector selfPosition = GetActorLocation();
		FVector rightVector = GetActorRightVector();
		FVector forwardVector = GetActorForwardVector();

		switch (m_direction)
		{
		case Direction::Right:
			p_SpawnLocation = selfPosition + -100.0f * rightVector;
			break;
		case Direction::Up:
			p_SpawnLocation = selfPosition + -100.0f * forwardVector;
			break;
		case Direction::Left:
			p_SpawnLocation = selfPosition + 100.0f * rightVector;
			break;
		case Direction::Down:
			p_SpawnLocation = selfPosition + 100.0f * forwardVector;
			break;
		default:
			break;
		}
	}
}