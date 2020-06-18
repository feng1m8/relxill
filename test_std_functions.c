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

    Copyright 2020 Thomas Dauser, Remeis Observatory & ECAP
*/

#include "test_std_functions.h"

#include "relutility.h"
#include "relbase.h"
#define LIMIT_PREC 1e-6
/** test the currently implemented relline table
 ** [current version used: rel_table_v0.4e]   */
void testRellineTableValues(int* status){

  PRINT_RELXILL_TEST_MSG(RELTABLE_FILENAME);

  relTable* tab=NULL;

    // load the table
    read_relline_table(RELTABLE_FILENAME, &tab, status);
    CHECK_RELXILL_ERROR("loading the rel table failed", status);
    CHECK_STATUS_VOID(*status);

    // test certain values
    assert(tab != NULL);
    assert(tab->arr != NULL);

    double aref_val = 0.9829549;
    if (fabs(tab->a[RELTABLE_NA - 2] - aref_val) > LIMIT_PREC) {
        printf(" testing spin: expecting value of %f, but found %f\n",
               aref_val, tab->a[RELTABLE_NA - 2]);
        RELXILL_ERROR("values in rel table not correct", status);
    }

    double mu0ref_val = 0.09476821;
    if (fabs(tab->mu0[1] - mu0ref_val) > LIMIT_PREC) {
        printf(" testing mu0: expecting value of %f, but found %f\n", mu0ref_val, tab->mu0[1]);
        RELXILL_ERROR("values in rel table not correct", status);
    }

    const int n = 5;
    const float ref_val[5] = {(float) 985.76074, (float) 0.01127052,
                              (float) 0.01120779, (float) 0.01121218, (float) 0.03022655};
    const float val[5] = {
            tab->arr[0][0]->r[1],
            tab->arr[0][0]->trff1[0][0],
            tab->arr[0][0]->trff1[0][1],
            tab->arr[0][0]->trff1[1][0],
            tab->arr[0][1]->trff1[1][0]
    };
    int ii;
    for (ii = 0; ii < n; ii++) {
        if (fabsf(ref_val[ii] - val[ii]) > LIMIT_PREC) {
            printf(" testing rel table: expecting value of %f, but found %f\n", ref_val[ii], val[ii]);
            RELXILL_ERROR("values in rel table not correct", status);
            continue;
        }
    }


  // free memory
  free_relTable(tab);

  print_relxill_test_result(*status);


}
/** test the currently implemented relline table
 ** [current version used: rel_table_v0.4e]   */
void testLPTableValues(int* status){

  PRINT_RELXILL_TEST_MSG(LPTABLE_FILENAME);
  lpTable* tab=NULL;

  // load the table
  read_lp_table(LPTABLE_FILENAME, &tab, status);
  CHECK_RELXILL_ERROR("loading the rel table failed", status);

  // test certain values
  assert(tab != NULL);
  assert(tab->dat != NULL);
  assert(tab->a != NULL);

  double aref_val = 0.98374581;
  if (fabs(tab->a[LPTABLE_NA - 2] - aref_val) > LIMIT_PREC) {
    printf(" testing spin: expecting value of %f, but found %f\n",
           aref_val, tab->a[LPTABLE_NA - 2]);
    RELXILL_ERROR("values in rel table not correct", status);
  }

  double href_val = 1.8294963;
  if (fabs(tab->dat[1]->h[1] - href_val) > LIMIT_PREC) {
    printf(" testing hgrid: expecting value of %f, but found %f\n", href_val, tab->dat[1]->h[1]);
    RELXILL_ERROR("values in rel table not correct", status);
  }

  const int n = 3;
  const float ref_val[3] = {(float) 7.618106e-05, (float) 2.6826601, (float) -1.2509402};
  const float val[3] = {
      tab->dat[1]->intens[2][1],
      tab->dat[1]->del[2][1],
      tab->dat[1]->del_inc[2][1]
  };
  int ii;
  for (ii = 0; ii < n; ii++) {
    if (fabsf((ref_val[ii] - val[ii]) / ref_val[ii]) > LIMIT_PREC) {
      printf(" testing lp table: expecting value of %e, but found %e\n", ref_val[ii], val[ii]);
      RELXILL_ERROR("values in lp table not correct", status);
      continue;
    }
  }

  // free memory
  free_lpTable(tab);

  print_relxill_test_result(*status);
}
static void testRebinSpectrum(int* status){

  PRINT_RELXILL_TEST_MSG_DEFAULT();

  int n0 = 6;
  int n = 5;
  double ener0[] = {1, 3, 4, 5, 6, 7, 9};
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

  for (int ii = 0; ii < n; ii++) {
    if (fabs(val[ii] - val_ref[ii]) > LIMIT_PREC) {
      RELXILL_ERROR(" TEST-ERROR: testing of the function 'rebin_spectrum' failed", status);
      continue;
    }
  }

  print_relxill_test_result(*status);

}
static void testRebinMeanFlux(int *status) {

  PRINT_RELXILL_TEST_MSG_DEFAULT();

  int n0 = 7;
  int n = 4;
  double ener0[] = {1, 2, 4, 5, 6, 7, 9, 10};
  double val0[] = {1, 4, 3, 2, 4, 1, 2};

  double ener[] = {1, 3, 4, 8.5, 10};
  double val[] = {0, 0, 0, 0};

  rebin_mean_flux(ener, val, n, ener0, val0, n0, status);

  double val_ref[4];
  val_ref[0] = 2.0;
  val_ref[1] = 3.6666666666666666666666;
  val_ref[2] = 3.5;
  val_ref[3] = 1.83333333333333333333;

  for (int ii = 0; ii < n; ii++) {
    if (fabs(val[ii] - val_ref[ii]) > LIMIT_PREC) {
      printf(" expecting %e but cacluated %e \n", val_ref[ii], val[ii]);
      RELXILL_ERROR(" TEST-ERROR: testing of the function 'rebin_spectrum' failed", status);
      continue;
    }
  }

  print_relxill_test_result(*status);
}
static void testInterpolationRoutines(int *status) {

  PRINT_RELXILL_TEST_MSG_DEFAULT();


  double ifac = 0.2;
  double rlo = 1.0;
  double rhi = 2.0;
  double val_1d = 1.2;
  if (fabs(interp_lin_1d(ifac, rlo, rhi) - val_1d) > LIMIT_PREC) {
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

    float rf11 = (float) 1.0;
    float rf12 = (float) 2.0;
    float rf21 = (float) 2.0;
    float rf22 = (float) 4.0;
	if (fabs(interp_lin_2d_float(ifac1,ifac2,rf11,rf12,rf21,rf22)-val_2d) > LIMIT_PREC){
		RELXILL_ERROR(" TEST-ERROR: 2d interpolation (float) does not produce the expected result",status);
	}

  print_relxill_test_result(*status);

}
void testStdFunctions(int* status) {

  printf("\n### TESTING Standard Functions ###\n");

  testRellineTableValues(status);

  testLPTableValues(status);

  testInterpolationRoutines(status);

  testRebinSpectrum(status);

  testRebinMeanFlux(status);

}
