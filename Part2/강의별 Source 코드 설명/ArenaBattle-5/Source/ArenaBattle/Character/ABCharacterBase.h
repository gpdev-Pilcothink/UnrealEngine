// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ABCharacterBase.generated.h"


UENUM()
enum class ECharacterControlType : uint8
{
	Shoulder,
	Quater
};


UCLASS()
class ARENABATTLE_API AABCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AABCharacterBase();

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

};
