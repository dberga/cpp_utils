#include <iostream>
#include <cstdlib>
#include <sstream>
#include "blocks_world.hxx"
//#include "search.hxx"

struct Options
{
  std::string prob_filename;
};

void show_help()
{
  std::cout << "Usage: blocks-solver <prob>" << std::endl;
  std::cout << "Examples:" << std::endl;

  std::cout << "             ./blocks-solver bw-10-38121" << std::endl;
  std::cout << "                 Solve  bw-10-38121" << std::endl;
}

bool process_command_line_args( int argc, char** argv, Options& opt  )
{
  if ( argc < 2 )
    {
      std::cerr << "Missing arguments!" << std::endl;
      show_help();
      return false;
    }

  opt.prob_filename = argv[1];
  // Test that file exists and can be opened
  std::stringstream command_buffer;
  command_buffer << "test -f " << opt.prob_filename;
  int res = system( command_buffer.str().c_str() );
  if ( res )
    {
      std::cout << "File " << opt.prob_filename << " does not exist!" << std::endl;
      std::exit(1);
    }
  return true;
}

int main( int argc, char** argv )
{
  Options prog_opts;
  if ( !process_command_line_args( argc, argv, prog_opts ) )
    std::exit(1);

  Blocks_World the_world;
  the_world.load_from( prog_opts.prob_filename );

  std::cout << "Number of actions: " << the_world.actions().size() << std::endl;

  std::cout << "Actions Available: " << std::endl;
  for( unsigned i = 0; i < the_world.actions().size(); i++)
    std::cout << "\t"<< the_world.actions()[ i ]->identifier() << " - " <<the_world.actions()[ i ]->to_string() << std::endl;

  std::cout << "Initial State: " << std::endl;
  std::cout << the_world.initial_state().to_string() << std::endl;

  std::cout << "Final State: " << std::endl;
  std::cout << the_world.final_state().to_string() << std::endl;

  State& init_state = the_world.initial_state();
  std::cout << "Applicable Actions in the initial state: " << std::endl;

  Action* last_applicable_action = NULL;
  for( unsigned i = 0; i < the_world.actions().size(); i++)
    if( the_world.actions()[ i ]->applicable( &init_state ) )
      {
	last_applicable_action = the_world.actions()[ i ];
	std::cout << "\t"<< the_world.actions()[ i ]->identifier() << " - " <<the_world.actions()[ i ]->to_string() << std::endl;
      }


  State* new_state = last_applicable_action->execute( &init_state );
  std::cout << "New state after applying " << last_applicable_action->to_string() << " to init_state: " << std::endl;

  std::cout << new_state->to_string() << std::endl;
	
  delete new_state;

  std::cout << "Starting search" << std::endl;

  //Search s;
  //s.solve(the_world);

  return 0;
}
