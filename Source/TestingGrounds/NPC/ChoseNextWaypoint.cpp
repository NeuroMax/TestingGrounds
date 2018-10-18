// Fill out your copyright notice in the Description page of Project Settings.

#include "ChoseNextWaypoint.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "PatrolRoute.h"

EBTNodeResult::Type UChoseNextWaypoint::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory) {
	
	// Get the patrol points
	auto PatrolRoute = OwnerComp.GetAIOwner()
		->GetPawn()
		->FindComponentByClass<UPatrolRoute>();

	if (!ensure(PatrolRoute)) return EBTNodeResult::Failed;
	auto PatrolPoints = PatrolRoute->GetPatrolPoints();

	if (PatrolPoints.Num() == 0) {

		UE_LOG(LogTemp, Warning, TEXT("A guard is missing patrol points!"));
		
		return EBTNodeResult::Failed;
	}

	// set next waypoint
	auto BlackboardComp = OwnerComp.GetBlackboardComponent();
	auto Index = BlackboardComp->GetValueAsInt(IndexKey.SelectedKeyName);
	BlackboardComp->SetValueAsObject(WaypointKey.SelectedKeyName, PatrolPoints[Index]);

	auto NextIndex = (Index + 1) % PatrolPoints.Num();
	BlackboardComp->SetValueAsInt(IndexKey.SelectedKeyName, NextIndex);

	return EBTNodeResult::Succeeded;
}
