// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/MineCharacterNonPlayer.h"

AMineCharacterNonPlayer::AMineCharacterNonPlayer()
{
}

void AMineCharacterNonPlayer::SetDead()
{
	Super::SetDead();

	FTimerHandle DeadTimerHandle;
	//�ؿ� ������ ���� ���� �Լ��� ����� Ÿ�̸� ��������Ʈ�� �Ｎ���� ����� ���������.
	GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle, FTimerDelegate::CreateLambda(
		[&]()
		{
			Destroy();
		}
	), DeadEventDelayTime, false);
}
