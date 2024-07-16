// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Hero/PHeroStruct.h"
#include "Hero/PHeroAIController.h"
#include "GameFramework/Character.h"
#include "PHero.generated.h"

UCLASS()
class PORTER_API APHero : public ACharacter
{
	GENERATED_BODY()

public:
	APHero();
	// Sets default values for this character's properties
	APHero(const FPHeroStruct& HeroInfo);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Test")
	FString name;

private:
	UPROPERTY()
	APHeroAIController* AIController = nullptr;
	
// Hero Stat
protected:
	UPROPERTY(EditAnywhere, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	FString HName;

	UPROPERTY(EditAnywhere, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	int Damage;

	UPROPERTY(EditAnywhere, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	int AttackSpeed;

	UPROPERTY(EditAnywhere, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	UAnimMontage* AttackAnim;

	UPROPERTY(EditAnywhere, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	float SightRadius;

	UPROPERTY(EditAnywhere, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	float VisionAngle;

};
