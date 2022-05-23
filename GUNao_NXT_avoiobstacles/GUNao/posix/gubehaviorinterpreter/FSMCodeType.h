#ifndef _FSM_CODE_TYPE_H_
#define _FSM_CODE_TYPE_H_

class fsmMachine;

/*!
 * This is the type of known functions. It is not even a class. This enable us to call
 * one known C++ function for each stage : on Entry, on Exit, or Internal.
 *
 * @param fsmMachine* a pointer to the finite state machine
 * @param void*       a programmer-defined pointer to a (machine specific) context
 * @param const char* an (optional) parameter defined in the state digram (A-file), can be a string (const char *) or a long (need to be cast to long or unsigned long) as per the language definition
*/
typedef bool (*fsm_callback_f)(fsmMachine *, void *, const char *);

#endif // _FSM_CODE_TYPE_H_
