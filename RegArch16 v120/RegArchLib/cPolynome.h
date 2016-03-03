#pragma once
#ifndef _CPOLYNOME_H_
#define _CPOLYNOME_H_
#include "StdAfxRegArchLib.h"



namespace RegArchLib {
	class cPolynome
	{
	public:
		cDVector mCoeff;
	public:
		cPolynome(uint theDegree=0);
		cPolynome(uint theDegree, double* theCoeff);
		cPolynome(cDVector& theCoeff);
		cPolynome(cPolynome& theSrc);
		~cPolynome();
		void Delete();
		uint GetNDegree(void);
		void Resize(uint theDegree);
		void Print(void);
		double& operator[](uint theUndex);
		cPolynome& operator=(const cPolynome& theSrc);
		cPolynome& operator+(cPolynome& theP);
		cPolynome& operator-(cPolynome& theP);
		cPolynome& operator*(cPolynome& theP);
		friend cPolynome& operator*(double theVal, cPolynome& theP);
		friend cPolynome& operator*(cPolynome& theP, double theVal);
		friend double BackwardPolOp(cPolynome& theP, cDVector& theYt, uint theIndex0 = 0);
	};

	extern cPolynome ourPolynom;

	extern void ComputeDeltaPowD(double theD, uint theDegree, cPolynome& theRes); // compute (1 - X)^d till theDegree
	extern void IncrPowDiv(cPolynome& theNum, cPolynome& theDen, uint theOrder, cPolynome& theQuot, cPolynome& theRest);
	extern cPolynome& TrunkMult(cPolynome& theP, cPolynome& theQ, uint theMaxDegree);
} // namespace
#endif // _POLYNOME_H_
