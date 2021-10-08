clear E E2
time = tWTem(:,1);
W = tWTem(:,2)*pi/30; % [rad/s]
T = tWTem(:,3);
dt = mean(diff(time));
Power = T.*W;
Energy = dt*(Power);

tvec = Tem;
wvec = Wem; 

for i = 2:length(tvec)
    for j = 2:length(wvec)
        ind1 = find(T>tvec(i-1));
        ind2 = find(T<tvec(i));
        ind3 = find(W>wvec(j-1));
        ind4 = find(W<wvec(j));
        indtmp = intersect(ind1,ind2);
        indtmp = intersect(indtmp,ind3);
        ind = intersect(indtmp,ind4);
        E(i-1,j-1) = sum(abs(Energy(ind)));
    end
end

E(E<sum(sum(E))/1000) = NaN;

E2 = reshape(E,[1,prod(size(E))]);
tvec2 = [];
for i = 1: length(wvec(2:end))
tvec2 = [tvec2 tvec(2:end)];
end

wvec2 = [];
for i = 1: length(wvec(2:end))
wvec2 = [wvec2 wvec(i+1)*ones(size(tvec(2:end)))];
end

figure
h=get(0,'Screensize');
set(gcf,'OuterPosition',[min(h(3:4))/2+10,50,min(h(3:4))/2-10,min(h(3:4))/2-10]);
clf

bargrid=0.8;
scatterbar3(wvec2*30/pi,tvec2,E2,(max(wvec)-min(wvec))/length(wvec)*30/pi*bargrid, (max(tvec)-min(tvec))/length(tvec)*bargrid)
xlabel('Traction Machine Speed [rpm]')
ylabel('Traction Machine Torque [Nm]')
zlabel('Energy [Ws]')
title('Traction Machine Energy converted [Ws]')
hold on
grid on
[C,h]=contour(Wem*30/pi,Tem,EtaEM,[0.85:0.01:1]);
clabel(C,h,[0.9:0.01:1])
view(-15,55)
% plot(W,T,'o')