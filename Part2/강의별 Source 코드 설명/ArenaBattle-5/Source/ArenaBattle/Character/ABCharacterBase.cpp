// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ABCharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ABCharacterControlData.h"
#include "Animation/AnimMontage.h"
#include "ABComboActionData.h"

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
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Pawn"));
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
	GetMesh()->SetCollisionProfileName(TEXT("CharacterMesh"));

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


