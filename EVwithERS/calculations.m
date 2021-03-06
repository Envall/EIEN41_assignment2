% ************************************************************************
%                            Parametrar
% ************************************************************************

% Battery

Batt_cost         = 100;       % [€/kWh]
Batt_max_yrs      = 15;        % TODO Hur ska vi använda denna?
Batt_max_km       = 500000;    % TODO fatta cyclecounter hörru
Batt_weight       = 6;         % [kg/kWh]   (battery pack weight)

pup_car_cost      = 1000;      % [€]
pup_truck_cost    = 3000;      % [€]
pup_base_weight_c = 70;        % [kg]  for a car
pup_base_weight_t = 120;       % [kg]  for a truck
pup_kw_weight     = 1;         % [kw/kW]
pup_lifetime      = 130000;    % [km]  valid for both truck and car

%Fast chargers

FC_per_ev         = 0.01;      % [fast chargers / nbr of EVs]
FC_ERS_lifetime   = 15;        % [years]

yearly_d_car      = 13000;     % [km/year]
yearly_d_truck    = 100000;    % [km/year]

electricity_cost  = 0.15;       % [€/kWh]
FC_cost           = 350*250;        % [€]  assuming 250 kW charger

k0 = 300;              % [€/kW]
k1 = 150e+3;           % [€/km]
k2 = 500e+3;           % [€/kW]

% ERS cost:
% P_ers -  Proportional to traffic flow
% L_ers -  Length of road (k_ers * L_ers = length of ERS)
% k_ers -  Proportion of road that is ERS:ed


