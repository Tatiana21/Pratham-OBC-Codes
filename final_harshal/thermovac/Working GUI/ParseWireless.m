%Written by Abhishek Potnis
function  [HM1,HM2, CRC1, CRC2, T1, T2, Lat, Lon, Alt, OC_bits1,OC_bits2, Load_stat_bits1,  Load_stat_bits2]= ParseWireless(data)
% "data" should be a row vector containing data of one set of reading
[row,col]=size(data);
disp(row);
disp(col);

%%
HM1=zeros(1,7);
HM2=zeros(1,7);
T1=zeros(1,4);
T2=zeros(1,4);
Lat=zeros(1,4);
Lon=zeros(1,4);
Alt=zeros(1,4);
OC_bits1=logical(zeros(1,8));
OC_bits2=logical(zeros(1,8));
Load_stat_bits1=logical(zeros(1,8));
Load_stat_bits2=logical(zeros(1,8));
CRC1 = zeros(1,2);
CRC2 = zeros(1,2);
payload = zeros(1,34);

%%
index=findpattern(data, [67 82 67 65]);
for i=1:2
    CRC1(i)=data(index+i+3);
end

index=findpattern(data, [67 82 67 66]);
for i=1:2
    CRC2(i)=data(index+i+3);
end

index=findpattern(data, [68 65 84 65]);
for i=1:34
    payload(i)=data(index+i+3);
end

for i=1:7
    HM1(i)=payload(i);
end

for i=1:4
    T1(i)=payload(7+i);
end

for i=1:7
    HM2(i)=payload(11+i);
end

for i=1:4
    T2(i)=payload(18+i);
end

for i=1:4
    Lat(i)=payload(22+i);
end

for i=1:4
    Lon(i)=payload(26+i);
end

for i=1:4
    Alt(i)=payload(30+i);
end


temp=HM1(7);%OC status
OC_bits1=extract_bits(temp);

temp=HM1(1);%Load status
Load_stat_bits1=extract_bits(temp);

temp=HM2(7);%OC status
OC_bits2=extract_bits(temp);

temp=HM2(1);%Load status
Load_stat_bits2=extract_bits(temp);

end