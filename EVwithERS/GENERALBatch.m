clear all

calculations;

kers_vector = [0.1:0.1:0.7];
ders_vector = [4000 12000 20000];
ADTcar_vector = [1000:3000:10000];
ADTtruck_vector = [1000:500:2000];
Wbatt_vector = [50:50:400];    % kWh


for i_kers=1:length(kers_vector)
    for i_ders=1:length(ders_vector)
        for i_Wbatt=1:length(Wbatt_vector)
            for i_ADTtruck=1:length(ADTtruck_vector)
                
    kers=kers_vector(i_kers);
    ders=ders_vector(i_ders);
    Wbatt=Wbatt_vector(i_Wbatt);
    ADTtruck=ADTtruck_vector(i_ADTtruck);
	batt_weight=Wbatt_vector(i_Wbatt)*6; % 6 kg/kWh
    
    InitiateEV_ERS;
    sim('EVwithERS')
    cycle_distance = DIST(length(DIST)) * 10; % DIST is in 10km
    

% ------------------------ Cost of battery --------------------------------

    lifeconsumed = CycleCounter(SOC(:,2));
    C_batt_wear(i_kers, i_ders, i_Wbatt, i_ADTtruck) = lifeconsumed * Batt_cost / cycle_distance; % €/km
    
    
% ------------------------ Cost of pickup ---------------------------------
    
    C_pup = pup_truck_cost / pup_lifetime;

% -------------------------- Cost of ERS ----------------------------------
    % ERS Power term
    P_ers = W_ERS_grid(length(W_ERS_grid)) / (endtime/60) / cycle_distance; % kW/(bil*km)
    % ERS HW/Distribution term
    L_ers = DIST(length(DIST)); % length of road segment with ERS
    
    N_lanes = 2;
    
    hej = (ADTtruck*365*15); % Delar upp distr.kostnad på alla biler under vägens livstid
    
    C_ers(i_kers, i_ders, i_Wbatt, i_ADTtruck) = (k0 * P_ers) + (k1 * L_ers / hej) + (k2*kers*L_ers*N_lanes / hej);
    
% ----------------------- Cost of electricity -----------------------------
    
    consumption = energy_cons_per_10km(length(energy_cons_per_10km));
    
    El_per_car = consumption / 10; % kWh/(km*bil)
    
    C_electricity(i_kers, i_ders, i_Wbatt, i_ADTtruck) = electricity_cost * El_per_car;
    
% ----------------------- Cost of electricity [€/(km*bil)] ----------------
    
    C_tot(i_kers, i_ders, i_Wbatt, i_ADTtruck) = C_batt_wear(i_kers, i_ders, i_Wbatt, i_ADTtruck) + C_ers(i_kers, i_ders, i_Wbatt, i_ADTtruck) + C_electricity(i_kers, i_ders, i_Wbatt, i_ADTtruck) + C_pup;
    
    
    i_kers
    i_ders
    i_Wbatt
    i_ADTtruck
            end
        end
    end
end

