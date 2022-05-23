#include "search.hxx"

void Search::getvalues(Blocks_World ourworld){
	m_initial_state = &ourworld.initial_state(); //std::cout << m_initial_state->to_string() << std::endl;
	m_final_state = &ourworld.final_state(); //std::cout << m_final_state->to_string() << std::endl;
	actions = ourworld.actions(); //for(unsigned i=0;i < actions.size();i++) std::cout << actions[i]->to_string() << std::endl;
	p_queue.push(new Node(&ourworld.initial_state())); //std::cout << p_queue.top()->s->to_string() << std::endl;
	closedstates.push_back(&ourworld.initial_state()); //std::cout << closedstates[0]->to_string() << std::endl;
	
}

unsigned Search::calcH(State &s1,State &s2) { //calcular H entre s1 y s2 (bloques diferentes)
	unsigned nmovs=0;
	for ( unsigned k = 1; k < s1.blocks().size(); k++ ){
		if( *s1.blocks()[k] != *s2.blocks()[k] )
		  nmovs++;
	}
  return nmovs;
}


unsigned Search::recorrer(Node *enew){

	enew->s = p_queue.top()->s; 
	enew->setH(p_queue.top()->H);
	enew->setG(p_queue.top()->G);
	//std::cout << "-----ESTADO ACTUAL-----"<< std::endl << enew->s->to_string() << std::endl;
		
		//si el nodo esta closed, no haremos lo de dentro de este if
		if(std::find(closedstates.begin(), closedstates.end(), enew->s) != closedstates.end()){ 
			//guardamos acciones aplicables
			std::vector<Action*> vectoraccionaplicables;
			for(unsigned i=0;i<actions.size();i++) 
				if(actions[i]->applicable(enew->s)==true)
					vectoraccionaplicables.push_back(actions[i]);
			//guardando estado visto
			closedstates.push_back(enew->s);
		
			//expandir hijos
			for(unsigned j=0; j < vectoraccionaplicables.size();j++){
				Node* subestat = new Node(vectoraccionaplicables[j]->execute(enew->s));
				subestat->setH(calcH(*subestat->s,*m_final_state));
				subestat->setG(enew->G+1);
				p_queue.push(subestat);
			}
		}
				
		//comprovacion
		if(enew->s == m_final_state){
			if (calcH(*enew->s,*m_final_state)<50){
				std::cout << "Admisible";
			}else{
				std::cout << "NO admisible, no optimo";
			}
			return enew->G;
		 }else{
			p_queue.pop(); //removemos el nodo que ya hemos trabajado de la cola
			return recorrer(enew);
		 }	 
}

	
void Search::solve(Blocks_World ourworld){
	std::cout << "-----------------  A*  -----------------" << std::endl;
	getvalues(ourworld);
	std::ofstream outfile("solution.dat");
	unsigned valor = recorrer(new Node(&ourworld.initial_state()));
	outfile << valor << std::endl;
	outfile.close(); 

}


