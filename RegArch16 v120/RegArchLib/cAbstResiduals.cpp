#include "StdAfxRegArchLib.h"
/*!
	\file cAbstResiduals.cpp
	\brief sources for abstract class cAbstResiduals methods.

	\author Jean-Baptiste DURAND, Ollivier TARAMASCO 
	\date dec-18-2006 - Last change feb-18-2011
*/
namespace RegArchLib {
	/*!
	 * \fn cAbstResiduals::cAbstResiduals(eDistrTypeEnum theDistr, cDVector* theModel, bool theSimulFlag)
	 * \param const eDistrTypeEnum theDistr: code for the conditional distribution.
	 * \param const cDVector* theModel: vector of parameters
	 * \param const bool theSimulFlag: true if created for simulation
	 */
	cAbstResiduals::cAbstResiduals(eDistrTypeEnum theDistr, cDVector* theDistrParam, bool theSimulFlag)
	{
		mvDistr = theDistr ;
		if (theSimulFlag)
		{	// random generator simulation
		gsl_rng_env_setup() ;
			mtR = gsl_rng_alloc(gsl_rng_default) ;
		#ifndef _DEBUG
			gsl_rng_set(mtR, (unsigned long int)time(NULL)) ;
		#else
			gsl_rng_set(mtR, 0) ; // Pour avoir toujours la meme serie simulee quand on teste
		#endif // _DEBUG
		}
		else
			mtR = NULL ;

		if (theDistrParam != NULL)
			mDistrParameter = *theDistrParam ;
		else
			mDistrParameter.Delete() ;
		MESS_CREAT("cAbstResiduals") ;
	}

	/*!
	 * \fn cAbstResiduals::~cAbstResiduals()
	 * \details mDistrParameter is deleted here.
	 */
	cAbstResiduals::~cAbstResiduals()
	{
		mDistrParameter.Delete() ;
		if (mtR != NULL)
		{	// random generator destructor
			gsl_rng_free(mtR) ;
			mtR = NULL ;
		}
		MESS_DESTR("cAbstResiduals") ;
	}

	/*!
	 * \fn cAbstResiduals::Delete(void)
	 * \param void
	 * \details mDistrParameter is deleted here.
	 */
	void cAbstResiduals::Delete(void)
	{
		mDistrParameter.Delete() ;
		if (mtR != NULL)
		{	// random generator destructor
			gsl_rng_free(mtR) ;
			mtR = NULL ;
		}
	}
	/*!
	 * \fn cAbstResiduals::SetSimul(void)
	 * \param void
	 */
	void cAbstResiduals::SetSimul(void)
	{
		if (mtR == NULL)
		{
			// random generator initialisation
			gsl_rng_env_setup() ;
			mtR = gsl_rng_alloc(gsl_rng_default) ;
		#ifndef _DEBUG
			gsl_rng_set(mtR, (unsigned long int)time(NULL)) ;
		#else
			gsl_rng_set(mtR, 0) ; // Pour avoir toujours la m�me s�rie simul�e quand on teste
		#endif // _DEBUG
		}
	}

	/*!
	 * \fn inline eDistrTypeEnum cAbstResiduals::GetDistrType(void) const
	  * \param void
	 */
	eDistrTypeEnum cAbstResiduals::GetDistrType(void) const
	{	return mvDistr ;
	}


	double cAbstResiduals::Get(uint theIndex)
	{
		if (mDistrParameter.GetSize() > theIndex)
			return mDistrParameter[theIndex] ;
		else
			throw cError("Wrong size in cAbstResiduals::Get") ;
	}

	void cAbstResiduals::Set(double theValue, uint theIndex)
	{
		if (mDistrParameter.GetSize() <= theIndex)
		{	mDistrParameter.ReAlloc(theIndex+1) ;
		}
		mDistrParameter[theIndex] = theValue ;
	}
		
	/*!
	 * \fn ostream& cAbstResiduals::operator <<(ostream& theOut, const cAbstResiduals& theAbstResisuals)
	 * \param ostream& theOut: output (file or screen).
	 * \param const cAbstResiduals& theAbstResisuals: the residuals model.
	 * \details Uses cAbstResiduals::Print method.
	 */
	ostream& operator <<(ostream& theOut, const cAbstResiduals& theAbstResisuals)
	{
		theAbstResisuals.Print(theOut) ;
		return theOut ;
	}


} // namespace

