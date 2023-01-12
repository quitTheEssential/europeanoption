#include<iostream>
#include<cmath>
#include"getOneGaussianByBoxMueller.h"
#include"EuropeanOption.h"


//definition of constructor
EuropeanOption::EuropeanOption(
	int nInt_,
	double strike_,
	double spot_,
	double vol_,
	double r_,
	double expiry_,
	double barrier_){
		nInt = nInt_;
		strike = strike_;
		spot = spot_;
		vol = vol_;
		r = r_;
		expiry = expiry_;
		barrier = barrier_;
		generatePath();
}

//method definition
void EuropeanOption::generatePath(){
	double thisDrift = (r * expiry - 0.5 * vol * vol * expiry) / double(nInt);
	double cumShocks = 0;
	thisPath.clear();

	for(int i = 0; i < nInt; i++){
		cumShocks += (thisDrift + vol * sqrt(expiry / double(nInt)) * getOneGaussianByBoxMueller());
		thisPath.push_back(spot * exp(cumShocks));
	}
}

//method definition
double EuropeanOption::getArithmeticMean(){

	double runningSum = 0.0;

	for(int i = 0; i < nInt; i++){
		runningSum += thisPath[i];
	}

	return runningSum/double(nInt);
}


//method definition
double EuropeanOption::getGeometricMean(){

	double runningSum = 0.0;

	for(int i = 0; i < nInt ; i++){
		runningSum += log(thisPath[i]);
	}

	return exp(runningSum/double(nInt));
}

//method definition
void EuropeanOption::printPath(){

	for(int i = 0;  i < nInt; i++){

		std::cout << thisPath[i] << "\n";

	}

}

//method definition
double EuropeanOption::getArithmeticEuropeanPutPrice(int nReps){

	double rollingSum = 0.0;
	double lastPrice = 0.0;
	//double output = 0.0;

	for(int i = 0; i < nReps; i++){
		generatePath();
		lastPrice=thisPath.back();
		//std::cout << i << ":" << lastPrice  ;
		size_t iMax=0,iMin=0;
		for(size_t i=1; i<thisPath.size(); ++i)
		{
        	if(thisPath[iMax] < thisPath[i])
                iMax=i;
        	if(thisPath[iMin] > thisPath[i])
                iMin=i;
		}
		//std::cout << "minimal price: " << thisPath[iMin] << " ";
		rollingSum += ((lastPrice < strike) && (thisPath[iMin] >= barrier)) ? (strike - lastPrice) : 0;
		//output = ((lastPrice < strike) && (thisPath[iMin] >= barrier)) ? (strike - lastPrice) : 0;
		//std::cout << " payoff: "  << output  << '\n';
	}

	return exp(-r*expiry)*rollingSum/double(nReps);

}

//method definition
double EuropeanOption::getArithmeticEuropeanCallPrice(int nReps){

	double rollingSum = 0.0;
	double lastPrice = 0.0;
	//double output = 0.0;

	for(int i = 0; i < nReps; i++){
		generatePath();
		lastPrice=thisPath.back();
		//std::cout << i << ":" << lastPrice  ;
		size_t iMax=0,iMin=0;
		for(size_t i=1; i<thisPath.size(); ++i)
		{
        	if(thisPath[iMax] < thisPath[i])
                iMax=i;
        	if(thisPath[iMin] > thisPath[i])
                iMin=i;
		}
		//std::cout << "minimal price: " << thisPath[iMin] << " ";
		rollingSum += ((lastPrice > strike) && (thisPath[iMin] <= barrier)) ? (lastPrice - strike) : 0;
		//output = ((lastPrice > strike) && (thisPath[iMin] <= barrier)) ? (lastPrice - strike) : 0;
		//std::cout << " payoff: "  << output  << '\n';
	}

	return exp(-r*expiry)*rollingSum/double(nReps);

}

//method definition
double EuropeanOption::getGeometricEuropeanCallPrice(int nReps){

	double rollingSum = 0.0;
	double thisMean = 0.0;

	for(int i = 0; i < nReps; i++){
		generatePath();
		thisMean=getGeometricMean();
		rollingSum += (thisMean > strike)? (thisMean-strike) : 0;
	}

	return exp(-r*expiry)*rollingSum/double(nReps);

}

//method definition
double EuropeanOption::getGeometricEuropeanPutPrice(int nReps){

	double rollingSum = 0.0;
	double thisMean = 0.0;

	for(int i = 0; i < nReps; i++){
		generatePath();
		thisMean=getGeometricMean();
		rollingSum += (thisMean < strike) ? (strike - thisMean) : 0;
	}

	return exp(-r*expiry)*rollingSum/double(nReps);

}

//overloaded operator ();
double EuropeanOption::operator()(char char1, char char2, int nReps){
	if ((char1 == 'A') & (char2 =='C'))      return getArithmeticEuropeanCallPrice(nReps);
	else if ((char1 == 'A') & (char2 =='P')) return getArithmeticEuropeanPutPrice(nReps);
	else if ((char1 == 'G') & (char2 =='C')) return getGeometricEuropeanCallPrice(nReps);
	else if ((char1 == 'G') & (char2 =='P')) return getGeometricEuropeanPutPrice(nReps);
	else return -99;
}
