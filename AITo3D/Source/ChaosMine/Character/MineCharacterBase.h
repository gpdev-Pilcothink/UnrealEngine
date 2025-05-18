// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Containers/Map.h"
#include "GameFramework/Character.h"
#include "Interface/MineAnimationAttackInterface.h"
#include "Interface/MineCharacterWidgetInterface.h"
#include "Interface/MineCharacterItemInterface.h"
#include "MineCharacterBase.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogMineCharacter, Log, All);

class UNiagaraSystem;
class UNiagaraComponent;


// Movement �׷� (�⺻, ����, ���)
UENUM(BlueprintType)
enum class ESpeedGroup : uint8
{
	Basic UMETA(DisplayName = "Basic"),
	Low   UMETA(DisplayName = "Low"),
	High  UMETA(DisplayName = "High")
};

// �̵� Ÿ�� (�ȱ�, �޸���, ���� �ȱ�, ���� �޸���, ����, �����޸���, ��������)
UENUM(BlueprintType)
enum class EMovementType : uint8
{
	Walk          UMETA(DisplayName = "Walk"),
	Sprint        UMETA(DisplayName = "Sprint"),
	CrouchWalk    UMETA(DisplayName = "CrouchWalk"),
	CrouchSprint  UMETA(DisplayName = "CrouchSprint"),
	Swim          UMETA(DisplayName = "Swim"),
	SwimSprint    UMETA(DisplayName = "SwimSprint"),
	JumpZVelocity UMETA(DisplayName = "JumpZVelocity")
};


//��Ī ����
UENUM()
enum class ECharacterControlType : uint8
{
	FirstPerson,
	ThirdPerson
};


//�������� ���� ��������Ʈ ����(�迭 ������ ���� ����ü�� �������)
DECLARE_DELEGATE_OneParam(FOnTakeItemDelegate, class UMineItemData* /*InItemData*/);
USTRUCT(BlueprintType)
struct FTakeItemDelegateWrapper
{
	GENERATED_BODY()
	FTakeItemDelegateWrapper() { }
	FTakeItemDelegateWrapper(const FOnTakeItemDelegate& InItemDelegate) : ItemDelegate(InItemDelegate) {}
	FOnTakeItemDelegate ItemDelegate;
};

UCLASS()
class CHAOSMINE_API AMineCharacterBase : public ACharacter, public IMineAnimationAttackInterface, public IMineCharacterWidgetInterface, public IMineCharacterItemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMineCharacterBase();

	virtual void PostInitializeComponents() override;

protected:
	virtual void SetCharacterControlData(const class UMineCharacterControlData* CharacterControlData);


	UPROPERTY(EditAnyWhere, Category = CharacterControl, Meta = (AllowPrivateAccess = "true"));
	TMap<ECharacterControlType, class UMineCharacterControlData *> CharacterControlManager;

	
/****************************************************
* ĳ���� �����Ʈ �⺻�� ���� (ĸ��ȭ)
*****************************************************/
public:
	/***********************************
	 * Getter / Setter - �ӵ� ������
	 ***********************************/

	 // Getter: �׷�� �̵�Ÿ�Կ� �ش��ϴ� �ӵ��� ��ȯ
	UFUNCTION(BlueprintCallable, Category = "Movement")
	float GetMovement(ESpeedGroup Group, EMovementType Type) const
	{
		const TPair<ESpeedGroup, EMovementType> Key(Group, Type);
		if (const float* Found = MovementMap.Find(Key))
		{
			return *Found;
		}
		return 0.f; // �⺻��
	}

	// Setter: �׷�� �̵�Ÿ�Կ� �ش��ϴ� �ӵ��� ����
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void SetMovement(ESpeedGroup Group, EMovementType Type, float NewSpeed)
	{
		MovementMap.Add(TPair<ESpeedGroup, EMovementType>(Group, Type), NewSpeed);
	}

	
private:
	/***********************************
	* ������ ������ TMap
	* Key: (SpeedGroup, MovementType)
	* Value: �ӵ� �� (float)
	***********************************/
	TMap<TPair<ESpeedGroup, EMovementType>, float> MovementMap;


// Combo Action Section
protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> ComboActionMontage; //�ߺ� �׼� ��Ÿ��
	UPROPERTY(EditAnyWhere, blueprintReadWrite, Category = Attack, Meta = (AllowPrivateAccess = "true"));
	TObjectPtr<class UMineComboActionData> ComboActionData; //�޺� �׼� ������

	void ProcessComboCommand();

	/**************************�޺��׼� ����**************************
	*ComboActionEnd�� ��Ÿ�ֿ� ������ ��������Ʈ�� ���� �ٷ� ȣ��� �� �ֵ��� �Ķ���͸� ���纼����.
	*�̷��� ��������Ʈ�� ���� �Ķ���͸� ���߰� �Ǹ� �̺�Ʈ ����� �Ǳ� ������ �ݹ��Լ�ó�� �۵��� 
	*���� ���� üũ�� �ʿ䰡 ���⿡ ���ҽ� ���� ���� �Ӵ��� �ڵ尡 �ܼ�����.
	****************************************************************/
	void ComboActionBegin();
	void ComboActionEnd(class UAnimMontage* TargetMontage, bool IsProperlyEnded); 
	void SetComboCheckTimer(); //Ÿ�̸Ӹ� �ߵ���ų �Լ�.
	void ComboCheck();//Ÿ�̸Ӱ� �۵��� �Է��� üũ�ϴ� �Լ�
	
	/****************************************************
	* FTimerHandle�̶�� �ϴ� ����ü�� �����ϸ� �𸮾� ���� ���忡�� �����Ǵ� Ÿ�̸� ����� Ȱ���Ͽ�
	* ���ϴ� �ð��� Ư���Լ��� ȣ���ϵ��� ���� ����.(�������� �׼��� �ϰų� �߰��� ���� ����.)
	* HasnextComboCommand�� �ߵ��� Ÿ�̸� ������ �Է� Ŀ�ǵ尡 ���Դ��� �����ϴ� Bool����.
	* ���� ���������� ����ϱ⿡ UPROPERTY�� ������� ����.
	****************************************************/
	int32 CurrentCombo = 0; //�޺��� ���� ��Ȳ(0�̸� ���� �޺��� ���۵� ����.)
	FTimerHandle ComboTimerHandle;
	bool HasNextComboCommand = false;



/******************************************************
* Attack Hit Section
******************************************************/
protected:
	virtual void AttackHitCheck() override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;



/******************************************************
* Dead Section
******************************************************/
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> DeadMontage;

	virtual void SetDead(); //�״� ����
	void PlayDeadAnimation(); //�״� ��Ÿ�� ���.

	float DeadEventDelayTime = 5.0f;


/******************************************************
* Stat Section
******************************************************/
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UMineCharacterStatComponent> Stat;



/******************************************************
* UI Widget Section
******************************************************/
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Widget, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UMineWidgetComponent> HpBar;

	virtual void SetupCharacterWidget(class UMineUserWidget* InUserWidget) override;

/******************************************************
* Item Section
******************************************************/

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Equipment, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USkeletalMeshComponent> Weapon;

	UPROPERTY()
	TArray<FTakeItemDelegateWrapper> TakeItemActions; //TakeItemActions ���� 3���� �Լ��� ���ε� �ɰ���.


	virtual void TakeItem(class UMineItemData* InItemData) override;
	virtual void DrinkPotion(class UMineItemData* InItemData);
	virtual void EquipWeapon(class UMineItemData* InItemData);
	virtual void ReadScroll(class UMineItemData* InItemData);


/******************************************************
* Niagara Section
******************************************************/
public:
	static class UNiagaraSystem* SwordFireFX;
	UPROPERTY(Transient)
	UNiagaraComponent* CurrentTrailFX = nullptr;
};