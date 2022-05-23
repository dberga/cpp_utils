#ifndef __BLOCKS_WORLD__
#define __BLOCKS_WORLD__

#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>

#define HANDEMPTY 0
#define TABLE 0
#define NON_POS 999999


/**
 * State Variable: Block
 */
class Block
{
public:
  Block( unsigned on, bool clear );
  Block( Block* c);
  ~Block();

  unsigned on() 	 const { return m_on; }
  bool on_table()        const { return m_on == TABLE; }
  bool clear()             const { return m_clear; }

  void set_on( unsigned new_blk ) 	{ m_on = new_blk; }
  void set_on_table()         {  m_on = TABLE; }
  void set_clear( bool is_clear )             { m_clear = is_clear ; }

  bool operator==(Block &b);
  bool operator!=(Block &b);
  
protected:
  unsigned	m_on;
  bool m_clear;
};

inline bool Block::operator==(Block &b) {
  return (b.on() == on()) &&
    (b.clear() == clear());
}

inline bool Block::operator!=(Block &b) {
  return ( *this == b ) ? false : true;
}
/**
 * Contains all the variables that represent a state
 */

class State
{
public:
  State();
  State( State* s );
  ~State();
  
  std::vector<Block*>&     blocks()   { return m_blocks; }
  unsigned&		hand() { return m_hand; }
  bool		handempty() const { return m_hand == HANDEMPTY; }

  void set_handempty() {  m_hand = HANDEMPTY; }
  
  std::string             to_string();
  
  bool operator==(State &s);
  bool operator!=(State &s);
  
protected:
  std::vector<Block*>      m_blocks;
  unsigned m_hand;
};

inline bool State::operator==(State &s) {
  if( s.blocks().size() != blocks().size() )
    return false;
  for ( unsigned k = 1; k < blocks().size(); k++ )
    if( *s.blocks()[k] != *blocks()[k] )
      return false;
  if( s.hand() != hand() )
    return false;
  return true;
}

inline bool State::operator!=(State &s) {
  return ( *this == s ) ? false : true;
}
/**
 * Abstract Action Definition
 */
class Action
{
public:
  Action( unsigned id, std::string name );
  virtual ~Action(){}
  
  virtual bool           applicable( State* s ) const = 0;
  virtual State*         execute( State* s ) = 0;
  std::string            to_string() const { return m_name; }
  
  unsigned       identifier() const { return m_identifier; }
protected:
  unsigned    m_identifier;
  std::string m_name;
};

/**
 * Pick up block blk_id from table
 */
class PickUp : public Action
{
public:
  PickUp( unsigned blk_id, unsigned id, std::string name );
  ~PickUp();
  
  bool applicable( State* s ) const;
  State* execute( State* s );
  
protected:

  unsigned m_blk_id;

};

/**
 * (clear ?ob) (on-table ?ob) (arm-empty)
 */
inline bool	PickUp::applicable( State* s ) const
{
  return s->blocks()[m_blk_id]->clear()
    && s->blocks()[m_blk_id]->on_table()
    && s->handempty();
}

/*
 * (holding ?ob) (not (clear ?ob)) (not (on-table ?ob)) (not (arm-empty))
 */
inline State*  PickUp::execute( State* s )
{
  State* sp = new State( s );

  sp->hand() = m_blk_id;
  sp->blocks()[m_blk_id]->set_on(NON_POS);
  sp->blocks()[m_blk_id]->set_clear(false);

  return sp;
}

/**
 * Put down block blk_id on table
 */
class PutDown : public Action
{
public:
  PutDown( unsigned blk_id, unsigned id, std::string name );
  ~PutDown();
  
  bool applicable( State* s ) const;
  State* execute( State* s );
  
protected:

  unsigned m_blk_id;

};

/**
 * (holding ?ob)
 */
inline bool	PutDown::applicable( State* s ) const
{
  return s->hand() == m_blk_id;
}

/*
 * (clear ?ob) (arm-empty) (on-table ?ob) (not (holding ?ob)) 
 */
inline State*  PutDown::execute( State* s )
{
  State* sp = new State( s );

  sp->blocks()[m_blk_id]->set_clear(true);
  sp->set_handempty();
  sp->blocks()[m_blk_id]->set_on_table();

  return sp;
}

/**
 * Stack blk_id on under_blk_id
 */
class Stack : public Action
{
public:
  Stack( unsigned blk_id, unsigned under_blk_id, unsigned id, std::string name );
  ~Stack();
  
  bool applicable( State* s ) const;
  State* execute( State* s );
  
protected:

  unsigned m_blk_id;
  unsigned m_under_blk_id;

};

/**
 * (clear ?underob) (holding ?ob)
 */
inline bool	Stack::applicable( State* s ) const
{
  return (s->hand() == m_blk_id) 
    && (s->blocks()[m_under_blk_id]->clear());
}

/*
 * (arm-empty) (clear ?ob) (on ?ob ?underob) (not (clear ?underob)) (not (holding ?ob))
 */
inline State*  Stack::execute( State* s )
{
  State* sp = new State( s );

  sp->set_handempty();
  sp->blocks()[m_blk_id]->set_clear(true);
  sp->blocks()[m_blk_id]->set_on( m_under_blk_id );
  sp->blocks()[m_under_blk_id]->set_clear(false);

  return sp;
}


/**
 * Unstack blk_id from under_blk_id
 */
class Unstack : public Action
{
public:
  Unstack( unsigned blk_id, unsigned under_blk_id, unsigned id, std::string name );
  ~Unstack();
  
  bool applicable( State* s ) const;
  State* execute( State* s );
  
protected:

  unsigned m_blk_id;
  unsigned m_under_blk_id;

};

/**
 * (on ?ob ?underob) (clear ?ob) (arm-empty)
 */
inline bool	Unstack::applicable( State* s ) const
{
  return (s->blocks()[m_blk_id]->on() == m_under_blk_id)
    && 	s->blocks()[m_blk_id]->clear()
    && 	s->handempty();
}

/*
 * (holding ?ob) (clear ?underob) (not (on ?ob ?underob)) (not (clear ?ob)) (not (arm-empty))
 */
inline State*  Unstack::execute( State* s )
{
  State* sp = new State( s );

  sp->hand() = m_blk_id;
  sp->blocks()[m_under_blk_id]->set_clear(true);
  sp->blocks()[m_blk_id]->set_clear(false);
  sp->blocks()[m_blk_id]->set_on(NON_POS);

  return sp;
}


/**
 * The World
 */

class Blocks_World
{
public:

  Blocks_World();
  ~Blocks_World();

  bool			load_from( std::string filename );
  std::vector<Action*>&	actions() { return m_actions; }
  State&                  initial_state() { return m_initial_state; }
  State&                  final_state() { return m_final_state; }

protected:

  State                   m_initial_state;
  State                   m_final_state;

  std::vector<Action*>	m_actions;

  unsigned                m_num_blocks;
};

#endif // blocks_world.hxx
