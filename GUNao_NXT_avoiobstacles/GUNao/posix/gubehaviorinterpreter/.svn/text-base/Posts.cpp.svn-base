/*************************************************************************
//	Class of Array of Posts in the Acivities of States of a finite 
//	state machine
//	(c) 2010 Mi-PAL Laboratory, Griffith University
//	Author: Vladimir Estivill-Castro
//	-----------------------------------------------
//	This code is released under GPL v3.0
//	You should have recieved a copy of the GNU General
//	Public License along with this program. If not, see
//	<http://www.gnu.org/license/> 
*************************************************************************/


#include "OnePost.h"
#include "Posts.h"

PossiblePosts:: PossiblePosts():postings()
	{ }

p_onePost PossiblePosts:: theFirstPosting()
	{
	   if (0 < postings.size()  ) 
		{ internalIterator=postings.begin();
	   	  return (*internalIterator);
		}
	   else
		return NULL;
	}


void PossiblePosts:: addOnePost(p_onePost newOne)
	 { 
	 	std::vector <p_onePost>:: iterator it;
	 	it=postings.begin();
	 	postings.insert(it,newOne);
	 }

p_onePost PossiblePosts :: theNextPosting()
	{ 
	   internalIterator++;
	   if (internalIterator < postings.end())
	   {
		   return (*internalIterator);
	   }
	   else
		return NULL;
	}

