#pragma once

#include "CoreMinimal.h"

#define CPROFILE_ABCAPSULE TEXT("ABCapsule")
#define CPROFILE_ABTRIGGER TEXT("ABTrigger")
//앞서서 생성한 두개의 프로필을  CPROFILE_ABCAPSULE/CPROFILE_ABTRIGGER 라고 이름을 지음
#define CCHANNEL_ABACTION ECC_GameTraceChannel1
//우리가 생성한  CCHANNEL_ABACTION이라는 전처리기 definition을 추가함.
//ECC_GameTraceChannel1은 앞서 살펴본 ini파일에서 지정된 채널 열거형 값으로 바로 변환 되는데 
//이렇게 우리가 전처리기를 추가하면 좀더 편리하게 코딩 진행이 가능하다.