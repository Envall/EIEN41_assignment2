clear all

Wbatt_vector   = [25:75:100];    % Traction battery size [kWh]
Pem_max_vector = [25:200:225];   % Electric traction machine power [kW]

length(Pem_max_vector)

for loop1=1:length(Wbatt_vector)
    for loop2=1:length(Pem_max_vector)
        
        [loop1 loop2]
        
        Wbatt  = Wbatt_vector(loop1)*3.6e6;
        Pem_max  = Pem_max_vector(loop2)*1000;
        
        InitiateEV;
        
        sim('GeneralFEVmodel')
        
        ECresult(loop1,loop2)=EC(length(EC)); % [kWh/10km], the end of the EC-vector in the Logg-block in Simulink
        RelDist(loop1,loop2)=DIST(length(DIST))/(max(cycle(:,3))/10000); % [%], the relative travelled distance, relative to
        % the intended length of the drive cycle. If this is to low, the
        % drivetrain (traction machine or battery) is to weak or to small.
        
        if RelDist(loop1,loop2)>0.99
            DoD(loop1,loop2)=max(max(SOC(:,2))) - min(min(SOC(:,2))); % [% SOC], to check that the DoD is not to deep
        else
            DoD(loop1,loop2) = nan;
        end
        
        WTdura;
        
        pause(3), close all
        
    end
end

figure
h=get(0,'Screensize');
set(gcf,'OuterPosition',[10,min(h(3:4))/2,min(h(3:4))-10,min(h(3:4))/2-10]);
clf

subplot(1,3,1)
mesh(Pem_max_vector,Wbatt_vector,ECresult)
xlabel('Traction Machine Power [kW]')
ylabel('Traction Battery Size [kWh]')
title('Energy consumption [kWh/10km]')

subplot(1,3,2)
mesh(Pem_max_vector,Wbatt_vector,RelDist)
xlabel('Traction Machine Power [kW]')
ylabel('Traction Battery Size [kWh]')
title('Relative Distance [-]')

subplot(1,3,3)
mesh(Pem_max_vector,Wbatt_vector,DoD)
xlabel('Traction Machine Power [kW]')
ylabel('Traction Battery Size [kWh]')
title('DoD [%]')

