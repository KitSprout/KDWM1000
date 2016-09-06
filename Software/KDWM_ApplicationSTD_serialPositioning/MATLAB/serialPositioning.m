
clear all;

delete(instrfindall);
info  = instrhwinfo('serial');
comPort = info.AvailableSerialPorts

s = serial(comPort, 'BaudRate', 115200, 'DataBits', 8, 'StopBits', 1, 'Parity', 'none', 'FlowControl', 'none');
s.ReadAsyncMode = 'continuous';
fopen(s);

data = zeros(4, 1024);

recvBufferMax  = 512;
recvBufferLens = 0;
recvBuffer = zeros(recvBufferMax, 1);

packetMax = 22;

indexMax  = fix(recvBufferMax / packetMax) + 1;
index = zeros(1, indexMax);


y_axisMax    = 10;
y_axisMin    =  0;
window_width = 800;
window_w = window_width;
window   = window_w * (-0.9);   % display window

fig = struct;
fig.main = figure(1);
set(fig.main, 'Position', [200, 100, 1200, 700], 'color', 'w');

fig.sub(1) = subplot(4, 3, 3);
fig.sub(2) = subplot(4, 3, 6);
fig.sub(3) = subplot(4, 3, 9);
fig.sub(4) = subplot(4, 3, 12);
fig.sub(5) = subplot(4, 3, [1:2, 4:5, 7:8, 10:11]);

fig.sub(1).Position = [0.5500, 0.7673, 0.4250, 0.1577];     % 0.6916, 0.7810, 0.2134, 0.1440
fig.sub(2).Position = [0.5500, 0.5482, 0.4250, 0.1577];     % 0.6916, 0.5482, 0.2134, 0.1577
fig.sub(3).Position = [0.5500, 0.3291, 0.4250, 0.1577];     % 0.6916, 0.3291, 0.2134, 0.1577
fig.sub(4).Position = [0.5500, 0.1100, 0.4250, 0.1577];     % 0.6916, 0.1100, 0.2134, 0.1577
fig.sub(5).Position = [0.0000, 0.0800, 0.5500, 0.8800];     % 0.1300, 0.1100, 0.4942, 0.8150

fig.text(1) = text(fig.sub(1), window + window_w * 0.92, 9, 'text', 'HorizontalAlignment', 'center');
fig.text(2) = text(fig.sub(2), window + window_w * 0.92, 9, 'text', 'HorizontalAlignment', 'center');
fig.text(3) = text(fig.sub(3), window + window_w * 0.92, 9, 'text', 'HorizontalAlignment', 'center');
fig.text(4) = text(fig.sub(4), window + window_w * 0.92, 9, 'text', 'HorizontalAlignment', 'center');

LAB_WIDTH  = 7.80;
LAB_LENGTH = 8.85;
LAB_HEIGHT = 2.78;

hold(fig.sub(5), 'on');
grid(fig.sub(5), 'on');
axis(fig.sub(5), 'equal');
axis(fig.sub(5), [-0.5, LAB_WIDTH + 0.5, -0.5, LAB_LENGTH + 0.5, 0, LAB_HEIGHT]);
xlabel(fig.sub(5),'x (m)');
ylabel(fig.sub(5),'y (m)');
zlabel(fig.sub(5),'z (m)');

global uwb;
uwb = struct;
uwb.dist   = zeros(4, 1);
uwb.mean   = zeros(4, 1);
uwb.error  = zeros(4, 1);

uwb.wall = rectangle(fig.sub(5), 'Position', [0, 0, LAB_WIDTH, LAB_LENGTH], 'LineWidth', 2);

uwb.targetPos  = [6.70, 3.95, 0.79];   % my seat
uwb.target     = plot(fig.sub(5), uwb.targetPos(1), uwb.targetPos(2), 'r.', 'MarkerSize', 32);
uwb.targetText = text(fig.sub(5), uwb.targetPos(1), 0.3 + uwb.targetPos(2), uwb.targetPos(3), num2str([uwb.targetPos(1), uwb.targetPos(2)], '(%6.3f, %6.3f)'), 'HorizontalAlignment', 'center');

uwb.anchorPos = zeros(4, 3);
uwb.anchorPos(1, :) = [6.70, 7.30, 2.50];   % anchor 10
uwb.anchorPos(2, :) = [1.10, 7.30, 2.50];   % anchor 11
uwb.anchorPos(3, :) = [6.70, 1.35, 2.50];   % anchor 12
uwb.anchorPos(4, :) = [1.10, 1.35, 2.50];   % anchor 13
uwb.anchorText(1) = text(fig.sub(5), uwb.anchorPos(1, 1), 0.5 + uwb.anchorPos(1, 2), uwb.anchorPos(1, 3), num2str([10, uwb.anchorPos(1, 1), uwb.anchorPos(1, 2)], 'A%2i\n(%4.2f, %4.2f)'), 'HorizontalAlignment', 'center');
uwb.anchorText(2) = text(fig.sub(5), uwb.anchorPos(2, 1), 0.5 + uwb.anchorPos(2, 2), uwb.anchorPos(1, 3), num2str([11, uwb.anchorPos(2, 1), uwb.anchorPos(2, 2)], 'A%2i\n(%4.2f, %4.2f)'), 'HorizontalAlignment', 'center');
uwb.anchorText(3) = text(fig.sub(5), uwb.anchorPos(3, 1), 0.5 + uwb.anchorPos(3, 2), uwb.anchorPos(1, 3), num2str([12, uwb.anchorPos(3, 1), uwb.anchorPos(3, 2)], 'A%2i\n(%4.2f, %4.2f)'), 'HorizontalAlignment', 'center');
uwb.anchorText(4) = text(fig.sub(5), uwb.anchorPos(4, 1), 0.5 + uwb.anchorPos(4, 2), uwb.anchorPos(1, 3), num2str([13, uwb.anchorPos(4, 1), uwb.anchorPos(4, 2)], 'A%2i\n(%4.2f, %4.2f)'), 'HorizontalAlignment', 'center');
uwb.anchor(1) = plot3(fig.sub(5), uwb.anchorPos(1, 1), uwb.anchorPos(1, 2), uwb.anchorPos(1, 3), 'g.', 'MarkerSize', 32);
uwb.anchor(2) = plot3(fig.sub(5), uwb.anchorPos(2, 1), uwb.anchorPos(2, 2), uwb.anchorPos(1, 3), 'g.', 'MarkerSize', 32);
uwb.anchor(3) = plot3(fig.sub(5), uwb.anchorPos(3, 1), uwb.anchorPos(3, 2), uwb.anchorPos(1, 3), 'g.', 'MarkerSize', 32);
uwb.anchor(4) = plot3(fig.sub(5), uwb.anchorPos(4, 1), uwb.anchorPos(4, 2), uwb.anchorPos(1, 3), 'g.', 'MarkerSize', 32);
uwb.anchorRange(1) = rectangle(fig.sub(5), 'EdgeColor', 'b', 'Position', [uwb.anchorPos(1, 1) - uwb.mean(1), uwb.anchorPos(1, 2) - uwb.mean(1), uwb.mean(1) * 2, uwb.mean(1) * 2], 'Curvature', [1, 1]);
uwb.anchorRange(2) = rectangle(fig.sub(5), 'EdgeColor', 'b', 'Position', [uwb.anchorPos(2, 1) - uwb.mean(2), uwb.anchorPos(2, 2) - uwb.mean(2), uwb.mean(2) * 2, uwb.mean(2) * 2], 'Curvature', [1, 1]);
uwb.anchorRange(3) = rectangle(fig.sub(5), 'EdgeColor', 'b', 'Position', [uwb.anchorPos(3, 1) - uwb.mean(3), uwb.anchorPos(3, 2) - uwb.mean(3), uwb.mean(3) * 2, uwb.mean(3) * 2], 'Curvature', [1, 1]);
uwb.anchorRange(4) = rectangle(fig.sub(5), 'EdgeColor', 'b', 'Position', [uwb.anchorPos(4, 1) - uwb.mean(4), uwb.anchorPos(4, 2) - uwb.mean(4), uwb.mean(4) * 2, uwb.mean(4) * 2], 'Curvature', [1, 1]);


packetCount = 0;
%for i = 1 : 10000
while ishandle(fig.main)
    nBytes = get(s, 'BytesAvailable');
    if nBytes > 0
        readData = fread(s, nBytes, 'uint8');
        recvBuffer(recvBufferLens + 1 : recvBufferLens + nBytes) = readData;
        recvBufferLens = recvBufferLens + nBytes;

        if recvBufferLens >= packetMax
            packetIndex = recvBufferLens - packetMax + 1;
            index(:) = 0;
            % find available packet
            while packetIndex > 0
                if isequal(recvBuffer(packetIndex : packetIndex + 3), [83; 45; 75; 83])     % S-KS
                    if isequal(recvBuffer(packetIndex + 20 : packetIndex + 21), [13; 10])   % \r\n
                        index = [packetIndex, index(1:end-1)];
                        packetIndex = packetIndex - packetMax + 1;
                        packetCount = packetCount + 1;
%fprintf('[%5i] %4i\n', i, packetCount);
                    end
                end
                packetIndex = packetIndex - 1;
            end
            % check available packet index
            indexLens = size(find(index ~= 0), 2);
            if indexLens ~= 0
                % get data from buffer
                fpdata = zeros(4, indexLens);
                for k = 1 : indexLens
                    fpdata(:, k) = typecast(uint8(recvBuffer(index(k) + 4 : index(k) + 19)), 'single');
                end
                data = [data(:, indexLens + 1 : end), fpdata];
                % update buffer & lengths
                recvBuffer = [recvBuffer(index(indexLens) + packetMax : end); zeros(index(indexLens) + packetMax - 1, 1)];
                recvBufferLens = recvBufferLens - (index(indexLens) + packetMax - 1);

                % plot data
                hold(fig.sub(1), 'off');
                hold(fig.sub(2), 'off');
                hold(fig.sub(3), 'off');
                hold(fig.sub(4), 'off');
                grid(fig.sub(1), 'on');
                grid(fig.sub(2), 'on');
                grid(fig.sub(3), 'on');
                grid(fig.sub(4), 'on');
                hold(fig.sub(1), 'on');
                hold(fig.sub(2), 'on');
                hold(fig.sub(3), 'on');
                hold(fig.sub(4), 'on');
                runtimes = (packetCount - window_width + 1) : packetCount;
                plot(fig.sub(1), runtimes, data(1, (end - window_width + 1) : end), 'b');
                plot(fig.sub(2), runtimes, data(2, (end - window_width + 1) : end), 'b');
                plot(fig.sub(3), runtimes, data(3, (end - window_width + 1) : end), 'b');
                plot(fig.sub(4), runtimes, data(4, (end - window_width + 1) : end), 'b');

                % move axis
                axis(fig.sub(1), [window, window + window_w, y_axisMin, y_axisMax]);
                axis(fig.sub(2), [window, window + window_w, y_axisMin, y_axisMax]);
                axis(fig.sub(3), [window, window + window_w, y_axisMin, y_axisMax]);
                axis(fig.sub(4), [window, window + window_w, y_axisMin, y_axisMax]);
                window = window + indexLens;
                drawnow

                % display data
                uwb.dist  = data(:, end);
                uwb.mean  = mean(data(:, (end - 9) : end), 2);
                uwb.error = sqrt((uwb.mean - uwb.dist).^2);
                fig.text(1).Position = [window + window_w * 0.95, 7.5, 1];
                fig.text(2).Position = [window + window_w * 0.95, 7.5, 1];
                fig.text(3).Position = [window + window_w * 0.95, 7.5, 1];
                fig.text(4).Position = [window + window_w * 0.95, 7.5, 1];
                fig.text(1).String = num2str([10, uwb.mean(1), uwb.error(1)], 'A%2i\n%6.3fm\n(%6.3f)');
                fig.text(2).String = num2str([11, uwb.mean(2), uwb.error(2)], 'A%2i\n%6.3fm\n(%6.3f)');
                fig.text(3).String = num2str([12, uwb.mean(3), uwb.error(3)], 'A%2i\n%6.3fm\n(%6.3f)');
                fig.text(4).String = num2str([13, uwb.mean(4), uwb.error(4)], 'A%2i\n%6.3fm\n(%6.3f)');
%{
fprintf('----- d10 --- d11 --- d12 --- d13 --\n');
fprintf('m = %6.3f, %6.3f, %6.3f, %6.3f m\n', uwb.mean);
fprintf('e = %6.3f, %6.3f, %6.3f, %6.3f m\n', uwb.error);
%}
                % update position
                ls_pos = positioning(uwb.anchorPos, uwb.mean);
                uwb.targetPos = [ls_pos', uwb.targetPos(3)];
                uwb.targetText.Position = [uwb.targetPos(1), 0.5 + uwb.targetPos(2), uwb.targetPos(3)];
                uwb.targetText.String = num2str([uwb.targetPos(1), uwb.targetPos(2)], 'Target\n(%6.3f, %6.3f)');
                uwb.target.XData = uwb.targetPos(1);
                uwb.target.YData = uwb.targetPos(2);
                uwb.target.ZData = uwb.targetPos(3);
                uwb.anchorRange(1).Position = [uwb.anchorPos(1, 1) - uwb.mean(1), uwb.anchorPos(1, 2) - uwb.mean(1), uwb.mean(1) * 2, uwb.mean(1) * 2];
                uwb.anchorRange(2).Position = [uwb.anchorPos(2, 1) - uwb.mean(2), uwb.anchorPos(2, 2) - uwb.mean(2), uwb.mean(2) * 2, uwb.mean(2) * 2];
                uwb.anchorRange(3).Position = [uwb.anchorPos(3, 1) - uwb.mean(3), uwb.anchorPos(3, 2) - uwb.mean(3), uwb.mean(3) * 2, uwb.mean(3) * 2];
                uwb.anchorRange(4).Position = [uwb.anchorPos(4, 1) - uwb.mean(4), uwb.anchorPos(4, 2) - uwb.mean(4), uwb.mean(4) * 2, uwb.mean(4) * 2];
            end
        end
    end
end

fclose(s);
delete(s);
