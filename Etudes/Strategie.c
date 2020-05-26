int Choose_Best_Action()
{
	int best_action_index;
	int best_action_value = 0;
	
	
	for(int i = 0; i < Actions.nomber;i++)
	{
			if(Actions[i].States == Terminee || Actions[i].States == Impossible)
				continue;
			
			if(Actions[i].Time_to_Start < Robot.TimeMatch && Actions[i].Maximum_Time_to_Execute > Robot.TimeMatch)
			{
				//This action is faisable
				Actions[i].Cost = Astar_Compute_Cost(Robot.Position, Actions[i].Start_Position);
				
				if(Actions[i].Cost < 0)
				{
					//Action not reachable
					continue;
				}
				
				if(is_Opponent_Present_in_area(Actions[i].Start_Position.x - 200, Actions[i].Start_Position.y - 200, Actions[i].Start_Position.x + 200, Actions[i].Start_Position.y + 200))
					continue;
				
				int temp_value;
				temp_value = Actions[i].Cost * Kdistance;
				temp_value += Actions[i].Nombre_Points * Kpts;
				temp_value += Actions[i].Piority * Kprio;
				....
				
				if(temp_value > best_action_value)
				{
					//we've found a new best action to do
					best_action_value = temp_value;
					best_action_index = i;
				}
			}
	}
	
	//Now we know the best action to execute
	return best_action_index;	
}




bool Action_Bleu_Gobelets(struct Actions* action)
{
	switch(action->Step)
	{
		case 0:
			if(!Deplacement_possible(Debut))
			{
				action->Time_to_Start = Robot.TimeMatch + 5000;
				action->State = En_Attente;
				return false;
			}
			Go_debut();
			Ramasse_Gobelet1();
			action->Step++;
			
		case 1:
			if(!Deplacement_possible(Gobelet2))
			{
				action->Time_to_Start = Robot.TimeMatch + 5000;
				action->State = En_Attente;
				return false;
			}
			Go_Gobelet2();
			Ramasse_Gobelet2();
			action->Step++;			
			
		case 2:
			if(!Deplacement_possible(zone_de_depart))
			{
				action->Time_to_Start = Robot.TimeMatch + 5000;
				action->State = En_Attente;
				return false;
			}
			
			Go_Zone_depart();
			Depose_Gobelets();
			action->State = Terminee;
			return true;
	}
}