// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_AttackHitCheck.h"
#include "Interface/ABAnimationAttackInterface.h"

void UAnimNotify_AttackHitCheck::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	
	/*
	* Notify함수의 인자들을 보면  애니메이션을 관리하는 스케레탈 메쉬 컴포넌트와 애니메이션 정보 그리고 추가적인 레퍼런스 정보를 요구한다.
	* 우선 이 정보로부터 우리가 액터에 명령을 내려야 된다.
	*/
	if (MeshComp)//만약 메쉬컴포넌트가 있을경우 우리는 Owner를 가져와야한다.
	{
		MeshComp->GetOwner(); 
		/*Owner를 가져오면 캐릭터인지 체크해주고 캐릭터라면 공격을 판저앟라고 명령을 내려야 한다.[캐릭터 헤더 추가해야한다.]
		다른 폴더의 헤더를 추가하는 것은 어쨋든 의존성이 생김.
		여러종류의 캐릭터가 있을때 같이 사용되는 것이 좋은데 이때마다 이렇게 헤더를 추가하는 것은 바람직 하지 않다.
		따라서 우리가 이것을 좀 더 범용적으로 사용하기 위해서는 인터페이스를 구현하는 것이 좋다.
		우리가 어쩔수 없는 의존성이 발생되는 경우에는 가급적 인터페이스를 통해서 구현하도록 설계하면 보다 범용적으로 이러한 노티파이 기능들을 사용할 수 있게 된다.
		*/

		IABAnimationAttackInterface* AttackPawn = Cast<IABAnimationAttackInterface>(MeshComp->GetOwner());
		if (AttackPawn)
		{
			AttackPawn->AttackHitCheck();
		}
	
	}
	
}
