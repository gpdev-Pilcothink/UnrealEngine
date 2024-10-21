// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/TFAnimationAttackInterface.h"
#include "Interface/TFCharacterWidgetInterface.h"
#include "Interface/TFCharacterItemInterface.h"
#include "TFCharacterBase.generated.h"


DECLARE_LOG_CATEGORY_EXTERN(LogTFCharacter, Log, All);

UENUM()
enum class ECharacterControlType : uint8
{
	Shoulder,
	Quater
};

DECLARE_DELEGATE_OneParam(FOnTakeItemDelegate, class UTFItemData* /*InItemData*/);
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
class THEIOSFLOWER_API ATFCharacterBase : public ACharacter, public ITFAnimationAttackInterface, public ITFCharacterWidgetInterface ,public ITFCharacterItemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATFCharacterBase();

	/*
	* ĳ���Ϳ��� �¾��� �����ϴ� �������� Begin Play�� ����Ǳ����� Stat�� ��������Ʈ�� ����Ͽ� �׾����� �״� ����� �����ϵ��� ó������.
	*/
	virtual void PostInitializeComponents() override; //begin play���� �����ϰų� �����ڿ��� ���ε� ����

public:
	virtual void SetCharacterControlData(const class UTFCharacterControlData* CharacterControlData);
	//SetCharacterControlData��� �Լ��� �߰��ϰ� ���⼭�� ĳ���� ��Ʈ�� ������ ������ �Է����� �޷η� �����Ұ���.

	//enum�� �������� �� ���� ���� ������Ʈ�� �ٷ� ���� �� �ֵ��� ��(TMap)���� ������ ����
	UPROPERTY(EditAnyWhere, Category = CharacterControl, Meta = (AllowPrivateAccess = "true"))
	TMap<ECharacterControlType, class UTFCharacterControlData*> CharacterControlManager;
	//TMap�� Ű�� �ڷ������� �ؽ����̺� �����̶� ��������.

/*�޺� �׼� �κ�*/
protected:

	UPROPERTY(EditAnyWhere, BluePrintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> ComboActionMontage;

	UPROPERTY(EditAnyWhere, BluePrintReadWrite, Category = Animation, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UTFComboActionData> ComboActionData;
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
	//���� Integer�� ����X

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


	// Stat Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UTFCharacterStatComponent> Stat;

	//UI Widget Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Widget, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UTFWidgetComponent> HpBar;

	virtual void SetupCharacterWidget(class UTFUserWidget* InUserWidget) override;


	//Item Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Equipment, Meta = (AllowPrivateAccess = "true"));
	TObjectPtr<class USkeletalMeshComponent> Weapon;


	UPROPERTY()
	TArray<FTakeItemDelegateWrapper> TakeItemActions;

	virtual void TakeItem(class UTFItemData* InItemData) override;
	virtual void DrinkPotion(class UTFItemData* InItemData);
	virtual void EquipWeapon(class UTFItemData* InItemData);
	virtual void ReadScroll(class UTFItemData* InItemData);



};