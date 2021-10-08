function [PtoTem,EtaEM,Tem,Wem,Tem_lim,EnergyConsEM] = CreateEMmap(Pem_max,wem_max,Tem_max,Type)

clear i j Psi Isx Isy IsyMAX Curr Tmax T Pout Pcopper Piron Pwindage Ploss Eta
fwr = wem_max/(Pem_max/(Tem_max));  % field weakening ratio

w = [0:fwr/20:fwr];
Tref = [-1:1/20:1];

for i=1:length(Tref),
    for j=1:length(w),
        Psi(i,j)= min(1,1/(eps+w(j)));
        if Type == 'PMSM',
            Isx(i,j)= min(1,max((w(j)-1),0)/(max(w)-1+eps));
            Isy(i,j)= min(abs(Tref(i))/Psi(i,j),sqrt(1-Isx(i,j)^2));
%             IsyMAX(i,j)= min(max(abs(Tref))/Psi(i,j),sqrt(1-Isx(i,j)^2));
        elseif Type == 'AM  '
            Isx(i,j)= 0.15*Psi(i,j);
            Isy(i,j)= min(abs(Tref(i))/Psi(i,j),sqrt(1-Isx(i,j)^2));
%             IsyMAX(i,j)= min(max(abs(Tref))/Psi(i,j),sqrt(1-Isx(i,j)^2));
        end
        Curr(i,j)= sqrt(Isx(i,j)^2+Isy(i,j)^2);
        Tmax(i,j)= min(Tem_max,Pem_max/(eps+w(j)));  % Pem_maxPsi(i,j)*IsyMAX(i,j);
        
        T(i,j) = min(Tref(i),Tmax(i,j));
        Pout(i,j)= w(j)*T(i,j);
        if Type == 'PMSM',
           Pcopper(i,j)=(Curr(i,j))^2*0.025; 
        elseif Type == 'AM  '
           Pcopper(i,j)=(Curr(i,j))^2*0.03 + Isy(i,j)^2*0.035;
        end
        Piron(i,j) = w(j)*0.01 + w(j)*Psi(i,j)^2*0.003 + (w(j)*Psi(i,j))^2*0.003;
        Pwindage(i,j)=(w(j)/max(w))^3*0.02;
        Ploss(i,j)= 0*0.005+Pcopper(i,j)+Piron(i,j)+Pwindage(i,j);
        if abs(Tref(i)*w(j))<=1
            Eta(i,j) = min(1,(max(0.05,(abs(Pout(i,j))/(abs(Pout(i,j))+Ploss(i,j))))));
        else
            Eta(i,j) = nan;
        end
        if T(i,j)==0,
           Eta(i,j) = 0.05;
        end
    end
end

for j=1:length(w)
    for i=ceil(length(Tref)/2):length(Tref)
        if isnan(Eta(i,j))
            Eta(i,j) = Eta(i-1,j);
        end
    end
    for i=floor(length(Tref)/2):-1:1
        if isnan(Eta(i,j))
            Eta(i,j) = Eta(i+1,j);
        end
    end
end

% figure, clf
% subplot(2,5,1)
% mesh(w,Tref,Psi)
% title(['Psi' Type])
% 
% subplot(2,5,2)
% mesh(w,Tref,Isx)
% title('Isx')
% 
% subplot(2,5,3)
% mesh(w,Tref,Isy)
% title('Isy')
% 
% subplot(2,5,4)
% mesh(w,Tref,T)
% title('T')
% 
% subplot(2,5,5)
% mesh(w,Tref,Pout)
% title('Pout')
% 
% subplot(2,5,6)
% mesh(w,Tref,Pcopper)
% title('Pcopper')
% 
% subplot(2,5,7)
% mesh(w,Tref,Piron)
% title('Piron')
% 
% subplot(2,5,8)
% mesh(w,Tref,Pwindage)
% title('Pwindage')
% 
% subplot(2,5,9)
% mesh(w,Tref,Ploss)
% title('Ploss')
% 
% subplot(2,5,10)
% mesh(w,Tref,Eta)
% title('Eta')

Tem = Tem_max.*Tref/max(Tref);
Wem = wem_max.*w/max(w);
Pem = [-Pem_max:Pem_max/50:Pem_max];

Tem_lim = min([Tem_max*ones(1,length(Wem)); Pem_max./Wem]);
EtaEM=Eta;

for i=1:length(Tem);
    for j=1:length(Wem);
        if Wem(j)*Tem(i)>=0
            EnergyConsEM(i,j) = min(Pem_max,Wem(j)*Tem(i))+Ploss(i,j)*Pem_max;
        else
            EnergyConsEM(i,j) = max(-Pem_max,Wem(j)*Tem(i))+Ploss(i,j)*Pem_max;
        end
    end
end

PtoTem = zeros(length(Pem),4);
PtoTem(:,1) = Pem;
       
for i=1:length(Pem),
    if Pem(i)==0;
        PtoTem((i),2) = 0;
        PtoTem((i),3) = eps;
        PtoTem((i),4) = 0;
    elseif Pem(i)<0;
        for j=1:floor(length(Tem)/2)
            Ttemp = Tem(j);
            wtemp = Pem(i)/Ttemp;
            eta(j) = interp2(Wem,Tem,EtaEM,wtemp,Ttemp,'linear');
            if eta(j) > PtoTem(i,3)
                PtoTem((i),2) = Tem(j);
                PtoTem((i),3) = eta(j);
                PtoTem((i),4) = wtemp;
            end
        end
    elseif Pem(i)>0;
        for j=ceil(length(Tem)/2):length(Tem)
            Ttemp = Tem(j);
            wtemp = Pem(i)/Ttemp;
            eta(j) = interp2(Wem,Tem,EtaEM,wtemp,Ttemp,'linear');
            if eta(j) > PtoTem(i,3)
                PtoTem((i),2) = Tem(j);
                PtoTem((i),3) = eta(j);
                PtoTem((i),4) = wtemp;
            end
        end
    end
end

for i=2:length(Pem)-1, % Smooth a bit to make simulation less jumpy
    PtoTem(i,2)=(PtoTem(i-1,2)+PtoTem(i,2)+PtoTem(i+1,2))/3;
    PtoTem(i,4)=(PtoTem(i-1,4)+PtoTem(i,4)+PtoTem(i+1,4))/3;
end

figure
h=get(0,'Screensize');
set(gcf,'OuterPosition',[10,50,min(h(3:4))-10,min(h(3:4))-60]);
clf

subplot(2,2,1)
mesh(Wem*30/pi,Tem,EtaEM)
xlabel('Speed [rpm]')
ylabel('Torque [Nm]')
xlabel('Efficiency')
title(Type)

subplot(2,2,2)
surfc(Wem,Tem,EnergyConsEM)
%axis([0 max(Wem) 0 max(Tem) 0 1])
xlabel('Speed [rad/s]')
ylabel('Torque [Nm]')
title('Electrical machine Energy Consumption')

subplot(2,2,3)
[C,h]=contour(Wem*30/pi,Tem,EtaEM,[0.85:0.01:1]);
hold on
plot(PtoTem(:,4)*30/pi,PtoTem(:,2),'b*-')
hold on
plot(Wem*30/pi,Tem_lim,'r');
hold on
plot(Wem*30/pi,-Tem_lim,'r');
xlabel('Speed [rpm]')
ylabel('Torque [Nm]')
title('Efficiency, Opt OP point & Torque limit')
clabel(C,h,[0.9:0.01:1])

subplot(2,2,4)
plot(PtoTem(:,1),PtoTem(:,3),'b')
hold on
%axis([0 Pem_max 0 1])
title('Optimal efficiency')
xlabel('Power [W]')
ylabel('Efficiency')
grid on



