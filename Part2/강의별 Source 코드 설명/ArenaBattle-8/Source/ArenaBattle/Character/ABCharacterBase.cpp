// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ABCharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ABCharacterControlData.h"
#include "Animation/AnimMontage.h"
#include "ABComboActionData.h"
#include "Physics/ABCollision.h"
#include "Engine//DamageEvents.h"
#include "CharacterStat/ABCharacterStatComponent.h"
#include "UI/ABWidgetComponent.h"
#include "UI/ABHpBarWidget.h"
#include "Item/ABWeaponItemData.h"


DEFINE_LOG_CATEGORY(LogABCharacter)


// Sets default values
AABCharacterBase::AABCharacterBase()
{
	/*폰의 회전 지정 부분*/
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
	//위 부분은 카메라를 다룰때 자세히 설명할거임

	/*캡슐에 대한 설정*/
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	GetCapsuleComponent()->SetCollisionProfileName(CPROFILE_ABCAPSULE);
	//GetCapsuleComponent은 실제 구현이 필요해서 헤더 추가 해야한다.


	/*움직임에 해당하는 컴포넌트 설정*/
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	/*메쉬에 대한 선언*/
	//이 캐릭터들을 우리가 모델링파일을 진행방향에 맞춰가지고 상대의 위치와 회전을 지정해줘야 한다.
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -100.0f), FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/InfinityBladeWarriors/Character/CompleteCharacters/SK_CharM_Cardboard.SK_CharM_Cardboard'"));
	if (CharacterMeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(CharacterMeshRef.Object);
	}
	//매쉬의 경우엔 오브젝트를 가져와서 SetSkeletalMesh함수를 통해서 앞서서 분수대를 만들때처럼 오브젝트를 지정해주면 됨


	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef(TEXT("/Game/ArenaBattle/Animation/ABP_ABCharacter.ABP_ABCharacter_C"));
	if (AnimInstanceClassRef.Class)
	{
		GetMesh()->SetAnimInstanceClass(AnimInstanceClassRef.Class);
	}
	//애니메이션의 경우엔 클래스를 지정하도록 되어 있다. 따라서 클래스 정보를 가져와서 AnimInstanceClass라는 값에다가 해당 클래스 값을 설정해주도록 설계가 가능하다.


	/*맵 자료구조를 생성자에서 완성하기*/
	static ConstructorHelpers::FObjectFinder<UABCharacterControlData> ShoulderDataRef(TEXT("/Script/ArenaBattle.ABCharacterControlData'/Game/ArenaBattle/CharacterControl/ABC_Shoulder.ABC_Shoulder'"));
	if (ShoulderDataRef.Object)
	{
		CharacterControlManager.Add(ECharacterControlType::Shoulder, ShoulderDataRef.Object);
	}
	static ConstructorHelpers::FObjectFinder<UABCharacterControlData> QuaterDataRef(TEXT("/Script/ArenaBattle.ABCharacterControlData'/Game/ArenaBattle/CharacterControl/ABC_Quater.ABC_Quater'"));
	if (QuaterDataRef.Object)
	{
		CharacterControlManager.Add(ECharacterControlType::Quater, QuaterDataRef.Object);
	}

	/*
	여기서 몽타주 에셋을 ConstructorHelper로 지정할수 있는데 몽타주를 변경할때마다
	매번 코딩을 고치는것은 별로 생산적이지 않는다. 
	따라서 이제부터 블루프린트로 ABCharacter클래스를 확장하여 몽타주 에셋은 블루프린트에서 지정할거다.
	*/


	static ConstructorHelpers::FObjectFinder<UAnimMontage> ComboActionMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/ArenaBattle/Animation/AM_ComboAttack.AM_ComboAttack'"));
	if (ComboActionMontageRef.Object)
	{
		ComboActionMontage = ComboActionMontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UABComboActionData> ComboActionDataRef(TEXT("/Script/ArenaBattle.ABComboActionData'/Game/ArenaBattle/CharacterAction/ABA_ComboAttack.ABA_ComboAttack'"));
	if (ComboActionDataRef.Object)
	{
		ComboActionData = ComboActionDataRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DeadMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/ArenaBattle/Animation/AM_Dead.AM_Dead'"));
	if (DeadMontageRef.Object)
	{
		DeadMontage = DeadMontageRef.Object;
	}

	//Stat Component
	Stat = CreateDefaultSubobject<UABCharacterStatComponent>(TEXT("Stat"));

	//Widget Component
	HpBar = CreateDefaultSubobject<UABWidgetComponent>(TEXT("Widget"));
	//여기 위젯 컴포넌트는 사실 비어있는 껍데기에 불과함
	//따라서 우리가 제작한 UIHpBarWidget을 넣어줘야함
	HpBar->SetupAttachment(GetMesh());
	HpBar->SetRelativeLocation(FVector(0.0f, 0.0f, 180.0f));
	//위젯 컴포넌트 위치를 키높이 정도로 올리자
	static ConstructorHelpers::FClassFinder<UUserWidget> HpBarWidgetRef(TEXT("/Game/ArenaBattle/UI/WBP_HpBar.WBP_HpBar_C"));
	//HpBar위젯의 경우 애니메이션션 블루프린트와 유사하게 클래스정보를 등록하여 실제로 BeginPlay가 시작되면 그때 클래스 정보로부터 인스턴스를 생성하는 형태로 구성됨
	//그렇기에 앞서서 우리가 제작한 HpBar위젯에 대한 레퍼런스를 가져와서 클래스 정보를 얻어와야 한다.
	if (HpBarWidgetRef.Class)
	{
		HpBar->SetWidgetClass(HpBarWidgetRef.Class); //클래스 정보 등록
		HpBar->SetWidgetSpace(EWidgetSpace::Screen); //위젯 형태는 3d(World)가 아닌 2d(Screen)으로 지정
		HpBar->SetDrawSize(FVector2D(150.0f, 15.0f)); //아주 가느다란 HP바 형태로 지정
		HpBar->SetCollisionEnabled(ECollisionEnabled::NoCollision); //UI의 충돌 기능을 없앰.
	}


	//Item Actions
	TakeItemActions.Add(FTakeItemDelegateWrapper(FOnTakeItemDelegate::CreateUObject(this, &AABCharacterBase::EquipWeapon)));
	TakeItemActions.Add(FTakeItemDelegateWrapper(FOnTakeItemDelegate::CreateUObject(this, &AABCharacterBase::DrinkPotion)));
	TakeItemActions.Add(FTakeItemDelegateWrapper(FOnTakeItemDelegate::CreateUObject(this, &AABCharacterBase::ReadScroll)));


	//Weapon Component
	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"));
	Weapon->SetupAttachment(GetMesh(), TEXT("hand_rSocket"));
	//무기를 부착할 때 그냥 트랜스폼을 지정하는것이 아니라 캐릭터의 특정 부분에 무기가 항상 부착되어 돌아다닐 수 있도록 소켓이름을 지정하자.
	//hand rSocket은 이미 스켈레탈 컴포넌트에 지정이 되어 있는데 우리가 사용하는 Infinity캐릭터 애셋에 지정되어 있는 소켓 이름이다.
	//이후 웨폰 아이템 에셋이 가지고 있었던 스켈레탈 메쉬를 우리가 지정해줘야 한다.(EquipWeapon 함수로 이동)


}

void AABCharacterBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	Stat->OnHpZero.AddUObject(this, &AABCharacterBase::SetDead);
}

void AABCharacterBase::SetCharacterControlData(const UABCharacterControlData* CharacterControlData)
{
	//Pawn 섹션과 관련된곳
	bUseControllerRotationYaw = CharacterControlData->bUseControllerRotationYaw;

	//Character무브먼트세션과 관련된 데이터
	GetCharacterMovement()->bOrientRotationToMovement = CharacterControlData->bOrientRotationToMovement;
	GetCharacterMovement()->bUseControllerDesiredRotation = CharacterControlData->bUseControllerDesiredRotation;
	GetCharacterMovement()->RotationRate = CharacterControlData->RotationRate;
}

void AABCharacterBase::ProcessComboCommand()
{
	if (CurrentCombo == 0)
	{
		ComboActionBegin();
		return;
	}

	if (!ComboTimerHandle.IsValid())
	{
		//설정이 안되어 있을때 입력이 들어왔다는 것은 이미 타이머가 발동이 되어 시기를 놓쳤거나 더이상 진행할 필요가 없다는 뜻임.
		HasNextComboCommand = false;
	}
	else//타이머가 유효하다면 체크하기 이전에 다음 섹션으로 이동시킬 커맨드가 
	{
		//타이머가 유효하다면 체크하기 이전에 다음 섹션으로 이동시킬 커맨드가 발동했다는 뜻임.
		HasNextComboCommand = true;
	}
}

void AABCharacterBase::ComboActionBegin()
{
	/*Combo Status*/
	CurrentCombo = 1;

	/*Movement Setting*/
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	/*Animation Setting*/
	const float AttackSpeedRate = 1.0f;
	//재생 속도는 바뀔수도 있기에 const로 지정함

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	//몽타주를 재생하기 위해서는 AnimInstance를 가져와야한다.
	AnimInstance->Montage_Play(ComboActionMontage, AttackSpeedRate);
	//Montage_Play에 몽타주 에셋을 지정하여 우리가 어떤 특정한 몽타주를 재생하도록 설정이 가능하다.
	//특정 몽타주 재생을 위해 몽타주 입력을 받도록 헤더파일에서 ComboActionMontage 설정하자.
	//우선 기본속도인 1.0으로 재생함.

	FOnMontageEnded EndDelegate; //구조체처럼 선언하고 관련 함수를 넣으면 된다.
	EndDelegate.BindUObject(this, &AABCharacterBase::ComboActionEnd);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, ComboActionMontage);
	//몽타주가 실행되자마자 우리는 몽타주가 종료될 때 ComboActionEnd함수가 호출이 되도록 몽타주를 넣어볼것임.
	//Montage_SetEndDelegate 여기다가 앞서 살펴본 EndDelegate를 넣어주어야 한다.
	//Montage_SetEndDelegate함수는 두번째 인자로 몽타주 지정해줘야한다.

	/*콤보가 시작할대 타이머가 발동해야한다.*/
	ComboTimerHandle.Invalidate(); //타이머 핸들이 무효화 하도록 초기화
	SetComboCheckTimer(); //SetComboCheckTimer함수를 호출한다.
}

void AABCharacterBase::ComboActionEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	ensure(CurrentCombo != 0);
	//콤보가 종료될때 CurrentCombo값은 절때 0이 되면 안되기 때문에 Ensure라는 Assertion함수를 사용하여 CurrentCombo가 0이 아닌지 검사한다.
	// 만약 CurrentCombo이 0 이 나오면 출력 로그에 에러가 발생한다.
	CurrentCombo = 0;
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

void AABCharacterBase::SetComboCheckTimer()
{
	int32 ComboIndex = CurrentCombo - 1;
	//인덱스 값조정해주고  에셋 액션에 설정되어 있는 각 프레임 정보가 저 인덱스와 유효한지  검사해야함
	//인덱스 유효 검사를 위해 헤더추가해야함.
	ensure(ComboActionData->EffectiveFrameCount.IsValidIndex(ComboIndex));
	//ComboActionData에 있는 EffectiveFrameCount가  InValidIndex값을 가지는지 조사하는데 false가 나오면 문제가 있다는 뜻임.
	
	const float AttackSpeedRate = 1.0f;
	float ComboEffectiveTime = (ComboActionData->EffectiveFrameCount[ComboIndex] / ComboActionData->FrameRate) / AttackSpeedRate;
	//발동할 시간 정보를 얻을 수가 있다.
	 
	if (ComboEffectiveTime > 0.0f) //ComboEffectiveTime가 0보다 작으면 발동할 이유가 없음
	{
		GetWorld()->GetTimerManager().SetTimer(ComboTimerHandle, this, &AABCharacterBase::ComboCheck, ComboEffectiveTime, false);
		//GetWorld로부터 시간 서비스를 받아볼것임.
		//SetTimer 함수를 호출하고 앞서 설정한 ComboTimerHandle 가져온다.
		//현재 클래스에 있는 ComboCheck 함수를 호출하되 계속 반복하지 않도록 한번만 발생하도록 설정한다.
	}
}

void AABCharacterBase::ComboCheck()
{
	/*타이머가 발동 될 경우*/
	ComboTimerHandle.Invalidate(); //ComboTimerHandle을 초기화한다 왜? 더이상 발동되면 안되니깐
	if (HasNextComboCommand) //HasnextComboCommand이 들어왔는지 조사하고
	{
		//만약 타이머 발동 전에 입력이 들어와서 HasnextComboCommand이 True가 되었다면 우리는 다음 섹션으로 넘겨준다.
		UAnimInstance* Animinstance = GetMesh()->GetAnimInstance(); //UAnimInstance포인터를 가져온다.
		CurrentCombo = FMath::Clamp(CurrentCombo + 1, 1, ComboActionData->MaxComboCount);
		//현재 콤보값을 추가하는데 우리가 지정한 콤보값을 벗어나면 안되기에 MaxComboCount카운트를 벗어나지 않게 클램핑을 걸어준다.
		FName NextSection = *FString::Printf(TEXT("%s%d"), *ComboActionData->MontageSectionNamePrefix, CurrentCombo);
		//다음 세션에 대한 이름정보 가져온다. 이때 String함수를 조합하기에 printf 함수 사용
		//*ComboActionData->MontageSectionNamePrefix를 통해 접두사 정보를 가져오고  
		//두번쨰는 CurrentCombo값을 가져와서 스트링으로 만들고 이것을 네임으로 변환하여 섹션 이름을 지정한다.
		Animinstance->Montage_JumpToSection(NextSection, ComboActionMontage);
		//Montage_JumpToSection에 NextSection과 ComboActionMontage를 넣어주면 해당 섹션으로 재생이 바로 점프가 된다.
		SetComboCheckTimer();//이후 바로 타이머를 걸어줘야한다.
		HasNextComboCommand = false; //입력값 초기화

	}
}

void AABCharacterBase::AttackHitCheck()
{
	//해당 함수에는 앞서 설명한 트레이스 채널을 활용하여 물체가 서로 충돌되는지를 검사하는 로직을 집어 넣어주자.
	FHitResult OutHitResult;
	FCollisionQueryParams Params(SCENE_QUERY_STAT(Attack), false, this);
	//InTraceComplex에다가 우리는 복잡한 콜리전까지 하지는 않을것이기에 false를 넣어주면 된다.
	//SCENE_QUERY_STAT의 경우 언리얼 엔진이 제공하는 분석 툴이 있다. 여기에 Attack이라는 태그로 우리가 수행한 작업에 대해서 조사할 수가 있게 태그를 추가해준다.


	const float AttackRange = 40.0f;
	const float AttackRadius = 50.0f;
	const float AttackDamage = 30.0f;
	const FVector Start = GetActorLocation() + GetActorForwardVector() * GetCapsuleComponent()->GetScaledCapsuleRadius();
	//시작 지점: 현재 액터의 위치 + 액터의 시선방향 + 캡슐 컴포넌트의 반지름 값 하여 액터의 위치에서 시작이 아닌 정면에 있는 캡슐의 위치에서부터 시작 됨. 
	const FVector End = Start + GetActorForwardVector() * AttackRange;
	//끝 지점 : 시작지점 + 40센치만큼 앞에서 끝나도록 설정함.

	bool HitDetected = GetWorld()->SweepSingleByChannel(OutHitResult, Start, End, FQuat::Identity, CCHANNEL_ABACTION, FCollisionShape::MakeSphere(AttackRadius), Params);
	/*
	*  SweepSingleByChannel이란 함수를 사용하는데 world가 제공하는 서비스라서 GetWorld()를 통해 포인터를 얻어옴
	* SweepSingleByChannel함수는 결과값을 받아올 수 있는 FHitResult구조체를 첫번째 인자로 넣어준다.
	* 우리가 구체를 만들어 투사시켜야 하는데 투사 시작지점과 끝지점을 넣어준다.
	* 구체의 경우 FCollisionShape이 제공하는 MakeShphere란 함수를 통하여 구체의 영역을 지정할 수 있다. 이때 반지름이 50cm인 구체를 생성함
	* 우리가 사용할 트레이스 채널은 앞선 definition인 전처리기에 추가한 CCHANNEL_ABACTION을 지정한다.
	* 마지막으로 파라미터들이 있는데 이 파라미터들은 여러가지 의미를 가지고 있다.(F12로 확인)
	* 파라미터 내용 : FCollisionQueryParams(FName InTraceTag, bool bInTraceComplex=false, const AActor* InIgnoreActor=NULL)
	* InTraceTag는 나중에 이 콜리전을 분석할 때 어떤 태그 정보로 분석할 때 식별자 정보로 사용이 되는 인장.
	* InTraceComplex의 경우에는 복잡한 형태의 충돌체, 즉 캡슐이나 구 같은 Convex라고 불리는 단순한 불륨, 볼록한 볼륨을 대상으로 충돌을 감지하면 빠르게 지정할 수가 있고, 물리적인 시뮬레이션도 구현이 가능한데 복잡한 메쉬도 지정이 가능하지만 이 경우엔 올라서는 행위만 가능 즉 복잡한 형태의 충돌체도 감지할지에 대한 옵션이다.
	* InIgnoreActor은 무시할 액터들인데 우리는 자기 자신에 대해서만 무시하면 되기 때문에 This를 넣어주면 된다.
	*/

	if (HitDetected) //HitDetected가 true면 무언가가 감지됐다는것을 의미함
	{
		FDamageEvent DamageEvent;
		//여기서 데미지를 전달할때 데미지의 종류를 지정할 수가 있는데 이것을 위해 헤더파일 하나를 추가해줘야한다.
		//#include "Engine//DamageEvents.h"
		OutHitResult.GetActor()->TakeDamage(AttackDamage, DamageEvent, GetController(), this);
	}

#if ENABLE_DRAW_DEBUG

	FVector CapsuleOrigin = Start + (End - Start) * 0.5f;
	//캡슐의 원점 : 시점에서 끝에서 시작지점을 뺀값을 절반으로 나눈 값
	float CapsuleHalfHeight = AttackRange * 0.5f;
	//HalfHeight은 AttackRange의 절반값을 곱하여 얻음
	FColor DrawColor = HitDetected ? FColor::Green : FColor::Red;
	//HitDetected가 참(충돌)이면 녹색 아니면(충돌 안함) 빨간색

	DrawDebugCapsule(GetWorld(), CapsuleOrigin, CapsuleHalfHeight, AttackRadius, FRotationMatrix::MakeFromZ(GetActorForwardVector()).ToQuat(), DrawColor, false, 5.0f);
	//DrawDebugCapsule는 world에서 제공하는 서비스임. 캡슐을 눕혀주기 위해서 시선방향으로 눕혀지도록 회전을 가해준다. 
	// 마지막 두 인자는 계속해서 유지할 것인지 몇초동안 그릴것인지 지정하는 거임. 우린 5초동안만 그리도록 설정함.
#endif
}

float AABCharacterBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	//Instigator는 나에게 피해를 입힌 가해자를 의미
	//DamageCauser같은 경우 가해자가 사용한 무기 또는 가해자가 빙의한 폰 등 액터정보들이 데미지 Causer로 들어옴
	//이를 활용하여 내가 누구한테서 어떤 공격을 받았는지를 파악하고, 최종 데미지를 계산할 수가 있다.
	//만약 방어력이 설정되어 있으면 여기서 들어온 데미지를 경감하여 리턴할 수 있다.
	
	

	Stat->ApplyDamage(DamageAmount);
	//데미지를 받으면 바로 죽도록 설정

	return DamageAmount;
	//우린 DamageAmount값을 그냥 리턴할거임
}

void AABCharacterBase::SetDead()
{
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	//죽었다면 우선 캐릭터의 이동을 제약해야한다.
	PlayDeadAnimation();
	//죽는 몽타주 재생
	SetActorEnableCollision(false);
	//죽었으면 콜리전 자체에 대한 기능을 다 꺼준다.(액터에 관련된 모든 기능 콜리전 끄기)
	//이렇게 설정하면 죽은 NPC캐릭터는 캐릭터의 이동을 방해하지 않음

	HpBar->SetHiddenInGame(true);
}



void AABCharacterBase::PlayDeadAnimation()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->StopAllMontages(0.0f);
	//애님인스턴스에서 기존에 진행했던 몽타주를 진행시킴
	AnimInstance->Montage_Play(DeadMontage, 1.0f);
	//몽타주 플레이 생성자에서 지정한 DeadMontage를 정상 속도로 재생
}

void AABCharacterBase::SetupCharacterWidget(UABUserWidget* InUserWidget)
{
	UABHpBarWidget* HpBarWidget = Cast<UABHpBarWidget>(InUserWidget);
	//위젯을 캐스팅해서 얻어오자.

	if (HpBarWidget)
	{
		HpBarWidget->SetMaxHp(Stat->GetMaxHp());
		HpBarWidget->UpdateHpBar(Stat->GetCurrentHp());

		
		Stat->OnHpChanged.AddUObject(HpBarWidget, &UABHpBarWidget::UpdateHpBar);
		//앞으로 Stat의 CurrentHp값이 변경될 때마다 UpdateHpBar함수가 호출되도록
		//Stat에 있는 델리게이트에다가 해당 인스턴스의 멤버함수를 등록하자. (두 컴포넌트 간에 느슨한 결합 완성)

	}

}

void AABCharacterBase::TakeItem(UABItemData* InItemData)
{
	//캐릭터가 아이템을 받았을 때 아이템 데이터에 있는 열거형 값에 따라 서로 다른 액션들을 수행해줘야 한다.
	//Swich문을 통해서도 가능하지만 다른 방식으로 할거임.
	if (InItemData)
	{
		//TakeItem이 수행이 됬을때 InItemData가 있다면
		TakeItemActions[(uint8)InItemData->Type].ItemDelegate.ExecuteIfBound(InItemData);
		//TakeItemActions에서 하나의 Type값을 가져오고 넘겨준다.
	}
	

}

void AABCharacterBase::DrinkPotion(UABItemData* InItemData)
{

	UE_LOG(LogABCharacter, Log, TEXT("DrinkPotion"));
}

void AABCharacterBase::EquipWeapon(UABItemData* InItemData)
{
	UABWeaponItemData* WeaponItemData = Cast< UABWeaponItemData>(InItemData);
	if (WeaponItemData)
	{
		if (WeaponItemData->WeaponMesh.IsPending())
		{
			WeaponItemData->WeaponMesh.LoadSynchronous();
		}
		Weapon->SetSkeletalMesh(WeaponItemData->WeaponMesh.Get());
	}
}

void AABCharacterBase::ReadScroll(UABItemData* InItemData)
{
	UE_LOG(LogABCharacter, Log, TEXT("Read Scroll"));
}


