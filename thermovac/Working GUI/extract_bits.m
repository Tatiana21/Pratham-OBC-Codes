function [ out ] = extract_bits( in )
if in<0 || in>255
    error('Input should be between 0 and 255');    
end
out=logical(zeros(1,8));
if in>0
    in=floor(in);
   out(8)=mod(in,2); 
   for i=1:7
       j=8-i+1;
      out(i)= (mod(in,2^j)-mod(in,2^(j-1)))/2^(j-1);
   end
end
end

