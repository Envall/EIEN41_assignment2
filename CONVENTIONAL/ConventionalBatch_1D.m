
cases=[1 1 1; 1 2 1;1 1 2; 1 2 2; 2 4 2; 3 5 2; 4 9 2];
clear FCresult
for casecount=1:length(cases)
    Vehicle=cases(casecount,1); Drive_cycle=cases(casecount,2);  Fuel=cases(casecount,3);
    InitiateCONVENTIONAL
    sim('Conventional')
    FCresult(casecount,1)=FC(length(FC));
    FCresult(casecount,2)=BrEqFC(length(BrEqFC));
    FCresult(casecount,3)=Vehicle;
    FCresult(casecount,4)=Drive_cycle;
    FCresult(casecount,5)=Fuel;
end

figure(1)
clf
bar(FCresult(:,1:2),'stacked')
ylabel('Fuel consumption [l/10 km]')

axis([0.5 (size(cases,1)+0.5) 0 0.1*ceil(((max(FCresult(:,1)))*12))])

%Labeling
text_sp = 0.1*ceil(((max(FCresult(:,1)))*12))-(max(FCresult(:,1)));
for i=1:size(cases,1)
%    text(i-0.4,(FCresult(i,1))+0.75*text_sp,strcat(Types(FCresult(i,2)+1),num2str(FCresult(i,4))));
    text(i-0.4,(FCresult(i,1))+0.15*text_sp,Drive_cycles(FCresult(i,4)));
    text(i-0.4,(FCresult(i,1))+0.55*text_sp,Vehicles(FCresult(i,3)));
    text(i-0.4,(FCresult(i,1))+0.35*text_sp,Fuels(FCresult(i,5)));
end