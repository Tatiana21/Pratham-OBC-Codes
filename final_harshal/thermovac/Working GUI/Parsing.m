%Written by Shantanu Shahane
function [ HM,OC_bits,Load_stat_bits, SS, GPS, Mag ] = Parsing(data,GPS_size)
% "data" should be a row vector containing data of one set of reading
[row,col]=size(data);
if row~=1 || col ~=323
    %error('The Parsing function should get a row vector of size 355 as input');
end
%%
HM=zeros(1,7);
%OC_bits=logical(zeros(1,8));
%Load_stat_bits=logical(zeros(1,8));
SS=zeros(1,6);
%GPS=zeros(1,9);
Mag=zeros(1,3);
%%
index=findpattern(data, [80 82 65]);
for i=1:7
    HM(i)=data(index+i+2);
end
temp=data(index+7+2);%OC status
OC_bits=extract_bits(temp);
disp(OC_bits);
temp=data(index+1+2);%Load status
Load_stat_bits=extract_bits(temp);
%%
index=findpattern(data, [80 82 67]);
for i=1:6
    SS(i)=data(index+2*i+1)+(2^8)*data(index+2*i+2);
end
%%
% index=findpattern(data, [63 63 04 172]);
% for i=1:3 %GPS position X, Y, Z
%     GPS(i)=data(index+4*i)+(2^8)*data(index+4*i+1)+(2^16)*data(index+4*i+2)...
%         +(2^24)*data(index+4*i+3);
% end
% index=findpattern(data, [63 63 05 172]);
% for i=1:3 %GPS velocity X, Y, Z
%     GPS(i+3)=data(index+4*i)+(2^8)*data(index+4*i+1)+(2^16)*data(index+4*i+2)...
%         +(2^24)*data(index+4*i+3);
% end
% index=findpattern(data, [63 63 14 172]);
% for i=1:3 %GPS lat, long, alt
%     GPS(i+6)=data(index+4+4*i)+(2^8)*data(index+4+4*i+1)+(2^16)*data(index+4+4*i+2)...
%         +(2^24)*data(index+4+4*i+3);
% end
index=findpattern(data, [80 82 66]);
%index1=findpattern(data, [63 63 04 172]);
GPS = data(index+3);
%%
index=findpattern(data, [80 82 68]);
for i=1:3 %Magnetic field X, Y, Z
    Mag(i)=(2^8)*data(index+2*i+1)+data(index+2*i+2);
    if Mag(i)>2^15
        Mag(i)=Mag(i)-2^16;
    end
end
end