// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ABCharacterNonPlayer.h"

AABCharacterNonPlayer::AABCharacterNonPlayer()
{
}

void AABCharacterNonPlayer::SetDead()
{
	Super::SetDead();
	//이벤트 시간 5초시간 이후에 바로 없어지도록 구현할건데 월드 타이머 서비스를 이용해볼것임.

	FTimerHandle DeadTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle, FTimerDelegate::CreateLambda(
		[&]()
		{
			Destroy();
		}
	), DeadEventDelayTime, false);
	//두번째 인자에 함수를 지정할 수가 있다.
	//5초가 지난 이후에 어떤 함수를 실행할지 호출이 가능한데 여기서 멤버 함수를 선언하여 매핑할 수도 있다.
	//하지만 액터를 없애는 함수를 구현할건데 이것을 위해 멤버 함수를 또 만들고 하는것은 굉장히 번거롭다.
	//따라서 간편하게 만들 수 있는 람다 함수를 즉석에서 만들어서 타이머 델리게이트 구조체에  부착을 시켜놓고 이것을 바로 넘겨볼것임.
	//맨 마지막 인자는 루핑할 필요가 없어서 false를 넣어준다.

}
