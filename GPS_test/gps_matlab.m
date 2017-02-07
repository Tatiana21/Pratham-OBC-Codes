clear all;
clc;   
%load('vishalnom_detumb+nom.mat');

s =serial('COM7');
set(s,'BaudRate',9600,'Databits',8,'Parity','none','Stopbits',2);

fopen(s);
flushinput(s);
comp_ss=zeros(120001,26);
i=1;
j=1;
m=1;

for m=1:120001
%for i=1:155
%   fwrite(s,GPS_data(m,i),'uint8');
%  end
for j=1:26
   comp_ss(m,j) = fread(s,1,'uint8');
end
m
end