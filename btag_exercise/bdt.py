#=================================================================================
# BDT trained on limited dataset from PhaseL1BTagAnalyzer
# test-Analyzer.py. Based on Exercise 4 from CMSDAS
# B-tagging exercise and 3-dense.ipynb from FNALLPC 
# machine-learning-hats.
# 
# Instructions:
# 1.  Convert the .ROOT file to a h5 format: at the terminal, run
#     % root2hdf5 -f <file names, separated by a space for multiple filenames>
# 2.  To run the script: % python bdt.py
# 
# A .pdf is output in the same directory.
#=================================================================================

# Import necessary libraries
import ROOT
import rootpy 
# import rootpy.plotting
import pprint
from DataFormats.FWLite import Events, Handle
import pandas as pd
import numpy as np
import h5py
ROOT.gStyle.SetOptStat(0)
ROOT.gStyle.SetOptTitle(0)

import matplotlib as mpl
import matplotlib.pyplot as plt
from sklearn.metrics import roc_curve, auc
from sklearn.model_selection import train_test_split
from sklearn.ensemble import GradientBoostingClassifier
from sklearn.multiclass import OneVsRestClassifier
import sklearn

#-----------------------------------------------
# Loading data into pandas DataFrame
#-----------------------------------------------

# fix random seed for reproducibility
seed = 7
np.random.seed(seed)

treename = 'L1BTagAnalyzer/efficiencyTree'
homedir  = '/afs/cern.ch/work/s/skkwan/public/triggerDevel/CMSSW_10_1_5/src/L1Trigger/phase2L1BTagAnalyzer/test/'
filename = 'analyzer_ttbar.h5'

h5File = h5py.File(homedir + filename, 'r')   # open read-only
params = h5File[treename][()]

VARS = ['hadronFlavor', 'recoPt', 'recoTk1IP']   # choose which variables to use

# Read data into pandas DataFrames
df = pd.DataFrame(params, columns = VARS)

# Cut out undefined variables 
df = df[(df[VARS[1]] > -99) & df[VARS[2]] > -99]

# Create 'isB' boolean array
df['isB'] = (df.hadronFlavor == 5)

#-----------------------------------------------
# Define and train the model
#-----------------------------------------------
# Set the true labels for the training/testing of the discriminator
df['binary_target'] = 0
df.loc[(df.hadronFlavor == 5), 'binary_target'] = 1 

# Split the data into two samples: training and testing
train, test = train_test_split(df,
                               test_size = 0.3,
                               random_state = 42)

# Define our models:
features = ['recoPt', 'recoTk1IP']
clf_binary = GradientBoostingClassifier(
    # These values are just copied from CMSDAS 2018 B-tagging Ex. 4
    learning_rate = 0.01, n_estimators = 1000,
    subsample = 0.8, random_state = 13,
    min_samples_leaf = int(0.01 * len(train)),
    max_depth = 5,
    verbose = 1,
)

# Training, takes a while
clf_binary.fit(train[features], train.binary_target)

#-----------------------------------------------------
# Store the output of the taggers and analyze results
#-----------------------------------------------------
# The binary classifier output a N by 2 matrix corresponding to
# prob(non-b|jet), prob(b|jet). 
# Ignore the warning coming from the command.
test['clf_binary'] = clf_binary.predict_proba(test[features])[:,1]

# We can now print the ROC curves
fig = plt.figure(figsize = (15, 15), dpi = 80, facecolor = 'w', edgecolor = 'k')

for algo, color in zip([
        'clf_binary',
        ], 'r'):
    for bkg, style in zip([4, 0], ['-','--']):
        mask = (test.hadronFlavor != bkg)
        jets = test[mask]
        fakes_positive_rate, true_positive_rate, _ = roc_curve(jets.isB, jets[algo])
        plt.plot(true_positive_rate,fakes_positive_rate, '%s%s' % (color, style))

import matplotlib.lines as mlines
plt.legend(
    loc = 'best',
    handles = [
        mlines.Line2D([], [],color = 'red', ls = '-', label = 'clf_binary')
        ])

plt.xlabel('Efficiency')
plt.ylabel('Fake rate')
plt.grid(True)
# Plot y-axis on y scale and clip non-positive y values to a small pos. number
plt.yscale('log', nonposy = 'clip')
plt.show()

plt.savefig("bdt_roc.pdf")




















