#include "StdAfxRegArchLib.h"
/*!
\file cArfima.cpp
\brief sources for class cAr methods.

\author Jean-Baptiste Durand, Ollivier TARAMASCO
\date feb-18-2016 - Last change feb-18-2016
*/

namespace RegArchLib {
	/*!
	* \fn cArfimar(uint theNAr = 0, uint theNMa = 0, double theFracD = 0, uint theNTruncLag = 100):cAbstCondMean(eArfima)
	* \param const uint theNAr: number of AR lags.
	*/
	
	cArfima::cArfima() :cAbstCondMean(eArfima)
	{
		mvAr.ReAlloc(0);
		mvMa.ReAlloc(0);
		mvFracD = 0;
		mvNTruncLag = 0;
		mvTruncMa = NULL;
		mvPolMa.Delete();
		mvGradPolMa.clear();
		MESS_CREAT("cArfima")
	}

	cArfima::cArfima(uint theNAr, uint theNMa, double theFracD , uint theNTruncLag) :cAbstCondMean(eArfima)
	{
		mvAr.ReAlloc(theNAr);
		mvMa.ReAlloc(theNMa);
		mvFracD = theFracD;
		mvNTruncLag = theNTruncLag;
		mvTruncMa = new cMa(mvNTruncLag);
		mvPolMa.Resize(mvNTruncLag);
	uint myNParam = theNAr + theNMa + 1;
		
	mvGradPolMa.resize(myNParam);
		for (register uint i = 0; i < myNParam; i++)
			mvGradPolMa.at(i) = new cPolynome(mvNTruncLag);
		MESS_CREAT("cArfima")
	}

	cArfima::cArfima(const cDVector& theAr, const cDVector& theMa, double theFracD, uint theNTruncLag) :cAbstCondMean(eArfima)
	{
		mvAr = theAr;
		mvMa = theMa;
		mvFracD = theFracD;
		mvNTruncLag = theNTruncLag;
		mvTruncMa = new cMa(mvNTruncLag);
		mvPolMa.Resize(mvNTruncLag);
		uint myNParam = theAr.GetSize() + theMa.GetSize() + 1;
		mvGradPolMa.resize(myNParam);
		for (register uint i = 0; i < myNParam; i++)
			mvGradPolMa.at(i) = new cPolynome(mvNTruncLag);
		MESS_CREAT("cArfima")
	}

	cArfima::cArfima(cAbstCondMean& theArfima) :cAbstCondMean(eArfima)
	{	
	cArfima* myArfima = dynamic_cast<cArfima *>(&theArfima);
		if (myArfima)
			*this = *myArfima;
		else
			throw cError("wrong class in cArfima(cAbstCondMean& theArfima");
		MESS_CREAT("cArfima")
	}

	cArfima:: ~cArfima()
	{
	uint myNParam = mvAr.GetSize() + mvMa.GetSize() + 1;
		mvAr.Delete();
		mvMa.Delete();
		if (mvTruncMa != NULL)
		{
			mvTruncMa->Delete();
			mvTruncMa = NULL;
		}
		mvPolMa.Delete();
		if (mvGradPolMa.size() != 0)
		{
//			for (register uint i = 0; i < myNParam; i++)
//				mvGradPolMa.at(i)->Delete();
			mvGradPolMa.clear();
		}
		MESS_DESTR("cArfima")
	}

	cAbstCondMean* cArfima::PtrCopy(void) const
	{
	cArfima* myArfima = new cArfima(*this);
		return myArfima;
	}

	void cArfima::Delete(void)
	{
	uint myNParam = mvAr.GetSize() + mvMa.GetSize() + 1;
		mvAr.Delete();
		mvMa.Delete();
		if (mvTruncMa != NULL)
		{
			mvTruncMa->Delete();
			mvTruncMa = NULL;
		}
		mvPolMa.Delete();
		if (mvGradPolMa.size() != 0)
		{
//			for (register uint i = 0; i < myNParam; i++)
//				mvGradPolMa.at(i)->Delete();
			mvGradPolMa.clear();
		}
	}

	void cArfima::Print(ostream& theOut) const
	{
	uint myNAr = mvAr.GetSize();
	uint myNMa = mvMa.GetSize();
		theOut << "ARFIMA(" << myNAr << ", d, " << myNMa << ") model with:" << endl;
		for (register uint i = 0; i < myNAr; i++)
			theOut << "\tAR[" << i + 1 << "]=" << mvAr[i] << endl;
		theOut << "\td=" << mvFracD << endl;
		for (register uint i = 0; i < myNMa; i++)
			theOut << "\tMA[" << i + 1 << "]=" << mvMa[i] << endl;
	}

	void  cArfima::SetDefaultInitPoint(double theMean, double theVar)
	{
		mvAr = 0.0;
		mvMa = 0.0;
		mvFracD = 0;

	}

	void  cArfima::ReAlloc(const uint theSize, const uint theNumParam)
	{
		switch (theNumParam)
		{
		case 0:
			mvAr.ReAlloc(theSize);
			break;
		case 1:
			mvMa.ReAlloc(theSize);
			break;
		default:
			throw cError("cArfima::ReAlloc - theNumParam must be in 0 or 1");
			break;
		}
	}
	
	void  cArfima::ReAlloc(const cDVector& theVectParam, const uint theNumParam)
	{
		switch (theNumParam)
		{
		case 0: // mvAr
			mvAr = theVectParam;
			break;
		case 1: // mvMa
			mvMa = theVectParam;
			break;
		default:
			throw cError("cArfima::ReAlloc - theNumParam must be in 0 or 1");
			break;
		}

	}
	
	void  cArfima::Set(const double theValue, const uint theIndex, const uint theNumParam)
	{
		switch (theNumParam)
		{
		case 0: // AR
			if (theIndex < mvAr.GetSize())
				mvAr[theIndex] = theValue;
			else
				throw cError("cArfima::Set - wrong index");
			break;
		case 1:
			if (theIndex < mvMa.GetSize())
				mvMa[theIndex] = theValue;
			else
				throw cError("cArfima::Set - wrong index");
			break;
		case 2:
			mvFracD = theValue;
			break;

		default:
			throw cError("cArfima::Set - theNumParam must be in 0, 1, 2");
			break;

		}
	}
	
	void  cArfima::Set(const cDVector& theVectParam, const uint theNumParam)
	{
		switch (theNumParam)
		{
		case 0:
			mvAr = theVectParam;
			break;
		case 1:
			mvMa = theVectParam;
			break;
		case 2:
			if (theVectParam.GetSize() > 0)
				mvFracD = theVectParam[0];
			else
				throw cError("cArfima::Set - Size of theVectParam must be > 0");
			break;
		default:
			throw cError("cArfima::Set - theNumParam must be in 0, 1, 2");
			break;
		}
	}
	
	double  cArfima::Get(const uint theIndex, const uint theNumParam)
	{
		switch (theNumParam)
		{
		case 0:
			return mvAr[theIndex];
			break;
		case 1:
			return mvMa[theIndex];
			break;
		case 2:
			return mvFracD;
			break;
		}

	}

	cArfima& cArfima::operator=(cArfima& theSrc)
	{
		mvFracD = theSrc.mvFracD;
		mvAr = theSrc.mvAr;
		mvMa = theSrc.mvMa;
		mvNTruncLag = theSrc.mvNTruncLag;
		mvTruncMa = new cMa(theSrc.mvTruncMa);
		mvPolMa = theSrc.mvPolMa;
	uint myNParam = mvAr.GetSize() + mvMa.GetSize() + 1;
		for (uint i = 0; i < mvGradPolMa.size(); i++)
			mvGradPolMa.at(i)->Delete();
		mvGradPolMa.clear();
		mvGradPolMa.resize(myNParam);
		for (register uint i = 0; i < myNParam; i++)
			mvGradPolMa.at(i) = theSrc.mvGradPolMa[i];
		return *this;
	}
	
	void  cArfima::UpdateProxyMeanParameters(void)
	{
	uint myNAr = GetNAr();
	uint myNMa = GetNMa();
	
	cPolynome myPhi(myNAr);
	cPolynome myTeta(myNMa);
		myPhi[0] = myTeta[0] = 1.0;
		for (register uint i = 1; i <= myNAr; i++)
			myPhi[i] = -mvAr[i-1];
		for (register uint i = 1; i <= myNMa; i++)
			myTeta[i] = mvMa[i-1];
	cPolynome myDelta(0);
		ComputeDeltaPowD(-mvFracD, mvNTruncLag, myDelta);
	cPolynome myPol1(0), myRest(0);
		IncrPowDiv(myTeta, myPhi, mvNTruncLag, myPol1, myRest);
	cPolynome myPol2(0);
		IncrPowDiv(myPol1, myPhi, mvNTruncLag, myPol2, myRest);
	cPolynome myPol3(0);
			myPol3 = TrunkMult(myDelta, myPol2, mvNTruncLag);
		mvPolMa = TrunkMult(myDelta, myPol1, mvNTruncLag);
	cPolynome myPol4(0);
		IncrPowDiv(myDelta, myPhi, mvNTruncLag, myPol4, myRest);
	cPolynome myPol5(mvNTruncLag);
		for (register uint i = 1; i <= mvNTruncLag; i++)
			myPol5[i] = -1 / (double)i;
	cPolynome myPol6(0);
		myPol6 = TrunkMult(myPol5, myDelta, mvNTruncLag);
	cPolynome myPol7(0);
		myPol7 = TrunkMult(myPol6, myPol1, mvNTruncLag);
		for (uint i = 1; i <= mvNTruncLag; i++)
			mvTruncMa->Set(mvPolMa[i], i - 1);
	
		for (uint i = 0; i < myNAr; i++)
			*mvGradPolMa.at(i) = myPol3;
		for (uint i = myNAr; i < myNAr + myNMa; i++)
			*mvGradPolMa.at(i) = myPol4;
		*mvGradPolMa.at(myNAr + myNMa) = myPol7;
	}

	double  cArfima::ComputeMean(uint theDate, const cRegArchValue& theData) const
	{
		return mvTruncMa->ComputeMean(theDate, theData);

	}
	
	uint  cArfima::GetNParam(void) const
	{
		return mvAr.GetSize() + mvMa.GetSize() + 1;
	}
	
	uint  cArfima::GetNAr(void) const
	{
		return mvAr.GetSize();
	}

	uint  cArfima::GetNMa(void) const
	{
		return mvMa.GetSize();
	}
		
	uint  cArfima::GetNLags(void) const
	{
		return MAX(MAX(mvMa.GetSize(), mvAr.GetSize()), mvNTruncLag);
	}

	void  cArfima::ComputeGrad(uint theDate, const cRegArchValue& theData, cRegArchGradient& theGradData, uint theBegIndex, cAbstResiduals* theResiduals)
	{

	}
	
	void  cArfima::RegArchParamToVector(cDVector& theDestVect, uint theIndex)
	{
	uint mySize = GetNParam();
		if (theDestVect.GetSize() < mySize + theIndex)
				throw cError("Wrong size");
		mvAr.SetSubVectorWithThis(theDestVect, theIndex);
		mvMa.SetSubVectorWithThis(theDestVect, theIndex + mvAr.GetSize());
		theDestVect[theIndex + mvAr.GetSize() + mvMa.GetSize()] = mvFracD;

	}
	
	void  cArfima::VectorToRegArchParam(const cDVector& theSrcVect, uint theIndex)
	{
	uint mySize = theSrcVect.GetSize();
		if (GetNParam() + theIndex > mySize)
			throw cError("Wrong size");
		mvAr.SetThisWithSubVector(theSrcVect, theIndex);
		mvMa.SetThisWithSubVector(theSrcVect, theIndex + mvAr.GetSize());
		mvFracD = theSrcVect[theIndex + mvAr.GetSize() + mvMa.GetSize()];


	}

	void  cArfima::ComputeHess(uint theDate, const cRegArchValue& theData, cRegArchGradient& theGradData, cRegArchHessien& theHessData, uint theBegIndex, cAbstResiduals* theResiduals)
	{

	}

} // namespace
