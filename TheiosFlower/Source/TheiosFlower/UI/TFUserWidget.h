// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TFUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class THEIOSFLOWER_API UTFUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	FORCEINLINE void SetOwningActor(AActor* NewOwner) { OwingActor = NewOwner; }
	
	
protected:
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = "Actor")
	TObjectPtr<AActor> OwingActor;//현재 위젯을 소유하고 있는 액터정보를 보관할 포인터 생성	

};
