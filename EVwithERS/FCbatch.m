clear all

calculations;

ADTcar_vector = [1000:3000:10000];
ADTtruck_vector = [1000:500:2000];
Wbatt_vector = [40:10:100];    % kWh  TODO truck


for i_Wbatt=1:length(Wbatt_vector)
    for i_ADTtruck=1:length(ADTtruck_vector)
                
    Wbatt=Wbatt_vector(i_Wbatt);
    %ADTcar=ADTcar_vector(i_ADTcar);
    ADTtruck=ADTtruck_vector(i_ADTtruck);
	batt_weight=Wbatt_vector(i_Wbatt)*6; % 6 kg/kWh
    
    nbrFC = ceil(ADTtruck / 100);   % 100 bilar per FC
    
    InitiateEV_ERS;
    sim('EVwithERS')
    cycle_distance = DIST(length(DIST)) * 10; % DIST is in 10km
    
    lifetime_dist = ADTtruck * yearly_d_car * FC_ERS_lifetime;

% ------------------------ Cost of battery --------------------------------

    lifeconsumed = CycleCounter(SOC(:,2));
    C_batt_wear(i_Wbatt, i_ADTtruck) = lifeconsumed * Batt_cost / cycle_distance; % €/km
    
    

% -------------------------- Cost of FC -----------------------------------
    
    C_fc(i_Wbatt, i_ADTtruck) = nbrFC * FC_cost / ADTtruck / cycle_distance;   % €/(bil*km)
    
% ----------------------- Cost of electricity -----------------------------
    
    consumption = energy_cons_per_10km(length(energy_cons_per_10km));
    
    El_per_car = consumption / 10; % kWh/(km*bil)
    
    C_electricity(i_Wbatt, i_ADTtruck) = electricity_cost * El_per_car;
    
% ----------------------- Cost of electricity [€/(km*bil)] ----------------
    
    C_tot(i_Wbatt, i_ADTtruck) = C_batt_wear(i_Wbatt, i_ADTtruck) + C_electricity(i_Wbatt, i_ADTtruck) + C_fc(i_Wbatt, i_ADTtruck);
    
   
    i_Wbatt
    i_ADTtruck
            end
        end
        

