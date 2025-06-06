// Fill out your copyright notice in the Description page of Project Settings.


#include "SnakePawn.h"
#include "SnakeWorld.h"

// Constants with math and physics
constexpr float GRAV_ACCEL = -982.0f; // Gravitational acceleration
constexpr float VERTICAL_JUMP_SPEED = 350.0f; // Average people can jump 3.5m/s vertically

// Sets default values
ASnakePawn::ASnakePawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));

	RootComponent = SceneComponent;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));

	CollisionComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ASnakePawn::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ASnakePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector Position = GetActorLocation();

	if (isAIControlled)
	{
		// AI controlled
		if (!m_destinationTileList.empty())
		{
			if (m_direction == Direction::None)
			{
				// set new direction
				// note: this list is in reverse order for performance concern
				// we read and remove from last
				Tile& destinationTile = *m_destinationTileList.back();
				int tile_x = 0;
				int tile_y = 0;
				destinationTile.GetTileLocation(tile_x, tile_y);

				int currentTileX = roundf(Position.X / 100.0f);
				int currentTileY = roundf(Position.Y / 100.0f);

				UE_LOG(LogTemp, Warning, TEXT("world: %f, %f, current: %d, %d, dest: %d, %d"), Position.X, Position.Y, currentTileX, currentTileY, tile_x, tile_y);

				// very weird world tile system
				// x-positive is UP and y-positive is RIGHT
				if (tile_x - currentTileX > 0)
				{
					// to the right
					m_direction = Direction::Up;
					//return;
				}

				if (tile_x - currentTileX < 0)
				{
					// to the left
					m_direction = Direction::Down;
					//return;
				}

				if (tile_y - currentTileY > 0)
				{
					// to the up
					m_direction = Direction::Right;
					//return;
				}

				if (tile_y - currentTileY < 0)
				{
					// to the down
					m_direction = Direction::Left;
					//return;
				}

				m_destinationTileList.pop_back();

			}
		}
	}


	HandleGravity(Position, DeltaTime);

	HandleGround(Position, DeltaTime);


	SetActorLocation(Position);

}

// Called to bind functionality to input
void ASnakePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}


void ASnakePawn::Jump()
{
	// average human can jump 3.5m/s vertically
	m_velocity_z = VERTICAL_JUMP_SPEED;
}

void ASnakePawn::SetDirection(Direction p_direction)
{
	// AI pawn won't take input
	if (isAIControlled == false)
	{
		m_nextDirection = p_direction;
	}
}

void ASnakePawn::HandleGravity(FVector& p_position, float p_deltaTime)
{
	p_position.Z += m_velocity_z * p_deltaTime + 0.5f * GRAV_ACCEL * p_deltaTime * p_deltaTime;
	m_velocity_z += GRAV_ACCEL * p_deltaTime;

	// don't fall through the floor
	if (p_position.Z < 0)
	{
		p_position.Z = 0.0f;
		m_velocity_z = 0.0f;
	}
}

void ASnakePawn::HandleGround(FVector& p_position, float p_deltaTime)
{
	if (!isAIControlled && m_direction == Direction::None && m_nextDirection == Direction::None)
	{
		return;
	}

	FVector rightVector = GetActorRightVector();
	FVector forwardVector = GetActorForwardVector();

	float totalMoveDistance = m_speed * p_deltaTime;

	float moveDistance = totalMoveDistance;

	if (m_movedTileDistance + moveDistance >= m_gridSize)
	{
		moveDistance = m_gridSize - m_movedTileDistance;
	}

	m_movedTileDistance += moveDistance;

	switch (m_direction)
	{
	case Direction::Right:
		p_position += rightVector * moveDistance;
		break;
	case Direction::Up:
		p_position += forwardVector * moveDistance;
		break;
	case Direction::Left:
		p_position += rightVector * moveDistance * -1.0f;
		break;
	case Direction::Down:
		p_position += forwardVector * moveDistance * -1.0f;
	default:
		break;
	}

	if (m_movedTileDistance >= m_gridSize)
	{
		m_movedTileDistance = 0.0f;
		if (!isAIControlled)
		{
			m_direction = m_nextDirection;
		}
		else
		{
			// AI has no direction cache; it will calculate in next tick
			m_direction = Direction::None;
		}
	}

	//UE_LOG(LogTemp, Warning, TEXT("moveDistance: %f, m_gridSize: %f"), m_movedTileDistance, m_gridSize);
}

void ASnakePawn::SetDestinationTileList(std::vector<Tile*>& p_route)
{
	// copy from route
	//m_direction = Direction::None;
	m_destinationTileList.assign(p_route.begin(), p_route.end());
}

void ASnakePawn::GetPawnTile(int& x, int& y)
{
	FVector Position = GetActorLocation();
	x = roundf(Position.X / 100.0f);
	y = roundf(Position.Y / 100.0f);
	UE_LOG(LogTemp, Warning, TEXT("GetPawnTile: posX %f, posY %f, direction=%d"), Position.X, Position.Y, m_direction);
}

void ASnakePawn::SetAIControlled(bool p_isAIControlled)
{
	isAIControlled = p_isAIControlled;
	if (isAIControlled == true)
	{
		// AI moves without input delay and observation, need to slow down
		m_speed = 400.0f;
	}
	else
	{
		m_speed = 500.0f;
	}
}

void ASnakePawn::Score()
{
	currentScore += 1;
	if (currentScore >= maxScore)
	{
		// next level
		m_snakeWorld->GoToNextLevel();
	}
}

void ASnakePawn::SetWorldActor(ASnakeWorld* p_snakeWorld)
{
	m_snakeWorld = p_snakeWorld;
}
