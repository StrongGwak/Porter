// Fill out your copyright notice in the Description page of Project Settings.


#include "Hero/PHeroWeaponAnimInstance.h"


UPHeroWeaponAnimInstance::UPHeroWeaponAnimInstance()
{
	/*static ConstructorHelpers::FObjectFinder<UAnimMontage> TempAnim(TEXT("/Script/Engine.AnimMontage'/Game/Porter/Develop/Hero/Animation/Soldier/AM_Dangpa.AM_Dangpa'"));
	if (TempAnim.Succeeded()) 
	{
		Attack = TempAnim.Object;
	}*/
}

void UPHeroWeaponAnimInstance::StartAttack()
{
	bIsAttack = true;
	Montage_Play(Attack);
}

void UPHeroWeaponAnimInstance::StopAttack()
{
	bIsAttack = false;
	if (Montage_IsPlaying(Attack))
	{
		Montage_Stop(0.25f, Attack);
	}
}
