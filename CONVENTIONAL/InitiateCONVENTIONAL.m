% 
% Drive_cycle = 12;
Vehicle = 1;
Fuel = 1;
Drive_cycle = 15;

Eta_gen = 0.6; % generator efficiency

% StopAndGo=0;    % 1 if stop & go is on (DEFAULT = 0)
Speedy = 0;       % 1 for "sporty" driving  (DEFAULT = 0)

%[Eta_gen*100 Pmax/1000]

%load drive cycles
Drive_cycles=cellstr(['NEDC     ';'US06     ';'Bus1     ';'CBR85    ';'Distr1   ';'LongHaul1';'LongHaul2';'LongHaul3';'LongHaul4']);
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
        cycle = LongHaul2;    case 8 % Long Haul 3
        load ..\DriveCycles\LongHaul3;
        cycle = LongHaul3;
    case 9 % Long Haul 4
        load ..\DriveCycles\LongHaul4;
        cycle = LongHaul4;
    case 10 % ramp
        load ..\DriveCycles\Ramp;
        cycle = Ramp;
    case 11 % Landskrona Slide IN Bus
        load ..\DriveCycles\LandskronaSI
        cycle = LAcycle;
    case 12 % SwedenRoute
        load ..\DriveCycles\SwedenRoute
        cycle = SwedenRoute;
    case 13 % WLTP1 (P[W]/M[kg] <= 22)
        load ..\DriveCycles\WLTP1;
        cycle = WLTP1;
    case 14 % WLTP2  (22 < P[W]/M[kg] <= 34)
        load ..\DriveCycles\WLTP2;
        cycle = WLTP2;
    case 15 % WLTP3 (34 < P[W]/M[kg])
        load ..\DriveCycles\WLTP3;
        cycle = WLTP3;
    case 16 % Tractor
        load ..\DriveCycles\plowing
        cycle = tractor;
end
endtime = max(cycle(:,1));

% Vehicle Choise *****************************************
Vehicles = cellstr(['Car       ';'CityBus   ';'DistrTruck';'LongHaul  ']);

switch Vehicle
    case 1 % Car
        Mv = 1600; % Vehicle weight
        rw = 0.30;	        % wheel radius (m)
        Cd = 0.26; 	        % air_resistance
        Cr = 0.008;	        % roll resistance
        Av = 2.55;	        % Front area
        vmax=180/3.6;       % maxhastighet
        Pice_max=100000;
        Number_of_gears = 6;
        MinShiftTime = 0.5;
        Paux = 1000;         % Without AC
    case 2 % City Bus
        Mv = 14000;         % Vehicle curb weight 
        rw = 0.506;	        % wheel radius (m)
        Cd = 0.79; 	        % air_resistance. (Sports Car 0.3-0.4, Ecomony Car 0.4-0.5, Pickup Truck 0.5, Tractor-Trailer,with fairings 0.6-0.7, Tractor-Trailer 0.7-0.9)
        Cr = 0.00576;	    % roll resistance (0.006...0.008 for low roll resist tires. 0.01...0.015 ordinary tires)
        Av = 2.550*2.924;   % Front area as Volvo FE6x2
        vmax=100/3.6;       % max maxhastighet
        Pice_max = 200000;
        Number_of_gears = 6;
        MinShiftTime = 2;
        Paux = 2000;         % Without AC
    case 3 % Distribution Truck
        Mv = 22000;         % Vehicle curb weight + 250 kg passenger and cargo
        rw = 0.506;	        % wheel radius (m)
        Cd = 0.79; 	        % air_resistance. (Sports Car 0.3-0.4, Ecomony Car 0.4-0.5, Pickup Truck 0.5, Tractor-Trailer,with fairings 0.6-0.7, Tractor-Trailer 0.7-0.9)
        Cr = 0.00576;	    % roll resistance (0.006...0.008 for low roll resist tires. 0.01...0.015 ordinary tires)
        Av = 2.500*2.924;   % Front area as Volvo FE6x2
        vmax=80/3.6;               % max maxhastighet
        Pice_max = 200000;
        Number_of_gears = 6;
        MinShiftTime = 2;
        Paux = 2000;    % Without AC
    case 4 %Long Haul Truck
        Mv = 40000;         % Vehicle curb weight + 250 kg passenger and cargo
        rw = 0.506;	        % wheel radius (m)
        Cd = 0.79; 	        % air_resistance. (Sports Car 0.3-0.4, Ecomony Car 0.4-0.5, Pickup Truck 0.5, Tractor-Trailer,with fairings 0.6-0.7, Tractor-Trailer 0.7-0.9)
        Cr = 0.00576;	    % roll resistance (0.006...0.008 for low roll resist tires. 0.01...0.015 ordinary tires)
        Av = 2.550*3.1;   % Front area as Volvo FE6x2
        vmax=120/3.6;       % max maxhastighet
        Pice_max = 350000;
        Number_of_gears = 12;
        MinShiftTime = 3;
        Paux = 2000;    % Without AC
    case 5 % Agricultural Tractor
        Mv = 10000;         % Vehicle curb weight + 250 kg passenger and cargo
        rw = 0.90;	        % wheel radius (m)
        Cd = 0.3; 	        % air_resistance. (Sports Car 0.3-0.4, Ecomony Car 0.4-0.5, Pickup Truck 0.5, Tractor-Trailer,with fairings 0.6-0.7, Tractor-Trailer 0.7-0.9)
        Cr = 0.07;	        % https://www.slideshare.net/abdulrahmanmaddallah/vehicle-dynamics-course, p72
        Av = 2.3*3.6;       % Front area as Volvo FE6x2
        vmax=40/3.6;        % max maxhastighet
        Pice_max= 400000;
        Number_of_gears = 12;
        MinShiftTime = 1;
        Paux = 3000;    % Without AC
        
        PullforceDensity = 90000; % [N/m2] 
        Plow_width = 3;  % Width of plow
        Plow_depth = 0.2; % Depth of plow
        
        Plow_force = PullforceDensity*Plow_width*Plow_depth; 
end
rho_air = 1.2;      % Air density
grav = 9.81;

% F�rbr�nningsmotorns parametrar ***************************************
if Fuel==1, % Gasoline
    load EtaICE_OTTO;
    EtaICE = EtaICE_OTTO;
    wice_max = 6000*2*pi/60;
elseif Fuel==2&Vehicle==1, % Car Diesel
    load EtaICE_DIESEL;
    EtaICE = EtaICE_DIESEL;
    wice_max = 4500*2*pi/60;
elseif Fuel==2&Vehicle>1, % Truck Diesel
    load EtaICE_HeavyDIESEL;
    EtaICE = EtaICE_HeavyDIESEL;
    wice_max = 2500*2*pi/60;
else
    'Erroneous fuel choice'
end

[value,row]=max(max(EtaICE'));

wice_min = 700*2*pi/60; % 800 rpm tomg�ng
Tice_max = Pice_max/(wice_max*row/(length(EtaICE)-1));

[PtoT,Tice,Wice,Tlim_ice,FuelConsICE]=CreateICEmap(Pice_max,wice_max,Tice_max,EtaICE);

% Transmission settings ******************************************

utvx_max = wice_max/((vmax/10)/rw); % "Ettan". P�hittat, antar att motorn vid l�gsta utv�xling varvar ut vid 1/10 av maxhastigheten
utvx_min = wice_max/(2*vmax/rw);  % "H�gsta v�xeln". Antar att motorn varvar ut vid 1.2 ggr max hastigheten p� h�gsta utv�xling

DCT=1;

Utvx_vect = zeros(1,Number_of_gears+1);
Utvx_vect(1,1) = utvx_min;
Utvx_vect(1,length(Utvx_vect(1,:))) = inf;
for i=2:Number_of_gears,
    Utvx_vect(1,i) =  Utvx_vect(1,i-1)*(utvx_max/utvx_min)^(1/(Number_of_gears-1));
end

EtaGEAR = 0.97;

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



