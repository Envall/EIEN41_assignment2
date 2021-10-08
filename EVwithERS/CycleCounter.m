function [lifeconsumed]=CycleCounter(soc)

DoD = [0 5 10 20 30 40 50 60 70 80 90 100];  % Energy Swing [%]
NoC = [1e7 5.8e5 1.7e5 5.05e4 2.3e4 1.1e4 6.1e3 4e3 3e3 2.5e3 2.1e3 1.8e3]; %*6/1.8;
DoDnew = 0.5:0.5:100;
NoCnew = interp1(DoD,NoC,DoDnew,'pchip','extrap');
DoD = DoDnew;
NoC = NoCnew;
figure(5); clf
semilogy(DoD,NoC,'b*')
ylabel('Number of cycles')
xlabel('Depth Of Discharge [%]')
grid

[~,hist,edges]=rainflow(soc);
numberof = sum(hist,2);
depth = edges(2:end);
%figure(1), clf
%plot(depth,numberof);

lifeconsumed=0;
for i=1:length(numberof)
    lifeconsumed = lifeconsumed + numberof(i)/interp1(DoD,NoC,depth(i),'linear','extrap')
end

% 1/lifeconsumed = amount of cycles we can do
