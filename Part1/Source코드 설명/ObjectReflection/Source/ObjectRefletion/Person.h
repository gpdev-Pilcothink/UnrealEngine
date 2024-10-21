// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Person.generated.h"

/**
 * 
 */
UCLASS()
class OBJECTREFLETION_API UPerson : public UObject
{
	GENERATED_BODY()
public:
	UPerson(); //�⺻�� �����ϱ� ���� ������ �ڵ� �߰�.

	UFUNCTION()
	virtual void DoLesson();

	const FString& GetName() const;
	void SetName(const FString& InName);

	//�ܺο��� ������Ƽ�� �ڵ�� �����ϱ� ���� getter�� setter �������
	//const�� ���۷����� ��ȯ�ϵ��� �ϰ� �����Ұ��� �ƴϴϱ� const �����ڸ� ��Ȯ�ϰ� ����.

protected: //������ ��Ʃ��Ʈ�� Ƽó�� ��ӹ��� �Ӽ��� �������.
	UPROPERTY()
	FString Name;

	UPROPERTY()
	int32 Year;

private:
	
};
