// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/MineCharacterNonPlayer.h"

AMineCharacterNonPlayer::AMineCharacterNonPlayer()
{
}

void AMineCharacterNonPlayer::SetDead()
{
	Super::SetDead();

	FTimerHandle DeadTimerHandle;
	//밑에 본문을 가진 람다 함수와 연결된 타이머 델리게이트를 즉석으로 만들어 연결시켜줌.
	GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle, FTimerDelegate::CreateLambda(
		[&]()
		{
			Destroy();
		}
	), DeadEventDelayTime, false);
}
