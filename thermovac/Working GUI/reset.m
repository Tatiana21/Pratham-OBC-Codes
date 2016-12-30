function varargout = reset(varargin)
% RESET MATLAB code for reset.fig
%      RESET, by itself, creates a new RESET or raises the existing
%      singleton*.
%
%      H = RESET returns the handle to a new RESET or the handle to
%      the existing singleton*.
%
%      RESET('CALLBACK',hObject,eventData,handles,...) calls the local
%      function named CALLBACK in RESET.M with the given input arguments.
%
%      RESET('Property','Value',...) creates a new RESET or raises the
%      existing singleton*.  Starting from the left, property value pairs are
%      applied to the GUI before reset_OpeningFcn gets called.  An
%      unrecognized property name or invalid value makes property application
%      stop.  All inputs are passed to reset_OpeningFcn via varargin.
%
%      *See GUI Options on GUIDE's Tools menu.  Choose "GUI allows only one
%      instance to run (singleton)".
%
% See also: GUIDE, GUIDATA, GUIHANDLES

% Edit the above text to modify the response to help reset

% Last Modified by GUIDE v2.5 13-Oct-2015 23:24:34

% Begin initialization code - DO NOT EDIT
gui_Singleton = 1;
gui_State = struct('gui_Name',       mfilename, ...
                   'gui_Singleton',  gui_Singleton, ...
                   'gui_OpeningFcn', @reset_OpeningFcn, ...
                   'gui_OutputFcn',  @reset_OutputFcn, ...
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


% --- Executes just before reset is made visible.
function reset_OpeningFcn(hObject, eventdata, handles, varargin)
% This function has no output args, see OutputFcn.
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% varargin   command line arguments to reset (see VARARGIN)

% Choose default command line output for reset
handles.output = hObject;

% Update handles structure
guidata(hObject, handles);

% UIWAIT makes reset wait for user response (see UIRESUME)
% uiwait(handles.figure1);


% --- Outputs from this function are returned to the command line.
function varargout = reset_OutputFcn(hObject, eventdata, handles) 
% varargout  cell array for returning output args (see VARARGOUT);
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Get default command line output from handles structure
varargout{1} = handles.output;


% --- Executes on button press in pushbutton1.
function pushbutton1_Callback(hObject, eventdata, handles)
% hObject    handle to pushbutton1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
s=serial('COM4');
set(s,'BaudRate',9600,'Databits',8,'Parity','none','stopbits',2);
fopen(s);
set(s, 'Timeout', 60);%set timeout in seconds
isvalid(s)
fwrite(s,'r');
fclose(s);

