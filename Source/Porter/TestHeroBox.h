// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TestHeroBox.generated.h"

UCLASS()
class PORTER_API ATestHeroBox : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATestHeroBox();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere)
	class APawn* Player;

	UPROPERTY(EditAnywhere)
	FVector Offset;

	UPROPERTY()
	int32 HeroNum;

};
