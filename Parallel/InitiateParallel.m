% ************************************************************************
% This script initiates all parameters, look up tables etc needed for
% simulation of a "parallel" hybrid Simulink model.
%
% The script is subject to continuous development and changes and a good
% understaning of the relation the Simulink program is needed for a useful
% outcome.
%
% Mats Alak�la, Fot�, January 24, 2016.
%
% ************************************************************************

rho_air = 1.2;  % Air density
grav = 9.81;    % Gravitational constant

% *** Select Drive Cycle, Vehicle type, Fuel=Engine and Battery Size
Drive_cycle = 15;
Vehicle = 1;
Fuel = 1;

% *** Load drive cycles **************************************************
% The following "switch" selects a drive cycle where the first 12
% alternatives are according to an "Lund University standard". The data is
% boiled down to a matrix "cycle" with 6 columns: 1) time [s], 2) speed
% reference [m/s], 3) position [m], 4) altitude [m], 5) slope [rad] and 6)
% slide in availability [-].

clear cycle
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

repeat=1;
slask=cycle;
if repeat>1,
    for i=2:repeat,
        slask = [slask; cycle];
        slask(length(slask(:,1))-length(cycle(:,1))+1:length(slask(:,1)),1)=slask(length(slask(:,1))-length(cycle(:,1))+1:length(slask(:,1)),1) + slask(length(slask(:,1))-length(cycle(:,1)),1);
    end
end
cycle=slask;
endtime = max(cycle(:,1));

cycle(1:length(cycle),6)=0; % make sure that no "Slide In" is indicated

% *** Vehicle Choise *****************************************************
% In this section the main vehicle parameters are selected, for 4 different
% types of vehicles.

switch Vehicle
    case 1 % Car
        version=2;  % NB! verion=1 gives a Hybrid setting, version=2 gives a Plug In hybrid setting
        if version==1,  %Hybrid
            Wbatt = 1.56*3.6e6;  % Battery Energy in [Ws], XX in [kWh]
            Mv = 1520 ;         % Vehicle weight [kg]
            rw = 0.30;	        % wheel radius (m)
            Cd = 0.29; 	        % air_resistance **** Check this!
            Cr = 0.007;	        % roll resistance **** Check this!
            Av = 1.545*1.805;           % Front area [m2]
            vmax = 164/3.6;     % Maxhastighet [m/s]
            Pem_max = 32000;      % Traction machine power [W]
            Pice_max = 77200;   % Combustion engine power [W]
            Number_of_gears = 6;
            Paux = 600;         % Without AC
            SOC_batt_ref_value = 70; % target value for SOC control
            SOC_batt_start_value = 70; % Start value of SOC
        elseif version==2,  % Plug IN Hybrid
            Wbatt = 8.9*3.6e6;  % Battery Energy in [Ws], XX in [kWh]
            Mv = 1628 ;         % Vehicle weight [kg]
            rw = 0.30;	        % wheel radius (m)
            Cd = 0.29; 	        % air_resistance **** Check this!
            Cr = 0.007;	        % roll resistance **** Check this!
            Av = 1.545*1.805;           % Front area [m2]
            vmax = 172/3.6;     % Maxhastighet [m/s]
            Pem_max = 44500;      % Traction machine power [W]
            Pice_max = 77200;     % Combustion engine power [W]
            Number_of_gears = 6;
            Paux = 600;         % Without AC
            SOC_batt_ref_value = 30; % target value for SOC control
            SOC_batt_start_value = 100; % Start value of SOC
        end
    case 2 % City Bus
        Mv = 6500;         % Vehicle curb weight
        rw = 0.506;	        % wheel radius (m)
        Cd = 0.79; 	        % air_resistance. (Sports Car 0.3-0.4, Ecomony Car 0.4-0.5, Pickup Truck 0.5, Tractor-Trailer,with fairings 0.6-0.7, Tractor-Trailer 0.7-0.9)
        Cr = 0.00576;	    % roll resistance (0.006...0.008 for low roll resist tires. 0.01...0.015 ordinary tires)
        Av = 2.550*2.924;   % Front area as Volvo FE6x2
        vmax=80/3.6;       % max maxhastighet
        Pmax = 150000;
        Number_of_gears = 2;
        MinShiftTime = 2;
        Paux = 2000;         % Without AC
    case 3 % Distribution Truck
        Mv = 20000;         % Vehicle curb weight + 250 kg passenger and cargo
        rw = 0.506;	        % wheel radius (m)
        Cd = 0.79; 	        % air_resistance. (Sports Car 0.3-0.4, Ecomony Car 0.4-0.5, Pickup Truck 0.5, Tractor-Trailer,with fairings 0.6-0.7, Tractor-Trailer 0.7-0.9)
        Cr = 0.00576;	    % roll resistance (0.006...0.008 for low roll resist tires. 0.01...0.015 ordinary tires)
        Av = 2.500*2.924;   % Front area as Volvo FE6x2
        vmax=100/3.6;       % max maxhastighet
        Pmax = 250000;
        Number_of_gears = 6;
        MinShiftTime = 2;
        Paux = 2000;    % Without AC
    case 4 % Long Haul Truck
        Mv = 40000;         % Vehicle curb weight + 250 kg passenger and cargo
        rw = 0.506;	        % wheel radius (m)
        Cd = 0.3; 	        % air_resistance. (Sports Car 0.3-0.4, Ecomony Car 0.4-0.5, Pickup Truck 0.5, Tractor-Trailer,with fairings 0.6-0.7, Tractor-Trailer 0.7-0.9)
        Cr = 0.0032;	    % roll resistance (0.006...0.008 for low roll resist tires. 0.01...0.015 ordinary tires)
        Av = 2.550*3.8;     % Front area as Volvo FE6x2
        vmax=90/3.6;       % max maxhastighet
        Pice_max= 300000;
        Pem_max = 300000;
        Number_of_gears = 12;
        MinShiftTime = 1;
        Paux = 3000;    % Without AC
      
end

% *** Combustion Engine parameters ***************************************
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

[PtoT,Tice,Wice,Tlim_ice,FuelConsICE]=CreateICEmap(Pice_max,wice_max,Tice_max,EtaICE.*270/230);

% *** Transmission settings **********************************************

utvx_min = wice_max/(vmax*10/rw);  % "Highest gear".
% utvx_max = wice_max/((vmax/7.5)/rw); % "Gear one".
utvx_max = wice_max/((vmax/5)/rw); % "Gear one".

DCT=1; % parameter for the transmission to eleminate torque interruptions
% when gearshifting

% the following lines create a sequence of gear ratios
Utvx_vect = zeros(1,Number_of_gears+1);
Utvx_vect(1,1) = utvx_min;
Utvx_vect(1,length(Utvx_vect(1,:))) = inf;
for i=2:Number_of_gears,
    Utvx_vect(1,i) =  Utvx_vect(1,i-1)*(utvx_max/utvx_min)^(1/(Number_of_gears-1));
end

EtaGEAR = 0.97; % Simplest possible efficiency model of the transmission
Eta_gen = 0.6;

% Fuel energy density in [Ws/liter], for fuel consumption calculations

switch Fuel
    case 1  % Gasoline
        Density = 31.4e6;
    case 2, % Diesel, 0.808 kg/liter
        Density = 35.8e6;
    case 3, % Ethanol (E85)
        Density = (85*19.6e6 + 15*32e6)/100;
end

% *** Electric machine parameters  ***************************************
% The following lines set the EM drive performance

fwr = 2.1; % Field weakening ration, i.e. ratio between max speed and base
%  speed
Tem_max = Pem_max/(wice_max/fwr);  % Peak continuous torque
wem_max = wice_max; % EM mounted on cranc shaft

% The call for "CreateEMmap" is used for the EM in the same way as the call
% for i "CreateICEmap_special" s used regarding the ICE. See related
% comment for the ICE above.
[PtoTem,EtaEM,Tem,Wem,Tem_lim,EnergyConsEM] = CreateEMmap(Pem_max,wem_max,Tem_max);

% *** Power Electronics efficiency ***************************************
% Since the simulation program is design NOT to express Voltages and
% Currents, only Powers, the simplest form of PEC efficiency mode is used.

EtaPE = 0.98; % PEC efficiency

% *** Battery parameters *************************************************
% The battery model is a constant voltage in series with a resistance. The
% electric variables voltage and current is recalculated into termina power
% and efficiencies. this is done in the call to "CreateBATTmap".

[EtaBATT,Pbatt,Mbatt,Pbatt_max_new,Wbatt_new]=CreateBATTmap(Pem_max,Wbatt);

SOC_tract_max = 98; % Max SOC level, no negative EM power allowed above.
SOC_tract_min = 12; % Min SOC level, no positive EM power allowed below.
SOC_batt_ref_value = 70;

% *** Controller parameters **********************************************

Tau_charge = 2;   % Dynamic limitation for the ICE operation

ksoc = max(Wbatt)/400/Tau_charge; % Max SOC control gain with respect to
% symmetric optimum
ksoc_vect = ksoc*tan(pi/2.05*[-1:1/15:1])*2; % Gain vector for SOC control



