

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <time.h> 
#include <stdlib.h>

using namespace std;

int main (int argc, char* argv[]) {

char* parameter;
const char* domainouput = "config-pddl/domain.pddl";
const char* problemoutput = "config-pddl/problem.pddl";

if(argc!=2) {cout << "no config parameter found"<<endl << "usage: ./app config-pddl/problem.pddl.txt" << endl; return 0;}
parameter = argv[1];


cout << "\n\n---------------CREATING DOMAIN---------------n\n";

  //creando domain
  ofstream domainfile;
  domainfile.open (domainouput); 
  domainfile << "(define (domain dice)\n";

  domainfile << "(:requirements :strips)\n";
  domainfile << "(:predicates\n (adjUL ?x ?y)\n (adjUR ?x ?y)\n (adjLL ?x ?y)\n (adjLR ?x ?y)\n (adjD ?x ?y)\n (actual ?x))\n\n";

  domainfile << "(:action spinUL\n";
  domainfile << ":parameters (?current ?next)\n";
  domainfile << ":precondition (and (adjUL ?current ?next) (actual ?current))\n";
  domainfile << ":effect (and (actual ?next) (not (actual ?current)))\n";
  domainfile << ")\n";

  domainfile << "(:action spinUR\n";
  domainfile << ":parameters (?current ?next)\n";
  domainfile << ":precondition (and (adjUR ?current ?next) (actual ?current))\n";
  domainfile << ":effect (and (actual ?next) (not (actual ?current)))\n";
  domainfile << ")\n";

  domainfile << "(:action spinLL\n";
  domainfile << ":parameters (?current ?next)\n";
  domainfile << ":precondition (and (adjLL ?current ?next) (actual ?current))\n";
  domainfile << ":effect (and (actual ?next) (not (actual ?current)))\n";
  domainfile << ")\n";
  
  domainfile << "(:action spinLR\n";
  domainfile << ":parameters (?current ?next)\n";
  domainfile << ":precondition (and (adjLR ?current ?next) (actual ?current))\n";
  domainfile << ":effect (and (actual ?next) (not (actual ?current)))\n";
  domainfile << ")\n";
  
  domainfile << "(:action spinD\n";
  domainfile << ":parameters (?current ?next)\n";
  domainfile << ":precondition (and (adjD ?current ?next) (actual ?current))\n";
  domainfile << ":effect (and (actual ?next) (not (actual ?current)))\n";
  domainfile << ")\n";

  domainfile << ")\n";
             
  domainfile.close();


cout << "Done!\n";


cout << "\n\n---------------GENERATING PROBLEM with config data---------------n\n";

  //loading config
  std::vector<std::string> gottennums;
  string line;
  string token;
  

  ifstream configfile;
  configfile.open (parameter); if(!configfile){ cout<< "couldn't find the config file"; return 0;}
  

   

	 for(int x=0; x<12 ; ){ 

		    getline(configfile,line);
		    stringstream ss(line);
		    ss >> token;
		    if(token=="cara"){
			    ss >> token;
			    ss >> token;
			    gottennums.push_back(token);
			    x++;
		    }
	            
	 }

	/*for(int x=0; x<12; x++){
		cout << gottennums[x];
		cout << endl;
	}*/


  
  configfile.close ();



  //crear problem
  srand (time(NULL));
  int randinitvalue = rand()%12;
  int randfinalvalue = rand()%12; 
 	while(randfinalvalue == randinitvalue) randfinalvalue = rand()%12;

  ofstream problemfile;
  problemfile.open (problemoutput);

  problemfile << "(define (problem dice12)\n";

	problemfile << "(:domain dice)\n";
	problemfile << "(:objects\n";
	problemfile << "    ";

	for(int x=0; x<12; x++)
	problemfile << gottennums[x] <<" ";
	problemfile << "\n";
	problemfile << ")\n";

	problemfile << "(:init\n";
	problemfile << "(actual "<< gottennums[randinitvalue] <<")\n";
	problemfile << "(adjUL "<<gottennums[0]<<" "<<gottennums[8]<<")\n";
	problemfile << "(adjUR "<<gottennums[0]<<" "<<gottennums[6]<<")\n";
	problemfile << "(adjLL "<<gottennums[0]<<" "<<gottennums[2]<<")\n";
	problemfile << "(adjLR "<<gottennums[0]<<" "<<gottennums[10]<<")\n";
	problemfile << "(adjD "<<gottennums[0]<<" "<<gottennums[4]<<")\n";

	problemfile << "(adjUL "<<gottennums[1]<<" "<<gottennums[5]<<")\n";
	problemfile << "(adjUR "<<gottennums[1]<<" "<<gottennums[11]<<")\n";
	problemfile << "(adjLL "<<gottennums[1]<<" "<<gottennums[2]<<")\n";
	problemfile << "(adjLR "<<gottennums[1]<<" "<<gottennums[7]<<")\n";
	problemfile << "(adjD "<<gottennums[1]<<" "<<gottennums[8]<<")\n";

	problemfile << "(adjUL "<<gottennums[2]<<" "<<gottennums[8]<<")\n";
	problemfile << "(adjUR "<<gottennums[2]<<" "<<gottennums[0]<<")\n";
	problemfile << "(adjLL "<<gottennums[2]<<" "<<gottennums[1]<<")\n";
	problemfile << "(adjLR "<<gottennums[2]<<" "<<gottennums[4]<<")\n";
	problemfile << "(adjD "<<gottennums[2]<<" "<<gottennums[5]<<")\n";

	problemfile << "(adjUL "<<gottennums[3]<<" "<<gottennums[5]<<")\n";
	problemfile << "(adjUR "<<gottennums[3]<<" "<<gottennums[4]<<")\n";
	problemfile << "(adjLL "<<gottennums[3]<<" "<<gottennums[11]<<")\n";
	problemfile << "(adjLR "<<gottennums[3]<<" "<<gottennums[10]<<")\n";
	problemfile << "(adjD "<<gottennums[3]<<" "<<gottennums[9]<<")\n";

	problemfile << "(adjUL "<<gottennums[4]<<" "<<gottennums[10]<<")\n";
	problemfile << "(adjUR "<<gottennums[4]<<" "<<gottennums[3]<<")\n";
	problemfile << "(adjLL "<<gottennums[4]<<" "<<gottennums[0]<<")\n";
	problemfile << "(adjLR "<<gottennums[4]<<" "<<gottennums[5]<<")\n";
	problemfile << "(adjD "<<gottennums[4]<<" "<<gottennums[2]<<")\n";

	problemfile << "(adjUL "<<gottennums[5]<<" "<<gottennums[2]<<")\n";
	problemfile << "(adjUR "<<gottennums[5]<<" "<<gottennums[4]<<")\n";
	problemfile << "(adjLL "<<gottennums[5]<<" "<<gottennums[1]<<")\n";
	problemfile << "(adjLR "<<gottennums[5]<<" "<<gottennums[3]<<")\n";
	problemfile << "(adjD "<<gottennums[5]<<" "<<gottennums[11]<<")\n";
	
	problemfile << "(adjUL "<<gottennums[6]<<" "<<gottennums[7]<<")\n";
	problemfile << "(adjUR "<<gottennums[6]<<" "<<gottennums[9]<<")\n";
	problemfile << "(adjLL "<<gottennums[6]<<" "<<gottennums[8]<<")\n";
	problemfile << "(adjLR "<<gottennums[6]<<" "<<gottennums[10]<<")\n";
	problemfile << "(adjD "<<gottennums[6]<<" "<<gottennums[0]<<")\n";

	problemfile << "(adjUL "<<gottennums[7]<<" "<<gottennums[11]<<")\n";
	problemfile << "(adjUR "<<gottennums[7]<<" "<<gottennums[9]<<")\n";
	problemfile << "(adjLL "<<gottennums[7]<<" "<<gottennums[1]<<")\n";
	problemfile << "(adjLR "<<gottennums[7]<<" "<<gottennums[6]<<")\n";
	problemfile << "(adjD "<<gottennums[7]<<" "<<gottennums[8]<<")\n";

	problemfile << "(adjUL "<<gottennums[8]<<" "<<gottennums[7]<<")\n";
	problemfile << "(adjUR "<<gottennums[8]<<" "<<gottennums[6]<<")\n";
	problemfile << "(adjLL "<<gottennums[8]<<" "<<gottennums[1]<<")\n";
	problemfile << "(adjLR "<<gottennums[8]<<" "<<gottennums[0]<<")\n";
	problemfile << "(adjD "<<gottennums[8]<<" "<<gottennums[2]<<")\n";

	problemfile << "(adjUL "<<gottennums[9]<<" "<<gottennums[11]<<")\n";
	problemfile << "(adjUR "<<gottennums[9]<<" "<<gottennums[3]<<")\n";
	problemfile << "(adjLL "<<gottennums[9]<<" "<<gottennums[7]<<")\n";
	problemfile << "(adjLR "<<gottennums[9]<<" "<<gottennums[10]<<")\n";
	problemfile << "(adjD "<<gottennums[9]<<" "<<gottennums[6]<<")\n";

	problemfile << "(adjUL "<<gottennums[10]<<" "<<gottennums[0]<<")\n";
	problemfile << "(adjUR "<<gottennums[10]<<" "<<gottennums[6]<<")\n";
	problemfile << "(adjLL "<<gottennums[10]<<" "<<gottennums[4]<<")\n";
	problemfile << "(adjLR "<<gottennums[10]<<" "<<gottennums[9]<<")\n";
	problemfile << "(adjD "<<gottennums[10]<<" "<<gottennums[3]<<")\n";

	problemfile << "(adjUL "<<gottennums[11]<<" "<<gottennums[5]<<")\n";
	problemfile << "(adjUR "<<gottennums[11]<<" "<<gottennums[3]<<")\n";
	problemfile << "(adjLL "<<gottennums[11]<<" "<<gottennums[1]<<")\n";
	problemfile << "(adjLR "<<gottennums[11]<<" "<<gottennums[9]<<")\n";
	problemfile << "(adjD "<<gottennums[11]<<" "<<gottennums[7]<<")\n";

	problemfile << ")\n";
	problemfile << "(:goal\n";
	problemfile << "(and\n";
	problemfile << "(actual "<<gottennums[randfinalvalue]<<")\n";
	problemfile << ")\n";
	problemfile << ")\n";
	problemfile << ")\n";



  problemfile.close();

cout << "Done!\n";



cout << "\n\n---------------RUNNING PROBE PLAN---------------n\n";


  system("./probe-files/probe -d config-pddl/domain.pddl -i config-pddl/problem.pddl -o probe-files/result.txt");

cout << "Done!\n";

cout << "\n\n---------------RESULTS---------------n\n";


ifstream resultfile;
resultfile.open("probe-files/result.txt.1");

while(!resultfile.eof()){
	
	getline(resultfile,line);
	cout << line;
	cout << endl;
}
  

resultfile.close();

cout << "\nDone!\n";
  return 0;
}



