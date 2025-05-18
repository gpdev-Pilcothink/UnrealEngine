// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_AttackHitCheck.h"
#include "Interface/MineAnimationAttackInterface.h"

void UAnimNotify_AttackHitCheck::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	/**********************************************
	*메쉬컴포넌트가 소유하고 있는 Owner를 가져온다.
	* 그리고 이 Owner가 우리가 선언한 캐리릭터인지 체크
	* 캐릭터가 맞다면 공격 명령을 내릴 수 있음.
	* ////////////////////////////////////////
	* 근데 여기서 캐릭터를 바로 사용하기 위해서는 헤더를 추가해줘야 한다.(의존성 발생)
	* 노티파이 기능의 경우 공용으로  여러 종류의 캐릭터가 있을때 같이 사용되는 것이 좋은데 이때마다 헤더를 추가하는건 바람직하지 않음
	* 따라서 우리는 범용적인 사용을 가능하게 위해 인터페이스를 구현하자.
	* 그리고 받아온 MeshComponent의 Owner가 인터페이스를 구현했는지 체크해주면 됨.
	**********************************************/

	Super::Notify(MeshComp, Animation, EventReference);
	if (MeshComp)
	{
		IMineAnimationAttackInterface* AttackPawn = Cast<IMineAnimationAttackInterface>(MeshComp->GetOwner());
		if (AttackPawn) //인터페이스 구현됬는지 체크
		{
			AttackPawn->AttackHitCheck();
		}
	}
}
