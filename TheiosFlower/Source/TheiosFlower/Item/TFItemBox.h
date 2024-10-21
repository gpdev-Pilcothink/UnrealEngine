// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TFItemBox.generated.h"

UCLASS()
class THEIOSFLOWER_API ATFItemBox : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATFItemBox();
protected:
	UPROPERTY(VisibleAnywhere, Category = Box)
	TObjectPtr<class UBoxComponent> Trigger;
	//�츮�� ������ �̺�Ʈ�� ����ؾ� �ϴ� Ʈ���� ������Ʈ���� �̹� ������ �̺�Ʈ ������ ���� ��������Ʈ�� ����.
	//���⿡ ����� �Լ��� ���Ϳ��ٰ� ����.

	UPROPERTY(VisibleAnywhere, Category = Box)
	TObjectPtr<class UStaticMeshComponent> Mesh;

	UPROPERTY(VisibleAnywhere, Category = Effect)
	TObjectPtr<class UParticleSystemComponent> Effect;


	UPROPERTY(EditAnywhere, Category = Item)
	TObjectPtr<class UTFItemData> Item;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult);

	UFUNCTION()
	void OnEffectFinished(class UParticleSystemComponent* ParticleSystem);

};