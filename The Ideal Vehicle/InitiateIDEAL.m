%load drive cycles
Drive_cycles=cellstr(['NEDC     ';'US06     ';'Bus1     ';'Bus2     ';'Distr1   ';'LongHaul1';'LongHaul2';'LongHaul3';'LongHaul4']);
Vehicle = 1;
Drive_cycle=12;
Fuel = 1;
switch Drive_cycle
    case 1 % nedc
        load ..\DriveCycles\nedc;
        cycle = nedc;
    case 2 % us06;
        load ..\DriveCycles\us06;
        cycle = us06;
    case 3 % Bus 1
        load ..\DriveCycles\london159;
        cycle = london159;
    case 4 % Bus 2
        load ..\DriveCycles\cbr85;
        cycle = cbr85;
    case 5 % Distribution 1
        load ..\DriveCycles\Distr1;
        cycle = Distr1;
    case 6 % Long Haul 1
        load ..\DriveCycles\LongHaul1;
        cycle = LongHaul1;
    case 7 % Long Haul 2
        load ..\DriveCycles\LongHaul2;
        cycle = LongHaul2;
    case 8 % Long Haul 3
        load ..\DriveCycles\LongHaul3;
        cycle = LongHaul3;
    case 9 % Long Haul 4
        load ..\DriveCycles\LongHaul4;
        cycle = LongHaul4;
    case 10 % WLTP1 (P[W]/M[kg] <= 22)
        load ..\DriveCycles\WLTP1;
        cycle = WLTP1;
    case 11 % WLTP2  (22 < P[W]/M[kg] <= 34)
        load ..\DriveCycles\WLTP2;
        cycle = WLTP2;
    case 12 % WLTP3 (34 < P[W]/M[kg])
        load ..\DriveCycles\WLTP3;
        cycle = WLTP3;
end
endtime = max(cycle(:,1));

% F�rbr�nningsmotorns parametrar  ++++++++++++++++++
T_max = inf;
% Ladda data f�r Volvo-Dieseln

% Mekaniska parametrar  ++++++++++++++++++
Vehicles = cellstr(['Car       ';'CityBus   ';'DistrTruck';'LongHaul  ']);

switch Vehicle
    case 1 % Car
        Mv = 1600;          % Vehicle weight
        rw = 0.30;	        % wheel radius (m)
        Cd = 0.26; 	        % air_resistance
        Cr = 0.008;	        % roll resistance
        Av = 2.55;	        % Front area
        vmax=180/3.6;       % 165 km/h maxhastighet
        Pmax=100000;
    case 2 % City Bus
        Mv = 14000;         % Vehicle curb weight 
        rw = 0.506;	        % wheel radius (m)
        Cd = 0.79; 	        % air_resistance. (Sports Car 0.3-0.4, Ecomony Car 0.4-0.5, Pickup Truck 0.5, Tractor-Trailer,with fairings 0.6-0.7, Tractor-Trailer 0.7-0.9)
        Cr = 0.00576;	    % roll resistance (0.006...0.008 for low roll resist tires. 0.01...0.015 ordinary tires)
        Av = 2.550*2.924;   % Front area as Volvo FE6x2
        vmax=100/3.6;       % max maxhastighet
        Pmax = 200000;
    case 3 % Distribution Truck
        Mv = 22000;         % Vehicle curb weight + 250 kg passenger and cargo
        rw = 0.506;	        % wheel radius (m)
        Cd = 0.79; 	        % air_resistance. (Sports Car 0.3-0.4, Ecomony Car 0.4-0.5, Pickup Truck 0.5, Tractor-Trailer,with fairings 0.6-0.7, Tractor-Trailer 0.7-0.9)
        Cr = 0.00576;	    % roll resistance (0.006...0.008 for low roll resist tires. 0.01...0.015 ordinary tires)
        Av = 2.500*2.924;   % Front area as Volvo FE6x2
        vmax=80/3.6;               % max maxhastighet
        Pmax = 250000;
    case 4 %Long Haul Truck
        Mv = 40000;         % Vehicle curb weight + 250 kg passenger and cargo
        rw = 0.506;	        % wheel radius (m)
        Cd = 0.79; 	        % air_resistance. (Sports Car 0.3-0.4, Ecomony Car 0.4-0.5, Pickup Truck 0.5, Tractor-Trailer,with fairings 0.6-0.7, Tractor-Trailer 0.7-0.9)
        Cr = 0.00576;	    % roll resistance (0.006...0.008 for low roll resist tires. 0.01...0.015 ordinary tires)
        Av = 2.550*3.1;   % Front area as Volvo FE6x2
        vmax=130/3.6;       % max maxhastighet
        Pmax = 450000;
end
rho_air = 1.2;      % Air density
grav = 9.81;
      
% Fuel energy density in [Ws/liter]
Fuels=cellstr(['Gasoline';'Diesel  ';'E85     ']);

switch Fuel
    case 1  % Gasoline
        Density = 31.4e6;
    case 2, % Diesel, 0.808 kg/liter
        Density = 35.8e6;
    case 3, % Ethanol (E85)
        Density = (85*19.6e6 + 15*32e6)/100;
end
