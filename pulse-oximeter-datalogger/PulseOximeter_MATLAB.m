% this was made by
% ab4647

clear; clc; % clear workspace and cmd window

% first define serial port and get connection from it
serialPort = "COM6";
baudRate = 115200;
s = serialport(serialPort, baudRate); % create object with serial port

% now create data storage arrays that will be used later
% good programming practice
dataTime = [];
dataHR = [];
dataSpO2 = [];
reporting_time = 30; % 30 seconds of collecting values

% create plot now so that it can be live
figure;

% animated line allows for line to be updated in real time
line1 = animatedline('Color', "r", 'DisplayName', "Heart Rate");
line2 = animatedline('Color', "g", 'DisplayName', "SpO2");
legend; % legend coupled with display is automatic

% add information to the graph
xlabel("Time(s)");
ylabel("Values");
title("Pulse Oximeter Data");
grid on; % makes the plot easier to read

timeStart = datetime("now"); % give starting time

% read data from the serial port
while seconds(datetime("now") - timeStart) < reporting_time
    try
        % try reading serial line of data
        serialLine = readline(s);
        disp(serialLine); % useful for debugging

        % use strsplit over split - makes an array of everything but
        % whitespace - but is more robust
        separatedData = strsplit(serialLine);

        % change data from string to numeric
        numericData = str2double(separatedData);
        heartRate = numericData(1);
        SpO2 = numericData(2);

        if heartRate > 200 || SpO2 > 100
            disp("Data is out of realistic range");
            continue;
        end

        % get time that has passed since the start
        timePassed = seconds(datetime("now") - timeStart);

        % append data to arrays (add new rows below)
        % not super efficient but fine for small runs
        dataTime = [dataTime; timePassed];
        dataHR = [dataHR; heartRate];
        dataSpO2 = [dataSpO2; SpO2];

        % above lines necessary for post-processing
        % update live plot
        addpoints(line1, timePassed, heartRate);
        addpoints(line2, timePassed, SpO2);
        drawnow; % updates figure

    catch MException % can name it whatever, standard is ME
        disp("Execution has been stopped or there has been an error");
        disp(MException.message);
        break;
    end
end

% close the serial port - important, prepares for future use
% best practice
clear s;

% since after the while loop terminates the live plot disappears
% add a final plot for both HR and SpO2
figure;

subplot(2, 1, 1); % two rows one column
plot(dataTime, dataHR, 'r', 'LineWidth', 1.5, 'LineStyle', '--');
xlabel("Time(s)");
ylabel("Heart Rate");
title("Heart Rate Data from Pulse Oximeter");
grid on;

subplot(2, 1, 2);
plot(dataTime, dataSpO2, 'g', 'LineWidth', 1.5, 'LineStyle', '--');
xlabel("Time(s)");
ylabel("SpO2");
title("SpO2 Data from the Pulse Oximeter");
grid on;

% add completion message
disp("All data should be complete and hopefully accurate!");
