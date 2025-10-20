%{
In this script I just import the files then I plot them onto the graph
with their respective lables which I did at the beginning. I plot
them on the graph by getting the data from the two columns of my data.
One of them for wind speed the other for voltage. Then I just added
legend and titles so that it is easy to comprehend.
This script serves
as one for all the files combine if you want to only use certain files
from certain tests just remove them so that they arent exported and
the code will still run just fine.
%}
% setting the figure to a certain size since publish it might shrink
figure('Position', [100, 100, 1200, 800]); % width 800 height 400
% Importing files and adding the names for the labels for Tuesday files
tuesday_files = {'data1.csv', 'data2.csv', 'data3.csv', 'data4.csv'};
tuesday_labels = {
'Medium motor, 100μF capacitor, 154mm 22° blade', ...
'Small motor, 100μF capacitor, 154mm 22° blade', ...
'Small motor, 100μF capacitor, 200mm 22° blade', ...
'Small motor, 100μF capacitor, 200mm 28° blade (ended early)'
};
% Importing files again but this time for Wednesday
wednesday_files = {'test1.csv', 'test2.csv', 'test4.csv'};
wednesday_labels = {
'Small motor, 68μF capacitor, 154mm 22° blade', ...
'Small motor, 47μF capacitor, 154mm 22° blade', ...
'Small motor, 100μF capacitor, 200mm 28° blade'
};
% Creating the colors
colors = {'b', 'r', 'g', 'y', 'm', 'c', 'k'};
figure;
hold on;
% Using a loop to plot the data for the Tuesday files
for i = 1:length(tuesday_files)
data = readtable(tuesday_files{i});
wind_speed = data{:, 1};
voltage = data{:, 2};
plot(wind_speed, voltage, 'Color', colors{i}, 'LineWidth', 1.5,
'DisplayName', tuesday_labels{i});
end
% Using a loop to plot the data for the Wednesday files
for i = 1:length(wednesday_files)
data = readtable(wednesday_files{i});
wind_speed = data{:, 1};
voltage = data{:, 2};
plot(wind_speed, voltage, 'Color', colors{i + length(tuesday_files)},
'LineWidth', 1.5, 'DisplayName', wednesday_labels{i});
end
1
% Adding all the labels, legends and text so that the graph is clear
hold off;
title('All Tests');
xlabel('Wind Speed (m/s)');
ylabel('Voltage (V)');
legend('Location', 'southoutside');
text(6.5, -0.1, 'ab4647', 'HorizontalAlignment', 'right',
'VerticalAlignment', 'bottom', 'FontSize', 16);
grid on;