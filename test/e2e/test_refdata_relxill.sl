#!/usr/bin/env isis-script
% -*- mode: slang; mode: fold -*-

require("isisscripts");
require("subs_fitfunctions.sl");
require("fits_model_struct");

require("test_setup.sl");

load_relxill_model_devel("build/librelxill.so");

define get_refdata_files(){ %{{{
   %% we can load any model
   variable ff = "*";
   if (length(__argv)>1){
      ff = __argv[1];
   }
   
   variable glob_str = sprintf("*/*_*_ref*_*.fits",ff);
   variable fnames = glob(LMOD_REFDATA_DIR+sprintf(glob_str));
   
   return fnames[array_sort(fnames)];   
}
%}}}

define conv2tex(str){ %{{{
   return strreplace(str,`_`,`\_`);
}
%}}}

define add_labels(pl,goodness,fn){ %{{{

   variable ff_tex = conv2tex(get_fit_fun());
   variable str_tex = conv2tex(fn); 
   pl.add_object(xfig_new_text(sprintf(`%s [goodness %.3e] --- %s`,
				       ff_tex,goodness,str_tex	       )),
		 0.5,1.05,0,0;world0);
   
   variable params = get_params();
   
   variable ii, n=length(params);
   
   variable str;
   _for ii(0,n-1,1){
      str = sprintf(`%s : %.3e`, strreplace(params[ii].name,`_`,`\_`),
		    params[ii].value);
      pl.add_object(xfig_new_text(str;
				 size=`\footnotesize`),
		    1.02,0.95,-0.5,0+0.8*ii;world0);            
   }
}
%}}}
define plot_model_comparison(fn,dat,goodness){ %{{{
   
   variable pl = xfig_plot_new(15,8);
   variable plr = xfig_plot_new(15,4);
   
   variable elo = dat.bin_lo;
   variable ehi = dat.bin_hi;
   variable m_dat = dat.model;
   
   pl.world(min(elo),max(elo),0.1,200;loglog);
   plr.world(min(elo),max(elo),0.989,1.011;loglog);
   
   
   pl.hplot([elo,ehi[-1]], dat.value/(ehi-elo)*(0.5*(elo+ehi))^2;
	    loglog);
   pl.hplot([elo,ehi[-1]], m_dat/(ehi-elo)*(0.5*(elo+ehi))^2 ;
	    loglog, color="red", depth=-1 );
   
   plr.hplot([elo,ehi[-1]], dat.value*0+1; loglog);
   
   plr.hplot([elo,ehi[-1]], m_dat/dat.value; loglog, color="red");
   
   pl.ylabel(" Energy Flux ");
   plr.ylabel(" Ratio New/Old ");
   plr.xlabel("Energy [keV]" );
   
        
   add_labels(pl,goodness,fn);
   
   
   xfig_multiplot(pl,plr).render(fn+".pdf");
}
%}}}

define check_single_model(fn){ %{{{

   variable status = EXIT_SUCCESS;

   variable dat = fits_read_model_struct(fn);
   variable m_dat = eval_fun_keV(dat.bin_lo,dat.bin_hi);
   dat = struct_combine(dat, struct{model=m_dat} );
   
   struct_filter(dat, where(dat.value != 0) );
   
   variable ind = where(dat.bin_lo>0.2);
   variable goodness =  sqrt(sum((dat.model[ind]/dat.value[ind]-1)^2))/length(dat.model[ind]);
   
   () = printf(" %s  \t deviation:  %.3e ",fn, goodness);   %% GOODNESS = sqr-distance / num_bins 
   
   if (goodness>goodness_limit_modelcomparison){
      status = EXIT_FAILURE;            
      message("    *** test FAILED *** ");
   } else {
      message("");
   }
   
   
   if ( status!=EXIT_SUCCESS  || qualifier_exists("plot") ){
      
      save_par(fn+".par");
      
      plot_model_comparison(fn,dat,goodness);
   }
      
   return status;
}
%}}}

define print_summary(status){ %{{{
   
   variable ntests = length(status);
   variable nfailed = length( where(status!=EXIT_SUCCESS) );
   
   message( " ******************************* ");
   vmessage(  " %i / %i tests successful ", ntests-nfailed, ntests);
   if (nfailed==0){      
      vmessage(  "    => SUCCESS ");
   } else {
      message(   "    => FAILED "); 
   }
   message( " ******************************* ");
   
}
%}}}



%%%% MAIN %%%% 

variable fnames = get_refdata_files();

variable ii, n = length(fnames);
variable status = Int_Type[n];

_for ii(0,n-1,1){
   status[ii] = check_single_model(fnames[ii]);   
}

print_summary(status);


if (length( where(status!=EXIT_SUCCESS)) > 0 ){
   exit(1);
}


%%%%%%%%%%%%%%
