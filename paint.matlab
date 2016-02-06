readChannelID = 84835;

[sht31, time] = thingSpeakRead(readChannelID, 'Fields', 1, 'NumPoints', 10);
am2302 = thingSpeakRead(readChannelID, 'Fields', 3, 'NumPoints', 10);
am2320 = thingSpeakRead(readChannelID, 'Fields', 5, 'NumPoints', 10);

thingSpeakPlot(time, [sht31, am2302, am2320], 'legend',...
    {'SHT-31', 'AM2302', 'AM2320'});

