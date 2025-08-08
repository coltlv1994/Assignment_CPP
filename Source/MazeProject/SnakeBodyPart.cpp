// Fill out your copyright notice in the Description page of Project Settings.


#include "SnakeBodyPart.h"

// Sets default values
ASnakeBodyPart::ASnakeBodyPart()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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

}

