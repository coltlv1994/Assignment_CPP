// Fill out your copyright notice in the Description page of Project Settings.


#include "SnakeWorld.h"
#include "SnakePlayerController.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASnakeWorld::ASnakeWorld()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	m_gridSize[0] = 0;
	m_gridSize[1] = 0;

	m_sceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));

	RootComponent = m_sceneComponent;

	m_instancedWalls_p = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("InstancedWalls"));
	m_instancedWalls_p->SetupAttachment(RootComponent);

	m_collisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	m_collisionBox->SetupAttachment(m_instancedWalls_p);
	//m_collisionBox
}

// Called when the game starts or when spawned
void ASnakeWorld::BeginPlay()
{
	Super::BeginPlay();

	appleOnMap->AppleEaten.AddDynamic(this, &ASnakeWorld::HandleAppleEatenEvent);

	GoToNextLevel();

	HandleAppleEatenEvent(appleOnMap);
}

// Called every frame
void ASnakeWorld::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASnakeWorld::ConstructWalls(void)
{
	m_tileMap.clear();
	//delete m_gridMapPointer;
	m_instancedWalls_p->ClearInstances();
	UGameStateSubsystem* MySubsystemPtr = GetWorld()->GetGameInstance()->GetSubsystem<UGameStateSubsystem>();

	TArray<FString> Lines;
	FString FilePath = FPaths::ProjectDir() + TEXT("Levels/level") + FString::FromInt(nextLevel) + TEXT(".txt");

	if (FPaths::FileExists(FilePath))
	{
		if (FFileHelper::LoadFileToStringArray(Lines, *FilePath))
		{
			int y = 0;
			m_gridSize[0] = Lines[0].Len(); // x-axis; how many tiles per row
			m_gridSize[1] = Lines.Num(); // y-axis; how many rows in this map

			for (const FString& line : Lines)
			{
				for (int x = 0; x < m_gridSize[0]; x++)
				{
					if (line[x] == '#')
					{
						m_instancedWalls_p->AddInstance(FTransform(FRotator::ZeroRotator, FVector(x * 100.0f, y * 100.0f, 0.0f)));
						m_tileMap[m_gridSize[0] * y + x] = TileProperty::Obstacle;
					}
					else
					{
						m_tileMap[m_gridSize[0] * y + x] = TileProperty::Space;
					}
				}

				y += 1;
			}

			m_gridMapPointer = new GridMap(m_gridSize[0], m_gridSize[1], m_tileMap);

			if (MySubsystemPtr)
			{
				MySubsystemPtr->SetGridMap(m_gridMapPointer);
			}
		}
	}
	else
	{
		// game over, change to outro_state
		//if (MySubsystemPtr)
		//{
		//	MySubsystemPtr->ChangeGameState(SnakeGameState::Outro);
		//}
		UWorld* TheWorld = GetWorld();
		UGameplayStatics::OpenLevel(TheWorld, "Outro");
	}
	
}

void ASnakeWorld::NewAppleLocation(int& p_AppleX, int& p_AppleY)
{
	int oldAppleTile = m_appleTile;
    int newAppleTile = 0;

	do
	{
		newAppleTile = FMath::RandRange(0, m_tileMap.size());
	} while (m_tileMap[newAppleTile] != TileProperty::Space);

	p_AppleX = newAppleTile % m_gridSize[1];
	p_AppleY = newAppleTile / m_gridSize[1];

	// change apple tile property
	m_tileMap[oldAppleTile] = TileProperty::Space;
	m_tileMap[newAppleTile] = TileProperty::Apple;

	// update apple tile
	m_appleTile = newAppleTile;

	m_gridMapPointer->SetTileProperty(p_AppleX, p_AppleY, TileProperty::Apple);
	m_gridMapPointer->SetTileProperty(oldAppleTile / m_gridSize[1], oldAppleTile % m_gridSize[1], TileProperty::Space);

	UE_LOG(LogTemp, Warning, TEXT("Apple location: %d, %d"), p_AppleX, p_AppleY);
}

void ASnakeWorld::HandleAppleEatenEvent(AApple* appleActor)
{
	int appleX = 0, appleY = 0;

	NewAppleLocation(appleX, appleY);

	appleActor->ChangeAppleLocation(appleX, appleY);

	if (IsValid(controllerRef))
	{
		controllerRef->NewAppleRoute(appleX, appleY);
	}
}

std::map<int, TileProperty>* ASnakeWorld::GetTileMapPointer(void)
{
	return &m_tileMap;
}

uint16_t ASnakeWorld::GetAppleTile(void)
{
	return m_appleTile;
}

std::array<int, 2>* ASnakeWorld::GetGridSize(void)
{
	// I know it is a bad practice and don't do that
	return &m_gridSize;
}

void ASnakeWorld::TranslateTileIndexToTiles(std::vector<int> &tileIndexVector, std::vector<Tile*> &targetTiles)
{
	m_gridMapPointer->ConstructPath(tileIndexVector, targetTiles);
}

void ASnakeWorld::GoToNextLevel()
{
	nextLevel += 1;
	ConstructWalls();

	// reset must be called after wall re-construction since it relies on new tile map to reset A*-algorithm
	// call controller, reset pawns
	if (nextLevel != 1 && IsValid(controllerRef))
	{
		controllerRef->ResetPawns();
	}
}