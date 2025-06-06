// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "GameFramework/Actor.h"
#include "Apple.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAppleEaten, AApple*, appleActor);

UCLASS()
class MAZEPROJECT_API AApple : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	USceneComponent* m_sceneComponent;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UInstancedStaticMeshComponent* m_instancedApple_p;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	USphereComponent* m_appleCollision;

	bool m_isAppleCreated = false;
	
public:	
	// Sets default values for this actor's properties
	AApple();

	UFUNCTION(BlueprintCallable)
	void EatApple();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintAssignable)
	FOnAppleEaten AppleEaten;

	void ChangeAppleLocation(int appleX, int appleY);

};
