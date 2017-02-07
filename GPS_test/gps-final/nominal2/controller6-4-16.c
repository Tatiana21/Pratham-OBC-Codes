/**
 *@file  controller.c
 *@brief Implements the Control Law
*/

#include "common.h"
#include "controller.h"
#include "mathutil.h"
#include "propagator.h"
#include "quest.h"
#include "timer.h"
#include "peripherals.h"
#include "frame.h"
#include "gps.h"
static vector v_B;
static vector v_w = { 0.0, 0.0, 0.0 };
	static vector v_ieu = {0,0,0};//{ -428.9,-232.7,318.97 };
static quaternion q_o;
static uint8_t first_B = 1, light, w_ctrl = 0;
int flag = 1;
int tol_time = 30;
int switch_n2d_tol_time = 0;
double mod_w;
int flag_N = 0,flag_D = 0;
uint64_t t_now = 0;
int8_t star_flag=0;
int16_t start_time=0;
double norm_m_d=0;
double md_start=0;
double avg_md=0;
double avg_md_new=0;
uint16_t num=2;
uint16_t flag_on=0;
double t_orbit = 5676.599;
double eclipse_time = 2270.6;
double gps_start = 2270.6;//0.4*t_orbit;
double light_dur = 3406;//0.6*t_orbit;
double gps_max = 34060;//6*t_orbit;
int gps_power= 0;
int16_t time_e = 0;
int16_t time_l = 0;
vector v_m_temp={0,0,0};
static matrix m_Kp = { { 2.61668972308302E-07, -1.51727717691841E-09, -8.02248396004395E-10 },
                       { -1.51727717691841E-09, 2.69070665454679E-07, -3.48754956099971E-09 },
                       { -8.02248396004395E-10, -3.48754956099971E-09, 2.58737491706997E-07 } };
                         
static matrix m_Ki = { { 5.32044130509255E-12, -3.08503682807288E-14, -1.63118900395154E-14 },
                       { -3.08503682807288E-14, 5.47093784121685E-12, -7.09113601594235E-14 },
                       { -1.63118900395154E-14, -7.09113601594235E-14, 5.26083633802797E-12 } };
                         
static matrix m_Kd = { { 0.000475908639758953, -2.75953740726193E-06, -1.45908374051095E-06 },
                       { -2.75953740726193E-06, 0.000489370418150681, -6.34295672515486E-06 },
                       { -1.45908374051095E-06, -6.34295672515486E-06, 0.000470577029621382 } };

void detumbling(vector v_m_D)
{
	static vector v_B_old;
	 v_B[0] = Current_state.mm.B_x;
	 v_B[1] = Current_state.mm.B_y;
	 v_B[2] = Current_state.mm.B_z;
	if(first_B)
	{
		
		copy_vector(v_B, v_B_old);
		first_B = 0;
		return;
	}
	
	vector v_B_avg, v_dB;
	double factor;
	uint8_t i;
	
	for(i = 0; i < 3; i++)
	{
		v_dB[i] = (v_B[i] - v_B_old[i]) / FRAME_TIME;
		v_B_avg[i] = (v_B[i] + v_B_old[i]) / 2;
	}
	
	factor = (-1 * K_DETUMBLING * MAG_B) / vector_norm(v_B_avg);                         //factor = -1 / vector_norm(v_B_avg);//??????
	
	for(i = 0; i < 3; i++)
	{
		v_m_D[i] = factor *v_dB[i];
		
	}
	
	copy_vector(v_B, v_B_old);
	
}
 union floatLongUnion{
	 float f;
	 uint32_t l;
 } x;

void nominal(vector v_m_N)
{
  //static vector v_ieu = { 0.0, 0.0, 0.0 };
	    v_B[0] = Current_state.mm.B_x/10000;
	    v_B[1] = Current_state.mm.B_y/10000;
	    v_B[2] = Current_state.mm.B_z/10000;
  vector v_eu;
  uint8_t i, j;
  float norm_B = vector_norm(v_B);
    
  for(i = 0; i < 3; i++)
  {
    v_eu[i] = 2 * q_o[i] * q_o[3];
    v_ieu[i] += v_eu[i] * FRAME_TIME;
  }
    
  for(i = 0; i < 3; i++)
  {
    v_m_temp[i] = 0;
    for(j = 0; j < 3; j++)
      v_m_temp[i] += v_eu[j] * m_Kp[i][j] + v_ieu[j] * m_Ki[i][j] + v_w[j] * m_Kd[i][j];
    v_m_temp[i] /= norm_B * norm_B;	
  }
    
  vector_cross_product(v_m_temp, v_B, v_m_N);
  scalar_into_vector(v_m_N, light);//scalar_into_vector(v_m_N, light);//values are very small as compared to matlab check
   /*
   int8_t sen,sen1;
   int16_t st;
   
   for (int i=0;i<3;i=i+1)
   {
	   //sen = ((int8_t)((lambda))/2);
	   st =(int16_t)(v_m_N[i]*1000);
	   sen = (int8_t)st;
	   sen1 = (int8_t)(st>>8);
	   transmit_UART0(sen);
	   transmit_UART0(sen1);
   }*/
   
}

void apply_torque(vector v_m)
{
	uint8_t i;
	/*uint8_t sen;
	for(int i=0;i<3;i=i+1)
	{
		sen = (v_m[i]*255);
		transmit_UART0(sen);
	}*/
    
	
	for(i = 0; i < 3; i++)
	{
		if(fabs(v_m[i]) > M_MAX)
		{
			if (fabs(v_m[i])/v_m[i] == -1)
			v_m[i] = -1*M_MAX;
			else
			v_m[i] = M_MAX;
		}
		
	}
	
	
	
	scalar_into_vector(v_m, 1.0 / (N_TURNS * AREA));  // nturns should be 60
	
	
	for(i = 0; i < 3; i++)
	{
		if(fabs(v_m[i]) > I_MAX)
		{
			if (fabs(v_m[i])/v_m[i] == -1)
			v_m[i] = -1*I_MAX;
			else
			v_m[i] = I_MAX;
		}
	}
	
	int8_t sen,sen1,sen2,sen3;
	int32_t st;
	// x.f=3.145689123;
	 uint8_t buf0,buf1,buf2,buf3;
	 
	for (int i=0;i<3;i=i+1)
	{
		//sen = ((int8_t)((lambda))/2);
		st =(int32_t)(v_m[i]*65535000);
		sen = (int8_t)st;
		sen1 = (int8_t)(st>>8);
		sen2 = (int8_t)(st>>16);
		sen3 = (int8_t)(st>>24);
		transmit_UART0(sen);
		transmit_UART0(sen1);
		transmit_UART0(sen2);
		transmit_UART0(sen3);
	/*x.f=v_m[i];	
	buf0=((x.l&0xFF000000)>>24);
	buf1=((x.l&0x00FF0000)>>16);
	buf2=((x.l&0x0000FF00)>>8);
	buf3=((x.l&0x000000FF)>>0);
	transmit_UART0(buf0);
	transmit_UART0(buf1);
	transmit_UART0(buf2);
	transmit_UART0(buf3);*/	
	}
	
	
	sen = Mode;
	transmit_UART0(sen);
	sen = gps_power;
	transmit_UART0(sen);
/*uint8_t sen;
for (int i=0;i<3;i++)
{
	
	sen = ((uint8_t)abs(v_m[i]))*100000;
	transmit_UART0(sen);
	
}*/

//Current_state.pwm.x = abs(v_m[0]*65535); //fabs((v_m_D[0] * PWM_RES) / I_MAX)+10000;
// int16_t x1 = Current_state.pwm.x/100;
// if (x1 == 0)
//     Current_state.pwm.x = Current_state.pwm.x + 100;
//if(v_m[0]>=0)
Current_state.pwm.x=v_m[0]*65535;
//else
//Current_state.pwm.x=(-1)*v_m[0]*65535;

/*uint8_t sen=0xf0;transmit_UART0(sen);
for(int i=0;i<3;i=i+1)
{
	if(Current_state.pwm.x==0)
	{ sen=0x0F;}
	else
	sen= 0xf0;   //v_r_ecef[i]
	transmit_UART0(sen);
}*/

Current_state.pwm.y = abs(v_m[1]*65535); //fabs((v_m_D[1] * PWM_RES) / I_MAX)+10000;
// x1 = Current_state.pwm.y/100;
// if (x1 == 0)
//     Current_state.pwm.y = Current_state.pwm.y + 100;

Current_state.pwm.z = abs(v_m[2]*65535); //fabs((v_m_D[2] * PWM_RES) / I_MAX)+10000;
// x1 = Current_state.pwm.z/100;
// if (x1 == 0)
//     Current_state.pwm.z = Current_state.pwm.z + 100;

if(v_m[0] < 0)
Current_state.pwm.x_dir = 1;
else
Current_state.pwm.x_dir = 0;

if(v_m[1] < 0)
Current_state.pwm.y_dir = 1;
else
Current_state.pwm.y_dir = 0;

if(v_m[2] < 0)
Current_state.pwm.z_dir = 1;
else
Current_state.pwm.z_dir = 0;
PORTA=0xA0;
}
/*
void apply_torque(vector v_m)
{
  uint8_t i;
  
  for(i = 0; i < 3; i++)
  {
    if(v_m[i] > M_MAX)
      v_m[i] = M_MAX;
  }
  
  scalar_into_vector(v_m, 1.0 / (N_TURNS * AREA));
  
  for(i = 0; i < 3; i++)
  {
    if(v_m[i] > I_MAX)
      v_m[i] = I_MAX;
  }
  
  Current_state.pwm.x = fabs((v_m[0] * PWM_RES) / I_MAX);
  Current_state.pwm.y = fabs((v_m[1] * PWM_RES) / I_MAX);
  Current_state.pwm.z = fabs((v_m[2] * PWM_RES) / I_MAX);
  
  if(v_m[0] < 0)
    Current_state.pwm.x_dir = 1;
  if(v_m[1] < 0)
    Current_state.pwm.y_dir = 1;
  if(v_m[2] < 0)
    Current_state.pwm.z_dir = 1;
}
*/
void control(void){
  
  vector v_m_D, v_m_N, v_sun_o, v_B_o;
  static uint64_t nominal_end = 0;
  
  ///Watchdog enabled for the control law
  //watch_dog(T_CONTROL);
  
  /// Take B readings from Magnetometer, clear torquer first
  //reset_PWM();
  
  //_delay_us(100);
  
  read_MM();
  detumbling(v_m_D);
  ///Set the torquer values calculated in the last frame
  //set_PWM();
  uint8_t c1,c2,d;
  //c1= (uint8_t)Current_state.pwm.x;
  //c2= (uint8_t)((Current_state.pwm.x)>>8);
  //d= Current_state.pwm.x_dir;
  c1=(uint8_t)0xFF;
  c2=(uint8_t)0xFF;
  d=(uint8_t)0xFF;
  transmit_UART0(c1);
  transmit_UART0(c2);
  transmit_UART0(d);
  
 // c1= (uint8_t)Current_state.pwm.y;
 // c2= (uint8_t)((Current_state.pwm.y)>>8);
 // d= Current_state.pwm.y_dir;
  transmit_UART0(c1);
  transmit_UART0(c2);
  transmit_UART0(d);
  
 // c1= (uint8_t)Current_state.pwm.z;
 // c2= (uint8_t)((Current_state.pwm.z)>>8);
 // d= Current_state.pwm.z_dir;
  transmit_UART0(c1);
  transmit_UART0(c2);
  transmit_UART0(d);
  
  v_B[0] = Current_state.mm.B_x;
  v_B[1] = Current_state.mm.B_y;
  v_B[2] = Current_state.mm.B_z;
  
  
 // detumbling(v_m_D);
  
  //if(((GPS_done == 0) && (Time % 600 == 0)) || ((GPS_done < 0) && (!first_B) && (vector_norm(v_m_D) < 2 )))
    ///* Switch on the GPS First
	//if((gps_power) == 2)
	//{
		if (gps_power==2)
		{
   uint8_t q= 90;
    transmit_UART0(q);
    read_GPS();
    while(UCSR0B & _BV(RXCIE0));
		}
	//}
	uint8_t a,b,c;
	
	/*for (int i=1;i<5;i=i+1)
	{
		uint8_t a1= receive_UART0();
	}
	*/
	/*
	a = (uint8_t)Current_state.gps.x;
	transmit_UART0(a);
	//Current_state.gps.x=Current_state.gps.x>>8;
	b = (uint8_t)(Current_state.gps.x>>8);
	transmit_UART0(b);
	c = (uint8_t)(Current_state.gps.x>>16);
	transmit_UART0(c);
	d = (uint8_t)(Current_state.gps.x>>24);
	transmit_UART0(d);
	
	a = (uint8_t)Current_state.gps.y;
	transmit_UART0(a);
	//Current_state.gps.x=Current_state.gps.x>>8;
	b = (uint8_t)(Current_state.gps.y>>8);
	transmit_UART0(b);
	c = (uint8_t)(Current_state.gps.y>>16);
	transmit_UART0(c);
	d = (uint8_t)(Current_state.gps.y>>24);
	transmit_UART0(d);
	
	a = (uint8_t)Current_state.gps.z;
	transmit_UART0(a);
	//Current_state.gps.x=Current_state.gps.x>>8;
	b = (uint8_t)(Current_state.gps.z>>8);
	transmit_UART0(b);
	c = (uint8_t)(Current_state.gps.z>>16);
	transmit_UART0(c);
	d = (uint8_t)(Current_state.gps.z>>24);
	transmit_UART0(d);*/
	//receiving faltu last 4 bytes of gps to be looked later
  
  
  /// Check if in Nominal or Detumbling Mode
  //if(GPS_done >= 0)
  //{
	   
    if(GPS_done == 1)
      copy_gps_reading();  //check
	  else
	  {
	  seconds_since_equinox += FRAME_TIME; //  ??jaroori hai ye plz dont delete??
	  seconds_since_pivot += FRAME_TIME;
	 // Current_state.gps.time_since_reading += FRAME_TIME;
	  }
   // uint8_t e=100;
    //transmit_UART0(e);
    read_SS();
	/*uint8_t sen;
	for(int i=0;i<6;i=i+1)
	{
		sen = (Current_state.ss.read[i]*255/3.3);
		transmit_UART0(sen);
	}*/
	/*
	uint8_t cs,ds;
	int i=0;
	while(i<6)
	{
		//PORTA=0x00;
		cs = (uint8_t) (Current_state.ss.reading[i]>>8);
		transmit_UART0(cs);
		PORTA=0x00;
		//Current_state.gps.x=Current_state.gps.x>>8;
		ds = (uint8_t)(Current_state.ss.reading[i]);
		transmit_UART0(ds);
		i=i+1;
	}
	Current_state.pwm.x ^= 0xffff;
	Current_state.pwm.y ^= 0xffff;
	Current_state.pwm.z ^= 0xffff;*/
    ///////////////////////////////////////////////////////////////////////////////////
	/*int8_t sen,sen1;
	int16_t st;
	
	for (int i=0;i<3;i=i+1)
	{
		//sen = ((int8_t)((lambda))/2);
		st =(int16_t)(v_B[i]*100000000);
		sen = (int8_t)st;
		sen1 = (int8_t)(st>>8);
		transmit_UART0(sen);
		transmit_UART0(sen1);
	}*/
	light = light_cal();
	if(GPS_done != 255)
	{
		
    sgp_orbit_propagator();
	
    sun_vector_estimator(v_sun_o);
	
    magnetic_field_estimator(v_B_o);

	
	
    light = quest(v_B_o, v_sun_o, q_o, &w_ctrl); // to be checked
    
    omega_estimation(q_o, v_w);
	
    scalar_into_vector(v_w, light*w_ctrl); //light * w_ctrl
    
	  uint8_t sent[3];
/*for (int i=0;i<3;i=i+1)
{
	sent[i] = (uint8_t)(abs(v_w[i]*10000));
	transmit_UART0(sent[i]);
	/*if(v_m[i]<0)
	transmit_UART0(0x01);
	else
	transmit_UART0(0x00);*/
//}  
    mod_w = vector_norm(v_w); 
    convert_unit_quaternion(q_o);
	
	/*int16_t st;
	int8_t sen,sen1;
	for (int i=0;i<4;i=i+1)
	{
		st = q_o[i]*10000;
		sen = (int8_t)st;
		sen1 = (int8_t)(st>>8);
		transmit_UART0(sen);
		transmit_UART0(sen1);
	}*/
	
	
    nominal(v_m_N);
	//uint8_t sen =Mode;
	//transmit_UART0(sen);
	//PORTA^=0x00;
	}//if((star_flag >= 10)||(Time > gps_max))   // 90
	if(star_flag >= 90)   // 90
		{
			star_flag = 91;                 // 91
			if (light ==1)
			{
				time_l = time_l +2;
				time_e =0;
				start_time = time_l % 690;   //690
				if (start_time <600)       //600
				{
					gps_power =0;          // 0 
				}
				else
				{
					gps_power =2;
				}
			}
			else
			{
				start_time =0;
				time_e = time_e +2;
				if (time_e > (eclipse_time-150))
				{
					gps_power =2;
				}
				else
				{
					gps_power =0;
				}
			}
		}

	else
		{
			norm_m_d = vector_norm(v_m_D);
			md_start = norm_m_d * light;
			if (md_start != 0)
			{
				if (num==2)
				{
					avg_md_new = (md_start + (num - 1)*md_start)/num;
				}
				else
				{
					avg_md_new = (md_start + (num-1)*avg_md)/num;
				}
				num = num +1;
			}
			else
			{
				num = 2;
				avg_md_new = 0;
			}
			if ((avg_md_new !=0) && (avg_md_new <0.04) || (avg_md_new ==0) && (avg_md !=0))
			{
				flag_on = flag_on +2;
			}
			else
			{
				flag_on = 0;
			}
			if (flag_on > gps_start)
			{
				gps_power = 2;
				star_flag = star_flag +2;
			}
			else
			{
				gps_power=0;
				star_flag = 0;                        
			}
			
		}

	
   if((mod_w < TOLW_D2N)&&(light*w_ctrl==1))
   {
	   flag_N = flag_N+1;
	   flag_D = 0;
   }
   else if (mod_w > TOLW_N2D)
   {
	   flag_D = flag_D +1;
	   flag_N = 0;
   }
   //sen = flag_N;
    //transmit_UART0(sen);  
	if (Time<2000)
	{
		Mode = DETUMBLING;
		t_now = 0;
	}
	else
	{
		if (star_flag >0)
		{
			if (Mode == DETUMBLING)
			{
				if (flag_N > tol_time)
				{
					Mode = NOMINAL;
					t_now = Time;
					flag_N = 0;
				}
				else
				{flag_D = 0;}	
			}	
		}
	
	 
	 if (Mode == NOMINAL)
	 {
		 if ((flag_D > tol_time) && (Time > (t_now+N2D_TIME)))
		 {
			 Mode = DETUMBLING;
			 flag_D = 0;
		 }
		 else
		 {
			 flag_N = 0;
		 }
	 }
	 }
	
	   
    //uint8_t sen;
    /*for (int i=0;i<3;i=i+1)
    {
	    sen = ((uint8_t)abs(v_sun_o[i]))*255;
	    transmit_UART0(sen);
		
		sen = ((uint8_t)abs(v_B_o[i]))*255;
		transmit_UART0(sen);
		
		sen = ((uint8_t)abs(v_m_N[i]))*255;
		transmit_UART0(sen);
    }*/
	
	/*for (int i=0;i<4;i=i+1)
	{
		sen = ((uint8_t)abs(q_o[i]))*100;
		transmit_UART0(sen);
	}*/
	
    
    /*
    float norm_w = vector_norm(v_w);
    
    //Confirm/ Check
    /*if((norm_w < TOLW_D2N) && ((light * w_ctrl) == 1) && Mode == DETUMBLING && Time > D_TIME)
    {
      Mode_prev = NOMINAL;
      nominal_end = Time + N2D_TIME;
    }
    else if((norm_w > TOLW_N2D) && Mode == NOMINAL && Time > nominal_end)
      Mode = DETUMBLING;
  
  
  if(Mode == DETUMBLING)
    apply_torque(v_m_D);
  
  else if(Mode == NOMINAL)*/
	//change here
   // if(Mode == DETUMBLING)
    apply_torque(v_m_D);
    
    //else if(Mode == NOMINAL)
    apply_torque(v_m_N);
	
	//Mode_prev = Mode;
    
}
