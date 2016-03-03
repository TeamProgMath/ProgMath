#include "StdAfxRegArchLib.h"

/*!
	\file cAbstCondVar.cpp
	\brief sources for abstract class cAbstCondVar methods.

	\author Jean-Baptiste DURAND, Ollivier TARAMASCO 
	\date dec-18-2006 - Last change feb-18-2011
*/

namespace RegArchLib {

	/*!
	 * \fn cAbstCondVar::cAbstCondVar(eCondVarEnum theType)
	 * \param eCondVarEnum theType: Conditional variance type code. Default eNotKnown.
	 */
	cAbstCondVar::cAbstCondVar(eCondVarEnum theType)
	{	mvCondVar = theType ;
	}

	/*!
	 * \fn cAbstCondVar::~cAbstCondVar()
	 * \details Nothing to do here
	 */
	cAbstCondVar::~cAbstCondVar()
	{
	}

	/*!
	 * \fn inline eCondVarEnum cAbstCondVar::GetCondVarType(void) const
	 * \param void
	 * \brief return the real conditional variance type code.
	*/
	eCondVarEnum cAbstCondVar::GetCondVarType(void) const
	{	return mvCondVar ;
	}

	/*!
	 * \fn ostream& operator <<(ostream& theOut, const cAbstCondVar& theAbstCondVar)
	 * \param ostream& theOut: output stream (file or screen). Default cout.
	 * \param const cAbstCondVar& theAbstCondVar: the variance model.
	 * \details Uses cAbstCondVar::Print
	 */
	ostream& operator <<(ostream& theOut, const cAbstCondVar& theAbstCondVar)
	{	theAbstCondVar.Print(theOut) ;
		return theOut ;
	}

}//namespace
