#include "StdAfxRegArchLib.h"
/*!
	\file RegArchCompute.cpp
	\brief implementation of the Simulation / Estimation procedures for general RegArchModel
	
	\author Jean-Baptiste DURAND, Ollivier TARAMASCO 
	\date dec-18-2006 - last change feb-18-2011
*/
namespace RegArchLib {

	/*!
	 * \fn void RegArchSimul(uint theNSample, const cRegArchModel& theModel, cRegArchValue& theData)
	 * \param uint theNSample: size of the sample
	 * \param const cRegArchModel& theModel: the RegArch model
	 * \param cRegArchValue& theData: output parameter. The Y(t) values are stored in theData.mYt
	 */
	void RegArchSimul(uint theNSample, const cRegArchModel& theModel, cRegArchValue& theData)
	{
		theData.ReAlloc(theNSample) ;
		theModel.mResids->Generate(theNSample, theData.mEpst) ;
		for (register uint t = 0 ; t < theNSample ; t++)
		{	theData.mHt[t] = theModel.mVar->ComputeVar(t, theData) ;
			if (theModel.mMean != NULL)
				theData.mMt[t] = theModel.mMean->ComputeMean(t, theData) ;
			theData.mUt[t] = sqrt(theData.mHt[t])*theData.mEpst[t] ;
			theData.mYt[t] = theData.mMt[t] + theData.mUt[t] ;
		}
	}


	/*!
	 * \fn void RegArchSimul(uint theNSample, const cRegArchModel& theModel, cDVector& theYt)
	 * \param uint theNSample: size of the sample
	 * \param const cRegArchModel& theModel: the RegArch model
	 * \param cDVector& theYt: output parameter. 
	 */
	void RegArchSimul(uint theNSample, const cRegArchModel& theModel, cDVector& theYt)
	{
	cRegArchValue myValue(theNSample) ;
		RegArchSimul(theNSample, theModel, myValue) ;
		theYt = myValue.mYt ;

	}


	/*!
	 * \fn double RegArchLLH(const cRegArchModel& theParam, cDVector* theYt, cDMatrix* theXt)
	 * \param const cRegArchModel& theParam: the model
	 * \param cDVector* theYt: the observations
	 * \param cDMatrix* theXt: the regressors matrix. Default NULL
	 * \brief return the log-likelihood value
	 */
	double RegArchLLH(const cRegArchModel& theParam, cDVector* theYt, cDMatrix* theXt)
	{
	cRegArchValue myValue(theYt, theXt) ;
		return RegArchLLH(theParam, myValue) ;
	}

	/*!
	 * \fn double RegArchLLH(const cRegArchModel& theParam,cRegArchValue& theData)
	 * \param const cRegArchModel& theParam: the model
	 * \param cRegArchValue& theData: theData.mYt contains the observations.
	 * \brief return the log-likelihood value
	 */
	double RegArchLLH(const cRegArchModel& theParam, cRegArchValue& theData)
	{
	int mySize = (int)theData.mYt.GetSize() ;
	double myRes = 0 ;
		theData.mEpst = theData.mHt = theData.mMt = theData.mUt = 0.0 ;
		for(register int t=0 ; t < mySize ; t++)
		{	theData.mHt[t] = theParam.mVar->ComputeVar(t, theData) ;
			if (theParam.mMean != NULL)
				theData.mMt[t] = theParam.mMean->ComputeMean(t, theData) ;
			theData.mUt[t] = theData.mYt[t] - theData.mMt[t] ;
			theData.mEpst[t] = theData.mUt[t]/sqrt(theData.mHt[t]) ;
			myRes += -0.5*log(theData.mHt[t]) + theParam.mResids->LogDensity(theData.mEpst[t]) ;
		}
		return myRes ;
	}

	/*!
	 * \fn void RegArchGradLt(int theDate, cRegArchModel& theParam, cRegArchValue& theValue, cRegArchGradient& theGradData, cDVector& theGradlt)
	 * \brief Compute the gradient of the log-likelihood, at current time theDate
	 * \param uint theDate: time at which conditional log-density is considered
	 * \param const cRegArchModel& theParam: the model
	 * \param cRegArchValue& theValue: contains the observations. Used to stored computed residuals, standardized residuals, etc.
	 * \param cRegArchGradient& theGradData: contains the gradient of different components of the model, at current time theDate
	 * \param cDVector& theGradlt: gradient of the log-likelihood, at current time theDate
	 */

	void RegArchGradLt(uint theDate, cRegArchModel& theParam, cRegArchValue& theValue, cRegArchGradient& theGradData, cDVector& theGradlt)
	{	theGradlt = 0.0 ;
		theValue.mHt[theDate] = theParam.mVar->ComputeVar(theDate, theValue) ;
		if (theParam.mMean != NULL)
			theValue.mMt[theDate] = theParam.mMean->ComputeMean(theDate, theValue) ;
		theValue.mUt[theDate] = theValue.mYt[theDate] - theValue.mMt[theDate] ;
		double mySigmat = sqrt(theValue.mHt[theDate]) ;
		theValue.mEpst[theDate] = theValue.mUt[theDate]/mySigmat ;
		theParam.mVar->ComputeGrad(theDate, theValue, theGradData, theParam.mResids) ;
		if (theParam.mMean != NULL)
			theParam.mMean->ComputeGrad(theDate, theValue, theGradData, theParam.mResids) ;
		theParam.mResids->ComputeGrad(theDate, theValue, theGradData) ;
		theGradData.mCurrentGradSigma = theGradData.mCurrentGradVar / (2.0 * mySigmat) ;
		theGradData.mCurrentGradEps = -1.0*(theValue.mEpst[theDate] * theGradData.mCurrentGradSigma + theGradData.mCurrentGradMu)/ mySigmat ;
		theGradlt =  (-1.0/mySigmat) * theGradData.mCurrentGradSigma  + theGradData.mCurrentGradDens[0] * theGradData.mCurrentGradEps ;
		uint myNLawParam = theGradData.GetNDistrParameter() ;
		uint myIndex = theGradData.GetNMeanParam() + theGradData.GetNVarParam() ;
		for (register uint i =  1 ; i <= myNLawParam ; i++)
			theGradlt[i+myIndex-1] += theGradData.mCurrentGradDens[i] ;
			// Update
		theGradData.Update() ;
	}

	void RegArchLtAndGradLt(int theDate, cRegArchModel& theParam, cRegArchValue& theValue, cRegArchGradient& theGradData, double& theLt, cDVector& theGradlt)
	{
		theGradlt = 0.0 ;
		theValue.mHt[theDate] = theParam.mVar->ComputeVar(theDate, theValue) ;
		if (theParam.mMean != NULL)
			theValue.mMt[theDate] = theParam.mMean->ComputeMean(theDate, theValue) ;
		theValue.mUt[theDate] = theValue.mYt[theDate] - theValue.mMt[theDate] ;
	double mySigmat = sqrt(theValue.mHt[theDate]) ;
		theValue.mEpst[theDate] = theValue.mUt[theDate]/mySigmat ;
		theParam.mVar->ComputeGrad(theDate, theValue, theGradData, theParam.mResids) ;
		if (theParam.mMean != NULL)
			theParam.mMean->ComputeGrad(theDate, theValue, theGradData, theParam.mResids) ;
		theParam.mResids->ComputeGrad(theDate, theValue, theGradData) ;
		theGradData.mCurrentGradSigma = theGradData.mCurrentGradVar / (2.0 * mySigmat) ;
		theGradData.mCurrentGradEps = -1.0*(theValue.mEpst[theDate] * theGradData.mCurrentGradSigma + theGradData.mCurrentGradMu)/ mySigmat ;
		theGradlt =  (-1.0/mySigmat) * theGradData.mCurrentGradSigma  + theGradData.mCurrentGradDens[0] * theGradData.mCurrentGradEps ;
	uint myNLawParam = theGradData.GetNDistrParameter() ;
	uint myIndex = theGradData.GetNMeanParam() + theGradData.GetNVarParam() ;	
		for (register uint i =  1 ; i <= myNLawParam ; i++)
			theGradlt[i+myIndex-1] += theGradData.mCurrentGradDens[i] ;
			// Update
		theGradData.Update() ;
		theLt = -0.5*log(theValue.mHt[theDate]) + theParam.mResids->LogDensity(theValue.mEpst[theDate]) ; 
	}

	void RegArchGradLLH(cRegArchModel& theParam, cRegArchValue& theData, cDVector& theGradLLH)
	{

	cRegArchGradient myGradData=cRegArchGradient(&theParam) ;
	cDVector myGradlt(myGradData.GetNParam()) ;
		theGradLLH = 0.0L ;
		for (register int t = 0 ; t < (int)theData.mYt.GetSize() ; t++)
		{	RegArchGradLt(t, theParam, theData, myGradData, myGradlt) ;
			theGradLLH += myGradlt ;
		}
	}

	void RegArchLLHAndGradLLH(cRegArchModel& theParam, cRegArchValue& theValue, double& theLLH, cDVector& theGradLLH)
	{
	cRegArchGradient myGradData(&theParam) ;
	cDVector myGradlt(myGradData.GetNParam()) ;
	double myLt ;
		theGradLLH = 0.0L ;
		theLLH = 0.0 ;
		for (register int t = 0 ; t < (int)theValue.mYt.GetSize() ; t++)
		{	RegArchLtAndGradLt(t, theParam, theValue, myGradData, myLt, myGradlt) ;
			theGradLLH += myGradlt ;
			theLLH += myLt ;
		}
	}

	void NumericRegArchGradLLH(cRegArchModel& theModel, cRegArchValue& theValue, cDVector& theGradLLH, double theh)
	{
	double myLLH0 = RegArchLLH(theModel, theValue) ;
	int myNParam = (int)theGradLLH.GetSize() ;

	cDVector myVectParam(myNParam), myVect0(myNParam) ;
	
		theModel.RegArchParamToVector(myVectParam) ;
		myVect0 = myVectParam ;
		for (register int i = 0 ; i < myNParam ; i++)
		{
		double myhh = fabs(theh * myVectParam[i]) ;
			if (myhh < 1e-16)
				myhh = theh ;
			myVectParam[i] += myhh ;
			theModel.VectorToRegArchParam(myVectParam) ;

		
		double myLLH1 = RegArchLLH(theModel, theValue) ;
			theGradLLH[i] = (myLLH1 - myLLH0)/myhh ;
			myVectParam[i] -= myhh ;
		}
		theModel.VectorToRegArchParam(myVect0) ;
	}

} //namespace
