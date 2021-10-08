clear all
close all

InitiateParallel;
sim('Parallel')
SOC_batt_start_value = SOC(length(SOC(:,1)),2)
sim('Parallel')
