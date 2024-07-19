// Fill out your copyright notice in the Description page of Project Settings.


#include "PPlayer.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/ArrowComponent.h"
#include "InputMappingContext.h"
#include "Engine/Engine.h"
#include "TestHeroBox.h"
#include "VectorTypes.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APPlayer::APPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<USkeletalMesh> SkeletalMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Mannequins/Meshes/SKM_Manny.SKM_Manny'"));
	if (SkeletalMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SkeletalMesh.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));
	}

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->SetWorldLocation(FVector(0, 0, 70));
	SpringArm->TargetArmLength = 400;
	SpringArm->bUsePawnControlRotation=true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
	bUseControllerRotationYaw = true; 

	FObjectFinderInputManager();
	MakeArrays();
	UpdateStats(PlayerAndHeroStats);
}

// Called when the game starts or when spawned
void APPlayer::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			SubSystem->AddMappingContext(IMC, 0);
		}
	}
}

// Called every frame
void APPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//
	// GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, FString::FromInt(HeroSpawnLocation->GetForwardVector().X * 100));
	
	// 달릴 수 없으면, 달리지 못한다 -> !끼리 묶는건 의미 x
	if (bIsRun && bCanRun)	// 달리고 있고, 달리기 누를 수 있는 상태
	{
		CurrentStamina -= DecreaseStamina * DeltaTime;
		if (CurrentStamina <= 0)
		{
			bCanRun = false;
			StopRun();
			CurrentStamina = 0;
		}
	}
	else if (!bIsRun && bCanRun && CurrentStamina < MaxStamina) // 달리고 있지 않은데, 달리기 누를 수는 있는 상태 == 휴식타임
	{
		CurrentStamina += IncreaseStamina * DeltaTime;
		if (CurrentStamina > MaxStamina)
		{
			CurrentStamina = MaxStamina;
		}
	}
	else if (!bCanRun)
	{
		CurrentStamina += ZeroToHundredIncreaseStamina * DeltaTime;
		if (CurrentStamina > MaxStamina)
		{
			CurrentStamina = MaxStamina;
			bCanRun = true;
		}
	}
	// GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, FString::FromInt(CurrentStamina));
}

// Called to bind functionality to input
void APPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APPlayer::Move);
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APPlayer::Look);
	EnhancedInputComponent->BindAction(TestUpAction, ETriggerEvent::Started, this, &APPlayer::Up);
	EnhancedInputComponent->BindAction(TestDownAction, ETriggerEvent::Started, this, &APPlayer::Down);
	EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Started, this, &APPlayer::Run);
	EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Completed, this, &APPlayer::StopRun);
	EnhancedInputComponent->BindAction(SwapAction, ETriggerEvent::Started, this, &APPlayer::PlaySwap);
}

// 반드시 같은 크기의 행렬만 온다는 가정하기 -> 따로 함수로 만들어야 함
void APPlayer::SwapHeroesByArr(TArray<int32> SwapArray)
{
	// SwapArray : 5 4 3 2 1 == 위치한 HeroNum. 1번부터 시작한다. 1~연속된 숫자가 들어가있어야 한다. 
	int32 ArrayLength = SwapArray.Num();
	// 비우기
	TempSwapArray.Empty();
	TempSwapArray.Append(HeroBoxArray);	// 0~4에 1~5번 히어로 들어가 있음 -> 위치 배치할 때만 +1 해주기. HeroNum이 1부터 시작하니까
	for (int i=0; i<ArrayLength; ++i)
	{
		HeroBoxArray.Emplace(TempSwapArray[SwapArray[i]-1]);
		HeroBoxArray.RemoveAtSwap(i);

		// 실체 위치 변경
		HeroBoxArray[i]->SetActorLocation(GetActorLocation() + GetActorForwardVector()*OffsetArr[i+1].X + GetActorRightVector()*OffsetArr[i+1].Y + FVector(0, 0, OffsetArr[i+1].Z));
	}
}

// 블루프린트에서 업데이트 할 사항
void APPlayer::UpdateStats(FPlayerStatsStruct UpdateStat)
{
	PlayerAndHeroStats = UpdateStat;
	
	// 바로 적용해야 할 것들 나중에 따로 정리 필요하긴 함
	// 틱에서 검사하는게 아니니까 이 과정이 필요하다. 
	GetCharacterMovement()->MaxWalkSpeed = PlayerAndHeroStats.PlayerWalkSpeed;

	// 현재 용병 수가 max를 넘게 될 경우 없애버리기
	while (HeroNum > PlayerAndHeroStats.MaxHeroNum)
	{
		Down();
	}

	MaxHeroHP += PlayerAndHeroStats.PlusMaxHeroHP;
	MaxStamina = PlayerAndHeroStats.PlayerMaxStamina;
	DecreaseStamina = PlayerAndHeroStats.PlayerDecreaseStamina;
	IncreaseStamina = PlayerAndHeroStats.PlayerIncreaseStamina;
	ZeroToHundredIncreaseStamina = PlayerAndHeroStats.PlayerZeroToHundredIncreaseStamina;
}

void APPlayer::PlusHP(int32 Heal)
{
	if (LastHeroHP == MaxHeroHP) LastHeroHP = MaxHeroHP; 
	else if (Heal + LastHeroHP >= MaxHeroHP) LastHeroHP = MaxHeroHP;
	else LastHeroHP += Heal;
	CurrentHP = MaxHeroHP * (HeroNum - 1) + LastHeroHP;
}

void APPlayer::MinusHP(int32 Damage)
{
	if (Damage > MaxHp)
	{
		MaxHp = 0;
		CurrentHP = 0;
		Die();
		return;
	}
	int32 NumHeroDie = Damage / MaxHeroHP;
	int32 UpdateLastHeroHP = Damage % MaxHeroHP;
	
	if (LastHeroHP > Damage)
	{
		LastHeroHP -= Damage;
		CurrentHP = MaxHeroHP * (HeroNum - 1) + LastHeroHP;
	}
	else if (LastHeroHP > UpdateLastHeroHP)
	{
		LastHeroHP -= UpdateLastHeroHP;
		for(int i=0; i<NumHeroDie; i++)
		{
			Down();
		}
	}
	else
	{
		LastHeroHP = LastHeroHP + MaxHeroHP - UpdateLastHeroHP;
		for(int i=0; i<NumHeroDie+1; i++)
		{
			Down();
		}
	}

	// GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, FString::FromInt(CurrentHP));
}

void APPlayer::Move(const FInputActionValue& Value)
{
	const FVector2D MovementVector = Value.Get<FVector2D>();
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDirection, MovementVector.Y);
	AddMovementInput(RightDirection, MovementVector.X);
}

void APPlayer::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	AddControllerPitchInput(LookAxisVector.Y * GetWorld()->DeltaTimeSeconds * mouseSpeed * -1);
	AddControllerYawInput(LookAxisVector.X * GetWorld()->DeltaTimeSeconds * mouseSpeed);
}

void APPlayer::Run()
{
	if(bCanRun)
	{
		GetCharacterMovement()->MaxWalkSpeed = PlayerAndHeroStats.PlayerRunSpeed;
		bIsRun = true;
	}
}

void APPlayer::StopRun()
{
	bIsRun = false;
	GetCharacterMovement()->MaxWalkSpeed = PlayerAndHeroStats.PlayerWalkSpeed;
}

// Hero 1개 생성
void APPlayer::Up()
{
	if (HeroNum < PlayerAndHeroStats.MaxHeroNum)
	{
		HeroNum++;
		SpringArm->TargetArmLength = 400 + PorterFloorArray[HeroNum] * AddCameraLength;
		FVector RelativeOffset = GetActorForwardVector()*OffsetArr[HeroNum].X + GetActorRightVector()*OffsetArr[HeroNum].Y + FVector(0, 0, OffsetArr[HeroNum].Z);
		FVector SpawnLocation = GetActorLocation() + RelativeOffset; 
		
		ACharacter* TestHeroBox = GetWorld()->SpawnActor<ACharacter>(HeroBoxSpawner, SpawnLocation, GetActorRotation());

		if(TestHeroBox)
		{
			HeroBoxArray.Emplace(TestHeroBox);
			TestHeroBox->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);
		}

		TestHeroBox->SetActorEnableCollision(false);
		// GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, FString::FromInt(HeroNum));
		// MaxHp, CurrentHP 업데이트
		MaxHp = MaxHeroHP * HeroNum;
		CurrentHP = MaxHeroHP * (HeroNum - 1) + LastHeroHP;
		// GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, FString::FromInt(CurrentHP));
	}
}

// Hero 1개 파괴
void APPlayer::Down()
{
	if (HeroNum > 0)
	{
		HeroNum--;
		SpringArm->TargetArmLength = 400 + PorterFloorArray[HeroNum] * AddCameraLength;

		if(HeroBoxArray.Num() > 0)
		{
			ACharacter* LastHeroBox = HeroBoxArray.Last();

			if(LastHeroBox)
			{
				LastHeroBox->Destroy();
				HeroBoxArray.RemoveAt(HeroNum);
			}
		}
		// MaxHp, CurrentHP 업데이트
		MaxHp = MaxHeroHP * HeroNum;
		CurrentHP = MaxHeroHP * (HeroNum - 1) + LastHeroHP;
		// GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, FString::FromInt(CurrentHP));
	}
}

// 생성자에서 미리 배열 만들어두기
void APPlayer::MakeArrays()
{
	int PorterFloor = 0;
	// 첫 값이 0이 들어가고, swap을 위한 임시 칸이 있어야 하므로 17개 제작
	for(int TempHeroNum=0; TempHeroNum<15+2; TempHeroNum++)
	{
		// Hero 수에 따른 카메라 레벨 == 쌓은 층 수
		PorterFloor = 0;
		while (true)
		{
			if (TempHeroNum <= PorterFloor * (PorterFloor + 1) / 2) break;
			else PorterFloor++;
		}
		// 배열 추가
		PorterFloorArray.Emplace(PorterFloor);
		
		// 층수로 계산하는 스폰 위치 Offset
		float OffsetY = 0;
		float CurrentSpawnNum = TempHeroNum - (PorterFloor-1)*PorterFloor / 2;
		int ForCount = round(PorterFloor);
		for(int i = 0; i<round(CurrentSpawnNum); i++)
		{
			if (i%2 == 1) OffsetY = -1*OffsetY - 2*PorterWidth;
			else OffsetY *= -1;
		}
		if (ForCount%2 == 0) OffsetY += PorterWidth;
		OffsetArr.Emplace(FVector(-100, OffsetY, PorterFloor*PorterHeight));
	}
}

void APPlayer::FObjectFinderInputManager()
{
	static ConstructorHelpers::FObjectFinder<UInputMappingContext>InputMappingContext(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Porter/Develop/Inputs/IMC_Player.IMC_Player'"));
	if (InputMappingContext.Object != nullptr)
	{
		IMC = InputMappingContext.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction>InputMove(TEXT("/Script/EnhancedInput.InputAction'/Game/Porter/Develop/Inputs/IA_Move.IA_Move'"));
	if (InputMove.Object != nullptr)
	{
		MoveAction = InputMove.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction>InputLook(TEXT("/Script/EnhancedInput.InputAction'/Game/Porter/Develop/Inputs/IA_Look.IA_Look'"));
	if (InputLook.Object != nullptr)
	{
		LookAction = InputLook.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction>InputUp(TEXT("/Script/EnhancedInput.InputAction'/Game/Porter/Develop/Inputs/IA_TestUp.IA_TestUp'"));
	if (InputLook.Object != nullptr)
	{
		TestUpAction = InputUp.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction>InputDown(TEXT("/Script/EnhancedInput.InputAction'/Game/Porter/Develop/Inputs/IA_TestDown.IA_TestDown'"));
	if (InputLook.Object != nullptr)
	{
		TestDownAction = InputDown.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction>InputRun(TEXT("/Script/EnhancedInput.InputAction'/Game/Porter/Develop/Inputs/IA_Run.IA_Run'"));
	if (InputLook.Object != nullptr)
	{
		RunAction = InputRun.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction>InputSwap(TEXT("/Script/EnhancedInput.InputAction'/Game/Porter/Develop/Inputs/IA_Swap24.IA_Swap24'"));
	if (InputLook.Object != nullptr)
	{
		SwapAction = InputSwap.Object;
	}
}

void APPlayer::Die()
{
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("YOU DIE"));
}

void APPlayer::PlaySwap()
{
	// 반드시 처음에는 0이 있어야 함 <- 아님. 그냥 생각하기
	SwapHeroesByArr(TArray<int32>{5, 4, 3, 2, 1});
}

