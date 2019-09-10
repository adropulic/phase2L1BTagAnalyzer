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

/* Function declarations */
void setMaxErrorTo1(TGraphAsymmErrors *graph);

/*******************************************************************/

/* Convert an efficiency TH1F to a TGraphAsymmErrors. */

void effHistToTGraph(TH1F* effHist, TGraphAsymmErrors* effGraph)
{
  /* Loop through effHist. */
  int nBins = effHist->GetNbinsX();

  /* Efficiency cannot be above 1.0 */
  float maxEff = 1.000000;
  float minEff = 0.000000;
  

  *effGraph = TGraphAsymmErrors(effHist);

  float x, y;
  float yErrOld, yErrNewLow, yErrNewHigh;

  for (int i = 1; i < nBins; i++)
    {
      x = effHist->GetBinCenter(i);
      y = effHist->GetBinContent(i);
      yErrOld = effHist->GetBinError(i);

      effGraph->SetPoint(i, x, y);
      effGraph->SetPointEXhigh(i, 0.0);
      effGraph->SetPointEXlow(i, 0.0);
    }

  setMaxErrorTo1(effGraph);
  
}

/*******************************************************************/

/* Set the maximum error of a TGraphAsymmErrors so that the max
   value is 1.0. */

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
