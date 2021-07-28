% -*- mode: slang; mode: fold -*-

require("isisscripts");
require("scripts/subs_testSetup.sl");
require("scripts/subs_returnrad.sl");
require("scripts/subs_filenames.sl");


variable spin = 0.998;  %% currently only done for this value

define getDiagnoseRframePlot(){ %{{{
   
   variable plotTitle = sprintf("Restframe Plot of Return (solid) and Primary (dashed) -- spin=%.3f",
				spin);


   variable files = getFilenameStruct();
   variable fnames  = [files.rframe.specRet,files.rframe.specPri];
   variable iRframe = [1,1];
   
   variable pl = get_2d_plot(fnames, iRframe, spin; title=plotTitle);
   
   pl.render(dir_plots+"diagnosePlotRframe.pdf");
   
   return EXIT_SUCCESS;
}
%}}}


define getDiagnoseXillverPrimPlot(){ %{{{
   
   variable plotTitle = sprintf("Plot of Real Primary  (solid) and Xillver Primary (dashed) used for the reflection -- spin=%.3f",
				spin);


   variable files = getFilenameStruct();
   variable fnames  = [files.rframe.specRet,files.rframe.xillPrim];
   variable iRframe = [1,1];
   
   variable pl = get_2d_plot(fnames, iRframe, spin; title=plotTitle, noSumSpec);
   
   pl.render(dir_plots+"diagnosePlotXillverPrim.pdf");
   
   return EXIT_SUCCESS;
}
%}}}


define getDiagnoseXillverPlot(){ %{{{
   
   variable plotTitle = sprintf("Rest-frame: Xillver (solid), and Xillver Prim (dashed) -- spin=%.3f",
				spin);


   variable files = getFilenameStruct();
   variable fnames  = [files.rframe.xillRefl,files.rframe.xillPrim,files.rframe.specRet];
   variable iRframe = [1,1,1];
   
   variable pl = get_2d_plot(fnames, iRframe, spin; title=plotTitle, noSumSpec);
   
   pl.render(dir_plots+"diagnosePlotXillver.pdf");
   
   return EXIT_SUCCESS;
}
%}}}


define getDiagnoseRelatSmearingPlot(){ %{{{
   
   variable plotTitle = sprintf("Observer: Relat Refl. (solid), Primary BBody (dashed), Returning/Mirrord BBody  -- spin=%.3f",
				spin);


   variable files = getFilenameStruct();
   variable fnames  = [files.fobs.reflect,files.fobs.primary,files.fobs.mirror];
   variable iRframe = [0,0,0];
   
   variable pl = get_2d_plot(fnames, iRframe, spin; title=plotTitle);
   
   pl.render(dir_plots+"diagnosePlotRelatSmearing.pdf");
   
   return EXIT_SUCCESS;
}
%}}}


define testDiagnosePlots(){


   if (getDiagnoseRframePlot() != EXIT_SUCCESS) return EXIT_FAILURE;
   if (getDiagnoseXillverPrimPlot() != EXIT_SUCCESS) return EXIT_FAILURE;
   if (getDiagnoseXillverPlot() != EXIT_SUCCESS) return EXIT_FAILURE;
   if (getDiagnoseRelatSmearingPlot() != EXIT_SUCCESS) return EXIT_FAILURE;
   
   
   
   return EXIT_SUCCESS;
}


if (length(__argv)>0){
   testDiagnosePlots();
}