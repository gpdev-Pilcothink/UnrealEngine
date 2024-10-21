// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/ABAnimationAttackinterface.h"
#include "Interface/ABCharacterWidgetInterface.h"
#include "Interface/ABCharacterItemInterface.h"
#include "ABCharacterBase.generated.h"


DECLARE_LOG_CATEGORY_EXTERN(LogABCharacter, Log, All);


UENUM()
enum class ECharacterControlType : uint8
{
	Shoulder,
	Quater
};


DECLARE_DELEGATE_OneParam(FOnTakeItemDelegate, class UABItemData* /*InItemData*/);
//�� ��������Ʈ�� ��� �츮�� �ټ��� �迭�� ������ �ҷ��� �ϴµ� �̰� ��ü�� ���ڷ� �� ���� ����.
//�׷��� �̰��� �迭�� �����ϱ� ���� ���� ����� �̰��� ���δ� ����ü�� �ϳ� ����� ���̴�.
USTRUCT(BlueprintType)

struct FTakeItemDelegateWrapper
{
	GENERATED_BODY()
	
	FTakeItemDelegateWrapper() {}
	FTakeItemDelegateWrapper(const FOnTakeItemDelegate& InItemDelegate) : ItemDelegate(InItemDelegate) {}
	
	FOnTakeItemDelegate ItemDelegate;

	//�̷��� �ؼ� ���� ��Ʈ��Ʈ ����ü�� ���� ���� �������� �� �ֵ��� Item Section���� ���� �迭�� ��������.
};


UCLASS()
class ARENABATTLE_API AABCharacterBase : public ACharacter, public IABAnimationAttackInterface, public IABCharacterWidgetInterface, public IABCharacterItemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AABCharacterBase();
	
	/*
	* ĳ���Ϳ��� �¾��� �����ϴ� �������� Begin Play�� ����Ǳ����� Stat�� ��������Ʈ�� ����Ͽ� �׾����� �״� ����� �����ϵ��� ó������. 
	*/
	virtual void PostInitializeComponents() override; //begin play���� �����ϰų� �����ڿ��� ���ε� ����


public:
	virtual void SetCharacterControlData(const class UABCharacterControlData* CharacterControlData);
	//SetCharacterControlData��� �Լ��� �߰��ϰ� ���⼭�� ĳ���� ��Ʈ�� ������ ������ �Է����� �޷η� �����Ұ���.

	//enum�� �������� �� ���� ���� ������Ʈ�� �ٷ� ���� �� �ֵ��� ��(TMap)���� ������ ����
	UPROPERTY(EditAnyWhere, Category = CharacterControl, Meta = (AllowPrivateAccess = "true"))
	TMap<ECharacterControlType, class UABCharacterControlData*> CharacterControlManager;
	//TMap�� Ű�� �ڷ������� �ؽ����̺� �����̶� ��������.


/*�޺� �׼� �κ�*/
protected:

	UPROPERTY(EditAnyWhere,BluePrintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> ComboActionMontage;

	UPROPERTY(EditAnyWhere, BluePrintReadWrite, Category = Animation, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UABComboActionData> ComboActionData;
	//������ �����ϵ��� ���� �߰�.

	void ProcessComboCommand();
	//�Է��� ���ؼ� Ŀ�ǵ尡 ���ð� ������ �̰��� ���ؼ� ��Ÿ�ָ� ������� �� ���̴�.

	void ComboActionBegin();
	//��Ÿ�ְ� ���۵� �� ȣ���ϴ� ComboActionBegin�Լ�
	void ComboActionEnd(class UAnimMontage* TargetMontage, bool IsProperlyEnded);
	//��Ÿ�ְ� ��� ����Ǿ����� ȣ���ϴ�  ComboActionBegin�Լ�
	//�̶� ComboActionBegin�� ��� ��Ÿ�ֿ� ������ ��������Ʈ�� ���ؼ� �ٷ� ȣ��� �� �ֵ��� �Ķ����[(class UAnimMontage* TargetMontage, bool IsProperlyEnded)]�� �������.

	void SetComboCheckTimer();//Ÿ�̸Ӹ� �ߵ���ų SetComboCheckTimer�Լ� ����
	void ComboCheck();//�Է��� ���Դ��� �ȵ��Դ����� üũ�ϴ� ComboCheck��� �Լ� ����

	int32 CurrentCombo = 0;
	//���� �޺��� ������ ����Ǵ����� �����ϱ� ���ؼ� integer������ CurrentCombo��� ���� �����Ұ���.
	//���ο����� ����Ұ��̱⿡ ������ UPROPERTY()�� ������ ��������.
	//�޺��� 0 �̸� ���� �޺��� ���۵��� �������̰� 1�̻��̸� �޺��� ���۵Ȱ���.
	FTimerHandle ComboTimerHandle;
	bool HasNextComboCommand = false;
	//HasnextComboCommand�� ���� �������������� ����ϱ⿡ UPROPERTY�� ������ ����.
	//���� �����ð��� ���� Integer�� ������ �ʿ䰡 ����.


//Attack Hit Section
protected:
	virtual void AttackHitCheck() override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	//�𸮾��� ���� ������ TakeDamage��� �Լ��� ���� [�̹� ���Ϳ��� ������ �Ǿ� �־ �������̵� �޾� ���� �ϸ� ��.]


//Dead Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> DeadMontage;
	//�ռ��� ������ Dead��Ÿ�ָ� ������

	virtual void SetDead();
	//�״� ���¸� �����ϴ� �Լ�
	void PlayDeadAnimation();


	float DeadEventDelayTime = 5.0f;


//Stat Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UABCharacterStatComponent> Stat;

//UI Widget Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Widget, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UABWidgetComponent> HpBar;


	virtual void SetupCharacterWidget(class UABUserWidget* InUserWidget) override;


//Item Section
protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Equipment, Meta = (AllowPrivateAccess = "true"));
	TObjectPtr<class USkeletalMeshComponent> Weapon;


	UPROPERTY()
	TArray<FTakeItemDelegateWrapper> TakeItemActions;

	virtual void TakeItem(class UABItemData* InItemData) override;
	virtual void DrinkPotion(class UABItemData* InItemData);
	virtual void EquipWeapon(class UABItemData* InItemData);
	virtual void ReadScroll(class UABItemData* InItemData);




	


};
