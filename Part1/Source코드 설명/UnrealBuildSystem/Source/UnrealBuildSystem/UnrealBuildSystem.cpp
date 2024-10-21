#include "UnrealBuildSystem.h"
#include "Modules/Modulemanager.h"

/*매크로를 사용하여 모듈의 뼈대를 구축할 클래스를 지정해줘야한다.*/

IMPLEMENT_PRIMARY_GAME_MODULE(FDefaultGameModuleImpl, UnrealBuildSystem, "UnrealBuildSystem");
//우리가 만드는 게임 프로젝트에는 '주 게임 모듈'(IMPLEMENT_PRIMARY_GAME_MODULE)이라 불리는 모듈을 지정해주자. 
//FDefaultGameModuleImpl 이라고 하는 언리얼 엔진이 제공하는 게임 모듈 기본 클래스를 지정하자.
//그다음에 모듈 이름과 "게임이름"을 지정하면 총 3가지를 지정해주게 된다.
//이렇게 하면 게임에 관련된 핵심 기능을 담당하는 모듈이 된다.