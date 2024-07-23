// Fill out your copyright notice in the Description page of Project Settings.


#include "PPlayer.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "Engine/Engine.h"
#include "TestHeroBox.h"
#include "VectorTypes.h"
#include "GameFramework/CharacterMovementComponent.h"

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
	EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Started, this, &APPlayer::Boost);
	EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Completed, this, &APPlayer::StopBoost);
	EnhancedInputComponent->BindAction(SwapAction, ETriggerEvent::Started, this, &APPlayer::PlaySwap);
}

// 반드시 같은 크기의 행렬만 온다는 가정하기 -> 따로 함수로 만들어야 함
void APPlayer::SwapHeroesByArr(TArray<int32> SwapArray)
{
	// SwapArray : 5 4 3 2 1 == 위치한 HeroNum. 1번부터 시작한다. 1~연속된 숫자가 들어가있어야 한다. 
	// 비우기
	TempSwapArray.Empty();
	TempSwapArray.Append(HeroBoxArray);	// 0~4에 1~5번 히어로 들어가 있음 -> 위치 배치할 때만 +1 해주기. HeroNum이 1부터 시작하니까
	for (int i=0; i<HeroNum; ++i)
	{
		HeroBoxArray.Emplace(TempSwapArray[SwapArray[i]-1]);
		HeroBoxArray.RemoveAtSwap(i);

		// 실체 위치 변경
		HeroBoxArray[i]->SetActorLocation(GetActorLocation() + GetActorForwardVector()*OffsetArray[i+1].X + GetActorRightVector()*OffsetArray[i+1].Y + FVector(0, 0, OffsetArray[i+1].Z));
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
	MaxStamina = PlayerAndHeroStats.PlayerMaxStamina;
	DecreaseStamina = PlayerAndHeroStats.PlayerDecreaseStamina;
	IncreaseStamina = PlayerAndHeroStats.PlayerIncreaseStamina;
	ZeroToHundredIncreaseStamina = PlayerAndHeroStats.PlayerZeroToHundredIncreaseStamina;
}

void APPlayer::PlusHP(int32 Heal)
{
	if (CurrentHP + Heal >= MaxHp )
	{
		CurrentHP = MaxHp;
	}
	else
	{
		CurrentHP += Heal;
	}
}

void APPlayer::MinusHP(int32 Damage)
{
	if (CurrentHP > Damage)
	{
		CurrentHP -= Damage;
	}
	else
	{
		Down();
		CurrentHP = MaxHp;
	}
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

void APPlayer::Boost()
{
	if(bCanBoost)
	{
		bIsBoost = true;
		StartBoostTime = GetWorld()->GetTimeSeconds();
		TempStamina = CurrentStamina;
		GetWorldTimerManager().ClearTimer(RestTimeHandle);
		GetWorldTimerManager().SetTimer(BoostTimeHandle, this, &APPlayer::UpdateBoost, 0.1f, true);
		GetCharacterMovement()->MaxWalkSpeed = PlayerAndHeroStats.PlayerRunSpeed;
	}
}

void APPlayer::StopBoost()
{
	bIsBoost = false;
	RestTime = GetWorld()->GetTimeSeconds();
	TempStamina = CurrentStamina;
	GetCharacterMovement()->MaxWalkSpeed = PlayerAndHeroStats.PlayerWalkSpeed;
	GetWorldTimerManager().ClearTimer(BoostTimeHandle);
	// 이렇게 하지 않으면 뗐을때도 RestTimer를 Set해서 계속해서 시간 측정함
	if (CurrentStamina < MaxStamina)
	{
		GetWorldTimerManager().SetTimer(RestTimeHandle, this, &APPlayer::UpdateBoost, 0.1f, true);
	}
}

// 시간이 흐름 주의
void APPlayer::UpdateBoost()
{
	// 스태미나가 남아서 달릴 수 있는 상태
	if (bIsBoost && bCanBoost)
	{
		// CurrentBoostTime : 누적해서 커짐 - 1차 함수
		float CurrentBoostTime = GetWorld()->GetTimeSeconds()-StartBoostTime;
		CurrentStamina = TempStamina - DecreaseStamina*CurrentBoostTime;
		if (CurrentStamina <= 0)
		{
			bCanBoost = false;
			StopBoost();
			CurrentStamina = 0;
			RestTime = GetWorld()->GetTimeSeconds();
		}
	}
	// 휴식 타임
	else if(!bIsBoost && CurrentStamina < MaxStamina)
	{
		float CurrentRestTime = GetWorld()->GetTimeSeconds() - RestTime;
		CurrentStamina = TempStamina + IncreaseStamina*CurrentRestTime;
		if (CurrentStamina > MaxStamina)
		{
			bCanBoost = true;
			GetWorldTimerManager().ClearTimer(RestTimeHandle);
			CurrentStamina = MaxStamina;
		}
	}
	// 방전
	else if(!bCanBoost)
	{
		float CurrentEmptyTime = GetWorld()->GetTimeSeconds() - RestTime;
		CurrentStamina = TempStamina + ZeroToHundredIncreaseStamina*CurrentEmptyTime;
		if (CurrentStamina > MaxStamina)
		{
			bCanBoost = true;
			GetWorldTimerManager().ClearTimer(RestTimeHandle);
			CurrentStamina = MaxStamina;
		}
	}
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, FString::FromInt(CurrentStamina));
}

// 나중에 변수로 빼야함
void APPlayer::Up()
{
	MakeHero(0);
}

// Hero 1개 생성 + 종류 추가
// HeroNum과 PorterNum를 구분해서 써야함
// 또한, HeroIndex라는 변수도 생각해야함 - 이 Index는 1부터 시작하고 ... <- 그냥 0부터 시작하게 하면 안돼? 다른걸 고쳐서
// 
void APPlayer::MakeHero(int32 Value)
{
	if (HeroNum < PlayerAndHeroStats.MaxHeroNum)
	{
		HeroNum++;
		SpringArm->TargetArmLength = 400 + PorterFloorArray[HeroNum] * AddCameraLength;
		FVector RelativeOffset = GetActorForwardVector()*OffsetArray[HeroNum].X + GetActorRightVector()*OffsetArray[HeroNum].Y + FVector(0, 0, OffsetArray[HeroNum].Z);
		FVector SpawnLocation = GetActorLocation() + RelativeOffset; 
		ACharacter* TestHeroBox = GetWorld()->SpawnActor<ACharacter>(HeroType[Value], SpawnLocation, GetActorRotation());
		
		if(TestHeroBox)
		{
			HeroBoxArray.Emplace(TestHeroBox);
			TestHeroBox->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);
		}

		TestHeroBox->SetActorEnableCollision(false);
		// GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, FString::FromInt(HeroNum));
		// GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, FString::FromInt(CurrentHP));
		// CheckArrayNum(HeroBoxArray);
	}
}

// Hero 1개 파괴(가장 끝) - 지게의 수만큼 돌리며 서치 필요함
// HeroNum에 위치하지 않을 수 있다. <- 이부분이 가장 중요. 가장 큰 변경점
// 빈자리에 nullptr이 들어가있어야 한다. -> 거꾸로 검색하기
void APPlayer::Down()
{
	if (HeroNum > 0)
	{
		HeroNum--;
		SpringArm->TargetArmLength = 400 + PorterFloorArray[HeroNum] * AddCameraLength;

		ACharacter* LastHeroBox = HeroBoxArray.Last();

		if(LastHeroBox)
		{
			LastHeroBox->Destroy();
			HeroBoxArray.RemoveAt(HeroNum);
		}
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
		OffsetArray.Emplace(FVector(-100, OffsetY, PorterFloor*PorterHeight));
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
	// 1~HeroNum
	SwapHeroesByArr(TArray<int32>{5, 4, 3, 2, 1});
}

// 특정 이벤트에서 배치했을 때 사용하자
void APPlayer::CheckArrayNum(TArray<ACharacter*> CheckCharacterArray)
{
	int32 EntireArrayNum = CheckCharacterArray.Num();
	HeroNum = 0;
	for (int i=0;i<EntireArrayNum;++i)
	{
		if (CheckCharacterArray[i] != nullptr)
		{
			HeroNum++;
		}
	}
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, FString::FromInt(HeroNum));
}



