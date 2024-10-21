// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MyGameInstance.generated.h"


//******����ü �н��κ�******
USTRUCT()
struct FStudentData
{
	GENERATED_BODY()
	//�ۼ��ϴ� ��ũ�ε��� public���� ������ �Ǵ°� ����ü�� ���� ������ �⺻ ���� �������� public�� �״�� ����ϸ� �ȴ�.

	FStudentData()
	{
		Name = TEXT("ȫ�浿");
		Order = -1;
	}
	
	FStudentData(FString InName, int32 InOrder) : Name(InName), Order(InOrder) {};

	/*FStudentData�� TSet�� ���� ���� ���� �κ�*/
	bool operator==(const FStudentData& InOther) const
	{
		return Order == InOther.Order;
	}
	friend FORCEINLINE uint32 GetTypeHash(const FStudentData& InStudentData)
	{
		//�ؽ����� uint32�� ��ȯ���ְ� ��Ÿ���ؽ��� ������ֵ� ���ڷ� ���۷����� �־��ش�.
		return GetTypeHash(InStudentData.Order);
		//�츮�� �̹� ������ �ִ� integer order�� ���� �ؽ����� �������ִ� ������ Student�� ���� �ؽ����� �������ش�.
	}
	/*FStudentData�� TSet�� ���� ���� ���� �κ�*/


	UPROPERTY() 
	/*
	�츮�� UPROPERTY�� �־ �ǰ� �ȳ־ �����ѵ� UPROPERTY�� �������� �ִ� ������ ��Ȯ�� �־�� �Ѵ�. 
	���� ��� ���÷����� ���� �����شٴ��� ��ũ��Ʈ �������� �������Ʈ�� 
	ȣȯ�� ��Ų�ٵ��� ���� �𸮾� ������Ʈ �����͸� ��������� �����ٸ� 
	UPROPERTY��ũ�θ� �־�� �Ѵ�.
	*/
	FString Name;

	UPROPERTY()
	int32 Order;
};
//******����ü �н��κ�******


/**
 * 
 */
UCLASS()
class UNREALCONTAINER_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	virtual void Init() override;

private:

	TArray<FStudentData> StudentsData;
	//�� ���� �� Ÿ���̱� ������ �޸𸮸� ������ �ʿ䰡 ����. 
	//���� ���÷��� ������� �հ� ��ȸ�ϴ°� �ƴ϶�� UPROPERTY�� ���� �ʿ䰡 ����.

	UPROPERTY() // TArray�� ���������� �����͸� �����ϰ� �ȴٸ� �̰� �� �������.
	TArray<TObjectPtr<class UStudent>> Students;
	//�𸮾� ������Ʈ ������� �𸮾� ������Ʈ �����͸� ������ ���� TObjectPtr�� ������� �Ѵ�.
	//���� ���漱������ ��� �������� �ּ�ȭ ��Ų��.
	//TArray�� ���������� �����͸� �����ϰ� �ȴٸ� �ݵ�� �ڵ����� �𸮾����� �޸𸮸� �����Ҽ� �ְ� UPROPERTY()��ũ�θ� �ݵ�� �ٿ���� �Ѵ�.


	TMap<int32, FString> StudentsMap;
	//Ȥ�ó� key�� value�� �𸮾� ������Ʈ �����Ͱ� ���� �ȴٸ� �ݵ�� UPROPERTY()�� ��������� �Ѵ�.
	
};
