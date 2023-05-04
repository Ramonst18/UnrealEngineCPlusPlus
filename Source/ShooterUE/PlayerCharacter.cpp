// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

#include "Camera/CameraComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
	//ESTE ES EL CONSTRUCTOR
	
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;	//Indicamos que la clase puede usar la funcion Tick

	//NOTA: CON -> ESTAMOS ACCEDIENDO A ATRIBUTOS DE LA CLASE
	
	//variable, se crea un subObjeto Defautl <Tipo del objeto>, El metodo nos pide el nombre que tendra en unreal
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringCamera"));	
	CameraBoom->SetupAttachment(RootComponent);		//Le asignamos una funcion para atar el componente al RootComponent
	CameraBoom->TargetArmLength = 180;	//indicamos la distancia entre el springCamera y los objetos que tenga unidos
	CameraBoom->bUsePawnControlRotation = true;	//que rote el spring cuando rote el jugador
	CameraBoom->SocketOffset = FVector(0,50,70);	//Tratando de poner la camara al hombro

	
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);	//atamos la camara con el spring camera
	//especificamente al Socket del springCamera
	FollowCamera->bUsePawnControlRotation = false;	//hacemos que la camara no rote
	//NOTA IMPORTANTE: todas variables con b al inicio significan que son booleanas

	//Damos valores a las variables del movimiento de la camara
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	//Hacemos que el personaje rote con el movimiento de la camara, solo la manera horizontal
	bUseControllerRotationYaw = true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	//Variables del zoom de la camara
	CameraZommedFOV = 35.f;
	SpeedZoomed = 20;
	
}//End constructor

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	//Obtenemos el FOV inicial de la camara, primero comprobando que exista el FollowCamera
	if (FollowCamera)
	{
		CameraDefaultFOV = FollowCamera->FieldOfView;
		CameraCurrentFOV = CameraDefaultFOV;
	}
	
	
	//variables
	//int numero = 3;
	//float numeroDecimal = 5.6;
	//FString cadena = "ADIOS";

	//NOTA: el %d indica que pasaremos variables enteras y sera la posición en donde estaran
	//el %f pasaremos variables flotantes
	//el %s pasaremos variables cadenas, debemos de poner un * al inicio del nombre para que pueda acceder al valor
	//ya que el FString es conciderado como un objeto
	
	//imprimir en consola
	//UE_LOG(LogTemp, Warning, TEXT("HOLA MUNDO AAAAAAAAAAA %s %f %d"), *cadena, numeroDecimal, numero);

	//comprobamos que existe
	/*if (GEngine)
	{
		//llamamos a la clase GEngine y llamamos una funcion
		//imprimir en pantalla
		GEngine->AddOnScreenDebugMessage(-1, 15, FColor::Yellow, TEXT("Esto es un mensaje de pruebas"));
	}*/
	
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	CameraInterpZoom(DeltaTime);
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//PASAREMOS AL PlayerInputComponent LAS TECLAS QUE NOS FUNCIONARAN COMO EL ACCIONADOR AL MOVIMIENTO DEL PERSONAJE
	//pasamos el nombre del axis, el objeto al que hara el movimiento y pasamos la funcion( accedemos la direccion de memoria de nuestro personaje y obtenemos la funcion)
	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight",this, &APlayerCharacter::MoveRight);

	//INPUTS DEL MOVIMIENTO DE LA CAMARA
	//movimiento mediante teclas
	PlayerInputComponent->BindAxis("TurnRate",this,&APlayerCharacter::TurnAtRate);	//Movimiento con funciones propias
	PlayerInputComponent->BindAxis("LookUpRate",this,&APlayerCharacter::LookAtRate);
	//Movimiento mediante el raton
	PlayerInputComponent->BindAxis("Turn",this,&APawn::AddControllerYawInput);	//Movimiento con funciones ya establecidas
	PlayerInputComponent->BindAxis("LookUp",this,&APawn::AddControllerPitchInput);

	//SALTO DEL PERSONAJE
	//BindAction espera: El nombre del actionMaping, en que momento se ejecutará(Cuando se presione, se suelte u otro), que clase lo hara y despues la funcion a ejecutar
	PlayerInputComponent->BindAction("Jump",IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump",IE_Released, this, &ACharacter::StopJumping);

	//DISPARO DEL PERSONAJE
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &APlayerCharacter::Fire);	//Click Dispara

	//ZOOM DE LA CAMARA
	PlayerInputComponent->BindAction("AiminButton",IE_Pressed,this, &APlayerCharacter::AiminButtonPressed);
	PlayerInputComponent->BindAction("AiminButton",IE_Released,this, &APlayerCharacter::AiminButtonReleased);

}

//FUNCIONES
void APlayerCharacter::MoveForward(float Value)
{
	//comprobamos que el controlador no sea nulo y que el valor sea distinto de cero
	if (Controller != nullptr && Value != 0.0f)
	{
		//obtenemos la rotacion
		const FRotator Rotation = Controller->GetControlRotation();

		//Obtenemos el Yaw rotation (la rotacion del eje z)
		const FRotator YawRotation(0, Rotation.Yaw,0);

		//Creamos una direccion, es a donde se movera el personaje
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);	//primero obtenemos la direccion en x
		AddMovementInput(Direction, Value);		//El movimiento se estará dando al character Movement
		
	}
}

void APlayerCharacter::MoveRight(float Value)
{
	//comprobamos que el controlador no sea nulo y que el valor sea distinto de cero
	if (Controller != nullptr && Value != 0.0f)
	{
		//obtenemos la rotacion
		const FRotator Rotation = Controller->GetControlRotation();

		//Obtenemos el Yaw rotation (la rotacion del eje z)
		const FRotator YawRotation(0, Rotation.Yaw,0);

		//Creamos una direccion, es a donde se movera el personaje
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);	//primero obtenemos la direccion en y
		AddMovementInput(Direction, Value);		//El movimiento se estará dando al character Movement
		
	}
}

void APlayerCharacter::TurnAtRate(float Rate)
{
	/*MOVIMIENTO DE LA CAMARA DE FORMA HORIZONTAL*/
	//Multiplicamos el valor obtenido por el BaseTurnRate y este por el GetWorld()->getDeltaSeconds
	//Este ultimo es para asegurar la velocidad del movimiento de la camara sea igual para casi todos
	//los fps actuales del juego
	AddControllerYawInput(Rate*BaseTurnRate*GetWorld()->GetDeltaSeconds());
	
}

void APlayerCharacter::LookAtRate(float Rate)
{
	/*MOVIMIENTO DE LA CAMARA DE FORMA VERTICAL*/
	
	AddControllerPitchInput(Rate*BaseLookUpRate*GetWorld()->GetDeltaSeconds());
}

void APlayerCharacter::Fire()
{
	//SONIDO
	//Comprobamos que existe el sonido de disparo
	if (FireSound)
	{
		//UGameplayStatics Nos permite obtener elementos de nuestro programa o ejecutar cosas
		//En este caso queremos ejecutar un sonido
		UGameplayStatics::PlaySound2D(this, FireSound);
	}
	
	//Animacion
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();	//Del mesh obtenemos el AnimINstance
	//Verificamos que tenemos el AnimINstance y el FireAnimMontage
	if (AnimInstance && FireAnimMontage)
	{
		//Ejecutamos un AnimMontage
		AnimInstance->Montage_Play(FireAnimMontage);
		//Del Montage nos iremos a la seccion StartFire y se ejecutara a partir de ahi
		AnimInstance->Montage_JumpToSection(FName("StartFire"));
	}

	//Disparos
	FireLineCast("Fire_SocketL");
	FireLineCast("Fire_SocketR");
}

bool APlayerCharacter::GetBeamEndLocation(const FVector& MuzzleSocketLocation, FHitResult& OutHitResult)
{
	//Crearemos la trayectoria del disparo
	FVector OutBeamLocation;
	FHitResult CrosshaitHitResult;
	bool bCrosshairHit = TraceUnderCrosshairs(CrosshaitHitResult, OutBeamLocation);	//Obtenemos si hubo colision del disparo

	//Comprobamos si hubo colision con el disparo
	if (bCrosshairHit)
	{
		//pasamos la posicion de impacto al OutBeamLocation
		OutBeamLocation = CrosshaitHitResult.Location;
	}
	
	//DIBUJO DEL TRAZO DE LA TRAYECTORIA DE NUESTRO DISPARO
	const FVector WeaponTraceStart = MuzzleSocketLocation;
	const FVector WeaponTraceEnd = OutBeamLocation;

	//lo ponemos en la posicion trazando una segunda linea (solo para asegurarnos)
	GetWorld()->LineTraceSingleByChannel(
		OutHitResult,
		WeaponTraceStart,
		WeaponTraceEnd,
		ECollisionChannel::ECC_Visibility
	);

	//preguntamos si no hay colision
	if (!OutHitResult.bBlockingHit)
	{
		OutHitResult.Location = OutBeamLocation;
		return false;
	}
	
	return true;
}

bool APlayerCharacter::TraceUnderCrosshairs(FHitResult& OutHitResult, FVector& OutHitLocation)
{
	//Variables locales
	FVector2d ViewPortSize;

	//Verificamos is tiene valor el GameViewPort, ya que estamos trabajando con punteros
	if (GEngine && GEngine->GameViewport)
	{
		//Le damos valor al ViewPortSize, el valor sera el tamano del Viewport
		GEngine->GameViewport->GetViewportSize(ViewPortSize);
	}

	//Obtenemos datos sobre la mira de apuntar
	FVector2d CrosshairLocation2D = FVector2D(ViewPortSize.X/2,ViewPortSize.Y/2);
	FVector CrosshairWorldLocation;
	FVector CrosshairWorldDirection;

	//obtenemos las coordenadas para las variables Crosshair
	bool bScreenToWorld = UGameplayStatics::DeprojectScreenToWorld(
		UGameplayStatics::GetPlayerController(this,0),
		CrosshairLocation2D,
		CrosshairWorldLocation,
		CrosshairWorldDirection
	);

	//Comprobamos que se pudo obtener las coordenadas
	if (bScreenToWorld)
	{
		//Creamos el vector de inicio y el final de la posicion del mundo
		const FVector Start = CrosshairWorldLocation;
		const FVector End = Start + (CrosshairWorldLocation*50000.f);
		OutHitLocation = End;

		//Trazamos una linea
		GetWorld()->LineTraceSingleByChannel(OutHitResult,Start, End, ECollisionChannel::ECC_Visibility);

		//Comprobamos si choca la linea con algo
		if (OutHitResult.bBlockingHit)
		{
			//Cambiamos la posicion del LiceTrace final a la posicion del impacto
			OutHitLocation = OutHitResult.Location;

			//Si colisiona regresamos el true
			return true;
		}
		
	}

	//Si no colisionó entonces regresamos un falso
	return false;
	
}

void APlayerCharacter::AiminButtonPressed()
{
	bAiming = true;

}

void APlayerCharacter::AiminButtonReleased()
{
	bAiming = false;

}

void APlayerCharacter::CameraInterpZoom(float DeltaTime)
{
	//Comprobamos si el puntero de la camara tiene valor
	if (FollowCamera)
	{
		//Preguntamos si esta apuntando para poder incrementar el zoom del apuntado
		if (bAiming)
		{
			//Incremento en cierta cantidad de tiempo, desde donde hasta donde, cada tiempo con una velocidad de.
			CameraCurrentFOV = FMath::FInterpTo(CameraCurrentFOV, CameraZommedFOV, DeltaTime, SpeedZoomed);
		}else{
			CameraCurrentFOV = FMath::FInterpTo(CameraCurrentFOV, CameraDefaultFOV, DeltaTime, SpeedZoomed);
		}

		//Pasamos los datos del FOV a la camara
		FollowCamera->SetFieldOfView(CameraCurrentFOV);
	}
}

void APlayerCharacter::FireLineCast(FName SocketName)
{
	//Obtenemos los sockets
	const USkeletalMeshSocket* Fire_Socket = GetMesh()->GetSocketByName(SocketName);
	//Comprobamos que existe el socket
	if (Fire_Socket)
	{
		//obtenemos el transform
		const FTransform Fire_SocketTransform = Fire_Socket->GetSocketTransform(GetMesh());	//Lo obtenemos del esqueleto del personaje

		//Verificamos que exista MuzzlesParticles
		if (MuzzlesParticles)
		{
			//Instanciamos las particulas
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MuzzlesParticles,
				Fire_SocketTransform.GetLocation());
			
		}

		FHitResult OutHitResult;
		bool bBeamEnd = GetBeamEndLocation(Fire_SocketTransform.GetLocation(), OutHitResult);
		
		//Comprobamos si el LineCast choco con algo
		if (bBeamEnd)
		{
			//comprobamos si existen las particulas de impacto
			if (ImpactParticles)
			{
				//Reproducimos las particulas en el punto de impacto
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticles, OutHitResult.ImpactPoint);
			}

			//Verificamos el puntero
			if (BeamParticles)
			{
				//Instanciamos la trayectoria de la bala
				UParticleSystemComponent* Beam = UGameplayStatics::SpawnEmitterAtLocation(
					GetWorld(),
					BeamParticles, Fire_SocketTransform);

				//Si la instancia se creo entonces
				if (Beam)
				{
					Beam->SetVectorParameter(FName("Target"),OutHitResult.ImpactPoint);
				}
				
			}
		}
		
		/*TEST DE DISPARO
		FHitResult FireHit;	//Guardará la informacion si nuestro disparo impacta con algo
		const FVector Start = Fire_SocketTransform.GetLocation();	//Obtenemos la posicion de inicio
		const FQuat Rotation = Fire_SocketTransform.GetRotation();	//Obtenemos la rotacion
		const FVector RotationAxis = Rotation.GetAxisX();	//Obtenemos la direccion de la rotacion
		const FVector End = (Start+RotationAxis*50000.f);	//Creamos la variable de la posicion final del disparo

		//Creamos el rayo, SOLO impactara con objetos visibles
		GetWorld()->LineTraceSingleByChannel(FireHit, Start, End, ECC_Visibility);

		//Comprobamos si chocó con algo
		if (FireHit.bBlockingHit)
		{
			//Dibujamos una linea (Solo pruebas)
			DrawDebugLine(GetWorld(),Start, End, FColor::Red,false,2.f);
			DrawDebugPoint(GetWorld(), FireHit.ImpactPoint, 30, FColor::Red, false, 2.f);

			//Comprobamos que no es nulas las particulas
			if (ImpactParticles)
			{
				//Ejecutamos las particulas
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticles, FireHit.ImpactPoint);
			}
		}*/
	}
}
