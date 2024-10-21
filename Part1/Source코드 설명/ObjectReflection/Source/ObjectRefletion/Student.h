// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Person.h"
#include "Student.generated.h"

/**
 * 
 */
UCLASS()
class OBJECTREFLETION_API UStudent : public UPerson
{
	GENERATED_BODY()
	
public:
	UStudent();
	virtual void DoLesson() override;
	
private:
	UPROPERTY();
	int32 Id;

};
/*
*person�� ��ӹ޾ƾ� �ϴµ� person�� ��ӹ������� ��������� include�ؾ� �Ѵ�.
*�𸮾� ������Ʈ ������� �ٸ� ����� include�ϰ� �Ǹ� �����ؾ� �ϴµ��𸮾� ��������� generated.h ���� �ؿ� include �߱� �����̴�.
*���� �𸮾� ������Ʈ���� �����Ҷ��� generated.h�� ���� �ؿ� �ִ°��̱⺻ ��Ģ�̴�.
*/

/*
* Person���� ������ DoLesson �Լ��� ������ �Ŀ� Student�� DoLesson�� ������ ����.
*/