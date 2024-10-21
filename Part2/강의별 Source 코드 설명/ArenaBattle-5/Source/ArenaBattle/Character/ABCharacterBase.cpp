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
	/*���� ȸ�� ���� �κ�*/
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
	//�� �κ��� ī�޶� �ٷ궧 �ڼ��� �����Ұ���

	/*ĸ���� ���� ����*/
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Pawn"));
	//GetCapsuleComponent�� ���� ������ �ʿ��ؼ� ��� �߰� �ؾ��Ѵ�.


	/*�����ӿ� �ش��ϴ� ������Ʈ ����*/
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	/*�޽��� ���� ����*/
	//�� ĳ���͵��� �츮�� �𵨸������� ������⿡ ���簡���� ����� ��ġ�� ȸ���� ��������� �Ѵ�.
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -100.0f), FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	GetMesh()->SetCollisionProfileName(TEXT("CharacterMesh"));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/InfinityBladeWarriors/Character/CompleteCharacters/SK_CharM_Cardboard.SK_CharM_Cardboard'"));
	if (CharacterMeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(CharacterMeshRef.Object);
	}
	//�Ž��� ��쿣 ������Ʈ�� �����ͼ� SetSkeletalMesh�Լ��� ���ؼ� �ռ��� �м��븦 ���鶧ó�� ������Ʈ�� �������ָ� ��


	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef(TEXT("/Game/ArenaBattle/Animation/ABP_ABCharacter.ABP_ABCharacter_C"));
	if (AnimInstanceClassRef.Class)
	{
		GetMesh()->SetAnimInstanceClass(AnimInstanceClassRef.Class);
	}
	//�ִϸ��̼��� ��쿣 Ŭ������ �����ϵ��� �Ǿ� �ִ�. ���� Ŭ���� ������ �����ͼ� AnimInstanceClass��� �����ٰ� �ش� Ŭ���� ���� �������ֵ��� ���谡 �����ϴ�.


	/*�� �ڷᱸ���� �����ڿ��� �ϼ��ϱ�*/
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
	���⼭ ��Ÿ�� ������ ConstructorHelper�� �����Ҽ� �ִµ� ��Ÿ�ָ� �����Ҷ�����
	�Ź� �ڵ��� ��ġ�°��� ���� ���������� �ʴ´�. 
	���� �������� �������Ʈ�� ABCharacterŬ������ Ȯ���Ͽ� ��Ÿ�� ������ �������Ʈ���� �����ҰŴ�.
	*/

}

void AABCharacterBase::SetCharacterControlData(const UABCharacterControlData* CharacterControlData)
{
	//Pawn ���ǰ� ���õȰ�
	bUseControllerRotationYaw = CharacterControlData->bUseControllerRotationYaw;

	//Character�����Ʈ���ǰ� ���õ� ������
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
		//������ �ȵǾ� ������ �Է��� ���Դٴ� ���� �̹� Ÿ�̸Ӱ� �ߵ��� �Ǿ� �ñ⸦ ���ưų� ���̻� ������ �ʿ䰡 ���ٴ� ����.
		HasNextComboCommand = false;
	}
	else//Ÿ�̸Ӱ� ��ȿ�ϴٸ� üũ�ϱ� ������ ���� �������� �̵���ų Ŀ�ǵ尡 
	{
		//Ÿ�̸Ӱ� ��ȿ�ϴٸ� üũ�ϱ� ������ ���� �������� �̵���ų Ŀ�ǵ尡 �ߵ��ߴٴ� ����.
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
	//��� �ӵ��� �ٲ���� �ֱ⿡ const�� ������

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	//��Ÿ�ָ� ����ϱ� ���ؼ��� AnimInstance�� �����;��Ѵ�.
	AnimInstance->Montage_Play(ComboActionMontage, AttackSpeedRate);
	//Montage_Play�� ��Ÿ�� ������ �����Ͽ� �츮�� � Ư���� ��Ÿ�ָ� ����ϵ��� ������ �����ϴ�.
	//Ư�� ��Ÿ�� ����� ���� ��Ÿ�� �Է��� �޵��� ������Ͽ��� ComboActionMontage ��������.
	//�켱 �⺻�ӵ��� 1.0���� �����.

	FOnMontageEnded EndDelegate; //����üó�� �����ϰ� ���� �Լ��� ������ �ȴ�.
	EndDelegate.BindUObject(this, &AABCharacterBase::ComboActionEnd);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, ComboActionMontage);
	//��Ÿ�ְ� ������ڸ��� �츮�� ��Ÿ�ְ� ����� �� ComboActionEnd�Լ��� ȣ���� �ǵ��� ��Ÿ�ָ� �־����.
	//Montage_SetEndDelegate ����ٰ� �ռ� ���캻 EndDelegate�� �־��־�� �Ѵ�.
	//Montage_SetEndDelegate�Լ��� �ι�° ���ڷ� ��Ÿ�� ����������Ѵ�.

	/*�޺��� �����Ҵ� Ÿ�̸Ӱ� �ߵ��ؾ��Ѵ�.*/
	ComboTimerHandle.Invalidate(); //Ÿ�̸� �ڵ��� ��ȿȭ �ϵ��� �ʱ�ȭ
	SetComboCheckTimer(); //SetComboCheckTimer�Լ��� ȣ���Ѵ�.
}

void AABCharacterBase::ComboActionEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	ensure(CurrentCombo != 0);
	//�޺��� ����ɶ� CurrentCombo���� ���� 0�� �Ǹ� �ȵǱ� ������ Ensure��� Assertion�Լ��� ����Ͽ� CurrentCombo�� 0�� �ƴ��� �˻��Ѵ�.
	// ���� CurrentCombo�� 0 �� ������ ��� �α׿� ������ �߻��Ѵ�.
	CurrentCombo = 0;
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

void AABCharacterBase::SetComboCheckTimer()
{
	int32 ComboIndex = CurrentCombo - 1;
	//�ε��� ���������ְ�  ���� �׼ǿ� �����Ǿ� �ִ� �� ������ ������ �� �ε����� ��ȿ����  �˻��ؾ���
	//�ε��� ��ȿ �˻縦 ���� ����߰��ؾ���.
	ensure(ComboActionData->EffectiveFrameCount.IsValidIndex(ComboIndex));
	//ComboActionData�� �ִ� EffectiveFrameCount��  InValidIndex���� �������� �����ϴµ� false�� ������ ������ �ִٴ� ����.
	
	const float AttackSpeedRate = 1.0f;
	float ComboEffectiveTime = (ComboActionData->EffectiveFrameCount[ComboIndex] / ComboActionData->FrameRate) / AttackSpeedRate;
	//�ߵ��� �ð� ������ ���� ���� �ִ�.
	 
	if (ComboEffectiveTime > 0.0f) //ComboEffectiveTime�� 0���� ������ �ߵ��� ������ ����
	{
		GetWorld()->GetTimerManager().SetTimer(ComboTimerHandle, this, &AABCharacterBase::ComboCheck, ComboEffectiveTime, false);
		//GetWorld�κ��� �ð� ���񽺸� �޾ƺ�����.
		//SetTimer �Լ��� ȣ���ϰ� �ռ� ������ ComboTimerHandle �����´�.
		//���� Ŭ������ �ִ� ComboCheck �Լ��� ȣ���ϵ� ��� �ݺ����� �ʵ��� �ѹ��� �߻��ϵ��� �����Ѵ�.
	}
}

void AABCharacterBase::ComboCheck()
{
	/*Ÿ�̸Ӱ� �ߵ� �� ���*/
	ComboTimerHandle.Invalidate(); //ComboTimerHandle�� �ʱ�ȭ�Ѵ� ��? ���̻� �ߵ��Ǹ� �ȵǴϱ�
	if (HasNextComboCommand) //HasnextComboCommand�� ���Դ��� �����ϰ�
	{
		//���� Ÿ�̸� �ߵ� ���� �Է��� ���ͼ� HasnextComboCommand�� True�� �Ǿ��ٸ� �츮�� ���� �������� �Ѱ��ش�.
		UAnimInstance* Animinstance = GetMesh()->GetAnimInstance(); //UAnimInstance�����͸� �����´�.
		CurrentCombo = FMath::Clamp(CurrentCombo + 1, 1, ComboActionData->MaxComboCount);
		//���� �޺����� �߰��ϴµ� �츮�� ������ �޺����� ����� �ȵǱ⿡ MaxComboCountī��Ʈ�� ����� �ʰ� Ŭ������ �ɾ��ش�.
		FName NextSection = *FString::Printf(TEXT("%s%d"), *ComboActionData->MontageSectionNamePrefix, CurrentCombo);
		//���� ���ǿ� ���� �̸����� �����´�. �̶� String�Լ��� �����ϱ⿡ printf �Լ� ���
		//*ComboActionData->MontageSectionNamePrefix�� ���� ���λ� ������ ��������  
		//�ι����� CurrentCombo���� �����ͼ� ��Ʈ������ ����� �̰��� �������� ��ȯ�Ͽ� ���� �̸��� �����Ѵ�.
		Animinstance->Montage_JumpToSection(NextSection, ComboActionMontage);
		//Montage_JumpToSection�� NextSection�� ComboActionMontage�� �־��ָ� �ش� �������� ����� �ٷ� ������ �ȴ�.
		SetComboCheckTimer();//���� �ٷ� Ÿ�̸Ӹ� �ɾ�����Ѵ�.
		HasNextComboCommand = false; //�Է°� �ʱ�ȭ

	}
}


