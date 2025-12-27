#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SAttributeComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnHealthChanged, AActor*, InstigatorActor, USAttributeComponent*, OwningComp, float, NewHealth, float, Delta);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnRageChanged, USAttributeComponent*, OwningComp, float, NewRage, float, Delta);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROGUELIKE_API USAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	USAttributeComponent();

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	static USAttributeComponent* GetAttributeComponent(AActor* FromActor);
	
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	static bool IsActorAlive(AActor* Actor);
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes")
	float Health{};
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes")
	float MaxHealth{100.0f};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes")
	float CurrentRage{};
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes")
	float MaxRage{100.0f};

public:
	
	UPROPERTY(BlueprintAssignable)
	FOnHealthChanged OnHealthChanged;
	
	UPROPERTY(BlueprintAssignable)
	FOnRageChanged OnRageChanged;
	
	UFUNCTION(BlueprintCallable)
	bool IsAlive() const;
	
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool ApplyHealthChange(AActor* InstigatorActor, float Delta);
	
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	void ApplyRageChange(const float RageAmount);
	
	UFUNCTION(BlueprintPure, Category = "Attributes")
	float GetHealth() const;
	
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool Kill();
	
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetRage();
	
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetMaxRage();
	
	// Getter para obtener el valor de la salud máxima.
	UFUNCTION(BlueprintPure, Category = "Attributes")
	float GetMaxHealth() const;
	bool IsFullHealth() const;
};
