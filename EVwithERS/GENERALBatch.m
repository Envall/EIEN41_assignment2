clear all

calculations;

kers_vector = [0.1:0.1:0.9];
ders_vector = [4000 12000 20000];
ADTcar_vector = [1000 5000 10000];
ADTtruck_vector = [500 750 1000 1250 1500 1750 2000];
Wbatt_vector = [40:10:100];    % kWh



for i_kers=1:length(kers_vector)
    for i_ders=1:length(ders_vector)
        for i_Wbatt=1:length(Wbatt_vector)
            for i_ADTcar=1:length(ADTcar_vector)
                
    kers=kers_vector(i_kers);
    ders=ders_vector(i_ders);
    Wbatt=Wbatt_vector(i_Wbatt);
    ADTcar=ADTcar_vector(i_ADTcar);
	batt_weight=Wbatt_vector(i_Wbatt)*6; % 6 kg/kWh
    
    InitiateEV_ERS;
    sim('EVwithERS')
    cycle_distance = DIST(length(DIST)) * 10; % DIST is in 10km
    
    lifetime_dist = ADTcar * yearly_d_car * FC_ERS_lifetime;
    
    
% ------------------------ Cost of battery --------------------------------

    lifeconsumed = CycleCounter(SOC(:,2));
    C_batt_wear(i_kers, i_ders, i_Wbatt, i_ADTcar) = lifeconsumed * ADTcar * Batt_cost / cycle_distance; % €/(km*dag)
    
    C_batt(i_kers, i_ders, i_Wbatt, i_ADTcar) = Wbatt * Batt_cost * ADTcar / Batt_max_km; % €/(km*dag)
    
    C_batt_tot(i_kers, i_ders, i_Wbatt, i_ADTcar) = C_batt_wear(i_kers, i_ders, i_Wbatt, i_ADTcar) + C_batt(i_kers, i_ders, i_Wbatt, i_ADTcar);  % €/(km*dag)

% -------------------------- Cost of ERS ----------------------------------
    % ERS Power term
    P_ers = ADTcar * W_ERS_grid(length(W_ERS_grid)) / 24 / cycle_distance; % kW/(km*dag)
    % ERS HW/Distribution term
    L_ers = DIST(length(DIST)); % length of road segment with ERS
    
    N_lanes = 1;
    
    C_ers(i_kers, i_ders, i_Wbatt, i_ADTcar) = k0 * P_ers + k1 * L_ers / (15*365) + k2*kers*L_ers*N_lanes / (15*365);
    
    
    
% ----------------------- Cost of electricity -----------------------------
    
    Daily_electricity = ADTcar * tot_energy_cons(length(tot_energy_cons)) / 10; % kWh/(dag*km)
    Time = 1;  % 1 dag ????
    
    C_electricity(i_kers, i_ders, i_Wbatt, i_ADTcar) = electricity_cost * Daily_electricity * Time / cycle_distance;
    
    
    C_tot(i_kers, i_ders, i_Wbatt, i_ADTcar) = C_batt_tot(i_kers, i_ders, i_Wbatt, i_ADTcar) + C_ers(i_kers, i_ders, i_Wbatt, i_ADTcar) + C_electricity(i_kers, i_ders, i_Wbatt, i_ADTcar);
    
    i_kers
    i_ders
    i_Wbatt
    i_ADTcar
            end
        end
    end
end

