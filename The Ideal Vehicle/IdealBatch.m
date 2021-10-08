% Drive_cycle = [1 2 4 7];
% Veh_type =[1 2 3 4];
% Fuel=[1 2];

cases=[1 1 1; 1 2 1; 2 4 2; 3 5 2; 4 8 2];

% cases=[1 1 1];
for i=1:length(cases)
    Vehicle=cases(i,1); Drive_cycle=cases(i,2);  Fuel=cases(i,3);
    InitiateIDEAL
    sim('idealvehicle')
    FCresult(i,1)=FC(length(FC));
    FCresult(i,2)=BrEqFC(length(BrEqFC));
    FCresult(i,3)=Drive_cycle;
    FCresult(i,4)=Vehicle;
    FCresult(i,5)=Fuel;
end

figure(1)
clf
colormap('default')
bar(FCresult(:,1:2),'stacked')
ylabel('Fuel consumption [l/10 km]')
axis([0.5 (size(cases,1)+0.5) 0 0.1*ceil(((max(FCresult(:,1)))*12))])

%Labeling
text_sp = 0.1*ceil(((max(FCresult(:,1)))*12))-(max(FCresult(:,1)));
for i=1:size(cases,1)
%    text(i-0.4,(FCresult(i,1))+0.75*text_sp,strcat(Types(FCresult(i,2)+1),num2str(FCresult(i,4))));
    text(i-0.4,(FCresult(i,1))+0.15*text_sp,Drive_cycles(FCresult(i,3)));
    text(i-0.4,(FCresult(i,1))+0.55*text_sp,Vehicles(FCresult(i,4)));
    text(i-0.4,(FCresult(i,1))+0.35*text_sp,Fuels(FCresult(i,5)));
end