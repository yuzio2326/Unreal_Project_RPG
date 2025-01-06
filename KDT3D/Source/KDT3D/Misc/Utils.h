#pragma once
#include "CoreMinimal.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"


namespace CollisionProfileName
{
	static inline FName PawnTrigger = TEXT("PawnTrigger");		// 플레이어, 적, Pawn을 감지하는 트리거
	static inline FName PlayerTrigger = TEXT("PlayerTrigger");	// 플레이어를 감지하는 트리거
	static inline FName Player = TEXT("Player");
	static inline FName Enemy = TEXT("Enemy");
	static inline FName Projectile = TEXT("Projectile");
}
namespace SocketName
{
	static inline FName Weapon = TEXT("Weapon");
	static inline FName Muzzle = TEXT("Muzzle"); // 총구
}

namespace MF_PostEffect
{
	static inline FName PaperBurnDissolve = TEXT("PaperBurnDissolve");
}

class FUtils
{
public:
	static const UInputAction* GetInputActionFromName(UInputMappingContext* IMC, const FName& InName)
	{
		const TArray<FEnhancedActionKeyMapping>& Mappings = IMC->GetMappings();
		for (auto& It : Mappings)
		{
			if (It.Action->GetFName() == InName)
			{
				return It.Action.Get();
			}
		}

		return nullptr;
	}
};