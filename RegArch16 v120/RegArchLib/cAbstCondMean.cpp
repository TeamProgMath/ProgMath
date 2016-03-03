#include "StdAfxRegArchLib.h"
/*!
	\file cAbstCondMean.cpp
	\brief Sources for abstract class cAbstCondMean methods.
	\author Jean-Baptiste DURAND, Ollivier TARAMASCO
	\date dec-18-2006 - Last change feb-18-2011
*/

namespace RegArchLib {

	/*!
	 * \fn cAbstCondMean::cAbstCondMean(const eCondMeanEnum theType)
	 * \param const eCondMeanEnum theType. Code of the conditional mean type. Default value: eUnknown
	 * \details Set the real type code of conditional mean component
	 */
	cAbstCondMean::cAbstCondMean(const eCondMeanEnum theType)
	{
		mvCondMeanType = theType ;
  		MESS_CREAT("cAbstCondMean")
	}

	/*!
	 * \fn cAbstCondMean::~cAbstCondMean()
	 */
	cAbstCondMean::~cAbstCondMean()
	{
		mvCondMeanType = eUnknown ;
  		MESS_DESTR("cAbstCondMean")
	}

	/*!
	 * \fn  inline eCondMeanEnum cAbstCondMean::GetCondMeanType(void)
	 * \param void
	 * \return a eCondMeanEnum value.
	 */
	eCondMeanEnum cAbstCondMean::GetCondMeanType(void) const
	{
		return mvCondMeanType ;
	}

	/*!
	 * \fn  void cAbstCondMean::SetCondMeanType(eCondMeanEnum theType)
	 * \param eCondMeanEnum theType
	 * \details mvCondMeanType = theType
	 */
	void cAbstCondMean::SetCondMeanType(eCondMeanEnum theType)
	{	mvCondMeanType = theType ;
	}

	/*!
	 * \fn ostream& operator <<(ostream& theOut, const cAbstCondMean& theAbstCondMean)
	 * \param ostream& theOut: output stream (file or screen). Default cout.
	 * \param const cAbstCondMean& theAbstCondMean: the mean component model.
	 * \details Uses cAbstCondMean::Print method.
	 */
	ostream& operator <<(ostream& theOut, const cAbstCondMean& theAbstCondMean)
	{
		theAbstCondMean.Print(theOut) ;
		return theOut ;
	}

	
}//namespace
