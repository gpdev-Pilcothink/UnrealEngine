// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "MineWidgetComponent.generated.h"

/**
 * 
 */
UCLASS()
class CHAOSMINE_API UMineWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()


protected:
	virtual void InitWidget() override;

	
};
