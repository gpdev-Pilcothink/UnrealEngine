// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/MineCharacterBase.h"
#include "MineCharacterNonPlayer.generated.h"

/**
 * 
 */
UCLASS()
class CHAOSMINE_API AMineCharacterNonPlayer : public AMineCharacterBase
{
	GENERATED_BODY()
public:
	AMineCharacterNonPlayer();

protected:
	void SetDead() override;
};
