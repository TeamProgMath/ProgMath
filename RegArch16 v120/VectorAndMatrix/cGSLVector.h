/*!
	\file cGSLVector.h
	\brief header for C++ encapsulation of GSL vectors definitions and functions.
	\author Ollivier TARAMASCO 
	\date oct-09-2008
*/
#pragma once
#ifndef _cGSLVector_H_
#define _cGSLVector_H_
	#ifndef _RDLL_
		#include <gsl/gsl_vector.h>
		#include <gsl/gsl_linalg.h> 
	#else
		#include "StdAfxGslEquiv.h"
	#endif // _RDLL_
	#include <cstdlib>
	#include <cstring>
	#include <cmath>
	#include <cfloat>
	#include <ostream>
	#include <iostream>



	// POUR DEBOGUER
	#ifdef _MESS_CREAT
		#ifdef _DEBUG
			#ifndef _RDLL_
				#define MESS_CREAT(p)  std::cout << "creation de " << p << std::endl ;
				#define MESS_DESTR(p)  std::cout << "destruction de " << p << std::endl ;
			#else
				#define MESS_CREAT(p) Rprintf("Creation de %s\n", p) ;
				#define MESS_DESTR(p)  Rprintf("Destruction de %s\n", p) ;
			#endif // _RDLL
		#else
			#define MESS_CREAT(p) ///< used for debugging 
			#define MESS_DESTR(p) ///< used for debugging
		#endif //_DEBUG
	#else
		#define MESS_CREAT(p) ///< used for debugging
		#define MESS_DESTR(p) ///< used for debugging
	#endif //_MESS_CREAT

	#ifdef _DEBUG
		#ifdef _MESS
			#ifdef _RDLL_
				#define MESS(p) Rprintf("%s\n", p) ;
			#else
				#define MESS(p) std:cout << p << std:endl;
			#endif //_RDLL
		#else
			#define MESS(p)
		#endif // _MESS
	#else
		#define MESS(p)
	#endif //_DEBUG


	#if _WIN32 != 1
		#include <sys/types.h>
		#define _UINT_
	#endif // Pour avoir le type uint sous autre chose que Windows


	#ifndef _UINT_
		#define _UINT_
		typedef unsigned int uint ;
	#endif // _UINT_

	namespace VectorAndMatrixNameSpace
	{
		using namespace ErrorNameSpace ;
		using namespace std ;
		#ifdef _RDLL_
				using namespace GslEquivNameSpace;
		#endif // _RDLL_

		class cGSLMatrix ;

			/*!
				\brief class cGSLVector
				\par
				1) Encapsulation of the "C" gsl structure gsl_vector
				2) Standard and usefull vector operations declarations
			*/	
			class _DLLEXPORT_ cGSLVector
			{
			private :
					gsl_vector* mvVect ; ///< data
			public :
				cGSLVector(int theSize = 0, double theVal = 0.0) ; ///< standard constructor
				cGSLVector(int theSize, double* theVect) ;///< mVect := theVect
				cGSLVector(const gsl_vector* theVect) ;///< mVect := theVect
				cGSLVector(const cGSLVector& theVect) ;///< mVect := theVect
				virtual ~cGSLVector() ;///< std destructor
				double& operator[](int theIndex) const ;///< returns mvect[theIndex]
				uint GetSize(void) const ;///< size of mVect
				gsl_vector* GetGSLVector(void) const ;///< returns mVect
				gsl_vector* GetVector(void) const ; ///< returns mVect
				void Delete(void) ;///< Destructor
				void ReAlloc(int theSize, double theVal=0.0) ;///< Reallocation and initialisation of mVect
				void ReAlloc(int theSize, double* theVect) ;///< Reallocation and initialisation of mVect
				void ReAlloc(const cGSLVector& theVect) ;///< Reallocation and initialisation of mVect
				void ReAlloc(const gsl_vector* theVect) ;///< Reallocation and initialisation of mVect
				cGSLVector& operator =(const cGSLVector& theSrcVect) ; ///< *this = theSrcVect
				cGSLVector& operator =(double theVal) ;///< *this[i] = theVal for all i 
				cGSLVector& operator =(double* theSrcVect) ;///< *this = theSrcVect
				cGSLVector& operator =(const cGSLMatrix& theMatrix) ;///< *this = theSrcVect
				cGSLVector& operator +=(const cGSLVector& theSrcVect) ;///< *this += theSrcVect
				cGSLVector& operator +=(double theVal) ;///< *this[i] += theVal for all i 
				cGSLVector& operator +=(double* theSrcVect) ;///<*this[i] += theSrcVect[i] for all i 
				cGSLVector& operator -=(const cGSLVector& theSrcVect) ;///< *this -= theSrcVect
				cGSLVector& operator -=(double theVal) ;///< *this[i] -= theVal for all i 
				cGSLVector& operator -=(double* theSrcVect) ;///< *this -= theSrcVect
				cGSLVector& operator *=(double theVal) ;///< *this *= theVal
				cGSLVector& operator /=(double theVal) ;///< *this /= theVal
				void SetThisWithSubVector(const cGSLVector& theSrcVect, const uint theBegIndex) ;///< *this[i] = theSrcVect[i+theBegIndex] for all i
				void SetSubVectorWithThis(cGSLVector& theDestVect, const uint theBegIndex) ;///< theDestVect[i+theBegIndex]=*this[i] for all i
				cGSLVector& operator +(double theVal) ;///< returns *this[i]+theVal for all i
				cGSLVector& operator +(const cGSLVector& theVect) ;///< returns *this[i]+theVect[i] for all i
				cGSLVector& operator +(double* theVect2) ;///< returns *this + theVect2
				cGSLVector& operator -(double theVal) ;///< returns theVect[i]-theVal for all i
				cGSLVector& operator -(const cGSLVector& theVect) ;///< returns *this[i]-theVect[i] for all i
				cGSLVector& operator -(double* theVect2) ;///< returns *this - theVect2
				cGSLVector& operator *(double theVal) ;///< returns theVal*theVect
				cGSLVector& operator /(double theVal) ;/// returns theVal/theVect[i] for all i
				void Print(ostream& theStream=cout) ;
			#ifdef _MSC_VER
				friend double _DLLEXPORT_ Norm (const cGSLVector& theVect) ;///< euclidian norm
				friend double _DLLEXPORT_ Sum(const cGSLVector& theVect) ; ///< sum
				friend _DLLEXPORT_ ostream& operator <<(ostream& theStream, const cGSLVector& theVect) ; ///< prints mVect	
				friend cGSLVector _DLLEXPORT_ operator +(double theVal, const cGSLVector& theVect) ;///< returns theVect[i]+theVal for all i
				friend cGSLVector _DLLEXPORT_ operator +(const cGSLVector& theVect1, double* theVect2) ;///< returns theVect1 + theVect2
				friend cGSLVector _DLLEXPORT_ operator -(double theVal, const cGSLVector& theVect) ;///< returns theVal-theVect[i] for all i
				friend cGSLVector _DLLEXPORT_ operator -(double* theVect1, const cGSLVector& theVect2) ;///< returns theVect1 - theVect2
				friend cGSLVector _DLLEXPORT_ operator *(double theVal, const cGSLVector& theVect) ;///< returns theVal*theVect
				friend cGSLVector _DLLEXPORT_ operator /(double theVal, const cGSLVector& theVect) ;/// returns theVal/theVect[i] for all i
				friend cGSLVector _DLLEXPORT_ Abs(const cGSLVector& theVect);
				friend double _DLLEXPORT_ Sum(const cGSLVector& theVect);
				friend double _DLLEXPORT_ Maxi(const cGSLVector& theVect);
				friend double _DLLEXPORT_ Mini(const cGSLVector& theVect);
				friend  _DLLEXPORT_ double* GSLVectorToDoubleStar(const cGSLVector& theVect);
#endif // _MSC_VERdouble* 
				} ; // class
#ifndef _MSC_VER
	extern double _DLLEXPORT_ Norm (const cGSLVector& theVect) ;///< euclidian norm
	extern double _DLLEXPORT_ Sum(const cGSLVector& theVect) ; ///< sum
	extern _DLLEXPORT_ ostream& operator <<(ostream& theStream, const cGSLVector& theVect) ; ///< prints mVect
	extern cGSLVector _DLLEXPORT_ operator +(double theVal, const cGSLVector& theVect) ;///< returns theVect[i]+theVal for all i
	extern cGSLVector _DLLEXPORT_ operator +(const cGSLVector& theVect1, double* theVect2) ;///< returns theVect1 + theVect2
	extern cGSLVector _DLLEXPORT_ operator +(double* theVect1, const cGSLVector& theVect2);///< returns theVect1 + theVect2
	extern cGSLVector _DLLEXPORT_ operator -(double theVal, const cGSLVector& theVect) ;///< returns theVal-theVect[i] for all i
	extern cGSLVector _DLLEXPORT_ operator -(double* theVect1, const cGSLVector& theVect2) ;///< returns theVect1 - theVect2
	extern cGSLVector _DLLEXPORT_ operator *(double theVal, const cGSLVector& theVect) ;///< returns theVal*theVect
	extern cGSLVector _DLLEXPORT_ operator /(double theVal, const cGSLVector& theVect) ;/// returns theVal/theVect[i] for all i
	extern  cGSLVector Abs(const cGSLVector& theVect);
	extern double _DLLEXPORT_ Sum(const cGSLVector& theVect);
	extern double _DLLEXPORT_ Maxi(const cGSLVector& theVect);
	extern double _DLLEXPORT_ Mini(const cGSLVector& theVect);
	extern _DLLEXPORT_ double* GSLVectorToDoubleStar(const cGSLVector& theVect);
#endif // _MSC_VER
	} // namespace

#endif //_cGSLVector_H_
