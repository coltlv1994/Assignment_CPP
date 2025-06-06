// Fill out your copyright notice in the Description page of Project Settings.


#include "Apple.h"
#include "Components/InstancedStaticMeshComponent.h"

// Sets default values
AApple::AApple()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	m_sceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));

	RootComponent = m_sceneComponent;

	m_instancedApple_p = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("InstancedApple"));
	m_instancedApple_p->SetupAttachment(RootComponent);

	m_appleCollision = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	m_appleCollision->SetupAttachment(m_instancedApple_p);

}

// Called when the game starts or when spawned
void AApple::BeginPlay()
{
	Super::BeginPlay();
	
	//AppleEaten.Broadcast(this);
}

// Called every frame
void AApple::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AApple::ChangeAppleLocation(int appleX, int appleY)
{
	if (m_isAppleCreated == false)
	{
		// instance a new apple in given location
		// ...
		m_instancedApple_p->ClearInstances();
		m_instancedApple_p->AddInstance(FTransform(FRotator::ZeroRotator, FVector(appleX * 100.0f, appleY * 100.0f, 0.0f)));
		m_isAppleCreated = true;
	}
	else
	{
		// move apple to new location
		FTransform appleTransform = FTransform(FRotator::ZeroRotator, FVector(appleX * 100.0f, appleY * 100.0f, 0.0f));
		m_instancedApple_p->UpdateInstanceTransform(0, appleTransform);
	}
}

void AApple::EatApple()
{
	AppleEaten.Broadcast(this);
}

