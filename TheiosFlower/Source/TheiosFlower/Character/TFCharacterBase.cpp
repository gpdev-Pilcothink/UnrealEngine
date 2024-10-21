// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/TFCharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TFCharacterControlData.h"
#include "Animation/AnimMontage.h"
#include "TFComboActionData.h"
#include "Physics/TFCollision.h"
#include "Engine//DamageEvents.h"
#include "CharacterStat/TFCharacterStatComponent.h"
#include "UI/TFWidgetComponent.h"
#include "UI/TFHpBarWidget.h "
#include "Item/TFWeaponItemData.h"

DEFINE_LOG_CATEGORY(LogTFCharacter)

// Sets default values
ATFCharacterBase::ATFCharacterBase()
{
	/*���� ȸ�� ���� �κ�*/
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	/*ĸ���� ���� ����*/
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	GetCapsuleComponent()->SetCollisionProfileName(CPROFILE_ABCAPSULE);


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
	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/Fantasy_Pack/Characters/Fantasy_Barbarian/Mesh/SK_Fantasy_Barbarian_Body.SK_Fantasy_Barbarian_Body'"));
	if (CharacterMeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(CharacterMeshRef.Object);
	}
	//�Ž��� ��쿣 ������Ʈ�� �����ͼ� SetSkeletalMesh�Լ��� ���ؼ� �ռ��� �м��븦 ���鶧ó�� ������Ʈ�� �������ָ� ��


	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef(TEXT("/Game/TheiosFlower/Animation/ABP_TFCharacter.ABP_TFCharacter_C"));
	if (AnimInstanceClassRef.Class)
	{
		GetMesh()->SetAnimInstanceClass(AnimInstanceClassRef.Class);
	}
	//�ִϸ��̼��� ��쿣 Ŭ������ �����ϵ��� �Ǿ� �ִ�. ���� Ŭ���� ������ �����ͼ� AnimInstanceClass��� �����ٰ� �ش� Ŭ���� ���� �������ֵ��� ���谡 �����ϴ�.

	/*�� �ڷᱸ���� �����ڿ��� �ϼ��ϱ�*/
	static ConstructorHelpers::FObjectFinder<UTFCharacterControlData> ShoulderDataRef(TEXT("/Script/TheiosFlower.TFCharacterControlData'/Game/TheiosFlower/CharacterController/TFC_Shoulder.TFC_Shoulder'"));
	if (ShoulderDataRef.Object)
	{
		CharacterControlManager.Add(ECharacterControlType::Shoulder, ShoulderDataRef.Object);
	}
	static ConstructorHelpers::FObjectFinder<UTFCharacterControlData> QuaterDataRef(TEXT("/Script/TheiosFlower.TFCharacterControlData'/Game/TheiosFlower/CharacterController/TFC_Quater.TFC_Quater'"));
	if (QuaterDataRef.Object)
	{
		CharacterControlManager.Add(ECharacterControlType::Quater, QuaterDataRef.Object);
	}
	/*
	���⼭ ��Ÿ�� ������ ConstructorHelper�� �����Ҽ� �ִµ� ��Ÿ�ָ� �����Ҷ�����
	�Ź� �ڵ��� ��ġ�°��� ���� ���������� �ʴ´�.
	���� �������� ��������Ʈ�� ABCharacterŬ������ Ȯ���Ͽ� ��Ÿ�� ������ ��������Ʈ���� �����ҰŴ�.
	*/


	static ConstructorHelpers::FObjectFinder<UAnimMontage> ComboActionMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/TheiosFlower/Animation/AM_ComboAttack.AM_ComboAttack'"));
	if (ComboActionMontageRef.Object)
	{
		ComboActionMontage = ComboActionMontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UTFComboActionData> ComboActionDataRef(TEXT("/Script/TheiosFlower.TFComboActionData'/Game/TheiosFlower/CharacterAction/TFA_ComboAttack.TFA_ComboAttack'"));
	if (ComboActionDataRef.Object)
	{
		ComboActionData = ComboActionDataRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> DeadMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/TheiosFlower/Animation/AM_Dead.AM_Dead'"));
	if (DeadMontageRef.Object)
	{
		DeadMontage = DeadMontageRef.Object;
	}

	//Stat Component
	Stat = CreateDefaultSubobject<UTFCharacterStatComponent>(TEXT("Stat"));

	//Widget Component
	HpBar = CreateDefaultSubobject<UTFWidgetComponent>(TEXT("Widget"));
	//���� ���� ������Ʈ�� ��� ����ִ� �����⿡ �Ұ���
	//���� �츮�� ������ UIHpBarWidget�� �־������
	HpBar->SetupAttachment(GetMesh());
	HpBar->SetRelativeLocation(FVector(0.0f, 0.0f, 180.0f));
	//���� ������Ʈ ��ġ�� Ű���� ������ �ø���
	static ConstructorHelpers::FClassFinder<UUserWidget> HpBarWidgetRef(TEXT("/Game/UI/WBP_HpBar.WBP_HpBar_C"));
	//HpBar������ ��� �ִϸ��̼Ǽ� ��������Ʈ�� �����ϰ� Ŭ���������� ����Ͽ� ������ BeginPlay�� ���۵Ǹ� �׶� Ŭ���� �����κ��� �ν��Ͻ��� �����ϴ� ���·� ������
	//�׷��⿡ �ռ��� �츮�� ������ HpBar������ ���� ���۷����� �����ͼ� Ŭ���� ������ ���;� �Ѵ�.
	if (HpBarWidgetRef.Class)
	{
		HpBar->SetWidgetClass(HpBarWidgetRef.Class); //Ŭ���� ���� ���
		HpBar->SetWidgetSpace(EWidgetSpace::Screen); //���� ���´� 3d(World)�� �ƴ� 2d(Screen)���� ����
		HpBar->SetDrawSize(FVector2D(150.0f, 15.0f)); //���� �����ٶ� HP�� ���·� ����
		HpBar->SetCollisionEnabled(ECollisionEnabled::NoCollision); //UI�� �浹 ����� ����.
	}

	//Item Actions
	TakeItemActions.Add(FTakeItemDelegateWrapper(FOnTakeItemDelegate::CreateUObject(this, &ATFCharacterBase::EquipWeapon)));
	TakeItemActions.Add(FTakeItemDelegateWrapper(FOnTakeItemDelegate::CreateUObject(this, &ATFCharacterBase::DrinkPotion)));
	TakeItemActions.Add(FTakeItemDelegateWrapper(FOnTakeItemDelegate::CreateUObject(this, &ATFCharacterBase::ReadScroll)));

	//Weapon Component
	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"));
	Weapon->SetupAttachment(GetMesh(), TEXT("middle_01_r"));
	//���⸦ ������ �� �׳� Ʈ�������� �����ϴ°��� �ƴ϶� ĳ������ Ư�� �κп� ���Ⱑ �׻� �����Ǿ� ���ƴٴ� �� �ֵ��� �����̸��� ��������.
	//hand rSocket�� �̹� ���̷�Ż ������Ʈ�� ������ �Ǿ� �ִµ� �츮�� ����ϴ� Infinityĳ���� �ּ¿� �����Ǿ� �ִ� ���� �̸��̴�.
	//���� ���� ������ ������ ������ �־��� ���̷�Ż �޽��� �츮�� ��������� �Ѵ�.(EquipWeapon �Լ��� �̵�)

}

void ATFCharacterBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	Stat->OnHpZero.AddUObject(this, &ATFCharacterBase::SetDead);
}

void ATFCharacterBase::SetCharacterControlData(const UTFCharacterControlData* CharacterControlData)
{
	//Pawn ���ǰ� ���õȰ�
	bUseControllerRotationYaw = CharacterControlData->bUseControllerRotationYaw;

	//Character�����Ʈ���ǰ� ���õ� ������
	GetCharacterMovement()->bOrientRotationToMovement = CharacterControlData->bOrientRotationToMovement;
	GetCharacterMovement()->bUseControllerDesiredRotation = CharacterControlData->bUseControllerDesiredRotation;
	GetCharacterMovement()->RotationRate = CharacterControlData->RotationRate;
}

void ATFCharacterBase::ProcessComboCommand()
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

void ATFCharacterBase::ComboActionBegin()
{
	/*Combo Status*/
	CurrentCombo = 1;

	/*Movement Setting*/
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

	/*Animation Setting*/
	const float AttackSpeedRate = 1.7f;
	//��� �ӵ��� �ٲ���� �ֱ⿡ const�� ������

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	//��Ÿ�ָ� ����ϱ� ���ؼ��� AnimInstance�� �����;��Ѵ�.
	AnimInstance->Montage_Play(ComboActionMontage, AttackSpeedRate);
	//Montage_Play�� ��Ÿ�� ������ �����Ͽ� �츮�� � Ư���� ��Ÿ�ָ� ����ϵ��� ������ �����ϴ�.
	//Ư�� ��Ÿ�� ����� ���� ��Ÿ�� �Է��� �޵��� ������Ͽ��� ComboActionMontage ��������.
	//�켱 �⺻�ӵ��� 1.0���� �����.

	FOnMontageEnded EndDelegate; //����üó�� �����ϰ� ���� �Լ��� ������ �ȴ�.
	EndDelegate.BindUObject(this, &ATFCharacterBase::ComboActionEnd);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, ComboActionMontage);
	//��Ÿ�ְ� ������ڸ��� �츮�� ��Ÿ�ְ� ����� �� ComboActionEnd�Լ��� ȣ���� �ǵ��� ��Ÿ�ָ� �־����.
	//Montage_SetEndDelegate ����ٰ� �ռ� ���캻 EndDelegate�� �־��־�� �Ѵ�.
	//Montage_SetEndDelegate�Լ��� �ι�° ���ڷ� ��Ÿ�� ����������Ѵ�.

	/*�޺��� �����Ҵ� Ÿ�̸Ӱ� �ߵ��ؾ��Ѵ�.*/
	ComboTimerHandle.Invalidate(); //Ÿ�̸� �ڵ��� ��ȿȭ �ϵ��� �ʱ�ȭ
	SetComboCheckTimer(); //SetComboCheckTimer�Լ��� ȣ���Ѵ�.
}

void ATFCharacterBase::ComboActionEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	ensure(CurrentCombo != 0);
	//�޺��� ����ɶ� CurrentCombo���� ���� 0�� �Ǹ� �ȵǱ� ������ Ensure��� Assertion�Լ��� ����Ͽ� CurrentCombo�� 0�� �ƴ��� �˻��Ѵ�.
	// ���� CurrentCombo�� 0 �� ������ ��� �α׿� ������ �߻��Ѵ�.
	CurrentCombo = 0;
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

}

void ATFCharacterBase::SetComboCheckTimer()
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
		GetWorld()->GetTimerManager().SetTimer(ComboTimerHandle, this, &ATFCharacterBase::ComboCheck, ComboEffectiveTime, false);
		//GetWorld�κ��� �ð� ���񽺸� �޾ƺ�����.
		//SetTimer �Լ��� ȣ���ϰ� �ռ� ������ ComboTimerHandle �����´�.
		//���� Ŭ������ �ִ� ComboCheck �Լ��� ȣ���ϵ� ��� �ݺ����� �ʵ��� �ѹ��� �߻��ϵ��� �����Ѵ�.
	}
}

void ATFCharacterBase::ComboCheck()
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

void ATFCharacterBase::AttackHitCheck()
{
	//�ش� �Լ����� �ռ� ������ Ʈ���̽� ä���� Ȱ���Ͽ� ��ü�� ���� �浹�Ǵ����� �˻��ϴ� ������ ���� �־�����.
	FHitResult OutHitResult;
	FCollisionQueryParams Params(SCENE_QUERY_STAT(Attack), false, this);
	//InTraceComplex���ٰ� �츮�� ������ �ݸ������� ������ �������̱⿡ false�� �־��ָ� �ȴ�.
	//SCENE_QUERY_STAT�� ��� �𸮾� ������ �����ϴ� �м� ���� �ִ�. ���⿡ Attack�̶�� �±׷� �츮�� ������ �۾��� ���ؼ� ������ ���� �ְ� �±׸� �߰����ش�.


	const float AttackRange = 40.0f;
	const float AttackRadius = 50.0f;
	const float AttackDamage = 30.0f;
	const FVector Start = GetActorLocation() + GetActorForwardVector() * GetCapsuleComponent()->GetScaledCapsuleRadius();
	//���� ����: ���� ������ ��ġ + ������ �ü����� + ĸ�� ������Ʈ�� ������ �� �Ͽ� ������ ��ġ���� ������ �ƴ� ���鿡 �ִ� ĸ���� ��ġ�������� ���� ��. 
	const FVector End = Start + GetActorForwardVector() * AttackRange;
	//�� ���� : �������� + 40��ġ��ŭ �տ��� �������� ������.

	bool HitDetected = GetWorld()->SweepSingleByChannel(OutHitResult, Start, End, FQuat::Identity, CCHANNEL_ABACTION, FCollisionShape::MakeSphere(AttackRadius), Params);
	/*
	*  SweepSingleByChannel�̶� �Լ��� ����ϴµ� world�� �����ϴ� ���񽺶� GetWorld()�� ���� �����͸� ����
	* SweepSingleByChannel�Լ��� ������� �޾ƿ� �� �ִ� FHitResult����ü�� ù��° ���ڷ� �־��ش�.
	* �츮�� ��ü�� ����� ������Ѿ� �ϴµ� ���� ���������� �������� �־��ش�.
	* ��ü�� ��� FCollisionShape�� �����ϴ� MakeShphere�� �Լ��� ���Ͽ� ��ü�� ������ ������ �� �ִ�. �̶� �������� 50cm�� ��ü�� ������
	* �츮�� ����� Ʈ���̽� ä���� �ռ� definition�� ��ó���⿡ �߰��� CCHANNEL_ABACTION�� �����Ѵ�.
	* ���������� �Ķ���͵��� �ִµ� �� �Ķ���͵��� �������� �ǹ̸� ������ �ִ�.(F12�� Ȯ��)
	* �Ķ���� ���� : FCollisionQueryParams(FName InTraceTag, bool bInTraceComplex=false, const AActor* InIgnoreActor=NULL)
	* InTraceTag�� ���߿� �� �ݸ����� �м��� �� � �±� ������ �м��� �� �ĺ��� ������ ����� �Ǵ� ����.
	* InTraceComplex�� ��쿡�� ������ ������ �浹ü, �� ĸ���̳� �� ���� Convex��� �Ҹ��� �ܼ��� �ҷ�, ������ ������ ������� �浹�� �����ϸ� ������ ������ ���� �ְ�, �������� �ùķ��̼ǵ� ������ �����ѵ� ������ �޽��� ������ ���������� �� ��쿣 �ö󼭴� ������ ���� �� ������ ������ �浹ü�� ���������� ���� �ɼ��̴�.
	* InIgnoreActor�� ������ ���͵��ε� �츮�� �ڱ� �ڽſ� ���ؼ��� �����ϸ� �Ǳ� ������ This�� �־��ָ� �ȴ�.
	*/

	if (HitDetected) //HitDetected�� true�� ���𰡰� �����ƴٴ°��� �ǹ���
	{
		FDamageEvent DamageEvent;
		//���⼭ �������� �����Ҷ� �������� ������ ������ ���� �ִµ� �̰��� ���� ������� �ϳ��� �߰�������Ѵ�.
		//#include "Engine//DamageEvents.h"
		OutHitResult.GetActor()->TakeDamage(AttackDamage, DamageEvent, GetController(), this);
	}

#if ENABLE_DRAW_DEBUG

	FVector CapsuleOrigin = Start + (End - Start) * 0.5f;
	//ĸ���� ���� : �������� ������ ���������� ������ �������� ���� ��
	float CapsuleHalfHeight = AttackRange * 0.5f;
	//HalfHeight�� AttackRange�� ���ݰ��� ���Ͽ� ����
	FColor DrawColor = HitDetected ? FColor::Green : FColor::Red;
	//HitDetected�� ��(�浹)�̸� ��� �ƴϸ�(�浹 ����) ������

	DrawDebugCapsule(GetWorld(), CapsuleOrigin, CapsuleHalfHeight, AttackRadius, FRotationMatrix::MakeFromZ(GetActorForwardVector()).ToQuat(), DrawColor, false, 5.0f);
	//DrawDebugCapsule�� world���� �����ϴ� ������. ĸ���� �����ֱ� ���ؼ� �ü��������� ���������� ȸ���� �����ش�. 
	// ������ �� ���ڴ� ����ؼ� ������ ������ ���ʵ��� �׸������� �����ϴ� ����. �츰 5�ʵ��ȸ� �׸����� ������.
#endif
}

float ATFCharacterBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	//Instigator�� ������ ���ظ� ���� �����ڸ� �ǹ�
	//DamageCauser���� ��� �����ڰ� ����� ���� �Ǵ� �����ڰ� ������ �� �� ������������ ������ Causer�� ����
	//�̸� Ȱ���Ͽ� ���� �������׼� � ������ �޾Ҵ����� �ľ��ϰ�, ���� �������� ����� ���� �ִ�.
	//���� ������ �����Ǿ� ������ ���⼭ ���� �������� �氨�Ͽ� ������ �� �ִ�.



	Stat->ApplyDamage(DamageAmount);
	//�������� ������ �ٷ� �׵��� ����

	return DamageAmount;
	//�츰 DamageAmount���� �׳� �����Ұ���
}

void ATFCharacterBase::SetDead()
{

	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	//�׾��ٸ� �켱 ĳ������ �̵��� �����ؾ��Ѵ�.
	PlayDeadAnimation();
	//�״� ��Ÿ�� ���
	SetActorEnableCollision(false);
	//�׾����� �ݸ��� ��ü�� ���� ����� �� ���ش�.(���Ϳ� ���õ� ��� ��� �ݸ��� ����)
	//�̷��� �����ϸ� ���� NPCĳ���ʹ� ĳ������ �̵��� �������� ����



	HpBar->SetHiddenInGame(true);
}

void ATFCharacterBase::PlayDeadAnimation()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->StopAllMontages(0.0f);
	//�ִ��ν��Ͻ����� ������ �����ߴ� ��Ÿ�ָ� �����Ŵ
	AnimInstance->Montage_Play(DeadMontage, 1.0f);
	//��Ÿ�� �÷��� �����ڿ��� ������ DeadMontage�� ���� �ӵ��� ���
}

void ATFCharacterBase::SetupCharacterWidget(UTFUserWidget* InUserWidget)
{
	UTFHpBarWidget* HpBarWidget = Cast<UTFHpBarWidget>(InUserWidget);
	//������ ĳ�����ؼ� ������.

	if (HpBarWidget)
	{
		HpBarWidget->SetMaxHp(Stat->GetMaxHp());
		HpBarWidget->UpdateHpBar(Stat->GetCurrentHp());


		Stat->OnHpChanged.AddUObject(HpBarWidget, &UTFHpBarWidget::UpdateHpBar);
		//������ Stat�� CurrentHp���� ����� ������ UpdateHpBar�Լ��� ȣ��ǵ���
		//Stat�� �ִ� ��������Ʈ���ٰ� �ش� �ν��Ͻ��� ����Լ��� �������. (�� ������Ʈ ���� ������ ���� �ϼ�)

	}
}

void ATFCharacterBase::TakeItem(UTFItemData* InItemData)
{
	//ĳ���Ͱ� �������� �޾��� �� ������ �����Ϳ� �ִ� ������ ���� ���� ���� �ٸ� �׼ǵ��� ��������� �Ѵ�.
	//Swich���� ���ؼ��� ���������� �ٸ� ������� �Ұ���.
	if (InItemData)
	{
		//TakeItem�� ������ ������ InItemData�� �ִٸ�
		TakeItemActions[(uint8)InItemData->Type].ItemDelegate.ExecuteIfBound(InItemData);
		//TakeItemActions���� �ϳ��� Type���� �������� �Ѱ��ش�.
	}
}

void ATFCharacterBase::DrinkPotion(UTFItemData* InItemData)
{
	UE_LOG(LogTFCharacter, Log, TEXT("Read Scroll"));
}

void ATFCharacterBase::EquipWeapon(UTFItemData* InItemData)
{
	UTFWeaponItemData* WeaponItemData = Cast<UTFWeaponItemData>(InItemData);
	if (WeaponItemData)
	{
		if (WeaponItemData->WeaponMesh.IsPending())
		{
			WeaponItemData->WeaponMesh.LoadSynchronous();
		}
		Weapon->SetSkeletalMesh(WeaponItemData->WeaponMesh.Get());
	}
}

void ATFCharacterBase::ReadScroll(UTFItemData* InItemData)
{
	UE_LOG(LogTFCharacter, Log, TEXT("Read Scroll"));
}


