clear all

calculations;

Vector1 = [3000 6000 12000];
Vector2 =[100 200];
k_ers = [0.2:0.2:1];
d_ers = [4:4:20];
ADT_car = [1000 4000 6000 8000 10000];
ADT_truck = [500 750 1000 1250 1500 1750 2000];
batt_size = [60:10:100];    % kWh
batt_size = 60;


figure(2), clf


for loop1=1:length(k_ers)
    for loop2=1:length(d_ers)
      
        [loop1 loop2]
        
        kers = k_ers(loop1);
        ders = d_ers(loop2);
        
        %InitiateEV_ERS;
        
        % sim('EVwithERS')
        
        batt_cost(loop1,loop2) = batt_size * Batt_cost;
        
        %ECresult(loop1,loop2)=EC(length(EC)); % [kWh/10km], the end of the EC-vector in the Logg-block in Simulink
        %RelDist(loop1,loop2)=DIST(length(DIST))/(max(cycle(:,3))/10000); % [%], the relative travelled distance, relative to
        % the intended length of the drive cycle. If this is to low, the
        % drivetrain (traction machine or battery) is to weak or to small.
        %[lifeconsumed]=CycleCounter(SOC(:,2));
        %LifeConsumed_rain(loop1,loop2)=lifeconsumed;
        
        %if RelDist(loop1,loop2)>0.99
        %    DoD(loop1,loop2)=max(max(SOC(:,2))) - min(min(SOC(:,2))); % [% SOC], to check that the DoD is not to deep
        %else
        %    DoD(loop1,loop2) = nan;
        %end
        
        %figure(2)
        %subplot(2,3,3*(loop2-1)+loop1)
        %WTdura;
        
        %pause(3), % close all
        
    end
end

% figure
% h=get(0,'Screensize');
% set(gcf,'OuterPosition',[10,min(h(3:4))/2,min(h(3:4))-10,min(h(3:4))/2-10]);
% clf
% 
% subplot(1,3,1)
% mesh(Vector2,Vector1,ECresult)
% xlabel('Traction Machine Power [kW]')
% ylabel('Traction Battery Size [kWh]')
% title('Energy consumption [kWh/10km]')
% 
% subplot(1,3,2)
% mesh(Vector2,Vector1,RelDist)
% xlabel('Traction Machine Power [kW]')
% ylabel('Traction Battery Size [kWh]')
% title('Relative Distance [-]')
% 
% subplot(1,3,3)
% mesh(Vector2,Vector1,DoD)
% xlabel('Traction Machine Power [kW]')
% ylabel('Traction Battery Size [kWh]')
% title('DoD [%]')

