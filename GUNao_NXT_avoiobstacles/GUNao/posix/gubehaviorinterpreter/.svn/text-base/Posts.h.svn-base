/*************************************************************************
//	Class of Array of Posts in the Activities of States of a finite 
//	state machine
//	(c) 2010 Mi-PAL Laboratory, Griffith University
//	Author: Vladimir Estivill-Castro
//	-----------------------------------------------
//	This code is released under GPL v3.0
//	You should have received a copy of the GNU General
//	Public License along with this program. If not, see
//	<http://www.gnu.org/license/> 
*************************************************************************/


#include <vector>

typedef onePost* p_onePost;

/*!
 *      Class to model a container of postings.
 *
*/

class PossiblePosts
{
	std::vector <p_onePost> postings;
	std::vector <p_onePost>::iterator internalIterator;
public:
	PossiblePosts();

/*!
 * Provides a way to navigate the container, this return the posting .
 * NULL is returned if the containers is empty (holds no postings).
 * POSTCONDITION: now theNextPosting can be called
 * INVARIANT: The container does not change
*/
	p_onePost theFirstPosting();



/*!
 * Provides a way to navigate and get postings of the container, this return the next posting out.
 * PRECONDITION: theFirstPosting() must have been called.
 * Return NULL when the postings out are exhausted.
 * INVARIANT: The container does not change
*/
	p_onePost theNextPosting();


/*!
 *	When constructing a FSM and a state in particular, and the activity of a state
 * we create containers for the On-Entry, On-Exit, and Internal activities (postings)
 * to the whiteboard. Perhaps this should not be public but accessed be friends only.
*/
	void addOnePost(p_onePost newOne);
};

