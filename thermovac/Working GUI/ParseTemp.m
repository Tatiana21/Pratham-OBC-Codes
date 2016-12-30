%Written by Abhishek Potnis
function [ temp ] = ParseTemp(tempData)

temp=zeros(1,4);

index=findpattern(tempData, [97 98]);
%index
temp(1)=tempData(index(1)+1+1);

index=findpattern(tempData, [99 100]);
%index
temp(2)=tempData(index(1)+1+1);

index=findpattern(tempData, [101 102]);
%index
temp(3)=tempData(index(1)+1+1);

index=findpattern(tempData, [103 104]);
%index
temp(4)=tempData(index(1)+1+1);

end