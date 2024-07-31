// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PHero.generated.h"

UCLASS()
class PORTER_API APHero : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APHero();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Index")
	int32 Index = -1;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Index")
	int32 HeroType = -1;

	UPROPERTY()
	class UPGameInstance* GI;
	
	UFUNCTION()
	void Die();
};
