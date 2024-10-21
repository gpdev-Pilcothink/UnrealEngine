// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/TFCharacterBase.h"
#include "TFCharacterNonPlayer.generated.h"

/**
 * 
 */
UCLASS()
class THEIOSFLOWER_API ATFCharacterNonPlayer : public ATFCharacterBase
{
	GENERATED_BODY()
	
	ATFCharacterNonPlayer();

protected:
	void SetDead() override;
};
