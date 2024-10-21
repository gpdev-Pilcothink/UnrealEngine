// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ABCharacterNonPlayer.h"

AABCharacterNonPlayer::AABCharacterNonPlayer()
{
}

void AABCharacterNonPlayer::SetDead()
{
	Super::SetDead();
	//�̺�Ʈ �ð� 5�ʽð� ���Ŀ� �ٷ� ���������� �����Ұǵ� ���� Ÿ�̸� ���񽺸� �̿��غ�����.

	FTimerHandle DeadTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle, FTimerDelegate::CreateLambda(
		[&]()
		{
			Destroy();
		}
	), DeadEventDelayTime, false);
	//�ι�° ���ڿ� �Լ��� ������ ���� �ִ�.
	//5�ʰ� ���� ���Ŀ� � �Լ��� �������� ȣ���� �����ѵ� ���⼭ ��� �Լ��� �����Ͽ� ������ ���� �ִ�.
	//������ ���͸� ���ִ� �Լ��� �����Ұǵ� �̰��� ���� ��� �Լ��� �� ����� �ϴ°��� ������ ���ŷӴ�.
	//���� �����ϰ� ���� �� �ִ� ���� �Լ��� �Ｎ���� ���� Ÿ�̸� ��������Ʈ ����ü��  ������ ���ѳ��� �̰��� �ٷ� �Ѱܺ�����.
	//�� ������ ���ڴ� ������ �ʿ䰡 ��� false�� �־��ش�.

}
