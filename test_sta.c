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

    Copyright 2019 Thomas Dauser, Remeis Observatory & ECAP
*/
#include "relbase.h"
#include "relmodels.h"
#include "relutility.h"
#include "reltable.h"


#include <sys/time.h> // can be removed!!! // TODO


#define LIMIT_PREC 1e-6

static void set_std_param_xillver(double* inp_par, int* status){
	inp_par[0] = 2.1;    // Gamma
	inp_par[1] = 1.0;    // Afe
	inp_par[2] = 300.0;  // Ecut
	inp_par[3] = 0.0;    // logxi
	inp_par[4] = 0.;     // redshift
	inp_par[5] = 45.0;   // inclination
	inp_par[6] = -1.0;   // refl. frac.
}


static void set_std_param_relline(double* inp_par, int* status){
	inp_par[0] = 6.4;
	inp_par[1] = 3.0;
	inp_par[2] = 3.0;
	inp_par[3] = 15.0;
	inp_par[4] = 0.998;
	inp_par[5] = 30.0;
	inp_par[6] = -1.1;
	inp_par[7] = 1000.;
	inp_par[8] = 0.0;
	inp_par[9] = 1.0;
}

static void set_std_param_relconv(double* inp_par, int* status){
	inp_par[0] = 3.0;
	inp_par[1] = 3.0;
	inp_par[2] = 15.0;
	inp_par[3] = 0.998;
	inp_par[4] = 30.0;
	inp_par[5] = -1.;
	inp_par[6] = 400.;
	inp_par[7] = 0.0;
}

static void set_std_param_relconvlp(double* inp_par, int* status){
	inp_par[0] = 3.0;
	inp_par[1] = 0.998;
	inp_par[2] = 30.0;
	inp_par[3] = -1.;
	inp_par[4] = 400.;
	inp_par[5] = 0.0;
	inp_par[6] = 2.0;
}


static void set_std_param_relxill(double* inp_par, int* status){
	inp_par[0]  = 3.0;
	inp_par[1]  = 3.0;
	inp_par[2]  = 15.0;
	inp_par[3]  = 0.998;
	inp_par[4]  = 60.0;
	inp_par[5]  = -1.0;
	inp_par[6]  = 400.;
	inp_par[7]  = 0.0;   // redshift
	inp_par[8]  = 2.1;   // pl Index
	inp_par[9]  = 0.0;   // logxi
	inp_par[10] = 1.0;   // Afe
	inp_par[11] = 300.0; // Ecut
	inp_par[12] = 3.0;   // refl_frac
}

static void set_std_param_relxill_nthcomp(double* inp_par, int* status){
	inp_par[0]  = 3.0;
	inp_par[1]  = 3.0;
	inp_par[2]  = 15.0;
	inp_par[3]  = 0.998;
	inp_par[4]  = 60.0;
	inp_par[5]  = -1.0;
	inp_par[6]  = 400.;
	inp_par[7]  = 0.0;   // redshift
	inp_par[8]  = 2.1;   // pl Index
	inp_par[9]  = 0.0;   // logxi
	inp_par[10] = 1.0;   // Afe
	inp_par[11] = 100.0; // kTe
	inp_par[12] = 0.0;   // refl_frac
}

static void set_std_param_relxilldens(double* inp_par, int* status){
	inp_par[0]  = 3.0;
	inp_par[1]  = 3.0;
	inp_par[2]  = 15.0;
	inp_par[3]  = 0.998;
	inp_par[4]  = 60.0;
	inp_par[5]  = -1.0;
	inp_par[6]  = 400.;
	inp_par[7]  = 0.0;   // redshift
	inp_par[8]  = 2.1;   // pl Index
	inp_par[9]  = 0.0;   // logxi
	inp_par[10] = 1.0;   // Afe
	inp_par[11] = 15.0; // logN
	inp_par[12] = 3.0;   // refl_frac
}


static void set_std_param_relxilllp(double* inp_par, int* status){
	inp_par[0]  = -1.1;   // height
	inp_par[1]  = 0.9798; // a
	inp_par[2]  = 60.0;  // incl
	inp_par[3]  = -1.0;  // rin
	inp_par[4]  = 1000.;  // rout
	inp_par[5]  = 0.0;    // redshift
	inp_par[6]  = 2.1;   // pl Index
	inp_par[7]  = 0.0;   // logxi
	inp_par[8]  = 1.0;   // Afe
	inp_par[9]  = 300.0; // Ecut
	inp_par[10] = 3.0;   // refl_frac
	inp_par[11] = 0.0;   // fixReflFrac
}


static void set_std_param_relxilllp_nthcomp(double* inp_par, int* status){
	inp_par[0]  = -1.1;   // height
	inp_par[1]  = 0.9798; // a
	inp_par[2]  = 60.0;  // incl
	inp_par[3]  = -1.0;  // rin
	inp_par[4]  = 400.;  // rout
	inp_par[5]  = 0.0;    // redshift
	inp_par[6]  = 2.1;   // pl Index
	inp_par[7]  = 0.0;   // logxi
	inp_par[8]  = 1.0;   // Afe
	inp_par[9]  = 100.0; // kTe
	inp_par[10] = 3.0;   // refl_frac
	inp_par[11] = 0.0;   // fixReflFrac
}

static void set_std_param_relxilllpdens(double* inp_par, int* status){
	inp_par[0]  = 3.0;   // height
	inp_par[1]  = 0.998; // a
	inp_par[2]  = 60.0;  // incl
	inp_par[3]  = -1.0;  // rin
	inp_par[4]  = 400.;  // rout
	inp_par[5]  = 0.0;    // redshift
	inp_par[6]  = 2.1;   // pl Index
	inp_par[7]  = 0.0;   // logxi
	inp_par[8]  = 1.0;   // Afe
	inp_par[9]  = 16.0; // logN
	inp_par[10] = -1.0;   // refl_frac
	inp_par[11] = 1.0;   // fixReflFrac
}


static void set_std_param_relline_lp(double* inp_par, int* status){
	inp_par[0] = 1.0;
	inp_par[1] = 3.0;
	inp_par[2] = 0.998;
	inp_par[3] = 30.0;
	inp_par[4] = -1.;
	inp_par[5] = 400.;
	inp_par[6] = 0.0;  // redshift
	inp_par[7] = 0.0;
	inp_par[8] = 2.0;  // gamma
}



static void set_std_param_relxilllpion(double* inp_par, int* status){
	inp_par[0]  = 6;   // height
	inp_par[1]  = 0.9; // a
	inp_par[2]  = 30.0;  // incl
	inp_par[3]  = -1.0;  // rin
	inp_par[4]  = 1000.;  // rout
	inp_par[5]  = 0.0;    // redshift
	inp_par[6]  = 2.0;   // pl Index
	inp_par[7]  = 3.00;   // logxi
	inp_par[8]  = 1.0;   // Afe
	inp_par[9]  = 300.0; // Ecut
	inp_par[10] = 0.0;     // beta
	inp_par[11] = 0;     // ion_grad_type
	inp_par[12] = 0.0;   // ion_grad_index
	inp_par[13] = 3.0;   // refl_frac
	inp_par[14] = 0.0;   // fixReflFrac
}

static void set_std_param_relxilllpion_nthcomp(double* inp_par, int* status){
	inp_par[0]  = 6;   // height
	inp_par[1]  = 0.9; // a
	inp_par[2]  = 30.0;  // incl
	inp_par[3]  = -1.0;  // rin
	inp_par[4]  = 1000.;  // rout
	inp_par[5]  = 0.0;    // redshift
	inp_par[6]  = 2.0;   // pl Index
	inp_par[7]  = 3.00;   // logxi
	inp_par[8]  = 1.0;   // Afe
	inp_par[9]  = 100.0; // kTe
	inp_par[10] = 0.0;     // beta
	inp_par[11] = 0;     // ion_grad_type
	inp_par[12] = 0.0;   // ion_grad_index
	inp_par[13] = 3.0;   // refl_frac
	inp_par[14] = 0.0;   // fixReflFrac
}


/** standard evaluation of the relline model **/
static void std_eval_relline(int* status, int n){

	printf("\n ==> Evaluating RELLINE MODEL \n");
	/* set the parameters */
	int n_param = NUM_PARAM_RELLINE;
	double inp_par[NUM_PARAM_RELLINE];
	set_std_param_relline(inp_par, status);
	CHECK_STATUS_VOID(*status);

	/* create an energy grid */
	int n_ener = 300;
	double ener[n_ener+1];
	get_log_grid(ener,n_ener+1,0.5,8.0);

	/* call the relline model */
	double photar[n_ener];
	int ii;
	if (n>2){
		for (ii=0; ii<n; ii++){
			inp_par[4] = 1.0*ii/(n-1)*0.998*2 - 0.998;
			tdrelline(ener,n_ener,photar,inp_par,n_param,status);
		}
	} else {
		inp_par[8] = 0.0;
		tdrelline(ener,n_ener,photar,inp_par,n_param,status);
		inp_par[8] = 1.0;
		tdrelline(ener,n_ener,photar,inp_par,n_param,status);
		inp_par[8] = 2.0;
		tdrelline(ener,n_ener,photar,inp_par,n_param,status);
		inp_par[8] = 0.0;
		tdrelline(ener,n_ener,photar,inp_par,n_param,status);
	}
}


/** standard evaluation of the relline model **/
static void std_eval_relconv(int* status, int n){

	printf("\n ==> Evaluating RELCONV MODEL \n");
	/* set the parameters */
	int n_param = NUM_PARAM_RELCONV;
	double inp_par[NUM_PARAM_RELCONV];
	set_std_param_relconv(inp_par, status);
	CHECK_STATUS_VOID(*status);

	/* create an energy grid */
	int n_ener = 2000;
	double ener[n_ener+1];
	get_log_grid(ener,n_ener+1,0.05,10.0);

	/* call the relline model */
	double photar[n_ener];

	int ii;
	double ener_line = 1.0;
	for (ii=0; ii<n_ener; ii++){
		photar[ii] = 0.0;
		if ((ener[ii]<ener_line) && ener[ii+1]>ener_line){
			photar[ii] = 1.0; // ener[1]-ener[0];
		}
	}

	// test output
	// save_xillver_spectrum(ener,photar,n_ener,"test_relconv_inp_spectrum.dat");
	tdrelconv(ener,n_ener,photar,inp_par,n_param,status);
	// save_xillver_spectrum(ener,photar,n_ener,"test_relconv_out_spectrum.dat");
}



/** standard evaluation of the relline model **/
static void std_eval_relconvlp(int* status, int n){

	printf("\n ==> Evaluating RELCONV_LP MODEL \n");
	/* set the parameters */
	int n_param = NUM_PARAM_RELCONVLP;
	double inp_par[NUM_PARAM_RELCONVLP];
	set_std_param_relconvlp(inp_par, status);
	CHECK_STATUS_VOID(*status);

	/* create an energy grid */
	int n_ener = 2000;
	double ener[n_ener+1];
	get_log_grid(ener,n_ener+1,0.05,10.0);

	/* call the relline model */
	double photar[n_ener];

	int ii;
	double ener_line = 1.0;
	for (ii=0; ii<n_ener; ii++){
		photar[ii] = 0.0;
		if ((ener[ii]<ener_line) && ener[ii+1]>ener_line){
			photar[ii] = 1.0; // ener[1]-ener[0];
		}
	}

	// test output
	// save_xillver_spectrum(ener,photar,n_ener,"test_relconv_inp_spectrum.dat");
	tdrelconvlp(ener,n_ener,photar,inp_par,n_param,status);
	save_xillver_spectrum(ener,photar,n_ener,"test_relconv_out_spectrum.dat");
}


/** standard evaluation of the relxill model **/
static void std_eval_relxill(int* status, int n){

	printf("\n ==> Evaluating RELXILL MODEL \n");
	/* set the parameters */
	int n_param = NUM_PARAM_RELXILL;
	double inp_par[NUM_PARAM_RELXILL];
	set_std_param_relxill(inp_par, status);
	CHECK_STATUS_VOID(*status);

	/* create an energy grid */
	int n_ener = 3000;
	double ener[n_ener+1];
	get_log_grid(ener,n_ener+1,0.1,1000.0);

	/* call the relline model */
	double photar[n_ener];

	int ii;
	 for (ii=0; ii<n; ii++){
		if (n>1){
			inp_par[3] = 1.0*ii/(n-1)*0.998*2 - 0.998;
			inp_par[7] = 1.0*ii/(n-1)*0.1 - 0.0;
			inp_par[7] = 0.0;
			inp_par[9] = 1.0*ii/(n-1)*0.1;
			tdrelxill(ener,n_ener,photar,inp_par,n_param,status);
		}
		// printf(" testing a=%.3f , lxi=%.2f \n",inp_par[3],inp_par[9]);
		//printf(" testing z=%.3f \n",inp_par[7]);
	}
	tdrelxill(ener,n_ener,photar,inp_par,n_param,status);

}

/** standard evaluation of the relxill model **/
static void std_eval_relxill_nthcomp(int* status, int n){

	printf("\n ==> Evaluating RELXILL NTHCOMP MODEL \n");
	/* set the parameters */
	int n_param = NUM_PARAM_RELXILL;
	double inp_par[NUM_PARAM_RELXILL];
	set_std_param_relxill_nthcomp(inp_par, status);
	CHECK_STATUS_VOID(*status);

	/* create an energy grid */
	int n_ener = 3000;
	double ener[n_ener+1];
	get_log_grid(ener,n_ener+1,0.1,1000.0);

	/* call the relline model */
	double photar[n_ener];

	/* int ii;
	 for (ii=0; ii<n; ii++){
		if (n>1){
			//			inp_par[3] = 1.0*ii/(n-1)*0.998*2 - 0.998;
			inp_par[7] = 1.0*ii/(n-1)*0.1 - 0.0;
			inp_par[7] = 0.0;
			// inp_par[9] = 1.0*ii/(n-1)*0.1;
		}
		// printf(" testing a=%.3f , lxi=%.2f \n",inp_par[3],inp_par[9]);
		printf(" testing z=%.3f \n",inp_par[7]);
	}*/
	tdrelxill_nthcomp(ener,n_ener,photar,inp_par,n_param,status);
	save_xillver_spectrum(ener,photar,n_ener,"test_relxillCp_spectrum.dat");
}


/** standard evaluation of the relxillD model **/
static void std_eval_relxilldens(int* status, int n){

	printf("\n ==> Evaluating RELXILL_DENS MODEL \n");
	/* set the parameters */
	int n_param = NUM_PARAM_RELXILL;
	double inp_par[NUM_PARAM_RELXILL];
	set_std_param_relxilldens(inp_par, status);
	CHECK_STATUS_VOID(*status);

	/* create an energy grid */
	int n_ener = 4000;
	double ener[n_ener+1];
	get_log_grid(ener,n_ener+1,0.1,1000.0);

	/* call the relline model */
	double photar[n_ener];
	int ii;
	for (ii=0; ii<n; ii++){
		if (n>1){
			inp_par[3] = 1.0*ii/(n-1)*0.998*2 - 0.998;
			inp_par[9] = 1.0*ii/(n-1)*4.7;
		}
		tdrelxilldens(ener,n_ener,photar,inp_par,n_param,status);
	}

}


/** standard evaluation of the relxill model **/
static void std_eval_relxilllp(int* status, int n){

	printf("\n ==> Evaluating RELXILLLP MODEL \n");
	/* set the parameters */
	int n_param = NUM_PARAM_RELXILLLP;
	double inp_par[NUM_PARAM_RELXILLLP];
	set_std_param_relxilllp(inp_par, status);
	CHECK_STATUS_VOID(*status);

	/* create an energy grid */
	const int n_ener = 100;
	double ener[n_ener+1];
	get_log_grid(ener,n_ener+1,0.1,1000.0);

	/* call the relline model */
	double photar[n_ener];
	int ii;


	struct timeval start, end;
	long seconds, useconds;
	gettimeofday(&start, NULL);


	for (ii=0; ii<n; ii++){
		if (n>1){
	//		inp_par[1] = 1.0*ii/(n-1)*0.998*2 - 0.998;
	//		inp_par[7] = 1.0*ii/(n-1)*4.7;
			printf(" relxilllp: testing a=%.3f , lxi=%.2f \n",inp_par[1],inp_par[7]);
		}
		tdrelxilllp(ener,n_ener,photar,inp_par,n_param,status);
		putenv("RELXILL_NUM_RZONES=2");
		tdrelxilllp(ener,n_ener,photar,inp_par,n_param,status);

	}
	double sum=0.0;
	int jj;
	for (jj=0;jj<n_ener;jj++){
		sum += photar[jj];
	}
	printf(" integ flux = %.2e \n",sum);

	gettimeofday(&end, NULL);


	if (n>1){
		seconds  = end.tv_sec  - start.tv_sec;
		useconds = end.tv_usec - start.tv_usec;

		double mtime = ((seconds) * 1000 +  useconds*0.001) / ((double) n );

		printf("time per relxilllp evaluation: %.1f milli seconds\n", mtime);
	}

	// test output
	// save_xillver_spectrum(ener,photar,n_ener,"test_relxilllp_spectrum.dat");

}



/** standard evaluation of the relxill model **/
static void std_eval_relxilllpion(int* status, int n){

	printf("\n ==> Evaluating RELXILLLPION MODEL \n");
	/* set the parameters */
	int n_param = NUM_PARAM_RELXILLLPION;
	double inp_par[NUM_PARAM_RELXILLLPION];
	set_std_param_relxilllpion(inp_par, status);
	CHECK_STATUS_VOID(*status);

	/* create an energy grid */
	const int n_ener = 1000;
	double ener[n_ener+1];
	get_log_grid(ener,n_ener+1,0.1,1000.0);

	/* call the relline model */
	double photar[n_ener];

	struct timeval start, end;
	long seconds, useconds;
	gettimeofday(&start, NULL);

	int ii;
	for (ii=0; ii<n; ii++){
		if (n>1){
			inp_par[1] = 1.0*ii/(n-1)*0.998*2 - 0.998;
	//		inp_par[7] = 1.0*ii/(n-1)*4.7;
			printf(" relxilllpion: testing a=%.3f , lxi=%.2f \n",inp_par[1],inp_par[7]);
		}
		tdrelxilllpion(ener,n_ener,photar,inp_par,n_param,status);
	}

	gettimeofday(&end, NULL);


	if (n>1){
		seconds  = end.tv_sec  - start.tv_sec;
		useconds = end.tv_usec - start.tv_usec;

		double mtime = ((seconds) * 1000 +  useconds*0.001) / ((double) n );

		printf("time per relxilllpion evaluation: %.1f milli seconds\n", mtime);
	}

}


/** standard evaluation of the relxill model **/
static void std_eval_relxilllp_nthcomp(int* status, int n){

	printf("\n ==> Evaluating RELXILLLP NTHCOMP MODEL \n");
	/* set the parameters */
	int n_param = NUM_PARAM_RELXILLLP;
	double inp_par[NUM_PARAM_RELXILLLP];
	set_std_param_relxilllp_nthcomp(inp_par, status);
	CHECK_STATUS_VOID(*status);

	/* create an energy grid */
	const int n_ener = 100;
	double ener[n_ener+1];
	get_log_grid(ener,n_ener+1,0.1,1000.0);

	/* call the relline model */
	double photar[n_ener];
	int ii;

	for (ii=0; ii<n; ii++){
		if (n>1){
			inp_par[1] = 1.0*ii/(n-1)*0.998*2 - 0.998;
	//		inp_par[7] = 1.0*ii/(n-1)*4.7;
			printf(" relxilllp (nthcomp): testing a=%.3f , lxi=%.2f \n",inp_par[1],inp_par[7]);
		}
		tdrelxilllp_nthcomp(ener,n_ener,photar,inp_par,n_param,status);

	}

	// save_xillver_spectrum(ener,photar,n_ener,"test_relxilllp_spectrum.dat");

}




/** standard evaluation of the relxill model **/
static void std_eval_relxilllpion_nthcomp(int* status, int n){

	printf("\n ==> Evaluating RELXILLLP ION NTHCOMP MODEL \n");
	/* set the parameters */
	int n_param = NUM_PARAM_RELXILLLP;
	double inp_par[NUM_PARAM_RELXILLLP];
	set_std_param_relxilllpion_nthcomp(inp_par, status);
	CHECK_STATUS_VOID(*status);

	/* create an energy grid */
	const int n_ener = 100;
	double ener[n_ener+1];
	get_log_grid(ener,n_ener+1,0.1,1000.0);

	/* call the relline model */
	double photar[n_ener];
	int ii;

	for (ii=0; ii<n; ii++){
		if (n>1){
			inp_par[1] = 1.0*ii/(n-1)*0.998*2 - 0.998;
			printf(" relxilllpion (nthcomp): testing a=%.3f , lxi=%.2f \n",inp_par[1],inp_par[7]);
		}
		tdrelxilllp_nthcomp(ener,n_ener,photar,inp_par,n_param,status);

	}

}


/** standard evaluation of the relxill model **/
static void std_eval_relxilllpdens(int* status, int n){

	printf("\n ==> Evaluating RELXILLLP_DENS MODEL \n");
	/* set the parameters */
	int n_param = NUM_PARAM_RELXILLLP;
	double inp_par[NUM_PARAM_RELXILLLP];
	set_std_param_relxilllpdens(inp_par, status);
	CHECK_STATUS_VOID(*status);

	/* create an energy grid */
	const int n_ener = 100;
	double ener[n_ener+1];
	get_log_grid(ener,n_ener+1,0.1,1000.0);

	/* call the relline model */
	double photar[n_ener];
	int ii;


	struct timeval start, end;
	long seconds, useconds;
	gettimeofday(&start, NULL);

	for (ii=0; ii<n; ii++){
		if (n>1){
			inp_par[1] = 1.0*ii/(n-1)*0.998*2 - 0.998;
			inp_par[7] = 1.0*ii/(n-1)*4.7;
			printf(" relxilllp: testing a=%.3f , lxi=%.2f \n",inp_par[1],inp_par[7]);
		}
		tdrelxilllpdens(ener,n_ener,photar,inp_par,n_param,status);
	}

	gettimeofday(&end, NULL);

	if (n>1){
		seconds  = end.tv_sec  - start.tv_sec;
		useconds = end.tv_usec - start.tv_usec;

		double mtime = ((seconds) * 1000 +  useconds*0.001) / ((double) n );

		printf("time per relxilllp evaluation: %.1f milli seconds\n", mtime);
	}

}

/** standard evaluation of the relline model **/
static void std_eval_relline_lp(int* status, int n){

	printf("\n ==> Evaluating RELLINE LP MODEL \n");
	/* set the parameters */
	int n_param = NUM_PARAM_RELLINELP;
	double inp_par_lp[n_param];
	set_std_param_relline_lp(inp_par_lp, status);
	CHECK_STATUS_VOID(*status);

	/* create an energy grid */
	int n_ener = 600;
	double ener[n_ener+1];
	get_log_grid(ener,n_ener+1,0.1,1.5);

	/* call the relline model */
	double photar[n_ener];
	int ii;
	for (ii=0; ii<n; ii++){
//		tdrellinelp(ener,n_ener,photar,inp_par_lp,n_param,status);
		inp_par_lp[2] = 0.998;
		tdrellinelp(ener,n_ener,photar,inp_par_lp,n_param,status);
		tdrellinelp(ener,n_ener,photar,inp_par_lp,n_param,status);
		inp_par_lp[2] = 0.99;
		tdrellinelp(ener,n_ener,photar,inp_par_lp,n_param,status);
		inp_par_lp[2] = 0.998;
		tdrellinelp(ener,n_ener,photar,inp_par_lp,n_param,status);
		CHECK_STATUS_VOID(*status);
	}



}

/** standard evaluation of the relline model **/
static void std_eval_xillver(int* status, int n){

	printf("\n ==> Evaluating XILLVER MODEL \n");
	/* set the parameters */
	int n_param = NUM_PARAM_XILLVER;
	double inp_par[n_param];
	set_std_param_xillver(inp_par, status);
	CHECK_STATUS_VOID(*status);

	/* create an energy grid */
	int n_ener = 2000;
	double ener[n_ener+1];
	get_log_grid(ener,n_ener+1,0.08,500.0);


	/* call the relline model */
	double photar[n_ener];
	int ii;
	for (ii=0; ii<n; ii++){
		tdxillver(ener,n_ener,photar,inp_par,n_param,status);
		CHECK_STATUS_VOID(*status);
	}

	// test output
	// save_xillver_spectrum(ener,photar,n_ener,"test_xillver_spectrum.dat");


}


/** test the currently implemented relline table
 ** [current version used: rel_table_v0.4e]   */
void test_relline_table(int* status){

	printf("\n ==> Testing RELLINE TABLE (%s) \n",RELTABLE_FILENAME);
	relTable* tab=NULL;

	do{
		// load the table
		read_relline_table(RELTABLE_FILENAME,&tab,status);
		CHECK_RELXILL_ERROR("loading the rel table failed",status);

		// test certain values
		assert(tab!=NULL);
		assert(tab->arr!=NULL);

		double aref_val = 0.9829549;
		if ( fabs(tab->a[RELTABLE_NA-2] - aref_val) > LIMIT_PREC ){
			printf(" testing spin: expecting value of %f, but found %f\n",
					aref_val,tab->a[RELTABLE_NA-2]);
			RELXILL_ERROR("values in rel table not correct",status);
			break;
		}

		double mu0ref_val = 0.09476821;
		if ( fabs(tab->mu0[1] - mu0ref_val) > LIMIT_PREC ){
			printf(" testing mu0: expecting value of %f, but found %f\n",mu0ref_val,tab->mu0[1]);
			RELXILL_ERROR("values in rel table not correct",status);
			break;
		}

		const int n = 5;
		const float ref_val[5] = {985.76074, 0.01127052,0.01120779 , 0.01121218, 0.03022655};
		const float val[5] = {
				tab->arr[0][0]->r[1],
				tab->arr[0][0]->trff1[0][0],
				tab->arr[0][0]->trff1[0][1],
				tab->arr[0][0]->trff1[1][0],
				tab->arr[0][1]->trff1[1][0]
			};
		for (int ii=0; ii<n; ii++){
			if ( fabs( ref_val[ii] - val[ii]) > LIMIT_PREC ){
				printf(" testing rel table: expecting value of %f, but found %f\n",ref_val[ii],val[ii]);
				RELXILL_ERROR("values in rel table not correct",status);
				break;
			}
		}
		printf("  ### all values of the relline table as expected\n");

	} while(0);

	// free memory
	free_relTable(tab);

	return;
}


/** test the currently implemented relline table
 ** [current version used: rel_table_v0.4e]   */
void test_lp_table(int* status){

	printf("\n ==> Testing LP TABLE (%s) \n",LPTABLE_FILENAME);
	lpTable* tab=NULL;

	do{
		// load the table
		read_lp_table(LPTABLE_FILENAME,&tab,status);
		CHECK_RELXILL_ERROR("loading the rel table failed",status);

		// test certain values
		assert(tab!=NULL);
		assert(tab->dat!=NULL);
		assert(tab->a!=NULL);

		double aref_val = 0.98374581;
		if ( fabs(tab->a[LPTABLE_NA-2] - aref_val) > LIMIT_PREC ){
			printf(" testing spin: expecting value of %f, but found %f\n",
					aref_val,tab->a[LPTABLE_NA-2]);
			RELXILL_ERROR("values in rel table not correct",status);
			break;
		}

		double href_val = 1.8294963;
		if ( fabs(tab->dat[1]->h[1] - href_val) > LIMIT_PREC ){
			printf(" testing hgrid: expecting value of %f, but found %f\n",href_val,tab->dat[1]->h[1]);
			RELXILL_ERROR("values in rel table not correct",status);
			break;
		}

		const int n = 3;
		const float ref_val[3] = {7.618106e-05, 2.6826601, -1.2509402 };
		const float val[3] = {
				tab->dat[1]->intens[2][1],
				tab->dat[1]->del[2][1],
				tab->dat[1]->del_inc[2][1]
			};
		for (int ii=0; ii<n; ii++){
			if ( fabs( (ref_val[ii] - val[ii]) / ref_val[ii] ) > LIMIT_PREC ){
				printf(" testing lp table: expecting value of %e, but found %e\n",ref_val[ii],val[ii]);
				RELXILL_ERROR("values in lp table not correct",status);
				break;
			}
		}
		printf("  ### all values of the LP table as expected\n");

	} while(0);

	// free memory
	free_lpTable(tab);

	return;
}


static void test_rebin_spectrum(int* status){

	const int n0 = 6;
	const int n = 4;
	double ener0[] = {1,3,4,5,6,7,9};
	double val0[] =  {1,1,1,2,1,1};

	double ener[] =  {0.5,2,4,5.5,7.5,8};
	double val[n];

	rebin_spectrum(ener,val,n,ener0,val0,n0);

	double val_ref[5];
	val_ref[0] = 0.5;
	val_ref[1] = 0.5+1;
	val_ref[2] = 1.0+1.0;
	val_ref[3] = 1.0+1.0+0.25;
	val_ref[4] = 0.25;

	printf("\n ==> Testing REBINNING Functions\n");

	int ii;
	for (ii=0; ii<n; ii++){
		// printf(" %i %e - %e \n",ii,val[ii],val_ref[ii]);
		if ( fabs(val[ii] - val_ref[ii] ) > LIMIT_PREC ){
			RELXILL_ERROR(" TEST-ERROR: testing of the function 'rebin_spectrum' failed",status);
			return;
		}
	}

}

static void test_interp(int* status){

	printf("\n ==> Testing INTERPOLATION Routines \n");

	double ifac = 0.2;
	double rlo = 1.0;
	double rhi = 2.0;
	double val_1d = 1.2;
	if (fabs(interp_lin_1d(ifac,rlo,rhi)-val_1d) > LIMIT_PREC){
		RELXILL_ERROR(" TEST-ERROR: 1d interpolation does not produce the expected result",status);
	}


	double ifac1 = 0.4;
	double ifac2 = 0.8;
	double r11 = 1.0;
	double r12 = 2.0;
	double r21 = 2.0;
	double r22 = 4.0;
	double val_2d = 2.52;
	if (fabs(interp_lin_2d(ifac1,ifac2,r11,r12,r21,r22)-val_2d) > LIMIT_PREC){
		RELXILL_ERROR(" TEST-ERROR: 2d interpolation does not produce the expected result",status);
		printf(" VALUE=%e instead of %e \n",interp_lin_2d(ifac1,ifac2,r11,r12,r21,r22),val_2d);
	}

	float rf11 = 1.0;
	float rf12 = 2.0;
	float rf21 = 2.0;
	float rf22 = 4.0;
	if (fabs(interp_lin_2d_float(ifac1,ifac2,rf11,rf12,rf21,rf22)-val_2d) > LIMIT_PREC){
		RELXILL_ERROR(" TEST-ERROR: 2d interpolation (float) does not produce the expected result",status);
	}


}

static void do_std_test(int* status){

	test_relline_table(status);
	CHECK_STATUS_VOID(*status);
	printf("     ---> successful \n");

	test_lp_table(status);
	CHECK_STATUS_VOID(*status);
	printf("     ---> successful \n");


	test_interp(status);
	CHECK_STATUS_VOID(*status);
	printf("     ---> successful \n");

	test_rebin_spectrum(status);
	CHECK_STATUS_VOID(*status);
	printf("     ---> successful \n");

}


int main(int argc, char *argv[]){
	char *buf;
	int status = EXIT_SUCCESS;



	int do_all = 1;
	int do_relline = 0;
	int do_rellinelp = 0;
	int do_relxill = 0;
	int do_relxilllp = 0;
	int do_relxilllpion = 0;
	int do_relxilldens = 0;
	int do_relxilllpdens = 0;
	int do_relxillnthcomp= 0;
	int do_relxilllpnthcomp = 0;
	int do_relxilllpionnthcomp = 0;
	int do_relconv = 0;

//	putenv("DEBUG_RELXILL=1");

	if (argc>=2){
		if (strcmp(argv[1],"version")==0){
			get_version_number(&buf,&status);
			printf("%s",buf);
			free(buf);
			return status;
		}

		if (strcmp(argv[1],"relxilllp")==0){
			do_relxilllp=1;
			do_all=0;
		} else if (strcmp(argv[1],"relline")==0){
				do_relline=1;
				do_all=0;
		} else if (strcmp(argv[1],"relxilllpion")==0){
				do_relxilllpion=1;
				do_all=0;
		} else if (strcmp(argv[1],"relxilllpionCp")==0){
				do_relxilllpionnthcomp=1;
				do_all=0;
		} else if (strcmp(argv[1],"relconv")==0){
				do_relconv=1;
				do_all=0;
		} else if (strcmp(argv[1],"rellinelp")==0){
				do_rellinelp=1;
				do_all=0;
		} else if (strcmp(argv[1],"relxilldens")==0){
				do_relxilldens=1;
				do_all=0;
		} else if (strcmp(argv[1],"relxill")==0){
				do_relxill=1;
				do_all=0;
		} else if (strcmp(argv[1],"relxilllpdens")==0){
			do_relxilllpdens=1;
			do_all=0;
		} else if (strcmp(argv[1],"relxillCp")==0){
				do_relxillnthcomp=1;
				do_all=0;
		} else if (strcmp(argv[1],"relxilllpCp")==0){
			do_relxilllpnthcomp=1;
			do_all=0;

		}

	}

	int n = 1;
	if (argc==3){
		n = atof(argv[2]);
	}

	do{
		get_version_number(&buf,&status);
		printf("\n === Starting RELXILL Version %s === \n\n",buf);
		free(buf);

		if (do_all){
			do_std_test(&status);
			CHECK_STATUS_BREAK(status);
		}

		if (do_all || do_relline){
			std_eval_relline(&status,n);
			CHECK_STATUS_BREAK(status);
			printf("     ---> successful \n");
		}

		if (do_all || do_rellinelp){
			std_eval_relline_lp(&status,1);
			CHECK_STATUS_BREAK(status);
			printf("     ---> successful \n");
		}

		if (do_relconv){

			std_eval_relconv(&status,1);
			CHECK_STATUS_BREAK(status);
			std_eval_relconvlp(&status,1);
			CHECK_STATUS_BREAK(status);
			printf("     ---> successful \n");

		}

		if (do_all | do_relxilllpion){
			std_eval_relxilllpion(&status,n);
			CHECK_STATUS_BREAK(status);
			printf("     ---> successful \n");

		}

		if (do_all | do_relxilllpionnthcomp){
			std_eval_relxilllpion_nthcomp(&status,n);
			CHECK_STATUS_BREAK(status);
			printf("     ---> successful \n");

		}


		if (do_all) {
			std_eval_xillver(&status,1);
			CHECK_STATUS_BREAK(status);
			printf("     ---> successful \n");

		}


		if (do_all || do_relxill){
			std_eval_relxill(&status,n);
			CHECK_STATUS_BREAK(status);
			printf("     ---> successful \n");
		}

		if (do_all || do_relxilllp){
			std_eval_relxilllp(&status,n);
			CHECK_STATUS_BREAK(status);
			printf("     ---> successful \n");
		}

		if (do_all || do_relxillnthcomp){
			std_eval_relxill_nthcomp(&status,n);
			CHECK_STATUS_BREAK(status);
			printf("     ---> successful \n");
		}

		if (do_all || do_relxilllpnthcomp){
			std_eval_relxilllp_nthcomp(&status,n);
			CHECK_STATUS_BREAK(status);
			printf("     ---> successful \n");
		}

		if (do_all || do_relxilldens){
			std_eval_relxilldens(&status,n);
			CHECK_STATUS_BREAK(status);
			printf("     ---> successful \n");
		}

		if (do_all || do_relxilllpdens){
			std_eval_relxilllpdens(&status,n);
			CHECK_STATUS_BREAK(status);
			printf("     ---> successful \n");
		}

		printf( "\n ==> Cleaning up and freeing cached structures\n");
		free_cached_tables();

	} while(0);

	if(status!=EXIT_SUCCESS){
		printf(" *** TESTING NOT SUCCESSFUL \n");
		// free tables
		printf( "\n *** Cleaning up and freeing cached structures\n");
		free_cached_tables();
	}

}
