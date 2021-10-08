figure(1)
clf
[c,h] = contour(Wem*30/pi,Tem,EtaEM,[0.1:0.2:0.9 0.92 0.94 0.96],'k-');
clabel(c,h)
hold on

for p=[5000:5000:40000],
    clear t
    w=[10:10:max(Wem)];
    for i=1:length(w);
        if p/w(i)>max(Tem),
            t(i)=nan;
        else
            t(i)=p/w(i);
        end
    end
    plot(w*30/pi,t,'b--')
    I=find(w*30/pi>4500,1);
    text(w(I)*30/pi,t(I),[num2str(p/1000) ' kW'],'Color','blue')
end

xlabel('Motor speed [rpm]')
ylabel('Motor torque [Nm]')
title('Electric Traction Machine efficiency')