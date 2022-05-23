#include <iostream>
#include <fstream>


// programa que adapta el formato

int main(int argc, char** argv){

	char* fileName = argv[1];
	char* newFileName = argv[2];

	std::ofstream out(newFileName);
	    
	std::ifstream f(fileName);

    int nVertices;
    int nPoligonos;

    f >> nVertices;
    
    out << nVertices << " ";


    f >> nPoligonos;
    
    out << nPoligonos << std::endl;
	
    float coord;
    for (int i = 0; i < nVertices; i++){
        f >> coord;
	out << coord << " ";
        f >> coord;
	out << coord << " ";
        f >> coord;
	out << coord << " ";
	out << std::endl;

    }
    for (int j = 0; j < nPoligonos; j++){
    
        int numVertices;
        f >> numVertices;
        
	out << numVertices;
        for(int k = 0; k < numVertices; ++k){
            int idVertice;
            f >> idVertice;
		out << " " << idVertice - 1; 
        }
	out << std::endl;

        
    }
}
