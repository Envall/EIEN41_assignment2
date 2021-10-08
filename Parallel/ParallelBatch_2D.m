clear all

% ICEpower_vector = [100:50:400]*1000;
BaseRPM_vector =[1100:100:1600];
EMpower_vector = [50:50:200]*1000;

for loop1=1:length(BaseRPM_vector)
    for loop2=1:length(EMpower_vector)
        [loop1 loop2]
        %Pice_max = ICEpower_vector(loop1);  
        BaseRPM = BaseRPM_vector(loop1);
        Pem_max = EMpower_vector(loop2);      
        InitiateParallel;
        close all
        sim('VolvoParallel')
        FCresult(loop1,loop2)=FC(length(FC));
        Distance(loop1,loop2)=DIST(length(DIST));
        SOCmin(loop1,loop2)=min(min(SOC));
    end
end

figure(1), clf
subplot(1,3,1)

mesh(EMpower_vector/1000,BaseRPM_vector,FCresult);
xlabel('EM power [kW]')
ylabel('BaseRPM_ [rpm]')
zlabel('FC [l/10km]')
% axis([min(EMpower_vector/1000) max(EMpower_vector/1000) min(BaseRPM_vector) max(BaseRPM_vector) 3.4 3.8])
grid on

subplot(1,3,2)
mesh(EMpower_vector/1000,BaseRPM_vector,100*Distance/max(max(Distance)));
xlabel('EM power [kW]')
ylabel('ICE power [kW]')
zlabel('Distance [% of full range]')
% axis([min(EMpower_vector/1000) max(EMpower_vector/1000) min(BaseRPM_vector) max(BaseRPM_vector) 95 100])
grid on

subplot(1,3,3)
mesh(EMpower_vector/1000,BaseRPM_vector,SOCmin);
xlabel('EM power [kW]')
ylabel('ICE power [kW]')
zlabel('min(SOC) [%]')
% axis([min(EMpower_vector/1000) max(EMpower_vector/1000) min(BaseRPM_vector) max(BaseRPM_vector) 50 100])
grid on

figure(2), clf
mesh(EMpower_vector/1000,BaseRPM_vector,FCresult);
xlabel('EM power [kW]')
ylabel('ICE power [kW]')
zlabel('FC [l/10km]')
% axis([min(EMpower_vector/1000) max(EMpower_vector/1000) min(BaseRPM_vector) max(BaseRPM_vector) 2.8 3.2])
grid on