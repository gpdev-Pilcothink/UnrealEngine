#include "UnrealBuildSystem.h"
#include "Modules/Modulemanager.h"

/*��ũ�θ� ����Ͽ� ����� ���븦 ������ Ŭ������ ����������Ѵ�.*/

IMPLEMENT_PRIMARY_GAME_MODULE(FDefaultGameModuleImpl, UnrealBuildSystem, "UnrealBuildSystem");
//�츮�� ����� ���� ������Ʈ���� '�� ���� ���'(IMPLEMENT_PRIMARY_GAME_MODULE)�̶� �Ҹ��� ����� ����������. 
//FDefaultGameModuleImpl �̶�� �ϴ� �𸮾� ������ �����ϴ� ���� ��� �⺻ Ŭ������ ��������.
//�״����� ��� �̸��� "�����̸�"�� �����ϸ� �� 3������ �������ְ� �ȴ�.
//�̷��� �ϸ� ���ӿ� ���õ� �ٽ� ����� ����ϴ� ����� �ȴ�.