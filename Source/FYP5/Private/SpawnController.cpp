// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnController.h"


#include "BlueSpawns.h"
#include "RedSpawns.h"
#include "Bot.h"
#include "Kismet/GameplayStatics.h"
#include "Math/Vector.h"

ASpawnController::ASpawnController()
{
 	//Primary Setup on Controller
	PrimaryActorTick.bCanEverTick = true;
	TeamSize = 5;
	BotListBlue.SetNum(TeamSize);
	BotListRed.SetNum(TeamSize);

}

void ASpawnController::BeginPlay()
{
	Super::BeginPlay();
	RoundNumber = 0;
	SpawnAI();

}

void ASpawnController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
void ASpawnController::SpawnAI()
{
	//Spawns Bots and Keeps them in List of Each Team
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABlueSpawns::StaticClass(), SpawnPointBlue);
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ARedSpawns::StaticClass(), SpawnPointRed);

	for ( int i = 0; i < TeamSize; i++ )
	{
		OffSet.X = i * 200;
		
		BotListBlue[i] = GetWorld()->SpawnActor<ABot>(AIToSpawn, SpawnPointBlue[0]->GetActorLocation() + OffSet , SpawnPointBlue[0]->GetActorRotation());
		if(BotListBlue[i] != nullptr)
		{
			BotListBlue[i]->SetBlueTeam();
		}
	}

	
	for ( int i = 0; i < TeamSize; i++ )
	{
		OffSet.X = i * 200;
		BotListRed[i] = GetWorld()->SpawnActor<ABot>(AIToSpawn, SpawnPointRed[0]->GetActorLocation() , SpawnPointRed[0]->GetActorRotation());
		if(BotListRed[i] != nullptr)
		{
			BotListRed[i]->SetRedTeam();
		}
	}
	
	
}

void ASpawnController::CheckEnemysLeft(bool isBlue)
{
	
	//Check run after Death to see if round is over
	if(isBlue == true)
	{
		DeadCountBlue = DeadCountBlue + 1;
		if(DeadCountBlue == TeamSize)
		{
			Score.X = Score.X + 1;
			ResetRound();
		}
	}
	else
	{
		DeadCountRed = DeadCountRed + 1;
		if(DeadCountRed == TeamSize)
		{
			Score.Y = Score.Y + 1;
			ResetRound();
		}
	}
}

void ASpawnController::ResetRound()
{
	//Reset Of Round Stats
	UE_LOG(LogTemp, Warning, TEXT("Round Reset"));
	DeadCountBlue = 0;
	DeadCountRed = 0;
	RoundNumber = RoundNumber + 1;
	if(RoundNumber == 6)
	{
		
		SpawnPointBlue[0]->SetActorLocation(FVector(1330,-890,50));
		SpawnPointRed[0]->SetActorLocation(FVector(1500,3180,50));
	}
	if(Score.X == 7)
	{
		BlueTeamWin = true;
	}
	if(Score.Y == 7)
	{
		RedTeamWin = true;
	}
	if(RedTeamWin == true || BlueTeamWin == true)
	{
		EndGame();
	}
	else
	{
		//Respawn the bot after all 5 die
		for ( int i = 0; i < TeamSize; i++ )
		{
			OffSet.X = i * 200;
			BotListBlue[i]->RespawnBlueBot(SpawnPointBlue[0]->GetActorLocation()+ OffSet);
		}
		for ( int j = 0; j < TeamSize; j++ )
		{
			OffSet.X = j * 200;
			BotListRed[j]->RespawnRedBot(SpawnPointRed[0]->GetActorLocation() + OffSet);
		}
	}

}

void ASpawnController::EndGame()
{
	OnRoundEnd.Broadcast();
	UE_LOG(LogTemp, Warning, TEXT("SpawnerCalled End Game"));
}
	

