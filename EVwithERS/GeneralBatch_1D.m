clear all
close all

Speed_scale_vector = [0.5:0.1:1];

for loop1=1:length(Speed_scale_vector)
    %Speed_scale = Speed_scale_vector(loop1)
    InitiateEV_ERS;
    sim('EVwithERS')
    ECresult(loop1)=EC(length(EC));
    Distance(loop1)=DIST(length(DIST));
    dSOC(loop1)=max(max(SOC(:,2))) - min(min(SOC(:,2)));
    
end
    
figure(1), clf
plot(Speed_scale_vector,ECresult/max(ECresult));
xlabel('Speed scale [-]')
ylabel('FC [l/10km]')
grid
