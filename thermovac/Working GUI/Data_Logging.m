s=serial('COM5');
set(s,'BaudRate',9600,'Databits',8,'Parity','none','Stopbits',2);
fopen(s);
set(s, 'Timeout', 60);%set timeout in seconds
isvalid(s)
GPS_size=4;%including indentifier PRB= 80 82 66
SS_size=15;
HM_size=10;
Mag_size=9;
row=500; %number of datasets
col=GPS_size+SS_size+HM_size+Mag_size+6;
data = ones(row,col); %size of each dataset is 355 bytes (10+15+321+9)
%(HM:10, Sunsensors:15, GPS: 312, Mag:9) time_stamp:6 
flushinput(s);
HM=zeros(row,6+HM_size-3-2);%first 6 entries are of time_stamp 
%for each entry 3 bytes of identifier removed
SS=zeros(row,6+6);%time_stamp: year month day hours minutes seconds
GPS=zeros(row,6+GPS_size);
Mag=zeros(row,6+3);
OC_bits=logical(zeros(row,8));
Load_stat_bits=logical(zeros(row,8));
for row1=1:row
    %settings of serial fread: http://ubuntuforums.org/showthread.php?t=2009879
    disp(row1);
    time_stamp=clock;%time_stamp: year month day hours minutes seconds
    data(row1,1:6)=time_stamp(1:6);
    for col1=1:col-6
        data(row1,col1+6)=fread(s,1,'uint8');
    end
    %clock
    data_row=data(row1,:);
    [HM1,OC_bits1,Load_stat_bits1,SS1,GPS1,Mag1]= Parsing(data_row,GPS_size);
    HM(row1,1:6)=time_stamp(1:6);
    HM(row1,6+1:6+5)=HM1(1:5);
    OC_bits(row,1:8)=OC_bits1(1:8);
    Load_stat_bits(row,1:8)=Load_stat_bits1(1:8);
    SS(row1,1:6)=time_stamp(1:6);
    SS(row1,6+1:6+6)=SS1(1:6);
    GPS(row1,1:6)=time_stamp(1:6);
    GPS(row1,6+1:6+4)=GPS1(1:4);
    Mag(row1,1:6)=time_stamp(1:6);
    Mag(row1,6+1:6+3)=Mag1(1:3);
    if mod(row1,2)==0 %periodically write the data to a mat file (to avoid data loss)
        %each mat file will be over-written after every 2 data sets
        save('HM_data.mat','HM');
        save('OC_bits_data.mat','OC_bits');
        save('Load_stat_bits_data.mat','Load_stat_bits');
        save('SS_data.mat','SS');
        save('GPS_data.mat','GPS');
        save('Mag_data.mat','Mag');
        save('Raw_data.mat','data');
    end
end
fclose(s);