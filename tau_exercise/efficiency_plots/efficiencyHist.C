/*******************************************************************/
/* efficiencyHist.C                                                */
/* Author: Stephanie Kwan                                          */
/*******************************************************************/

#include "TGraphAsymmErrors.h"
#include "TH1.h"
#include "TH1F.h"

#ifndef EFFICIENCY_HIST_INCL
#define EFFICIENCY_HIST_INCL

/*******************************************************************/

/* Sets the maximum and minimum error of an efficiency
   TGraphAsymmErrors to be 1.0 and 0.0 respectively. */

void setMaxErrorTo1(TGraphAsymmErrors *graph)
{
  for (int i = 1; i < graph->GetN(); i++)
    {
      Double_t errorY = graph->GetErrorY(i);
      Double_t pointX, pointY;

      if (graph->GetPoint(i, pointX, pointY) < 0)
	printf("Error getting point\n");
      
      Double_t errorUp = pointY + errorY;
      Double_t errorLow = pointY - errorY;
      
      if (errorUp > 1)
	graph->SetPointEYhigh(i, 1 - pointY);
      else if (errorLow < 0)
	graph->SetPointEYlow(i, pointY);
      
    } 
}

/*******************************************************************/

#endif 
