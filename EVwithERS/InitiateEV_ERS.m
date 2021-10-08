% ************************************************************************
% This script initiates all parameters, look up tables etc needed for
% simulation of a "parallel" hybrid Simulink model.
%
% The script is subject to continuous development and changes and a good
% understaning of the relation the Simulink program is needed for a useful
% outcome.
%
% Mats Alaküla, Lund, September 22, 2020.
%
% ************************************************************************
close all

rho_air = 1.2;  % Air density
grav = 9.81;    % Gravitational constant

% *** Vehicle Choise *****************************************************
% In this section the main vehicle parameters are selected, for 4 different
% types of vehicles.

Vehicle = 1;

switch Vehicle
    case 1 % Car
        %Wbatt = 10*3.6e6;  % Battery Energy in [Ws], XX in [kWh]
        Mv = 1864+batt_weight;         % Vehicle weight [kg]
        rw = 0.30;	        % wheel radius (m)
        Cd = 0.29; 	        % air_resistance Check this!
        Cr = 0.007;	        % roll resistance Check this!
        Av = 1.545*1.805;   % Front area [m2]
        vmax = 167/3.6;     % Maxhastighet [m/s]
        Pem_max = 300000;   % Traction machine power [W]
        Number_of_gears = 3;
        MinShiftTime = 0.5;
        Paux = 1000;         % Without AC, assumed
    case 2 % City Bus (Coman Maxirider)
        Wbatt=78*3.6e6;
        Mv = 5990;       % Vehicle curb weight
        rw = 0.349;   %0.506;	        % wheel radius (m)
        Cd = 0.79; 	        % air_resistance. (Sports Car 0.3-0.4, Ecomony Car 0.4-0.5, Pickup Truck 0.5, Tractor-Trailer,with fairings 0.6-0.7, Tractor-Trailer 0.7-0.9)
        Cr = 0.00576;	    % roll resistance (0.006...0.008 for low roll resist tires. 0.01...0.015 ordinary tires)
        Av = 2.23*2.7;   % Front area as Volvo FE6x2
        vmax = 90/3.6;       % max maxhastighet
        Pem_max = 120000;
        Number_of_gears = 2;
        MinShiftTime = 2;
        Paux = 2000;         % Without AC
    case 3 %  City Bus (Volvo 7900 Electric)
        Wbatt=76*3.6e6;
        Mv = 19000;         % Vehicle curb weight + 250 kg passenger and cargo
        rw = 0.506;	        % wheel radius (m)
        Cd = 0.79; 	        % air_resistance. (Sports Car 0.3-0.4, Ecomony Car 0.4-0.5, Pickup Truck 0.5, Tractor-Trailer,with fairings 0.6-0.7, Tractor-Trailer 0.7-0.9)
        Cr = 0.00576;	    % roll resistance (0.006...0.008 for low roll resist tires. 0.01...0.015 ordinary tires)
        Av = 2.55*3.28;   % Front area as Volvo FE6x2
        vmax=90/3.6;       % max maxhastighet
        Pem_max = 160000;
        Number_of_gears = 3;
        MinShiftTime = 2;
        % Paux = 4000;      % Without AC
    case 4 % Long Haul Truck
        Wbatt=10000*3.6e6;
        Mv = 40000;         % Vehicle curb weight + 250 kg passenger and cargo
        rw = 0.506;	        % wheel radius (m)
        Cd = 0.6; 	        % air_resistance. (Sports Car 0.3-0.4, Ecomony Car 0.4-0.5, Pickup Truck 0.5, Tractor-Trailer,with fairings 0.6-0.7, Tractor-Trailer 0.7-0.9)
        Cr = 0.0032;	    % roll resistance (0.006...0.008 for low roll resist tires. 0.01...0.015 ordinary tires)
        Av = 2.550*3.8;     % Front area as Volvo FE6x2
        vmax = 90/3.6;       % max maxhastighet
        Pem_max = 450000;
        Number_of_gears = 4;
        MinShiftTime = 1;
        Paux = 3000;    % Without AC
end


% *** Select Drive Cycle, Vehicle type, Range Extender, ERS parameters 
% *** Efficiencies etc

Drive_cycle = 15;
Vehicle = 1;

ERS = 1;                        % 0 = No range extender, 1 = ERS range extender
%ders = 1000;                    % Length at which ERS tracks are repeated 
%kers = 0.5;                     % Relative part of "ders" that is covered with ERS tracks
SlowStatChaEff = 0.95;          % Efficiency from grid power to battery when slow charging e.g during nighttime
ERS_ChaEff = 0.92;              % Efficiency from grid power to battery when charging from an ERS
Pers_max = 300000; 

PWR = Pem_max/Mv;
if PWR<=22,
    Drive_cycle = 13;
elseif (PWR>22)&(PWR<=34)
    Drive_cycle = 14;
elseif (PWR>34)
    Drive_cycle = 15;
end

Drive_cycle = 15;
    
% *** Load drive cycles **************************************************
% The following "switch" selects a drive cycle where the first 12
% alternatives are according to an "Lund University standard" and cycle
% 13ff are according to the Volvo VSIM standard. IN any case, the data is
% boiled down to a matrix "cycle" with 6 columns: 1) time [s], 2) speed
% reference [m/s], 3) position [m], 4) altitude [m], 5) slope [rad] and 6)
% slide in availability [-]. The "slide in availability" is in some models
% used to indicathe the presence of an Electric Road, but that is not used
% in the Parallel Hybrid model supplied with this script.

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

%%% Some code to repeat the drive cycle and plot some of it:

% repeat=5;
% slask=cycle;
% if repeat>1,
%     for i=2:repeat,
%         slask = [slask; cycle];
%         slask(length(slask(:,1))-length(cycle(:,1))+1:length(slask(:,1)),1) = slask(length(slask(:,1))-length(cycle(:,1))+1:length(slask(:,1)),1) + slask(length(slask(:,1))-length(cycle(:,1)),1);
%         slask(length(slask(:,1))-length(cycle(:,1))+1:length(slask(:,1)),3) = slask(length(slask(:,1))-length(cycle(:,1))+1:length(slask(:,1)),3) + slask(length(slask(:,1))-length(cycle(:,1)),3);
%     end
% end
% cycle=slask;
endtime = max(cycle(:,1));

%  Introduce ERS trigger when applicable
if ERS == 0,          % Pure EV
    cycle(1:length(cycle),6)=0; % No ERS with pure EV
elseif ERS == 1,        % EV with ERS 
    for i=1:length(cycle(:,3)),
        if (rem(cycle(i,3),ders)>(ders*(1/2-kers/2)))&(rem(cycle(i,3),ders)<(ders*(1/2+kers/2)))
            cycle(i,6)=1;
        else
            cycle(i,6)=0;
        end
    end
end

% figure, clf
% set(0,'units','pixels')
% h=get(0,'Screensize');
% set(gcf,'OuterPosition',[h(3)/2,h(4)/4,h(3)/2-10,h(4)/2-10]);
% 
% subplot(2,1,1)
% plot(cycle(:,3)/1000,cycle(:,2)*3.6)
% title('Speed reference [km/h]')
% xlabel('Distance [km]')
% axis([0 max(cycle(:,3))/1000 0 150]), grid
% subplot(2,1,2)
% plot(cycle(:,3)/1000,cycle(:,6))
% title('ERS precense')
% xlabel('Distance [km]')
% axis([0 max(cycle(:,3))/1000 -0.1 1.1]), grid


% ******************** Road and Performance *******************************
Max_start_slope = 8; % [%]
Max_curb_hight = 0.065;  %155;  % [m]
hc=Max_curb_hight;
Maxle = 0.7*Mv;
Tw_curb = rw*Maxle*grav/sin(acos(-(hc-rw)/rw));
Tw_slope = rw*Mv*grav*sin(atan(Max_start_slope/100));
Tw_roll = Mv*grav*Cr*rw;
Tw_drag = rw*0.5*Av*Cd*rho_air*vmax^2;
Tw_acc = Maxle*(80/3.6/15)*rw;

v = [0:vmax/99:vmax];  % [m/s]

v_base_set = 0;
for i=1:length(v)
    if i==1
        Tmax(i)=Tw_curb+Tw_slope; % Needed to start
    else
        Tmax(i) = min(Tw_acc + Tw_slope + Tw_roll + rw*0.5*Av*Cd*rho_air*v(i)^2,Pem_max/(v(i)/rw)); % Needed to drive and accelerate
        Tdrag(i) = rw*0.5*Av*Cd*rho_air*v(i)^2;
    end
    if (i>3 & Tmax(i)<Tmax(i-1) & v_base_set==0);
        v_base = v(i);
        v_base_set = 1;
    end
end

% *** Electric machine parameters  ***************************************
% The following lines set the EM drive performance

fwr = 15000/5000; % Field weakening ratio, i.e. ratio between max speed and base
%  speed
wem_min = 0;
wem_max = 15000*pi/30; % EM max speed
wem_vmax = 7500*pi/30; % EM Speed at max vehicle speed
Tem_max = Pem_max/(wem_max/fwr);  % Peak continuous torque
% The call for "CreateEMmap" is used for the EM in the same way as the call
% for i "CreateICEmap_special" s used regarding the ICE. See related
% comment for the ICE above.
[PtoTem,EtaEM,Tem,Wem,Tem_lim,EnergyConsEM] = CreateEMmapII(Pem_max,wem_max,Tem_max,'PMSM');   % 'AM  '

% *** Transmission settings **********************************************

utvx_min = wem_vmax/(vmax/rw);  % "Highest gear";
utvx_max = Tmax(1)/max(Tem_lim); % "Gear one".

DCT=1; % parameter for the transmission to eleminate torque interruptions when gearshifting

% the following lines create a sequence of gear ratios
Utvx_vect = zeros(1,Number_of_gears+1);
Utvx_vect(1,1) = utvx_min;
Utvx_vect(1,length(Utvx_vect(1,:))) = inf;
for i=2:Number_of_gears,
    Utvx_vect(1,i) =  Utvx_vect(1,i-1)*(utvx_max/utvx_min)^(1/(Number_of_gears-1));
end

EtaGEAR = 0.97; % Simplest possible efficiency model of the transmission

% gr1 = utvx_max;
% gr2 = utvx_min;


% *** Power Electronics efficiency ***************************************
% Since the simulation program is design NOT to express Voltages and
% Currents, only Powers, the simplest form of PEC efficiency mode is used.

EtaPE = 0.97; % PEC efficiency

% *** Battery parameters *************************************************
% The battery model is a constant voltage in series with a resistance. The
% electric variables voltage and current is recalculated into termina power
% and efficiencies. this is done in the call to "CreateBATTmap".

[EtaBATT,Pbatt,Mbatt,Pbatt_max_new,Wbatt_new]=CreateBATTmap(Pem_max,Wbatt);
% SOC_batt_ref_value = 20; % target value for SOC control
SOC_batt_start_value = 90; % Start value of SOC
SOC_tract_max = 95; % Max SOC level, no negative EM power allowed above.
SOC_tract_min = 10; % Min SOC level, no positive EM power allowed below.

% *** Controller parameters **********************************************

Tau_charge = 2;
ksoc = max(Wbatt)/400/Tau_charge; % Max SOC control gain with respect to
% symmetric optimum
ksoc_vect = ksoc*tan(pi/2.05*[-1:1/15:1])*2; % Gain vector for SOC control



