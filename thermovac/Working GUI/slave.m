function varargout = slave(varargin)
% SLAVE MATLAB code for slave.fig
%      SLAVE, by itself, creates a new SLAVE or raises the existing
%      singleton*.
%
%      H = SLAVE returns the handle to a new SLAVE or the handle to
%      the existing singleton*.
%
%      SLAVE('CALLBACK',hObject,eventData,handles,...) calls the local
%      function named CALLBACK in SLAVE.M with the given input arguments.
%
%      SLAVE('Property','Value',...) creates a new SLAVE or raises the
%      existing singleton*.  Starting from the left, property value pairs are
%      applied to the GUI before slave_OpeningFcn gets called.  An
%      unrecognized property name or invalid value makes property application
%      handles.stop.  All inputs are passed to slave_OpeningFcn via varargin.
%
%      *See GUI Options on GUIDE's Tools menu.  Choose "GUI allows only one
%      instance to run (singleton)".
%
% See also: GUIDE, GUIDATA, GUIHANDLES

% Edit the above text to modify the response to help slave

% Last Modified by GUIDE v2.5 09-Jul-2015 22:53:26

% Begin initialization code - DO NOT EDIT
gui_Singleton = 1;
gui_State = struct('gui_Name',       mfilename, ...
                   'gui_Singleton',  gui_Singleton, ...
                   'gui_OpeningFcn', @slave_OpeningFcn, ...
                   'gui_OutputFcn',  @slave_OutputFcn, ...
                   'gui_LayoutFcn',  [] , ...
                   'gui_Callback',   []);
if nargin && ischar(varargin{1})
    gui_State.gui_Callback = str2func(varargin{1});
end

if nargout
    [varargout{1:nargout}] = gui_mainfcn(gui_State, varargin{:});
else
    gui_mainfcn(gui_State, varargin{:});
end
% End initialization code - DO NOT EDIT


%Comm Port - Global Variable



% --- Executes just before slave is made visible.
function slave_OpeningFcn(hObject, eventdata, handles, varargin)
% This function has no output args, see OutputFcn.
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% varargin   command line arguments to slave (see VARARGIN)

% Choose default command line output for slave
handles.output = hObject;
%handles.stop = 0;
% Update handles structure
guidata(hObject, handles);

% UIWAIT makes slave wait for user response (see UIRESUME)
% uiwait(handles.figure1);


% --- Outputs from this function are returned to the command line.
function varargout = slave_OutputFcn(hObject, eventdata, handles) 
% varargout  cell array for returning output args (see VARARGOUT);
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Get default command line output from handles structure
varargout{1} = handles.output;


% --- Executes on button press in pushbutton1.
function pushbutton1_Callback(hObject, eventdata, handles)
% hObject    handle to Start (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% a='hello';
% current_index=5;
% for current_index=1:20
%     current_index
%     %string1 = sprintf('%.0f', current_index);
%     set(solar_I_obs,'String',sprintf('%.0f', current_index));
%     if(current_index>=5)
%         set(solar_I_obs,'BackgroundColor','green');
%     else
%       set(solar_I_obs,'BackgroundColor','red');  
%     end
     drawnow
%     pause(0.25);
%     save('current_index.mat','current_index');
% end
% for i=1:7
%  
%     set(handles.LS_Bea_obs,'BackgroundColor','green');
%     pause(0.25);
%     set(handles.LS_Tor_obs,'BackgroundColor','green');
%     pause(0.25);
%     set(handles.LS_Down_obs,'BackgroundColor','green');
%     pause(0.25);
%     set(handles.LS_Up_obs,'BackgroundColor','green');
%     pause(0.25);
%     set(handles.LS_GPS_obs,'BackgroundColor','green');
%     pause(0.25);    
%     set(handles.LS_OBC_obs,'BackgroundColor','green');
%     pause(0.25);
%     set(handles.LS_Mag_obs,'BackgroundColor','green');
%     pause(0.25);
%     
%     set(handles.OC_bea,'BackgroundColor','green');
%     pause(0.25);
%     set(handles.OC_tor,'BackgroundColor','green');
%     pause(0.25);
%     set(handles.OC_down,'BackgroundColor','green');
%     pause(0.25);
%     set(handles.OC_GPS,'BackgroundColor','green');
%     pause(0.25);
%     set(handles.OC_OBC,'BackgroundColor','green');
%     pause(0.25);
%     
%     set(handles.LS_Bea_obs,'BackgroundColor','white');
%     pause(0.25);
%     set(handles.LS_Tor_obs,'BackgroundColor','white');
%     pause(0.25);
%     set(handles.LS_Down_obs,'BackgroundColor','white');
%     pause(0.25);
%     set(handles.LS_Up_obs,'BackgroundColor','white');
%     pause(0.25);
%     set(handles.LS_GPS_obs,'BackgroundColor','white');
%     pause(0.25);    
%     set(handles.LS_OBC_obs,'BackgroundColor','white');
%     pause(0.25);
%     set(handles.LS_Mag_obs,'BackgroundColor','white');
%     pause(0.25);
%     
%     set(handles.OC_bea,'BackgroundColor','white');
%     pause(0.25);
%     set(handles.OC_tor,'BackgroundColor','white');
%     pause(0.25);
%     set(handles.OC_down,'BackgroundColor','white');
%     pause(0.25);
%     set(handles.OC_GPS,'BackgroundColor','white');
%     pause(0.25);
%     set(handles.OC_OBC,'BackgroundColor','white');
%     pause(0.25);
% 
%    
% end
%%

% s=serial('COM6');
s1=serial('COM10');
% set(s,'BaudRate',9600,'Databits',8,'Parity','none','stopbits',2);
set(s1,'BaudRate',9600,'Databits',8,'Parity','none','stopbits',2);
fopen(s1);
% fopen(s);
% set(s, 'Timeout', 60);%set timeout in seconds
set(s1, 'Timeout', 600);%set timeout in seconds
% isvalid(s)
isvalid(s1)

GPS_size=4;%including indentifier PRB= 80 82 66
SS_size=15;
HM_size=10;
Mag_size=10;
row=100000; %number of datasets
col=GPS_size+SS_size+HM_size+Mag_size+6;
data = ones(row,col); %size of each dataset is 355 bytes (10+15+321+9)
tempData = ones(row,30);
temp = ones(row, 10);
%(HM:10, Sunsensors:15, GPS: 312, Mag:9) time_stamp:6 
% flushinput(s);
flushinput(s1);
% handles.s = s;
% HM=zeros(row,6+HM_size-3);%first 6 entries are of time_stamp 
%for each entry 3 bytes of identifier removed
% SS=zeros(row,6+6);%time_stamp: year month day hours minutes seconds
% GPS=zeros(row,6+GPS_size-3);
% Mag=zeros(row,6+3);
% OC_bits=logical(zeros(row,8));
% Load_stat_bits=logical(zeros(row,8));
iteration = 0;
logFlag = 0;

tmin1 = 100;
tmin2 =100;
tmin3 = 100;
tmin4 =100;
tmax1 = 0;
tmax2 =0;
tmax3 = 0;
tmax4 = 0;

while true
    iteration  = iteration + 1;
    iString = num2str(iteration)
    mkdir(iString);
for row1=1:row
   % if handles.stop == 0
    %settings of serial fread: http://ubuntuforums.org/showthread.php?t=2009879
    disp(row1);
    set(handles.reading,'String',sprintf('%.0f', row1));
    time_stamp=clock;%time_stamp: year month day hours minutes seconds
    data(row1,1:6)=time_stamp(1:6);
%     for col1=1:col-6
%         data(row1,col1+6)=fread(s,1,'uint8');
%     end
    
    tempData(row1,1:6)=time_stamp(1:6);
    
    %Now listen to Slave
    for col1=1:24
        tempData(row1,col1+6)=fread(s1,1,'uint8');
    end

    %clock
%     data_row=data(row1,:);
%     [HM1,OC_bits1,Load_stat_bits1,SS1,GPS1,Mag1]= Parsing(data_row,GPS_size);
    tempData_row=tempData(row1,:);
    [temp1] = ParseTemp(tempData_row); 
%     HM(row1,1:6)=time_stamp(1:6);
%     HM(row1,6+1:6+7)=HM1(1:7);
%     OC_bits(row1,1:8)=OC_bits1;
%     %OC_bits1
%     Load_stat_bits(row1,1:8)=Load_stat_bits1;
%     SS(row1,1:6)=time_stamp(1:6);
%     SS(row1,6+1:6+6)=SS1(1:6);
%     GPS(row1,1:6)=time_stamp(1:6);
%     GPS(row1,6+1)=GPS1;
%     Mag(row1,1:6)=time_stamp(1:6);
%     Mag(row1,6+1:6+3)=Mag1(1:3);
%     %if mod(row1,2)==0 %periodically write the data to a mat file (to avoid data loss)
%         %each mat file will be over-written after every 2 data sets
%         save('HM_data.mat','HM');
%         save('OC_bits_data.mat','OC_bits');
%         save('Load_stat_bits_data.mat','Load_stat_bits');
%         save('SS_data.mat','SS');
%         save('GPS_data.mat','GPS');
%         save('Mag_data.mat','Mag');
%         save('Raw_data.mat','data');
      
    %end
    %temp1
    drawnow
    
    v1 = 3.3/255*temp1(1);
    r1 = 1/(3.3/v1-1)*10000;
    t11 = (log(r1/47000));
    temp(row1,1:6)=clock;
    temp(row1,7) = (1/(0.003354+0.000252*t11+0.00000351*t11*t11))-273;
    if temp(row1,7) <= tmin1
        tmin1 = temp(row1,7);
    end
    if temp(row1,7) >= tmax1
        tmax1 = temp(row1,7);
    end
    set(handles.edit132,'String',sprintf('%.2f',tmin1));
    set(handles.edit133,'String',sprintf('%.2f',tmax1));
    
    v2 = 3.3/255*temp1(2);
    r2 = 1/(3.3/v2-1)*10000;
    t12 = (log(r2/47000));
    temp(row1,8) = (1/(0.003354+0.000252*t12+0.00000351*t12*t12))-273;
    if temp(row1,8) < tmin2
    tmin2 = temp(row1,8);
    end
    if temp(row1,8) >= tmax2
        tmax2 = temp(row1,8);
    end
    set(handles.edit135,'String',sprintf('%.2f',tmin2));
    set(handles.edit136,'String',sprintf('%.2f',tmax2));
    
    
    v3 = 3.3/255*temp1(3);
    r3 = 1/(3.3/v3-1)*10000;
    t13 = (log(r3/47000));
    temp(row1,9) = (1/(0.003354+0.000252*t13+0.00000351*t13*t13))-273;
    if temp(row1,9) < tmin3
    tmin3 = temp(row1,9);
    end
    if temp(row1,9) >= tmax3
        tmax3 = temp(row1,9);
    end
    set(handles.edit138,'String',sprintf('%.2f',tmin3));
    set(handles.edit139,'String',sprintf('%.2f',tmax3));
    
    
    
    v4 = 3.3/255*temp1(4);
    r4 = 1/(3.3/v4-1)*10000;
    t14 = (log(r4/47000));
    temp(row1,10) = (1/(0.003354+0.000252*t13+0.00000351*t14*t14))-273;
    if temp(row1,10) < tmin4
    tmin4 = temp(row1,10);
    end
    if temp(row1,10) >= tmax4
        tmax4 = temp(row1,10);
    end
    set(handles.edit141,'String',sprintf('%.2f',tmin4));
    set(handles.edit142,'String',sprintf('%.2f',tmax4));
    
%     if exist( strcat(iString,'/temp.mat') , 'file') && logFlag == 0
%         logFlag = 1;
%         old = load(strcat(iString,'/temp.mat'), '-mat','temp');
%         oldTemp = old.temp;
%         whos oldTemp;
%         temp = 0;
%         temp = [oldTemp; temp];
%         whos new;
%         disp('concating');
%         save(strcat(iString,'/temp.mat'),'temp');
%     else
%     %save(strcat(iString,'/temp.mat'),'temp');
%     end
    save(strcat(iString,'/temp.mat'),'temp');
    set(handles.paBeacon,'String',sprintf('%.2f', temp(row1,7)));
    set(handles.paDown,'String',sprintf('%.2f', temp(row1,8)));
    set(handles.lna,'String',sprintf('%.2f', temp(row1,9)));
    set(handles.bat,'String',sprintf('%.2f', temp(row1,10)));
    
    % set(handles.gps_obs,'String',sprintf('%.0f', 0));
%     
%     set(handles.ss1_obs,'String',sprintf('%.0f', SS1(1)));
%     set(handles.ss2_obs,'String',sprintf('%.0f', SS1(2)));
%     set(handles.ss3_obs,'String',sprintf('%.0f', SS1(3)));
%     set(handles.ss4_obs,'String',sprintf('%.0f', SS1(4)));
%     set(handles.ss5_obs,'String',sprintf('%.0f', SS1(5)));
%     set(handles.ss6_obs,'String',sprintf('%.0f', SS1(6))); 
%     
%     set(handles.mag_x_obs,'String',sprintf('%.0f', Mag1(1)));
%     set(handles.mag_y_obs,'String',sprintf('%.0f', Mag1(2)));
%     set(handles.mag_z_obs,'String',sprintf('%.0f', Mag1(3)));
%     
%     obc_v = double(double(HM1(3)/255) * 3.3 * 2);
%     bat_v = double(double(HM1(2)/255)*3.3 * 3 + 0.3); 
%     set(handles.V_OBC_obs,'String',sprintf('%.2f', obc_v));
%     set(handles.V_bat_obs,'String',sprintf('%.2f',bat_v));
%     
%     solarI = double(double(double(HM1(5)/255)*3.3)/0.005);
%     set(handles.Solar_I_obs,'String',sprintf('%.2f', solarI));
%     loadI = double(double(double(double(HM1(6)/255)*3.3)/0.005)/50);
%     set(handles.Load_I_obs,'String',sprintf('%.5f', loadI));
%     
%     downV = double(double(HM1(4)/255) * 3.3 * 2);
%     set(handles.V_down_obs,'String',sprintf('%.2f', downV));
%     
%     if Load_stat_bits1(8) == 1
%     set(handles.LS_Bea_obs,'BackgroundColor','green');
%     else
%        set(handles.LS_Bea_obs,'BackgroundColor','white');
%     end
%     if Load_stat_bits1(7) == 1
%        set(handles.LS_Tor_obs,'BackgroundColor','green');
%     else
%         set(handles.LS_Tor_obs,'BackgroundColor','white');
%     end 
%     if Load_stat_bits1(6) == 1
%        set(handles.LS_GPS_obs,'BackgroundColor','green');
%     else
%        set(handles.LS_GPS_obs,'BackgroundColor','white');
%     end
%     if Load_stat_bits1(5) == 1
%        set(handles.LS_Down_obs,'BackgroundColor','green');
%     else
%         set(handles.LS_Down_obs,'BackgroundColor','white');
%     end    
%     if Load_stat_bits1(4) == 1
%        set(handles.LS_OBC_obs,'BackgroundColor','green');
%     else
%         set(handles.LS_OBC_obs,'BackgroundColor','white');
%     end
%     if Load_stat_bits1(3) == 1
%        set(handles.LS_Mag_obs,'BackgroundColor','green');
%     else
%        set(handles.LS_Mag_obs,'BackgroundColor','white');
%     end     
%     if Load_stat_bits1(2) == 1
%        set(handles.LS_Up_obs,'BackgroundColor','green');
%     else
%        set(handles.LS_Up_obs,'BackgroundColor','white');
%     end
%     
%     
%     if OC_bits1(8) == 0
%     set(handles.OC_bea,'BackgroundColor','red');
%     else
%        set(handles.OC_bea,'BackgroundColor','white');
%     end
%     if OC_bits1(7) == 0
%        set(handles.OC_tor,'BackgroundColor','red');
%     else
%        set(handles.OC_tor,'BackgroundColor','white');
%     end 
%     if OC_bits1(6) == 0
%        set(handles.OC_GPS,'BackgroundColor','red');
%     else
%        set(handles.OC_GPS,'BackgroundColor','white');
%     end
%     if OC_bits1(5) == 0
%        set(handles.OC_down,'BackgroundColor','red');
%     else
%         set(handles.OC_down,'BackgroundColor','white');
%     end    
%     if OC_bits1(4) == 0
%        set(handles.OC_OBC,'BackgroundColor','red');
%     else
%         set(handles.OC_OBC,'BackgroundColor','white');
%     end
% 
%     pause(0.25);
%     else
%         break
%     end    

    %end
% Update handles structure
handles.output = hObject;
guidata(hObject, handles);
end
end
    fclose(s1);
%%
% hObject    handle to pushbutton1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)



function edit55_Callback(hObject, eventdata, handles)
% hObject    handle to edit55 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit55 as text
%        str2double(get(hObject,'String')) returns contents of edit55 as a double


% --- Executes during object creation, after setting all properties.
function edit55_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edit55 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function edit56_Callback(hObject, eventdata, handles)
% hObject    handle to edit56 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit56 as text
%        str2double(get(hObject,'String')) returns contents of edit56 as a double


% --- Executes during object creation, after setting all properties.
function edit56_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edit56 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function edit57_Callback(hObject, eventdata, handles)
% hObject    handle to edit57 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit57 as text
%        str2double(get(hObject,'String')) returns contents of edit57 as a double


% --- Executes during object creation, after setting all properties.
function edit57_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edit57 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function edit58_Callback(hObject, eventdata, handles)
% hObject    handle to edit58 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit58 as text
%        str2double(get(hObject,'String')) returns contents of edit58 as a double


% --- Executes during object creation, after setting all properties.
function edit58_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edit58 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function edit59_Callback(hObject, eventdata, handles)
% hObject    handle to edit59 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit59 as text
%        str2double(get(hObject,'String')) returns contents of edit59 as a double


% --- Executes during object creation, after setting all properties.
function edit59_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edit59 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function edit60_Callback(hObject, eventdata, handles)
% hObject    handle to edit60 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit60 as text
%        str2double(get(hObject,'String')) returns contents of edit60 as a double


% --- Executes during object creation, after setting all properties.
function edit60_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edit60 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function edit61_Callback(hObject, eventdata, handles)
% hObject    handle to edit61 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit61 as text
%        str2double(get(hObject,'String')) returns contents of edit61 as a double


% --- Executes during object creation, after setting all properties.
function edit61_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edit61 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function edit62_Callback(hObject, eventdata, handles)
% hObject    handle to edit62 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit62 as text
%        str2double(get(hObject,'String')) returns contents of edit62 as a double


% --- Executes during object creation, after setting all properties.
function edit62_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edit62 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function edit63_Callback(hObject, eventdata, handles)
% hObject    handle to edit63 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit63 as text
%        str2double(get(hObject,'String')) returns contents of edit63 as a double


% --- Executes during object creation, after setting all properties.
function edit63_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edit63 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function edit64_Callback(hObject, eventdata, handles)
% hObject    handle to edit64 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit64 as text
%        str2double(get(hObject,'String')) returns contents of edit64 as a double


% --- Executes during object creation, after setting all properties.
function edit64_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edit64 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function edit65_Callback(hObject, eventdata, handles)
% hObject    handle to edit65 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit65 as text
%        str2double(get(hObject,'String')) returns contents of edit65 as a double


% --- Executes during object creation, after setting all properties.
function edit65_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edit65 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function edit66_Callback(hObject, eventdata, handles)
% hObject    handle to edit66 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit66 as text
%        str2double(get(hObject,'String')) returns contents of edit66 as a double


% --- Executes during object creation, after setting all properties.
function edit66_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edit66 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function edit67_Callback(hObject, eventdata, handles)
% hObject    handle to edit67 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit67 as text
%        str2double(get(hObject,'String')) returns contents of edit67 as a double


% --- Executes during object creation, after setting all properties.
function edit67_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edit67 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function edit68_Callback(hObject, eventdata, handles)
% hObject    handle to edit68 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit68 as text
%        str2double(get(hObject,'String')) returns contents of edit68 as a double


% --- Executes during object creation, after setting all properties.
function edit68_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edit68 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function edit69_Callback(hObject, eventdata, handles)
% hObject    handle to edit69 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit69 as text
%        str2double(get(hObject,'String')) returns contents of edit69 as a double


% --- Executes during object creation, after setting all properties.
function edit69_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edit69 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function edit70_Callback(hObject, eventdata, handles)
% hObject    handle to edit70 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit70 as text
%        str2double(get(hObject,'String')) returns contents of edit70 as a double


% --- Executes during object creation, after setting all properties.
function edit70_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edit70 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function edit71_Callback(hObject, eventdata, handles)
% hObject    handle to edit71 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit71 as text
%        str2double(get(hObject,'String')) returns contents of edit71 as a double


% --- Executes during object creation, after setting all properties.
function edit71_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edit71 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function edit72_Callback(hObject, eventdata, handles)
% hObject    handle to edit72 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit72 as text
%        str2double(get(hObject,'String')) returns contents of edit72 as a double


% --- Executes during object creation, after setting all properties.
function edit72_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edit72 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function edit73_Callback(hObject, eventdata, handles)
% hObject    handle to edit73 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit73 as text
%        str2double(get(hObject,'String')) returns contents of edit73 as a double


% --- Executes during object creation, after setting all properties.
function edit73_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edit73 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function edit74_Callback(hObject, eventdata, handles)
% hObject    handle to edit74 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit74 as text
%        str2double(get(hObject,'String')) returns contents of edit74 as a double


% --- Executes during object creation, after setting all properties.
function edit74_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edit74 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function edit75_Callback(hObject, eventdata, handles)
% hObject    handle to edit75 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit75 as text
%        str2double(get(hObject,'String')) returns contents of edit75 as a double


% --- Executes during object creation, after setting all properties.
function edit75_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edit75 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function edit76_Callback(hObject, eventdata, handles)
% hObject    handle to edit76 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit76 as text
%        str2double(get(hObject,'String')) returns contents of edit76 as a double


% --- Executes during object creation, after setting all properties.
function edit76_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edit76 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function edit77_Callback(hObject, eventdata, handles)
% hObject    handle to edit77 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit77 as text
%        str2double(get(hObject,'String')) returns contents of edit77 as a double


% --- Executes during object creation, after setting all properties.
function edit77_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edit77 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function edit78_Callback(hObject, eventdata, handles)
% hObject    handle to edit78 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit78 as text
%        str2double(get(hObject,'String')) returns contents of edit78 as a double


% --- Executes during object creation, after setting all properties.
function edit78_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edit78 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function edit79_Callback(hObject, eventdata, handles)
% hObject    handle to edit79 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit79 as text
%        str2double(get(hObject,'String')) returns contents of edit79 as a double


% --- Executes during object creation, after setting all properties.
function edit79_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edit79 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function edit80_Callback(hObject, eventdata, handles)
% hObject    handle to edit80 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit80 as text
%        str2double(get(hObject,'String')) returns contents of edit80 as a double


% --- Executes during object creation, after setting all properties.
function edit80_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edit80 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function edit81_Callback(hObject, eventdata, handles)
% hObject    handle to edit81 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit81 as text
%        str2double(get(hObject,'String')) returns contents of edit81 as a double


% --- Executes during object creation, after setting all properties.
function edit81_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edit81 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function edit82_Callback(hObject, eventdata, handles)
% hObject    handle to edit82 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit82 as text
%        str2double(get(hObject,'String')) returns contents of edit82 as a double


% --- Executes during object creation, after setting all properties.
function edit82_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edit82 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function edit83_Callback(hObject, eventdata, handles)
% hObject    handle to edit83 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit83 as text
%        str2double(get(hObject,'String')) returns contents of edit83 as a double


% --- Executes during object creation, after setting all properties.
function edit83_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edit83 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function edit84_Callback(hObject, eventdata, handles)
% hObject    handle to edit84 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit84 as text
%        str2double(get(hObject,'String')) returns contents of edit84 as a double


% --- Executes during object creation, after setting all properties.
function edit84_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edit84 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function edit85_Callback(hObject, eventdata, handles)
% hObject    handle to edit85 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit85 as text
%        str2double(get(hObject,'String')) returns contents of edit85 as a double


% --- Executes during object creation, after setting all properties.
function edit85_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edit85 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function edit86_Callback(hObject, eventdata, handles)
% hObject    handle to edit86 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit86 as text
%        str2double(get(hObject,'String')) returns contents of edit86 as a double


% --- Executes during object creation, after setting all properties.
function edit86_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edit86 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function edit87_Callback(hObject, eventdata, handles)
% hObject    handle to edit87 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit87 as text
%        str2double(get(hObject,'String')) returns contents of edit87 as a double


% --- Executes during object creation, after setting all properties.
function edit87_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edit87 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function edit88_Callback(hObject, eventdata, handles)
% hObject    handle to edit88 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit88 as text
%        str2double(get(hObject,'String')) returns contents of edit88 as a double


% --- Executes during object creation, after setting all properties.
function edit88_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edit88 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function mag_x_obs_Callback(hObject, eventdata, handles)
% hObject    handle to mag_x_obs (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of mag_x_obs as text
%        str2double(get(hObject,'String')) returns contents of mag_x_obs as a double


% --- Executes during object creation, after setting all properties.
function mag_x_obs_CreateFcn(hObject, eventdata, handles)
% hObject    handle to mag_x_obs (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function edit90_Callback(hObject, eventdata, handles)
% hObject    handle to edit90 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit90 as text
%        str2double(get(hObject,'String')) returns contents of edit90 as a double


% --- Executes during object creation, after setting all properties.
function edit90_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edit90 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function mag_y_obs_Callback(hObject, eventdata, handles)
% hObject    handle to mag_y_obs (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of mag_y_obs as text
%        str2double(get(hObject,'String')) returns contents of mag_y_obs as a double


% --- Executes during object creation, after setting all properties.
function mag_y_obs_CreateFcn(hObject, eventdata, handles)
% hObject    handle to mag_y_obs (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function edit92_Callback(hObject, eventdata, handles)
% hObject    handle to edit92 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit92 as text
%        str2double(get(hObject,'String')) returns contents of edit92 as a double


% --- Executes during object creation, after setting all properties.
function edit92_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edit92 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function mag_z_obs_Callback(hObject, eventdata, handles)
% hObject    handle to mag_z_obs (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of mag_z_obs as text
%        str2double(get(hObject,'String')) returns contents of mag_z_obs as a double


% --- Executes during object creation, after setting all properties.
function mag_z_obs_CreateFcn(hObject, eventdata, handles)
% hObject    handle to mag_z_obs (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function edit94_Callback(hObject, eventdata, handles)
% hObject    handle to edit94 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit94 as text
%        str2double(get(hObject,'String')) returns contents of edit94 as a double


% --- Executes during object creation, after setting all properties.
function edit94_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edit94 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function gps_obs_Callback(hObject, eventdata, handles)
% hObject    handle to gps_obs (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of gps_obs as text
%        str2double(get(hObject,'String')) returns contents of gps_obs as a double


% --- Executes during object creation, after setting all properties.
function gps_obs_CreateFcn(hObject, eventdata, handles)
% hObject    handle to gps_obs (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function edit96_Callback(hObject, eventdata, handles)
% hObject    handle to edit96 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit96 as text
%        str2double(get(hObject,'String')) returns contents of edit96 as a double


% --- Executes during object creation, after setting all properties.
function edit96_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edit96 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function ss1_exp_Callback(hObject, eventdata, handles)
% hObject    handle to ss1_exp (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of ss1_exp as text
%        str2double(get(hObject,'String')) returns contents of ss1_exp as a double


% --- Executes during object creation, after setting all properties.
function ss1_exp_CreateFcn(hObject, eventdata, handles)
% hObject    handle to ss1_exp (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function ss6_exp_Callback(hObject, eventdata, handles)
% hObject    handle to ss6_exp (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of ss6_exp as text
%        str2double(get(hObject,'String')) returns contents of ss6_exp as a double


% --- Executes during object creation, after setting all properties.
function ss6_exp_CreateFcn(hObject, eventdata, handles)
% hObject    handle to ss6_exp (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function ss6_obs_Callback(hObject, eventdata, handles)
% hObject    handle to ss6_obs (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of ss6_obs as text
%        str2double(get(hObject,'String')) returns contents of ss6_obs as a double


% --- Executes during object creation, after setting all properties.
function ss6_obs_CreateFcn(hObject, eventdata, handles)
% hObject    handle to ss6_obs (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function ss5_exp_Callback(hObject, eventdata, handles)
% hObject    handle to ss5_exp (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of ss5_exp as text
%        str2double(get(hObject,'String')) returns contents of ss5_exp as a double


% --- Executes during object creation, after setting all properties.
function ss5_exp_CreateFcn(hObject, eventdata, handles)
% hObject    handle to ss5_exp (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function ss5_obs_Callback(hObject, eventdata, handles)
% hObject    handle to ss5_obs (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of ss5_obs as text
%        str2double(get(hObject,'String')) returns contents of ss5_obs as a double


% --- Executes during object creation, after setting all properties.
function ss5_obs_CreateFcn(hObject, eventdata, handles)
% hObject    handle to ss5_obs (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function ss4_exp_Callback(hObject, eventdata, handles)
% hObject    handle to ss4_exp (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of ss4_exp as text
%        str2double(get(hObject,'String')) returns contents of ss4_exp as a double


% --- Executes during object creation, after setting all properties.
function ss4_exp_CreateFcn(hObject, eventdata, handles)
% hObject    handle to ss4_exp (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function ss4_obs_Callback(hObject, eventdata, handles)
% hObject    handle to ss4_obs (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of ss4_obs as text
%        str2double(get(hObject,'String')) returns contents of ss4_obs as a double


% --- Executes during object creation, after setting all properties.
function ss4_obs_CreateFcn(hObject, eventdata, handles)
% hObject    handle to ss4_obs (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function ss3_exp_Callback(hObject, eventdata, handles)
% hObject    handle to ss3_exp (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of ss3_exp as text
%        str2double(get(hObject,'String')) returns contents of ss3_exp as a double


% --- Executes during object creation, after setting all properties.
function ss3_exp_CreateFcn(hObject, eventdata, handles)
% hObject    handle to ss3_exp (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function ss3_obs_Callback(hObject, eventdata, handles)
% hObject    handle to ss3_obs (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of ss3_obs as text
%        str2double(get(hObject,'String')) returns contents of ss3_obs as a double


% --- Executes during object creation, after setting all properties.
function ss3_obs_CreateFcn(hObject, eventdata, handles)
% hObject    handle to ss3_obs (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function ss2_exp_Callback(hObject, eventdata, handles)
% hObject    handle to ss2_exp (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of ss2_exp as text
%        str2double(get(hObject,'String')) returns contents of ss2_exp as a double


% --- Executes during object creation, after setting all properties.
function ss2_exp_CreateFcn(hObject, eventdata, handles)
% hObject    handle to ss2_exp (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function ss2_obs_Callback(hObject, eventdata, handles)
% hObject    handle to ss2_obs (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of ss2_obs as text
%        str2double(get(hObject,'String')) returns contents of ss2_obs as a double


% --- Executes during object creation, after setting all properties.
function ss2_obs_CreateFcn(hObject, eventdata, handles)
% hObject    handle to ss2_obs (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function ss1_obs_Callback(hObject, eventdata, handles)
% hObject    handle to ss1_obs (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of ss1_obs as text
%        str2double(get(hObject,'String')) returns contents of ss1_obs as a double


% --- Executes during object creation, after setting all properties.
function ss1_obs_CreateFcn(hObject, eventdata, handles)
% hObject    handle to ss1_obs (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on button press in pushbutton2.
function pushbutton2_Callback(hObject, eventdata, handles)
handles.stop = 1;
handles.stop
handles.output = hObject;

% Update handles structure
guidata(hObject, handles);
% hObject    handle to pushbutton2 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)




function Solar_I_obs_Callback(hObject, eventdata, handles)
% hObject    handle to Solar_I_obs (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of Solar_I_obs as text
%        str2double(get(hObject,'String')) returns contents of Solar_I_obs as a double


% --- Executes during object creation, after setting all properties.
function Solar_I_obs_CreateFcn(hObject, eventdata, handles)
% hObject    handle to Solar_I_obs (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function Solar_I_exp_Callback(hObject, eventdata, handles)
% hObject    handle to Solar_I_exp (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of Solar_I_exp as text
%        str2double(get(hObject,'String')) returns contents of Solar_I_exp as a double


% --- Executes during object creation, after setting all properties.
function Solar_I_exp_CreateFcn(hObject, eventdata, handles)
% hObject    handle to Solar_I_exp (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function Load_I_obs_Callback(hObject, eventdata, handles)
% hObject    handle to Load_I_obs (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of Load_I_obs as text
%        str2double(get(hObject,'String')) returns contents of Load_I_obs as a double


% --- Executes during object creation, after setting all properties.
function Load_I_obs_CreateFcn(hObject, eventdata, handles)
% hObject    handle to Load_I_obs (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function Load_I_exp_Callback(hObject, eventdata, handles)
% hObject    handle to Load_I_exp (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of Load_I_exp as text
%        str2double(get(hObject,'String')) returns contents of Load_I_exp as a double


% --- Executes during object creation, after setting all properties.
function Load_I_exp_CreateFcn(hObject, eventdata, handles)
% hObject    handle to Load_I_exp (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function V_bat_obs_Callback(hObject, eventdata, handles)
% hObject    handle to V_bat_obs (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of V_bat_obs as text
%        str2double(get(hObject,'String')) returns contents of V_bat_obs as a double


% --- Executes during object creation, after setting all properties.
function V_bat_obs_CreateFcn(hObject, eventdata, handles)
% hObject    handle to V_bat_obs (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function V_bat_exp_Callback(hObject, eventdata, handles)
% hObject    handle to V_bat_exp (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of V_bat_exp as text
%        str2double(get(hObject,'String')) returns contents of V_bat_exp as a double


% --- Executes during object creation, after setting all properties.
function V_bat_exp_CreateFcn(hObject, eventdata, handles)
% hObject    handle to V_bat_exp (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function V_down_obs_Callback(hObject, eventdata, handles)
% hObject    handle to V_down_obs (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of V_down_obs as text
%        str2double(get(hObject,'String')) returns contents of V_down_obs as a double


% --- Executes during object creation, after setting all properties.
function V_down_obs_CreateFcn(hObject, eventdata, handles)
% hObject    handle to V_down_obs (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function V_down_exp_Callback(hObject, eventdata, handles)
% hObject    handle to V_down_exp (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of V_down_exp as text
%        str2double(get(hObject,'String')) returns contents of V_down_exp as a double


% --- Executes during object creation, after setting all properties.
function V_down_exp_CreateFcn(hObject, eventdata, handles)
% hObject    handle to V_down_exp (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function V_OBC_obs_Callback(hObject, eventdata, handles)
% hObject    handle to V_OBC_obs (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of V_OBC_obs as text
%        str2double(get(hObject,'String')) returns contents of V_OBC_obs as a double


% --- Executes during object creation, after setting all properties.
function V_OBC_obs_CreateFcn(hObject, eventdata, handles)
% hObject    handle to V_OBC_obs (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function V_OBC_exp_Callback(hObject, eventdata, handles)
% hObject    handle to V_OBC_exp (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of V_OBC_exp as text
%        str2double(get(hObject,'String')) returns contents of V_OBC_exp as a double


% --- Executes during object creation, after setting all properties.
function V_OBC_exp_CreateFcn(hObject, eventdata, handles)
% hObject    handle to V_OBC_exp (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function LS_Bea_obs_Callback(hObject, eventdata, handles)
% hObject    handle to LS_Bea_obs (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of LS_Bea_obs as text
%        str2double(get(hObject,'String')) returns contents of LS_Bea_obs as a double


% --- Executes during object creation, after setting all properties.
function LS_Bea_obs_CreateFcn(hObject, eventdata, handles)
% hObject    handle to LS_Bea_obs (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function LS_Tor_obs_Callback(hObject, eventdata, handles)
% hObject    handle to LS_Tor_obs (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of LS_Tor_obs as text
%        str2double(get(hObject,'String')) returns contents of LS_Tor_obs as a double


% --- Executes during object creation, after setting all properties.
function LS_Tor_obs_CreateFcn(hObject, eventdata, handles)
% hObject    handle to LS_Tor_obs (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function LS_Down_obs_Callback(hObject, eventdata, handles)
% hObject    handle to LS_Down_obs (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of LS_Down_obs as text
%        str2double(get(hObject,'String')) returns contents of LS_Down_obs as a double


% --- Executes during object creation, after setting all properties.
function LS_Down_obs_CreateFcn(hObject, eventdata, handles)
% hObject    handle to LS_Down_obs (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function LS_Up_obs_Callback(hObject, eventdata, handles)
% hObject    handle to LS_Up_obs (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of LS_Up_obs as text
%        str2double(get(hObject,'String')) returns contents of LS_Up_obs as a double


% --- Executes during object creation, after setting all properties.
function LS_Up_obs_CreateFcn(hObject, eventdata, handles)
% hObject    handle to LS_Up_obs (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function LS_GPS_obs_Callback(hObject, eventdata, handles)
% hObject    handle to LS_GPS_obs (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of LS_GPS_obs as text
%        str2double(get(hObject,'String')) returns contents of LS_GPS_obs as a double


% --- Executes during object creation, after setting all properties.
function LS_GPS_obs_CreateFcn(hObject, eventdata, handles)
% hObject    handle to LS_GPS_obs (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function LS_OBC_obs_Callback(hObject, eventdata, handles)
% hObject    handle to LS_OBC_obs (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of LS_OBC_obs as text
%        str2double(get(hObject,'String')) returns contents of LS_OBC_obs as a double


% --- Executes during object creation, after setting all properties.
function LS_OBC_obs_CreateFcn(hObject, eventdata, handles)
% hObject    handle to LS_OBC_obs (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function LS_Mag_obs_Callback(hObject, eventdata, handles)
% hObject    handle to LS_Mag_obs (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of LS_Mag_obs as text
%        str2double(get(hObject,'String')) returns contents of LS_Mag_obs as a double


% --- Executes during object creation, after setting all properties.
function LS_Mag_obs_CreateFcn(hObject, eventdata, handles)
% hObject    handle to LS_Mag_obs (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function OC_bea_Callback(hObject, eventdata, handles)
% hObject    handle to OC_bea (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of OC_bea as text
%        str2double(get(hObject,'String')) returns contents of OC_bea as a double


% --- Executes during object creation, after setting all properties.
function OC_bea_CreateFcn(hObject, eventdata, handles)
% hObject    handle to OC_bea (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function OC_tor_Callback(hObject, eventdata, handles)
% hObject    handle to OC_tor (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of OC_tor as text
%        str2double(get(hObject,'String')) returns contents of OC_tor as a double


% --- Executes during object creation, after setting all properties.
function OC_tor_CreateFcn(hObject, eventdata, handles)
% hObject    handle to OC_tor (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function OC_down_Callback(hObject, eventdata, handles)
% hObject    handle to OC_down (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of OC_down as text
%        str2double(get(hObject,'String')) returns contents of OC_down as a double


% --- Executes during object creation, after setting all properties.
function OC_down_CreateFcn(hObject, eventdata, handles)
% hObject    handle to OC_down (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function OC_GPS_Callback(hObject, eventdata, handles)
% hObject    handle to OC_GPS (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of OC_GPS as text
%        str2double(get(hObject,'String')) returns contents of OC_GPS as a double


% --- Executes during object creation, after setting all properties.
function OC_GPS_CreateFcn(hObject, eventdata, handles)
% hObject    handle to OC_GPS (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function OC_OBC_Callback(hObject, eventdata, handles)
% hObject    handle to OC_OBC (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of OC_OBC as text
%        str2double(get(hObject,'String')) returns contents of OC_OBC as a double


% --- Executes during object creation, after setting all properties.
function OC_OBC_CreateFcn(hObject, eventdata, handles)
% hObject    handle to OC_OBC (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function paBeacon_Callback(hObject, eventdata, handles)
% hObject    handle to paBeacon (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of paBeacon as text
%        str2double(get(hObject,'String')) returns contents of paBeacon as a double


% --- Executes during object creation, after setting all properties.
function paBeacon_CreateFcn(hObject, eventdata, handles)
% hObject    handle to paBeacon (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function edit132_Callback(hObject, eventdata, handles)
% hObject    handle to edit132 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit132 as text
%        str2double(get(hObject,'String')) returns contents of edit132 as a double


% --- Executes during object creation, after setting all properties.
function edit132_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edit132 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function edit133_Callback(hObject, eventdata, handles)
% hObject    handle to edit133 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit133 as text
%        str2double(get(hObject,'String')) returns contents of edit133 as a double


% --- Executes during object creation, after setting all properties.
function edit133_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edit133 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function paDown_Callback(hObject, eventdata, handles)
% hObject    handle to paDown (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of paDown as text
%        str2double(get(hObject,'String')) returns contents of paDown as a double


% --- Executes during object creation, after setting all properties.
function paDown_CreateFcn(hObject, eventdata, handles)
% hObject    handle to paDown (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function edit135_Callback(hObject, eventdata, handles)
% hObject    handle to edit135 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit135 as text
%        str2double(get(hObject,'String')) returns contents of edit135 as a double


% --- Executes during object creation, after setting all properties.
function edit135_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edit135 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function edit136_Callback(hObject, eventdata, handles)
% hObject    handle to edit136 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit136 as text
%        str2double(get(hObject,'String')) returns contents of edit136 as a double


% --- Executes during object creation, after setting all properties.
function edit136_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edit136 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function lna_Callback(hObject, eventdata, handles)
% hObject    handle to lna (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of lna as text
%        str2double(get(hObject,'String')) returns contents of lna as a double


% --- Executes during object creation, after setting all properties.
function lna_CreateFcn(hObject, eventdata, handles)
% hObject    handle to lna (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function edit138_Callback(hObject, eventdata, handles)
% hObject    handle to edit138 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit138 as text
%        str2double(get(hObject,'String')) returns contents of edit138 as a double


% --- Executes during object creation, after setting all properties.
function edit138_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edit138 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function edit139_Callback(hObject, eventdata, handles)
% hObject    handle to edit139 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit139 as text
%        str2double(get(hObject,'String')) returns contents of edit139 as a double


% --- Executes during object creation, after setting all properties.
function edit139_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edit139 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function bat_Callback(hObject, eventdata, handles)
% hObject    handle to bat (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of bat as text
%        str2double(get(hObject,'String')) returns contents of bat as a double


% --- Executes during object creation, after setting all properties.
function bat_CreateFcn(hObject, eventdata, handles)
% hObject    handle to bat (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function edit141_Callback(hObject, eventdata, handles)
% hObject    handle to edit141 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit141 as text
%        str2double(get(hObject,'String')) returns contents of edit141 as a double


% --- Executes during object creation, after setting all properties.
function edit141_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edit141 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function edit142_Callback(hObject, eventdata, handles)
% hObject    handle to edit142 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit142 as text
%        str2double(get(hObject,'String')) returns contents of edit142 as a double


% --- Executes during object creation, after setting all properties.
function edit142_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edit142 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function reading_Callback(hObject, eventdata, handles)
% hObject    handle to reading (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of reading as text
%        str2double(get(hObject,'String')) returns contents of reading as a double


% --- Executes during object creation, after setting all properties.
function reading_CreateFcn(hObject, eventdata, handles)
% hObject    handle to reading (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on button press in pushbutton3.
function pushbutton3_Callback(hObject, eventdata, handles)
% hObject    handle to pushbutton3 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

fwrite(handles.s,'*');


% --- Executes on button press in pushbutton4.
function pushbutton4_Callback(hObject, eventdata, handles)
% hObject    handle to pushbutton4 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
fwrite(handles.s,'&');
