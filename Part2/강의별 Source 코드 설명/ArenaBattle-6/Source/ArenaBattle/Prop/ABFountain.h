// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ABFountain.generated.h"

UCLASS()
class ARENABATTLE_API AABFountain : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AABFountain();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, Category=Mesh)
	//지정자는 객체타입으로  VisibleAnywhere로 선언
	//블루프린트는 상속받았을때 읽고 쓸 수 있게 끔 BlueprintReadWrite
	//카테고리는 Mesh라고 하면 언리얼 헤더툴에 의해서 카테고리가 분석되어 자동으로 메쉬한 대한 값을 편집할 수가 있게 에디터 UI가 제공이 된다.
	TObjectPtr<class UStaticMeshComponent> Body;


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Mesh)
	TObjectPtr<class UStaticMeshComponent> Water;
};
