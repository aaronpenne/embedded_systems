
clear all; close all; clc;
load('memm_matlab.mat')
% Plots
%- Execution time vs array sizes
%- L1 and L2 cache miss number/rate vs. array sizes
%- execution time vs. cycle count
%-execution time vs. L1 and L2 cache miss numbers/rates
%CPU energy vs. CPU frequency for different array sizes

% Execution time vs array sizes 
figure;
x = array_max(3,:);
y1 = array_max(5,:);
y2 = array_min(5,:);
plot(x,y1); hold on;
plot(x,y2);
title('Exe Time for Varied Array Sizes');
xlabel('Array Sizes (bytes)'); 
ylabel('Execution Time (\mus)'); 
legend('Max Freq', 'Min Freq', 'location', 'southeast');
axis([-inf inf -inf inf]);
% ax = gca; 
% ax.XTick = -0.5:0.125:0.5; 
% ax.YTick = 0:5:70;
% ax.YTickLabel = {0, 3.123456789, 5, 5, 3};
grid on;

% L1 L2 miss rates vs array sizes 
figure;
x = array_max(3,:);
y1 = array_max(9,:);
y2 = array_max(12,:);
y3 = array_min(9,:);
y4 = array_min(12,:);
plot(x,y1); hold on;
plot(x,y2); hold on;
plot(x,y3); hold on;
plot(x,y4);
title('L1 and L2 Miss % for Varied Array Sizes');
xlabel('Array Sizes (bytes)'); 
ylabel('Cache Miss Rate (%)'); 
legend('L1 Max Freq', 'L2 Max Freq', 'L1 Min Freq', 'L2 Min Freq', 'location', 'southeast');
axis([-inf inf -inf inf]);
% ax = gca; 
% ax.XTick = -0.5:0.125:0.5; 
% ax.YTick = 0:5:70;
% ax.YTickLabel = {0, 3.123456789, 5, 5, 3};
grid on;

% exe time vs cycle count
figure;
x1 = array_max(5,:);
y1 = array_max(6,:);
x2 = array_min(5,:);
y2 = array_min(6,:);
plot(x1,y1); hold on;
plot(x2,y2);
title('Exe Time vs Cycle Count');
xlabel('Execution Time (\mus)'); 
ylabel('Cycle Count'); 
legend('Max Freq', 'Min Freq', 'location', 'southeast');
axis([-inf inf -inf inf]);
% ax = gca; 
% ax.XTick = -0.5:0.125:0.5; 
% ax.YTick = 0:5:70;
% ax.YTickLabel = {0, 3.123456789, 5, 5, 3};
grid on;

% exe time vs L1 and L2 rate
figure;
x1 = array_max(5,:);
y1 = array_max(9,:);
y2 = array_max(12,:);
x2 = array_min(5,:);
y3 = array_min(9,:);
y4 = array_min(12,:);
plot(x1,y1); hold on;
plot(x1,y2); hold on;
plot(x2,y3); hold on;
plot(x2,y4);
title('Exe Time vs L1 and L2 Miss %');
xlabel('Execution Time (\mus)'); 
ylabel('Cache Miss Rate (%)'); 
legend('L1 Max Freq', 'L2 Max Freq', 'L1 Min Freq', 'L2 Min Freq', 'location', 'southeast');
axis([-inf inf -inf inf]);
% ax = gca; 
% ax.XTick = -0.5:0.125:0.5; 
% ax.YTick = 0:5:70;
% ax.YTickLabel = {0, 3.123456789, 5, 5, 3};
grid on;

% CPU energy, freq, array
figure;
x = array_max(3,:);
y1 = array_max(14,:);
y2 = array_min(14,:);
plot(x,y1); hold on;
plot(x,y2); 
title('CPU Energy for Varied Array Sizes');
xlabel('Array Sizes (bytes)'); 
ylabel('CPU Energy (\muJ)'); 
legend('Max Freq', 'Min Freq', 'location', 'southeast');
axis([-inf inf -inf inf]);
% ax = gca; 
% ax.XTick = -0.5:0.125:0.5; 
% ax.YTick = 0:5:70;
% ax.YTickLabel = {0, 3.123456789, 5, 5, 3};
grid on;

% iteration vs miss rates
figure;
x = iteration(1,:)
y1 = iteration(9,:);    % L1
y2 = iteration(12,:);   % L2
plot(x,y1); hold on;
plot(x,y2);
title('L1 and L2 Miss % for Varied DEF\_ITERATION');
xlabel('DEF\_ITERATION'); 
ylabel('Cache Miss Rate (%)'); 
legend('L1 Miss %', 'L2 Miss %');
axis([-inf inf -inf inf]);
% ax = gca; 
% ax.XTick = -0.5:0.125:0.5; 
% ax.YTick = 0:5:70;
grid on;

% iteration vs exe time
figure;
x = iteration(1,:);
y = iteration(5,:);
plot(x,y);
title('Exe Time for Varied DEF\_ITERATION');
xlabel('DEF\_ITERATION'); 
ylabel('Execution Time (\mus)');
legend('Exe Time', 'location', 'southeast');
axis([-inf inf -inf inf]);
% ax = gca; 
% ax.XTick = -0.5:0.125:0.5; 
% ax.YTick = 0:5:70;
% ax.YTickLabel = {0, 3.123456789, 5, 5, 3};
grid on;

% stride vs miss rates
figure;
x = stride(2,:);
y1 = stride(9,:);    % L1
y2 = stride(12,:);   % L2
plot(x,y1); hold on;
plot(x,y2);
title('L1 and L2 Miss % for Varied DEF\_STRIDE');
xlabel('DEF\_STRIDE'); 
ylabel('Cache Miss Rate (%)'); 
legend('L1 Miss %', 'L2 Miss %', 'location', 'southeast');
axis([-inf inf -inf inf]);
% ax = gca; 
% ax.XTick = -0.5:0.125:0.5; 
% ax.YTick = 0:5:70;
grid on;

% stride vs exe time
figure;
x = stride(2,:);
y = stride(5,:);
plot(x,y);
title('Exe Time for Varied DEF\_STRIDE');
xlabel('DEF\_STRIDE'); 
ylabel('Execution Time (\mus)');
legend('Exe Time', 'location', 'southeast');
axis([-inf inf -inf inf]);
% ax = gca; 
% ax.XTick = -0.5:0.125:0.5; 
% ax.YTick = 0:5:70;
grid on;

