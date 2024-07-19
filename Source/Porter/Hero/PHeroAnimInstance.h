// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PHeroAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PORTER_API UPHeroAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPHeroAnimInstance();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy")
	bool Attack;
	
};
