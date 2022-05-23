#include "blocks_world.hxx"
#include "string_utils.hxx"
#include <assert.h>
#include <map>
#include <fstream>
#include <algorithm>

Action::Action( unsigned id, std::string name )
  : m_identifier( id ), m_name( name )
{}

PickUp::PickUp(  unsigned blk_id, unsigned id, std::string name )
  : Action( id, name ), m_blk_id( blk_id )
{
}

PickUp::~PickUp()
{
}

PutDown::PutDown(  unsigned blk_id, unsigned id, std::string name )
  : Action( id, name ), m_blk_id( blk_id )
{
}

PutDown::~PutDown()
{
}


Stack::Stack( unsigned blk_id, unsigned under_blk_id, unsigned id, std::string name )
  : Action( id, name ), m_blk_id( blk_id ), m_under_blk_id( under_blk_id )
{
}

Stack::~Stack()
{
}

Unstack::Unstack( unsigned blk_id, unsigned under_blk_id, unsigned id, std::string name )
  : Action( id, name ), m_blk_id( blk_id ), m_under_blk_id( under_blk_id )
{
}

Unstack::~Unstack()
{
}



Block::Block( unsigned on, bool clear )
  : m_on( on ),  m_clear( clear )
{
}

Block::Block( Block* b )
{
  m_on = b->on();
  m_clear =  b->clear();
}

Block::~Block()
{
}

State::State()
  :  m_hand( HANDEMPTY )
{}

State::State( State* s )
{
  m_blocks.push_back( 0 );
  for ( unsigned k = 1; k < s->blocks().size(); k++ )
    m_blocks.push_back( new Block( s->blocks()[k] ) );
  m_hand = s->hand();
}

State::~State()
{
  for ( unsigned k = 1; k < blocks().size(); k++ )
    delete m_blocks[k];
}

std::string State::to_string()
{
  std::stringstream ss;
  
  if( handempty() )
    ss << "Hand is empty" << std::endl;
  else
    ss << "Hand is holding block " << hand() << std::endl;
  for ( unsigned k = 1; k < blocks().size(); k++ )
    {
      ss << "Block " << k;;
      if( blocks()[k]->on_table() )
	ss << " on table";
      else if( blocks()[k]->on() != NON_POS )
	ss << " on block " << blocks()[k]->on() << " and";
      ss << " is " << (blocks()[k]->clear() ? "clear" : "not clear") << std::endl;
    }
  
  return ss.str();
}

Blocks_World::Blocks_World()
  :m_num_blocks(0)
{
}

Blocks_World::~Blocks_World()
{
  for ( unsigned k = 0; k < m_actions.size(); k++ )
    delete m_actions[k];

}

bool	Blocks_World::load_from( std::string filename )
{
  std::ifstream input( filename.c_str() );

  if ( input.fail() )
    {
      std::cerr << "Could not open " << filename << "!!" << std::endl;
      std::exit(1);
    }

  char 				line_buffer[256000];
  std::string 			line;
  unsigned                        line_number = 0;
  unsigned                        read_total = 0;
  unsigned                        read_no_more = 0;
  int                             in_state = -1;
  State*             states[2];
  states[0]  = &m_initial_state;
  states[1]  = &m_final_state;
  m_num_blocks = -1;
	
  while ( !input.eof() )
    {
      input.getline( line_buffer, 255999, '\n' );
      line_number++;
      line.assign( line_buffer );
      if (line.empty()) continue;
	    
      if( read_no_more >= 2 )
	{
	  std::cout << "File read" << std::endl;
	  continue;
	}
      else if( read_total == read_no_more )
	{
	  std::string total_str = "total: ";
	  size_t pos = line.find( total_str );
	  if( pos == std::string::npos )
	    {
	      std::cerr << "Error: in line " << line_number << std::endl;
	      std::exit(1);
	    }
	  if( read_total == 0 )
	    {
	      std::string b_str = line.substr( pos+total_str.length() );
	      from_string( m_num_blocks, b_str, std::dec );
	    }
	  if(0)
	    std::cout << "Total blocks" << m_num_blocks << std::endl;
	  read_total++;
	  in_state++;
	  states[in_state]->blocks().push_back(0); // No block in 0th position.
	  states[in_state]->blocks().resize(m_num_blocks+1);
	}
      else if( read_total == read_no_more + 1 ) // In a state
	{
	  std::string more_str =  "more";
	  size_t pos = line.find( more_str );
	  if( pos != std::string::npos )
	    {
	      read_no_more++;
	      if(0)
		std::cout << "no more blocks" << std::endl;
	    }
	  else
	    {
	      std::string block_str = "block ";
	      std::string rest = line.substr( line.find( block_str ) + block_str.length() );
	      unsigned on1 = NON_POS;
	      from_string( on1, rest, std::dec );
	      unsigned on2 = NON_POS;
	      if( line.find("table") != std::string::npos )
		on2 = 0; // table
	      else
		{
		  std::string top_str = "top of block ";
		  std::string rest2 = line.substr( line.find( top_str ) 
						   + top_str.length() );
		  from_string( on2, rest2, std::dec );
		}
	      assert( on1 > 0 && on2 >= 0 );
	      if(0)
		std::cout << "on(" << on1 << "," << on2 << ")" << std::endl;
	      states[in_state]->blocks()[on1] = ( new Block(on2,true) );
	    }
	}
    }
  input.close();
	
  for( unsigned i = 0; i < 2; i++ )
    {
      assert( states[i]->blocks().size() == m_num_blocks + 1 );
      for( unsigned j = 0; j < states[in_state]->blocks().size(); j++ )
	if( states[i]->blocks()[j] != 0 &&
	    !(states[i]->blocks()[j]->on_table()) )
	  states[i]->blocks()
	    [ states[i]->blocks()[j]->on() ]
	    ->set_clear(false);
    }
  // Make actions
  unsigned id_action = 0;
  for ( unsigned id_blk = 1; id_blk < m_num_blocks+1; id_blk++ )
    {
      {
	std::stringstream ss;
	ss <<  "PickUp(" << id_blk <<")";
	m_actions.push_back( new PickUp( id_blk, id_action++, ss.str() ) );
      }
      {
	std::stringstream ss;
	ss <<  "PutDown(" << id_blk <<")";
	m_actions.push_back( new PutDown( id_blk, id_action++, ss.str() ) );
      }
      for ( unsigned id_blk_2 = 1; id_blk_2 < m_num_blocks+1; id_blk_2++ )
	if( id_blk != id_blk_2 )
	  {
	    {
	      std::stringstream ss;
	      ss <<  "Stack(" << id_blk << ", " << id_blk_2 <<")";
	      m_actions.push_back( new Stack( id_blk, id_blk_2, id_action++, ss.str() ) );
	    }
	    {
	      std::stringstream ss;
	      ss <<  "Unstack(" << id_blk << ", " << id_blk_2 <<")";
	      m_actions.push_back( new Unstack( id_blk, id_blk_2, id_action++, ss.str() ) );
	    }
		  
	  }
    }
  return true;
	
}

