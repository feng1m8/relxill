/*
   This file is part of the RELXILL model code.

   RELXILL is free software: you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   any later version.

   RELXILL is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
   GNU General Public License for more details.
   For a copy of the GNU General Public License see
   <http://www.gnu.org/licenses/>.

    Copyright 2016 Thomas Dauser, Remeis Observatory & ECAP
*/

#include "relutility.h"



// TODO: CHECK IF WE NEED "INLINE" HERE for maximal speed
/** linear interpolation in 1 dimension **/
double interp_lin_1d(double ifac_r, double rlo, double rhi){
	return ifac_r*rhi + (1.0-ifac_r)*rlo;
}

// TODO: CHECK IF WE NEED "INLINE" HERE for maximal speed
/** linear interpolation in 2 dimensions **/
double interp_lin_2d(double ifac1, double ifac2, double r11, double r12, double r21, double r22){
	return (1.0 - ifac1) * (1.0 - ifac2) * r11 +
		   (ifac1)       * (1.0 - ifac2) * r12 +
		   (1.0 - ifac1) * (ifac2)       * r21 +
		   (ifac1)       * (ifac2)       * r22;
}

double interp_lin_2d_float(double ifac1, double ifac2, float r11, float r12, float r21, float r22){
	return (1.0 - ifac1) * (1.0 - ifac2) * r11 +
		   (ifac1)       * (1.0 - ifac2) * r12 +
		   (1.0 - ifac1) * (ifac2)       * r21 +
		   (ifac1)       * (ifac2)       * r22;
}

void relxill_error(const char* const func, const char* const msg, int* status){
	*status = EXIT_FAILURE;
	printf(" *** error in relxill (%s): %s!\n", func, msg);
}

void check_relxill_error(const char* const func, const char* const msg, int* status){
	if (*status!=EXIT_SUCCESS){
		*status = EXIT_FAILURE;
		printf(" *** error in relxill (%s): %s!\n", func, msg);
	}
}

void get_version_number(char** vstr, int* status){
	if (asprintf(vstr, "%i.%i.%i", version_major, version_minor, version_build) == -1){
		RELXILL_ERROR("failed to get version number",status);
	}
}

/**  search for value "val" in array "arr" (sorted ASCENDING!) with length n and
 	 return bin k for which arr[k]<=val<arr[k+1] **/
int binary_search_float(float* arr,int n,float val){

	int klo=0;
	int khi=n-1;
	int k=-1;
	while ( (khi-klo) > 1 ){
		k=(khi+klo)/2;
		if(arr[k]>val){
			khi=k;
		} else {
			klo=k;
		}
	}
	return klo;
}

/**  search for value "val" in array "arr" (sorted DESCENDING!) with length n and
 	 return bin k for which arr[k]<=val<arr[k+1] **/
int inv_binary_search_float(float* arr,int n,float val){

	int klo=0;
	int khi=n-1;
	int k=-1;
	while ( (khi-klo) > 1 ){
		k=(khi+klo)/2;
		if(arr[k]<val){
			khi=k;
		} else {
			klo=k;
		}
	}
	return klo;
}


/**  search for value "val" in array "arr" (sorted DESCENDING!) with length n and
 	 return bin k for which arr[k]<=val<arr[k+1] **/
int inv_binary_search(double* arr,int n,double val){

	int klo=0;
	int khi=n-1;
	int k=-1;
	while ( (khi-klo) > 1 ){
		k=(khi+klo)/2;
		if(arr[k]<val){
			khi=k;
		} else {
			klo=k;
		}
	}
	return klo;
}


/* get a logarithmic grid from emin to emax with n_ener bins  */
void get_log_grid(double* ener, int n_ener, double emin, double emax){
	for (int ii=0; ii<n_ener; ii++){
		ener[ii] = 1.0*ii / (n_ener-1) * ( log(emax) - log(emin)) + log(emin);
		ener[ii] = exp(ener[ii]);
	}
}

/* get RMS (ISCO) for the Kerr Case */
double kerr_rms(double a){
	//	 accounts for negative spin
  double sign = 1.0;
  if (a<0) {
     sign = -1.0;
  }

  double Z1 = 1.0+pow(1.0-a*a,1.0/3.0)*(pow(1.0+a,1.0/3.0)+pow(1.0-a,1.0/3.0));
  double Z2=sqrt((3.0*a*a)+(Z1*Z1));

  return 3.0+Z2-sign*sqrt((3.0-Z1)*(3.0+Z1+(2*Z2)));
}
