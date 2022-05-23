/*************************************************************************
//	Class of Activities in the States of a finite state machine
//	(c) 2010 Mi-PAL Laboratory, Griffith University
//	Author:  Vladimir Estivill-Castro and Rene Hexel
//	-----------------------------------------------
//	This code is released under GPL v3.0
//	You should have received a copy of the GNU General
//	Public License along with this program. If not, see
//	<http://www.gnu.org/license/> 
*************************************************************************/

#include <string>
#include <iostream>
#include <gu_util.h>

/*!
 *      Class to model the logic expression that label transitions of a FSM.
*/

class fsmExpression
{
	std::string toEvaluate;

public:
	fsmExpression (std::string theThingToEvaluate) { toEvaluate=theThingToEvaluate;}
/*!
 * Getter for the logic expression
*/
	std::string getWhatToEvaluate() {return toEvaluate;}

/*!
 * Return the DPL HASKELL atom for Expression
*/
	std::string atom4Expression() 
	{
		std::string expr(toEvaluate);

	  if (expr.length()>0)
	  {
		if ('~'==expr[0])
		{
			expr.erase(0,1);
		}
		for (int i=0; i< expr.length(); i++)
			if (!isalnum(expr[i]))
			{
				expr.replace(i, 1,1, '_');
			}
	   }
		DBG(std::cerr << expr << "\n");
		return expr;
 	}
	
/*!
 * Return if Expression starts with '~'. Only one tilde is possible in an expression.
 * The code does not handle things like double negation, ie . '~~'.
*/
	bool isNegation() {
		DBG(std::cerr << toEvaluate[0] << "\n");
	return '~'==toEvaluate[0]; }   

};
