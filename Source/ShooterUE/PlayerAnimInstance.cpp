// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnimInstance.h"
#include "PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void UPlayerAnimInstance::UpdateAnimationProperties(float Deltatime)
{
	//ESTE ESTÁ OBSOLETO
	//Comprobamos que el PlayerCharacter no sea nulo
	if (PlayerCharacter == nullptr)
	{
		//Tratamos de obtener de nuevo el Pawn
		PlayerCharacter = Cast<APlayerCharacter>(TryGetPawnOwner());
	}else
	{
		//Creamos algunas variables a utilizar
		FVector Velocity = PlayerCharacter->GetVelocity();	//Obtenemos la velocidad del PlayerCharacter
		Velocity.Z = 0;	//Lo igualamos a zero ya que nomas queremos la velocidad en el suelo
		Speed = Velocity.Size();	//Pasamos la velocidad general a Speed

		//Comprobamos si está en el aire o no
		bIsInAir = PlayerCharacter->GetCharacterMovement()->IsFalling();

		//Comprobamos la aceleración, comprobamos si está el personaje acelerando
		/*if (PlayerCharacter->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.0)
		{
			bIsAccelerating = true;
		}else
		{
			bIsAccelerating = false;
		} */
		//Esta manera es igual a la de arriba, pero corta d:, recomendada en algunos casos
		bIsAccelerating = (PlayerCharacter->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.0);

		
	}
}

void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	//Comprobamos que el PlayerCharacter no sea nulo
	if (PlayerCharacter == nullptr)
	{
		//Tratamos de obtener de nuevo el Pawn
		PlayerCharacter = Cast<APlayerCharacter>(TryGetPawnOwner());
	}else
	{
		//Creamos algunas variables a utilizar
		FVector Velocity = PlayerCharacter->GetVelocity();	//Obtenemos la velocidad del PlayerCharacter
		Velocity.Z = 0;	//Lo igualamos a zero ya que nomas queremos la velocidad en el suelo
		Speed = Velocity.Size();	//Pasamos la velocidad general a Speed

		//Comprobamos si está en el aire o no
		bIsInAir = PlayerCharacter->GetCharacterMovement()->IsFalling();

		//Comprobamos la aceleración, comprobamos si está el personaje acelerando
		/*if (PlayerCharacter->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.0)
		{
			bIsAccelerating = true;
		}else
		{
			bIsAccelerating = false;
		} */
		//Esta manera es igual a la de arriba, pero corta d:, recomendada en algunos casos
		bIsAccelerating = (PlayerCharacter->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.0);

		
	}
}

void UPlayerAnimInstance::NativeInitializeAnimation()
{
	//FUNCION QUE NOS SERVIRÁ COMO UN INICIADOR DE ELEMENTOS
	Super::NativeInitializeAnimation();	//Aplicamos la herencia

	//Buscamos nuestro player Character, con cast estamos diciendo que la variable que tomamos ahora la transformaremos
	//a PlayerCharacter, en el caso de que no se pueda convertir sería nulo
	//Se intentará obtener el Pawn de una clase y la convertira a PlayerCharacter
	PlayerCharacter = Cast<APlayerCharacter>(TryGetPawnOwner());
}
