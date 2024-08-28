// Fill out your copyright notice in the Description page of Project Settings.


#include "Hero/PHeroWeaponAnimInstance.h"


UPHeroWeaponAnimInstance::UPHeroWeaponAnimInstance()
{
	
}

void UPHeroWeaponAnimInstance::StartAttack()
{
	bIsAttack = true;
}

void UPHeroWeaponAnimInstance::StopAttack()
{
	bIsAttack = false;
}
