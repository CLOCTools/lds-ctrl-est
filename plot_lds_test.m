clc
clear
close all
set(0,'defaultAxesFontSize',20)
set(0,'DefaultLineLineWidth',2)

%%























%%
return



load('test_plds_sctrl.mat')

figure(1)
clf
subplot(5,1,1)
hold on
plot(lambdaHat,'k')
plot(lambdaTrue,'b')
plot(lambdaRef,'g')
legend('FR hat','FR true','ref')

subplot(5,1,2)
plot(state,'m','LineWidth',2)
legend('switch state')

subplot(5,1,3)
plot(u,'b')
legend('stim')

subplot(5,1,4)
hold on
plot(xHat,'r')
plot(xTrue,'k:')
legend('x_{hat}','x_{true}')

subplot(5,1,5)
plot(z,'k','LineWidth',1)
legend('spikes')