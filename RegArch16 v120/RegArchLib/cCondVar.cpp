#include "StdAfxRegArchLib.h"

/*!
 \file cAbstCondVar.cpp
 \brief sources for abstract class cAbstCondVar methods.

 \author Jean-Baptiste DURAND, Ollivier TARAMASCO 
 \date dec-18-2006 - Last change feb-18-2011
*/

namespace RegArchLib {
	/*!
	 * \fn cCondVar::cCondVar(uint theNCondVar)
	 * \param uint theNCondVar: number of conditional variances
	 * \details mvCondVar = theNCondVar
	 */
	cCondVar::cCondVar(uint theNCondVar)
	: mvCondVar()
	{	mvNCondVar = theNCondVar ;

		MESS_CREAT("cCondVar") ;
	}

	/*!
	 * \fn cCondVar::cCondVar(const cCondVar& theCondVar)
	 * \param cCondVar& theCondVar: conditional variance
	 * \details Recopy constructor
	 */
	cCondVar::cCondVar(const cCondVar& theCondVar)
	: mvNCondVar(theCondVar.GetNVar()),
	  mvCondVar(theCondVar.GetNVar())
	{
		std::vector<cAbstCondVar*> myCondVar = theCondVar.GetCondVar() ;
		for (register uint i = 0 ; i < mvNCondVar ; i++)
			mvCondVar[i] = myCondVar[i]->PtrCopy() ;
		MESS_CREAT("cCondVar") ;
	}

	/*!
	 * \fn cCondVar::~cCondVar()
	 * \param None
	 * \details simple destructor
	 */
	cCondVar::~cCondVar()
	{	Delete() ;
		MESS_DESTR("cCondVar") ;
	}

	/*!
	 * \fn void cCondVar::Delete(void)
	 * \param void
	 * \details free memory used par the cCondVar class
	 */
	void cCondVar::Delete(void)
	{	if (mvNCondVar > 0)
		{	for (register uint i = 0 ; i < mvNCondVar ; i++)
				if (mvCondVar[i] != NULL)
				{	mvCondVar[i]->Delete() ;
					delete mvCondVar[i] ;
					mvCondVar[i] = (cAbstCondVar *)NULL ;
				}
		}
		mvNCondVar = 0 ;
	}

	/*!
	 * \fn inline uint cCondVar::GetNVar(void) const
	 * \param void
	 * \brief return mvNCondVar
	 */
	uint cCondVar::GetNVar(void) const
	{
		return mvNCondVar ;
	}

	/*!
	 * \fn  void cCondVar::SetOneVar(uint theWhatVar, cAbstCondVar* theAbstCondVar)
	 * \param uint theWhatVar: index of the conditional variance component
	 * \param cAbstCondVar* theAbstCondVar: conditional variance component to be copied in the mCondVar array.
	 * \brief *mvCondVar[theWhatVar] = *theAbstCondVar
	 */
	void cCondVar::SetOneVar(uint theWhatVar, cAbstCondVar& theAbstCondVar)
	{
		if (theWhatVar > mvNCondVar)
			throw cError("cCondVar::GetOneVar bad index") ;
		else
		{
			if (theWhatVar < mvNCondVar)
			{
				if (mvCondVar[theWhatVar] != NULL)
					delete mvCondVar[theWhatVar] ;
				mvCondVar[theWhatVar] = theAbstCondVar.PtrCopy();
			}
			else
			{
				mvCondVar.push_back(theAbstCondVar.PtrCopy()) ;
				mvNCondVar += 1 ;
			}
		}
	}

	/*!
	 * \fn cAbstCondVar** cCondVar::GetCondVar(void)
	 * \\details return mvCondVar
	 */
	std::vector<cAbstCondVar*> cCondVar::GetCondVar(void) const
	{	return mvCondVar ;
		
	}
	/*!
	 * \fn cAbstCondVar* cCondVar::GetOneVar(uint theIndex) const
	 * \param uint theIndex: index of component to be returned
	 * \\details return mvCondVar[theWhateVar]
	 */
	cAbstCondVar* cCondVar::GetOneVar(uint theIndex) const
	{
		if (theIndex < mvNCondVar)
			return mvCondVar[theIndex] ;
		else
			throw cError("cCondVar::GetOneVar bad index") ;
	}

	/*!
	 * \fn void cCondVar::GetCondVarType(eCondVarEnum* theCodeType) const
	 * \param eCondVarEnum* theCodeType: array of all conditional variance component codes
	 * \details fill theCodeType array
	 */
	void cCondVar::GetCondVarType(eCondVarEnum* theCodeType) const
	{	for (register uint i = 0 ; i < mvNCondVar ; i++)
			theCodeType[i] = mvCondVar[i]->GetCondVarType()  ;
	}

	/*!
	 * \fn void cCondVar::Print(ostream& theOut) const
	 * \param ostream& theOut: output stream (file or screen). Default cout.
	 */
	void cCondVar::Print(ostream& theOut) const
	{	theOut << "Conditional variance parameters:" << endl ;
		theOut << "----------------------------" << endl ;
		for (register uint i = 0 ; i < mvNCondVar ; i++)
			mvCondVar[i]->Print(theOut) ;
	}

	/*!
	 * \fn double cCondVar::Get(uint theNumVar, uint theIndex, uint theNumParam)
	 * \param uint theNumVar: index of
	 * \param ostream& theOut: output stream (file or screen). Default cout.
	 */
	double cCondVar::Get(uint theNumVar, uint theIndex, uint theNumParam)
	{
		return mvCondVar[theNumVar]->Get(theIndex, theNumParam) ;
	}
	
	/*!
	 * \fn ostream& operator <<(ostream& theOut, const cCondVar& theCondVar)
	 * \param ostream& theOut: output (file or screen).
	 * \param const cCondVar& theCondVar: the conditional variance class to be printed.
	 */
	ostream& operator <<(ostream& theOut, const cCondVar& theCondVar)
	{	theOut << "Conditional variance parameters:" << endl ;
		theOut << "----------------------------" << endl ;
		for (register uint i = 0 ; i < theCondVar.mvNCondVar ; i++)
		{	theCondVar.mvCondVar[i]->Print(theOut) ;
			theOut << endl ;
		}
		return theOut ;
	}

	/*!
	 * \fn cCondVar& cCondVar::operator =(cCondVar& theSrc)
	 * \param cCondVar& theSrc: source class
	 */
	cCondVar& cCondVar::operator =(cCondVar& theSrc)
	{	Delete() ;
	
		mvNCondVar = theSrc.GetNVar() ;
		mvCondVar.resize(mvNCondVar) ;
		for (register uint i = 0 ; i < mvNCondVar ; i++)
			mvCondVar[i] = theSrc.GetOneVar(i)->PtrCopy() ;
		return *this ;
	}

	/*!
	 * \fn double cCondVar::ComputeVar(uint theDate, const cRegArchValue& theData) const
	 * \param int theDate: date of computation
	 * \param const cRegArchValue& theData: past datas.
	 * \details Compute the value of the conditional variance at date theDate.
	 * theData is not updated here.
	 */
	double cCondVar::ComputeVar(uint theDate, const cRegArchValue& theData) const
	{
		double myVar = 0.0 ;
		uint myNVar = GetNVar() ;
		for (uint i = 0 ; i < myNVar ; i++)
			myVar += mvCondVar[i]->ComputeVar(theDate, theData) ;
		return myVar ;
	}

	uint cCondVar::GetNParam(void) const
	{
		uint myNParam = 0 ;
		for (register uint i = 0 ; i < mvNCondVar ; i++)
			myNParam += mvCondVar[i]->GetNParam() ;
		return myNParam ;
	}

	uint cCondVar::GetNLags(void) const
	{
		uint myNLags = 0 ;
		for (register uint i = 0 ; i < mvNCondVar ; i++)
			myNLags = MAX(myNLags, mvCondVar[i]->GetNLags());
		return myNLags ;
	}

	void cCondVar::ComputeGrad(uint theDate, const cRegArchValue& theValue, cRegArchGradient& theGradData, cAbstResiduals* theResids)
	{
		//uint myIndex = 0 ;
		theGradData.mCurrentGradVar = 0.0L ;
		for (register uint i = 0 ; i < mvNCondVar ; i++)
		{	
			//mvCondVar[i]->ComputeGrad(theDate, theValue, theGradData, myIndex, theResids) ;
			mvCondVar[i]->ComputeGrad(theDate, theValue, theGradData, theResids);
			//myIndex += mvCondVar[i]->GetNParam() ;
		}
	}


	void cCondVar::RegArchParamToVector(cDVector& theDestVect, uint theIndex) const
	{
		uint myIndexCour = theIndex ;
		for (register uint i = 0 ; i < mvNCondVar ; i++)
		{	mvCondVar[i]->RegArchParamToVector(theDestVect, myIndexCour) ;
			myIndexCour += mvCondVar[i]->GetNParam() ;
		}
	}

	void cCondVar::VectorToRegArchParam(const cDVector& theSrcVect, uint theIndex)
	{
		uint myIndexCour = theIndex ;
		for (register uint i = 0 ; i < mvNCondVar ; i++)
		{	mvCondVar[i]->VectorToRegArchParam(theSrcVect, myIndexCour) ;
			myIndexCour += mvCondVar[i]->GetNParam() ;
		}
	}

	void cCondVar::ComputeHess(uint theDate, const cRegArchValue & theData, cRegArchGradient & theGradData, cRegArchHessien & theHessData, cAbstResiduals * theResiduals)
	{
	}

}//namespace
