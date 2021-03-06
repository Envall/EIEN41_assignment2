function [PtoT,T,W,Tlim,FuelConsICE]=SkapaPtoT(Pice_max,wice_max,Tice_max,EtaICE)

P = [0:Pice_max/(length(EtaICE(:,1))-1):Pice_max]'; 
T = [0:Tice_max/(length(EtaICE(:,1))-1):Tice_max]';
W = [0:wice_max/(length(EtaICE(1,:))-1):wice_max];

PtoT = zeros(length(P),4);
PtoT(:,1) = P;

for i=1:length(T);
    for j=1:length(W);
        FuelConsICE(i,j)=W(i)*T(j)/(EtaICE(i,j)+eps);
    end
end
FuelConsICE(1,:)=W(:)'./EtaICE(2,:)*T(2);


for i=2:length(P),
    for j=2:length(T)
        Ttemp=T(j);
        wtemp = (P(i) / Ttemp);
        if wtemp > wice_max
            wtemp = wice_max;
            eta(j)=0;
        else
            eta(j) = interp2(W,T,EtaICE,wtemp,Ttemp,'spline');     %linear');
        end
        if eta(j) > PtoT((i),3)
            PtoT((i),2) = T(j);
            PtoT((i),3) = eta(j);
            PtoT((i),4) = wtemp;
        end
    end
end

for i=2:length(P)-1, % Smooth a bit to make simulation less jumpy
    PtoT(i,2)=(PtoT(i-1,2)+PtoT(i,2)+PtoT(i+1,2))/3;
    PtoT(i,4)=(PtoT(i-1,4)+PtoT(i,4)+PtoT(i+1,4))/3;
end

Topt = spline(PtoT(:,4)',PtoT(:,2)',W);
for i=1:length(W),
    Tlim(i) = min([Tice_max 2*Topt(i) Pice_max/(W(i)+eps)]); % Synthetically made upper torque limitation
end

figure(1)
set(gcf,'OuterPosition',[10,360,1300,350])
set(gcf, 'Renderer', 'zbuffer');
clf

subplot(1,4,1)
surfc(W,T,EtaICE)
xlabel('Speed [rad/s]')
ylabel('Torque [Nm]')
title('Efficiency map')
axis([0 max(W) 0 max(T) 0 max(max(EtaICE))])

subplot(1,4,2)
surfc(W,T,FuelConsICE)
xlabel('Speed [rad/s]')
ylabel('Torque [Nm]')
title('FuelConsumption [W]')
axis([0 max(W) 0 max(T) 0 max(max(FuelConsICE))])

subplot(1,4,3)
plot(PtoT(:,4),PtoT(:,2),'b')
hold on
plot(W,Tlim,'r');
axis([0 wice_max 0 Tice_max*1.1])
title('Optimal and maximum torque [Nm]')
xlabel('Speed [rad/s]')
ylabel('Torque [Nm]')
grid on

subplot(1,4,4)
plot(PtoT(:,1),PtoT(:,3),'b')
hold on
axis([0 Pice_max 0 1])
title('Optimal efficiency')
xlabel('Power [W]')
ylabel('Efficiency')
grid on
