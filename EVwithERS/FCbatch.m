clear all

calculations;

ADTcar_vector = [5000];
ADTtruck_vector = [500 750 1000 1250 1500 1750 2000];
Wbatt_vector = [50:25:100];    % kWh


for i_Wbatt=1:length(Wbatt_vector)
    for i_ADTcar=1:length(ADTcar_vector)
                
    Wbatt=Wbatt_vector(i_Wbatt);
    ADTcar=ADTcar_vector(i_ADTcar);
	batt_weight=Wbatt_vector(i_Wbatt)*6; % 6 kg/kWh
    
    nbrFC = ceil(ADTcar / 100);   % 100 bilar per FC
    
    InitiateEV_ERS;
    sim('EVwithERS')
    cycle_distance = DIST(length(DIST)) * 10; % DIST is in 10km
    
    lifetime_dist = ADTcar * yearly_d_car * FC_ERS_lifetime;

% ------------------------ Cost of battery --------------------------------

    lifeconsumed = CycleCounter(SOC(:,2));
    C_batt_wear(i_kers, i_ders, i_Wbatt, i_ADTcar) = lifeconsumed * Batt_cost / cycle_distance; % €/km
    
    
% ------------------------ Cost of pickup ---------------------------------
    
    C_pup = pup_car_cost / pup_lifetime;

% -------------------------- Cost of FC -----------------------------------
    
    C_fc = nbrFC * FC_cost / ADTcar / cycle_distance);   % €/(bil*km)
    
% ----------------------- Cost of electricity -----------------------------
    
    consumption = energy_cons_per_10km(length(energy_cons_per_10km));
    
    El_per_car = consumption / 10; % kWh/(km*bil)
    
    C_electricity(i_kers, i_ders, i_Wbatt, i_ADTcar) = electricity_cost * El_per_car;
    
% ----------------------- Cost of electricity [€/(km*bil)] ----------------
    
    C_tot(i_kers, i_ders, i_Wbatt, i_ADTcar) = C_batt_wear(i_kers, i_ders, i_Wbatt, i_ADTcar) + C_ers(i_kers, i_ders, i_Wbatt, i_ADTcar) + C_electricity(i_kers, i_ders, i_Wbatt, i_ADTcar) + C_pup;
    
    
    i_kers
    i_ders
    i_Wbatt
    i_ADTcar
            end
        end
    end
end

