clc
clear
close all
set(0,'defaultAxesFontSize',20)
set(0,'DefaultLineLineWidth',2)
addpath('/Users/adam/Documents/MATLAB/hmmNeuron/dataFuns')
addpath('~/Documents/MATLAB/ctrl/ssFuns')
addpath('~/Documents/MATLAB/ctrl/StAWC')
%%
%{

written to help compare MB's CLOC codebase: https://github.gatech.edu/StanleyLab/ldsCtrlEst
to AW's C++ StAC codebase: https://github.com/awillats/plds_adam/tree/master/plds_adamX/plds_adamX
and AW's Matlab StAC codebase: MATLAB/StAwC/...

ahead of July 2020 experiments to test StAC invivo

%}

darkCyan = [0,1,1]*.9
darkGreen = [0,1,0]*.5

%%

testData = load('test_plds_sctrl.mat')

ctrlType = 'P' %P or PI
doOverwriteK = true;%true;true;false;
doSineTarget = false;true;%true;


if ctrlType=='PI'
    doSkipAug=true;
else
   doSpikeAug = false;
end

model_order=1;

%   run params
dt = 1e-3; TS=1;
duration = 30;
t = 0:dt:(duration-dt);
nt = length(t);

%   plant params
gainMod = 2
a = .98
b = 0.03
b1=b*gainMod
b2=b
c =  1      
d_ = log(dt)
d = 0*log(1*dt) %-6.98?


%nl_fun = @(x) exp(1*x+0)

%z = exp(cx+d)
%log(z) = cx+d
%(log(z)-d)/c = x

nlf = @(x) exp(c*x+d_);
nlf_FR = @(x) exp(c*x+d_);

inlf = @(z) (log(z+1e-3)-d_)/c;



%% ctrl params
%lqrqmag = 1e15*0.01 %?
%lqrqmag = 1e10 %?
%lqrqmag = 1e6 %?
%lqrqmag = 1e12
%lqrqmag = 1e4;
%lqrqmag = 1;

if ctrlType == 'P'
    lqrqmag = 1e1
else
    lqrqmag = 1e2;%1e2
end
%lqrQPI = [lqrqmag, lqrqmag*2e2] %this differential scaling promotes faster closed-loop responses
lqrQPI = [lqrqmag, lqrqmag*2e2] %this differential scaling promotes faster closed-loop responses

% aux params
noiseMag = 1e-2
QspfMag = noiseMag*1e-2;%1e-2;


%%

 %sys1 = ss(a,b1,c,d,dt);
 %sys2 = ss(a,b2,c,d,dt);
 %sysAvg = ss(a,(b1+b2)/2,c,d,dt);
 

%build system assuming parameters are for continuous time version
sys1d = ss(a,b1,c,d,dt);
sys2d = ss(a,b2,c,d,dt);
sysAvgd = ss(a,(b1+b2)/2,c,d,dt);
%these discrete models would be what MB is using

 
sys1 = d2c(sys1d);
sys2 = d2c(sys2d);
sysAvg = d2c(sysAvgd);

%{
figure(11)
clf

subplot(3,1,1)
step(sys1d,sys1)

subplot(3,1,2)
step(sys2d,sys2)
%}
 
%hand-calculate Nbar/ 
stepin1 = stepinfo(sys1);
stepin2 = stepinfo(sys2);
gain1 = stepin1.SettlingMax
gain2 = stepin2.SettlingMax
 
 Sys_array = {sys1,sys2,sysAvg}

 [~,BrTrue] = aug4PI(sys1,model_order); %just grabs the structure of the augmented Br

if ctrlType == 'P'
    disp('skipping PI augmentation')
    BrTrue = NaN;
    %the point of this is to be subtracted from
    %the integrator
    SysAug = Sys_array;
else
    SysAug = myCellfun(@(S) aug4PI(S,model_order), Sys_array);
end
 %augment state vector to allow integral control



%build design matrices
if ctrlType == 'P'
   buildCtrl = @(sys,Br)  lqr_sysP_struct(sys, lqrqmag);
else
    buildCtrl = @(sys,Br)  lqr_sysPI_struct(sys, Br, lqrQPI);
    disp('building PI')
end

SysCtrl = myCellfun(@(S) buildCtrl(S,BrTrue), SysAug);


gains = [gain1,gain2]/gain1;
K_mb = [1,25];
if doOverwriteK
    for i = 1:2
        sc_ = SysCtrl(i)
        
        
        if ctrlType=='P'
            Knew = K_mb(1)/gains(i)
        else
            Knew = K_mb
            Knew(1)/gains(i)
        end
        %}
        %Knew = sc_.K
        
        if ctrlType=='P'
            newSys = close_sysP(sc_.sys_ol, Knew)
        else
            newSys = close_sysPI(sc_.sys_ol, BrTrue, Knew)
        end
        SysCtrl(i).sys_cl = newSys
        SysCtrl(i).clA = newSys.A
        SysCtrl(i).clB = newSys.B
        SysCtrl(i).clC = newSys.C
        SysCtrl(i).clD = newSys.D
        SysCtrl(i).K = Knew
    end
end
%{
K_mb = [1,25];
disp('overwriting gains')
SysCtrl(1).K = K_mb;
SysCtrl(2).K = K_mb;
SysCtrl(3).K = K_mb
%}


SysInf = SysCtrl;

stept=2;
lineColors = lines(10);

figure(10)
clf
subplot(1,2,1)
hold on
for i = 1:2
    stepPlot(SysCtrl(i).sys_ol, stept, lineColors(i,:));
end
xlim([0,stept])
title('open loop')

subplot(1,2,2)
hold on
%stepPlot(SysCtrl(1).sys_ol/gain1);

for i = 1:2
    stepPlot(SysCtrl(i).sys_cl,stept,lineColors(i,:));
end
stepPlot(SysCtrl(1).sys_ol/gain1,stept,'c');

xlim([0,stept])
title('closed loop')
set(gcf,'Position',[   440   628   919   170])

%stepPlot(sysCNew,stept,'m');

%%
% aux params



switchmode = 0
q_tru = testData.state; q_tru(1)=1;
q_flat = ones(size(q_tru));

q_sim = q_tru;%2*q_flat

%q_tru = 3-q_tru;
eseq = q_tru; %should be spikes, but we're using ground truth decoding?
%r = ones(size(testData.lambdaRef));
r = testData.lambdaRef; %times Nbar?
if doSineTarget
    r = ((sin(t)+1.0001)/2)*max(r)
end
S_tru = SysCtrl;
S_inf = SysCtrl;

Te = NaN*ones(2);
Ee = NaN*ones(2);

%%
[x_,y_,u_,q_,zspk_,k_, xhat_spf_, yhat_spf_,z_,buff] = loopSimSwitchCtrlSSPPF_mismatch_flexnl(noiseMag,QspfMag,switchmode,q_sim,eseq,inlf(r),SysCtrl,SysInf, t,TS,Te,Ee);

%%

%u_effective = -u_ + buff./ (SysCtrl(1).olB);
u_effective = buff;

%{
figure(2)
clf
subplot(3,1,1)
hold on
plot(t,testData.u,'c')

plot(t,u_,'b:')
plot(t,u_effective,'b')

hold off
%}
%%
 xself = @(X) X(1,:);

%x_recon = lsim(sys1,testData.u./q_tru,t)
%x_recon = ((testData.u)./(q_tru)/gain1)
%x_recon = lsim(sys1,testData.u./(q_tru),t);
%x_recon = lsim(SysCtrl(1).sys_ol,(u_./q_tru),t);
x_recon = lsim(SysCtrl(1).sys_ol,u_effective./q_tru,t);

figure(1)
clf
subplot(5,1,1)
hold on
plot(t,r,'Color',darkGreen,'LineWidth',3)
plot(t,testData.lambdaTrue,'r')
plot(t,z_,'-','Color',darkCyan)
ylabel('spks/sample')
hold off

subplot(5,1,2)
hold on
plot(t,q_,'m','LineWidth',3)
plot(t,testData.state,'Color',[1,0,1]/2)
ylim([1-.1,2+.1])


subplot(5,1,3)
hold on
plot(t, testData.u,'c')
plot(t, u_effective,'b','LineWidth',2)

plot(t, u_,'b:','LineWidth',1)
hold off
ylim([-.5,3])


subplot(5,1,4)
hold on
plot(t,inlf(r),'-','Color',darkGreen,'LineWidth',4)
plot(t,testData.xTrue,'r')
plot(t,testData.xHat,'k')
plot(t,y_,'Color',darkCyan)
plot(t,x_recon,':','Color',[1,.5,0])

%plot(t,squeeze(xhat_spf_(1,1,2,:)),'b','LineWidth',3)

subplot(5,1,5)
hold on
plot(t,zspk_+1,'k')
plot(t,testData.z,'Color',[1,1,1]/2)
%ylim([0,2])

set(gcf,'Position',[    39    97   601   705])
%%


%%

%{
figure(3)
clf
title('nonlinearity')
x1 = xself(x_);
%x1 = log(y_)-d_;


subplot(1,2,1)
hold on
plot(testData.xHat,testData.lambdaHat/dt,'c')
plot(testData.xHat,nlf(testData.xHat)/dt,'ko')
hold off
xlim([0,5])
ylim([0,50])
ylabel('FR')
xlabel('x')

subplot(1,2,2)
hold on

plot(y_,nlf(y_)/dt)
xlim([0,5])

ylim([0,50])

%plot(x1,nlf(x1),'b.')
%plot(x1,y_,'r+')
%plot(x1,z_,'ko')

%xlim([0,5])
%ylim([0,.1])
%}
    
    
    
%{    
figure(5)
clf
subplot(2,1,1)
hold on
plot(t,xself(x_),'LineWidth',3)
plot(t,xself(xhat_spf_))

subplot(2,1,2)
%plot(xself(x_),xself(xhat_spf_))
hold on

plot(t,testData.u./testData.xTrue)
plot([0,duration],[1,1]*b1*10,'m:')
plot([0,duration],[1,1]*b2*10,'m:')

ylim([0,2])
%}
%{
    %plot(testData.xHat, 3-testData.xHat,'k.')  
    plot(testData.xHat, 4-testData.xHat,'k.')  
    plot(testData.xHat, 2*(4-testData.xHat),'k.')  

  plot(testData.xHat(100:end-100),testData.u(100:end-100),'r.')  

    %}

%{
figure(7)
clf
plot(x_recon, testData.xTrue,'k.')
xlim([0,7])
 ylim([0,7])
    %}
    
    
    
    