// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/MineCharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MineCharacterControlData.h"
#include "Animation/AnimMontage.h"
#include "MineComboActionData.h"
#include "Physics/MineCollision.h"
#include "Engine/DamageEvents.h"
#include "CharacterStat/MineCharacterStatComponent.h"
#include "UI/MineWidgetComponent.h"
#include "UI/MineHpBarWidget.h"
#include "Item/MineWeaponItemData.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"

//log
DEFINE_LOG_CATEGORY(LogMineCharacter);

UNiagaraSystem* AMineCharacterBase::SwordFireFX = nullptr;



// Sets default values
AMineCharacterBase::AMineCharacterBase()
{
	/***********************************
	* 무브먼트 TMap 기본값 설정
	***********************************/
	//기본 무브먼트
	MovementMap.Add({ ESpeedGroup::Basic, EMovementType::Walk }, 200.f);
	MovementMap.Add({ ESpeedGroup::Basic, EMovementType::Sprint }, 500.f);
	MovementMap.Add({ ESpeedGroup::Basic, EMovementType::CrouchWalk }, 150.f);
	MovementMap.Add({ ESpeedGroup::Basic, EMovementType::CrouchSprint }, 300.f);
	MovementMap.Add({ ESpeedGroup::Basic, EMovementType::Swim }, 200.f);
	MovementMap.Add({ ESpeedGroup::Basic, EMovementType::SwimSprint }, 400.f);
	MovementMap.Add({ ESpeedGroup::Basic, EMovementType::JumpZVelocity }, 400.f);

	//저속 무브먼트
	MovementMap.Add({ ESpeedGroup::Low, EMovementType::Walk }, 200.f);
	MovementMap.Add({ ESpeedGroup::Low, EMovementType::Sprint }, 500.f);
	MovementMap.Add({ ESpeedGroup::Low, EMovementType::CrouchWalk }, 150.f);
	MovementMap.Add({ ESpeedGroup::Low, EMovementType::CrouchSprint }, 300.f);
	MovementMap.Add({ ESpeedGroup::Low, EMovementType::Swim }, 200.f);
	MovementMap.Add({ ESpeedGroup::Low, EMovementType::SwimSprint }, 400.f);
	MovementMap.Add({ ESpeedGroup::Low, EMovementType::JumpZVelocity }, 400.f);

	//고속 무브먼트
	MovementMap.Add({ ESpeedGroup::High, EMovementType::Walk }, 200.f);
	MovementMap.Add({ ESpeedGroup::High, EMovementType::Sprint }, 500.f);
	MovementMap.Add({ ESpeedGroup::High, EMovementType::CrouchWalk }, 150.f);
	MovementMap.Add({ ESpeedGroup::High, EMovementType::CrouchSprint }, 300.f);
	MovementMap.Add({ ESpeedGroup::High, EMovementType::Swim }, 200.f);
	MovementMap.Add({ ESpeedGroup::High, EMovementType::SwimSprint }, 400.f);
	MovementMap.Add({ ESpeedGroup::High, EMovementType::JumpZVelocity }, 400.f);




	// Capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	GetCapsuleComponent()->SetCollisionProfileName(CPROFILE_MINECAPSULE);

	// Movement(Basic)
	GetCharacterMovement()->JumpZVelocity = GetMovement(ESpeedGroup::Basic, EMovementType::JumpZVelocity);
	GetCharacterMovement()->MaxWalkSpeed = GetMovement(ESpeedGroup::Basic, EMovementType::Walk);
	GetCharacterMovement()->MaxWalkSpeedCrouched =GetMovement(ESpeedGroup::Basic, EMovementType::CrouchWalk);
	GetCharacterMovement()->AirControl = 0.0f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Mesh
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -100.0f), FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/AIMakeAssset/Youtube/Devil_Women/DevilWomen.DevilWomen'"));
	if (CharacterMeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(CharacterMeshRef.Object);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef(TEXT("/Game/AIMakeAssset/Youtube/Devil_Women/Animation/ABP_MineCharacter.ABP_MineCharacter_C"));
	if (AnimInstanceClassRef.Class)
	{
		GetMesh()->SetAnimInstanceClass(AnimInstanceClassRef.Class);
	}

	//CharacterControlData Section
	static ConstructorHelpers::FObjectFinder<UMineCharacterControlData> FirstPersonDataRef(TEXT("/Script/ChaosMine.MineCharacterControlData'/Game/ChaosMine/CharacterControl/Mine_FirstPerson.Mine_FirstPerson'"));
	if (FirstPersonDataRef.Object)
	{
		CharacterControlManager.Add(ECharacterControlType::FirstPerson, FirstPersonDataRef.Object);
	}

	static ConstructorHelpers::FObjectFinder<UMineCharacterControlData> ThirdDataRef(TEXT("/Script/ChaosMine.MineCharacterControlData'/Game/ChaosMine/CharacterControl/Mine_ThirdPerson.Mine_ThirdPerson'"));
	if (ThirdDataRef.Object)
	{
		CharacterControlManager.Add(ECharacterControlType::ThirdPerson, ThirdDataRef.Object);
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ComboActionMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/AIMakeAssset/CharacterPlayer/Animation/AM_ComboAttack.AM_ComboAttack'"));
	if (ComboActionMontageRef.Object)
	{
		ComboActionMontage = ComboActionMontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UMineComboActionData> ComboActionDataRef(TEXT("/Script/ChaosMine.MineComboActionData'/Game/ChaosMine/CharacterAction/MinePlayer_ComboAttack.MinePlayer_ComboAttack'"));
	if (ComboActionDataRef.Object)
	{
		ComboActionData = ComboActionDataRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> DeadMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/AIMakeAssset/CharacterPlayer/Animation/AM_Dead.AM_Dead'"));
	if (DeadMontageRef.Object)
	{
		DeadMontage = DeadMontageRef.Object;
	}
	

	//Stat Component
	Stat = CreateDefaultSubobject<UMineCharacterStatComponent>(TEXT("Stat"));



	//Widget Component
	HpBar = CreateDefaultSubobject<UMineWidgetComponent>(TEXT("Widget"));
	HpBar->SetupAttachment(GetMesh());
	HpBar->SetRelativeLocation(FVector(0.0f, 0.0f, 185.0f));
	static ConstructorHelpers::FClassFinder<UUserWidget> HpBarWidgetRef(TEXT("/Game/ChaosMine/UI/WBP_HpBar.WBP_HpBar_C"));
	if (HpBarWidgetRef.Class)
	{
		HpBar->SetWidgetClass(HpBarWidgetRef.Class);
		HpBar->SetWidgetSpace(EWidgetSpace::Screen);
		HpBar->SetDrawSize(FVector2D(150.0f, 15.0f));
		HpBar->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}


	//Item Actions
	TakeItemActions.Add(FTakeItemDelegateWrapper(FOnTakeItemDelegate::CreateUObject(this, &AMineCharacterBase::EquipWeapon)));
	TakeItemActions.Add(FTakeItemDelegateWrapper(FOnTakeItemDelegate::CreateUObject(this, &AMineCharacterBase::DrinkPotion)));
	TakeItemActions.Add(FTakeItemDelegateWrapper(FOnTakeItemDelegate::CreateUObject(this, &AMineCharacterBase::ReadScroll)));

	// Weapon Component
	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"));
	Weapon->SetupAttachment(GetMesh(), TEXT("RightHandSocket"));


	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> SwordFXRef(
		TEXT("/Script/Niagara.NiagaraSystem'/Game/SlashTrailElemental/Niagara/SlashTrail/NS_SlashTrail_Fire.NS_SlashTrail_Fire'"));   // ← 당신 FX 경로
	if (SwordFXRef.Succeeded())
	{
		SwordFireFX = SwordFXRef.Object;                 // ★★★ 2. 포인터 대입 ★★★
	}

}

void AMineCharacterBase::PostInitializeComponents()
{
	//BeginPlay에서도 바인딩 해줘도 되고 생성자에서 바인딩도 가능함.
	Super::PostInitializeComponents();

	Stat->OnHpZero.AddUObject(this, &AMineCharacterBase::SetDead);
}

void AMineCharacterBase::SetCharacterControlData(const UMineCharacterControlData* CharacterControlData)
{
	// Pawn
	bUseControllerRotationYaw = CharacterControlData->bUseControllerRotationYaw;
	bUseControllerRotationPitch = CharacterControlData->bUseControllerRotationPitch;
	bUseControllerRotationRoll = CharacterControlData->bUseControllerRotationRoll;

	// CharacterMovement
	GetCharacterMovement()->bOrientRotationToMovement = CharacterControlData->bOrientRotationToMovement;
	GetCharacterMovement()->bUseControllerDesiredRotation = CharacterControlData->bUseControllerDesiredRotation;
	GetCharacterMovement()->RotationRate = CharacterControlData->RotationRate;
}

void AMineCharacterBase::ProcessComboCommand()
{
	if (CurrentCombo == 0)
	{
		ComboActionBegin();
		return;
	}

	/*설정이 안되어있는데 입력이 들어올때*
	*1. 이미 타이머가 발동이 되어서 시기를 놓쳤거나
	*2. 더이상 진행할 필요가 없다는 뜻 따라서 
	* 이경우엔 HasNextComboCommand = false
	*
	**만약 타이머가 유효하다면*
	*1. 체크하기 이전에 다음 섹션으로 이동시킬 커맨드가 발동했다는 뜻이기에 HasNextComboCommand를 true로 설정
	**********************************/
	if (!ComboTimerHandle.IsValid())
	{
		HasNextComboCommand = false;
	}
	else
	{
		HasNextComboCommand = true;
	}


}

void AMineCharacterBase::ComboActionBegin()
{
	/************Combo Status************/
	CurrentCombo = 1;

	/************Movement Setting************/
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	/************Animation Setting************/
	const float AttackSpeedRate = 1.0f; //애니메이션 재생 속도
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(ComboActionMontage,AttackSpeedRate);



	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &AMineCharacterBase::ComboActionEnd);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, ComboActionMontage); //몽타주가 종료될때 자동으로 ComboActionEnd함수 호출

	//콤보가 시작할때 타이머를 발동시켜야 한다.
	ComboTimerHandle.Invalidate();//타이머 핸들이무효화 되도록 초기화
	SetComboCheckTimer();
}

void AMineCharacterBase::ComboActionEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	/*******몽타주가 종료 되었을때********/
	//몽타주가 종료되었을때 CurrentCombo값이 0이되면 절대로 안되기 때문에 Assertion함수를 통해 체크
	ensure(CurrentCombo != 0); //만약 0이라면 출력로그에 에러가 발생함.
	CurrentCombo = 0; //콤보를 0으로 재설정
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking); //무브먼트값 재설정

	if (CurrentTrailFX)
	{
		CurrentTrailFX->Deactivate();
		CurrentTrailFX = nullptr;     // 포인터 해제
	}
}

void AMineCharacterBase::SetComboCheckTimer()
{
	//배열에 선언되어있는 프레임 정보를 얻기위해 인덱스값 조정
	//Data에셋을 보면 알겠지만. Combo의 인덱스는 0부터 시작 하지만 Current콤보는 초기는 0이지만 시작시 바로 1로 고정됨 
	//즉 마지막 콤보의 경우 CurrentCombo는 4지만 Data에셋의 콤보 인덱스는 3이기에 -1을 하여 조정하는 과정이 필요함.
	int32 ComboIndex = CurrentCombo - 1;

	//애셋 액션에 설정되어있는 각프레임 저보가 저 인덱스와 유효한지 검사도 해보자.
	ensure(ComboActionData->EffectiveFrameCount.IsValidIndex(ComboIndex));

	const float AttackSpeedRate = 1.0f;
	
	/****************ComboEffectiveTime*******************
	* ComboEffectiveTime정상 속도로 진행했을 때 소요될 시간 계산(우리가 코보를 발동할 시간 정보를 얻을 수 있음.
	* ComboEffectiveTime가 0보다 작으면 굳이 발동할필요가 없게됨.
	* 월드로부터 시간 서비스를 받아 현재 클래스에 있는 ComboCheck함수를 호출하되 계속 반복하지 않도록 false로 설정[한번만 발생하도록]
	* 즉 이타이머가 발동한다면 ComboCheck에서 타이머 핸들을 초기화 함[더이상 발동 되면 안되기에]
	* 
	/****************************************************/
	float ComboEffectiveTime = (ComboActionData->EffectiveFrameCount[ComboIndex] / ComboActionData->FrameRate) / AttackSpeedRate;
	if (ComboEffectiveTime > 0.0f)
	{
		GetWorld()->GetTimerManager().SetTimer(ComboTimerHandle, this, &AMineCharacterBase::ComboCheck, ComboEffectiveTime, false);
	}


}

void AMineCharacterBase::ComboCheck()
{
	ComboTimerHandle.Invalidate();

	if (HasNextComboCommand) //타이머 발동전에 입력이 들어와 HasnextComboCommand가 true가 된다면
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		CurrentCombo = FMath::Clamp(CurrentCombo + 1, 1, ComboActionData->MaxComboCount); //콤보 카운트가 증가하되 max를 넘지 않도록 Clamp해버림.

		/****다음 섹션에 대한 이름정보 가져올거임.****
		* 다음 섹션에 대한 정보를 가져올땐 스트링을 조합한다.(printf 함수 사용)
		* 두개로 구성되는데 하나는 애셋에 선언한 MontageSectionNamePrefix의 접두사 정보를 가져옴
		* 두번째는 콤보값을 조합하여 스트링으로 만들고 네임으로 변환하여 섹션 이름을 지정함.
		* AnimInstance에서 제공한 Montage_JumpToSection이라고 있는데 이 함수에다가 NextSection값을 넣어주고 COmboActionMontage를 지정해주면
		* 해당 섹션으로 재생이 바로 점프가 됨.
		* 그리고 바로 다시 타이머를 걸어줘야한다[SetComboCheckTimer()].
		* 그리고 입력값을 다시 초기화[HasNextComboCommand=false]
		******************************************/
		FName NextSection = *FString::Printf(TEXT("%s%d"), *ComboActionData->MontageSectionNamePrefix, CurrentCombo);;
		AnimInstance->Montage_JumpToSection(NextSection, ComboActionMontage);
		SetComboCheckTimer();
		HasNextComboCommand = false;
	}
}

void AMineCharacterBase::AttackHitCheck()
{
	/**************************************************
	* 이 함수는 트레이스 채널을 활용하여 물체가 서로 충돌되는지를 검사하는 로직임.
	**************************************************/
	
	/************************
	* SweepSingleByChannel 함수를 사용하는데 월드가 제공하는 서비스이기에 GetWorld를 통해 포인터를 얻어옴.
	* SweepSingleByChannel 함수는 결과값을 가져오는  HitResult라는 구조체를 첫번째 인자에 넣어줌
	* 두번째 세번째 인자에는 투사 시작지점과 끝지점을 넣어줌.
	* MakeSphere 함수를 통해 구체의 영역을 지정할 수가 있음.(밑에서는 우선 반지름이 50인 구체를 생성함.
	* 우리가 사용할 트레이스 채널은 전처리기에 추가한 CCHANNEL_MINEACTION 을 지정함.
	* ///////////////////////////////////////
	* FCollisionQueryParams 
	* 첫번째 인자는 태그 정보로 분석 할때 식별자 정보로 사용됨. SCENE_QUERY_STAT는 언리얼 엔진이 제공하는 분석툴이 있는데 Attack이라는 태그로 우리가 수행한 작업을 조사할 수 있게 태그 추가.
	* 두 번째 인자는 복잡한 형태의 충돌체도 감지할지에 대한 옵션(여기서는 복잡한건 고려하지 않기에 false로 지정)
	* 세 번째 인자는 무시할 액터들인데 자기자신만 무시할것이기에 this만 넣어줌.
	************************/
	FHitResult OutHitResult;
	FCollisionQueryParams Params(SCENE_QUERY_STAT(Attack), false, this);

	const float AttackRange = 40.0f;
	const float AttackRadius = 50.0f;
	const float AttackDamage = 30.0f;
	const FVector Start = GetActorLocation() + GetActorForwardVector() * GetCapsuleComponent()->GetScaledCapsuleRadius();
	const FVector End = Start + GetActorForwardVector() * AttackRange;

	bool HitDetected = GetWorld()->SweepSingleByChannel(OutHitResult, Start, End, FQuat::Identity, CCHANNEL_MINEACTION, FCollisionShape::MakeSphere(AttackRadius), Params);

	if (HitDetected) //공격이 판정된다면 감지되었을 경우
	{
		FDamageEvent DamageEvent;
		OutHitResult.GetActor()->TakeDamage(AttackDamage, DamageEvent, GetController(), this);
	}
	
	//드로우 디버그 세션.
#if ENABLE_DRAW_DEBUG
 
	FVector CapsuleOrigin = Start + (End - Start) * 0.5f;
	float CapsuleHalfHeight = AttackRange * 0.5f;
	FColor DrawColor = HitDetected ? FColor::Green : FColor::Red; //충돌하면 녹색 아니면 빨간색.

	DrawDebugCapsule(GetWorld(), CapsuleOrigin, CapsuleHalfHeight, AttackRadius, FRotationMatrix::MakeFromZ(GetActorForwardVector()).ToQuat(), DrawColor, false, 5.0f);

#endif


	if (!CurrentTrailFX || !CurrentTrailFX->IsActive())
	{
		CurrentTrailFX = UNiagaraFunctionLibrary::SpawnSystemAttached(
			SwordFireFX,
			GetMesh(),
			TEXT("RightHandSocket"),
			FVector::ZeroVector,
			FRotator::ZeroRotator,
			EAttachLocation::SnapToTarget,
			true      // AutoDestroy (Deactivate 후 파괴)
		);
	}
}

float AMineCharacterBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	//내가 누구한테 공격을 받았는지 파악할 수 있고, 최종 데미지 계산 가능(방어력등을 고려)
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	Stat->ApplyDamage(DamageAmount);
	
	
	
	return DamageAmount;
}

void AMineCharacterBase::SetDead()
{
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	PlayDeadAnimation();
	SetActorEnableCollision(false); //이렇게 설정하면 죽은 NPC는 캐릭터의 이동을 방해하지 않음.
	HpBar->SetHiddenInGame(true);


}

void AMineCharacterBase::PlayDeadAnimation()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->StopAllMontages(0.0f); //모든 몽타주 중지.
	AnimInstance->Montage_Play(DeadMontage, 1.0f);
}

void AMineCharacterBase::SetupCharacterWidget(UMineUserWidget* InUserWidget)
{
	UMineHpBarWidget* HpBarWidget = Cast<UMineHpBarWidget>(InUserWidget);
	if (HpBarWidget)
	{
		HpBarWidget->SetMaxHp(Stat->GetMaxHp());
		HpBarWidget->UpdateHpBar(Stat->GetCurrentHp());
		Stat->OnHpChanged.AddUObject(HpBarWidget, &UMineHpBarWidget::UpdateHpBar);
	}
}

void AMineCharacterBase::TakeItem(UMineItemData* InItemData)
{
	if (InItemData)
	{
		TakeItemActions[(uint8)InItemData->Type].ItemDelegate.ExecuteIfBound(InItemData);
	}
}

void AMineCharacterBase::DrinkPotion(UMineItemData* InItemData)
{
	UE_LOG(LogMineCharacter, Log, TEXT("Drink Potion"));
}

void AMineCharacterBase::EquipWeapon(UMineItemData* InItemData)
{
	UMineWeaponItemData* WeaponItemData = Cast<UMineWeaponItemData>(InItemData);
	if (WeaponItemData)
	{
		if (WeaponItemData->WeaponMesh.IsPending())
		{
			WeaponItemData->WeaponMesh.LoadSynchronous();
		}
		Weapon->SetSkeletalMesh(WeaponItemData->WeaponMesh.Get());
	}
}

void AMineCharacterBase::ReadScroll(UMineItemData* InItemData)
{
	UE_LOG(LogMineCharacter, Log, TEXT("Read Scroll"));
}
