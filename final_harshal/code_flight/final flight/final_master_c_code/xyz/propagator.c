/**
 *@file  propagator.c
 *@brief Implements SGP Propagator model
*/

#include "common.h"
#include "mathutil.h"
#include "frame.h"
#include "peripherals.h"
#include "propagator.h"
#include "igrf.h"
#include "gps.h"
static vector v_r, r_ecef_ash;
static vector v_v;
static float frtm=0;
vector v_sat;

extern volatile struct GPS_reading gps;

void copy_gps_reading(void)
{
	
  static vector v_r_ecef, v_v_ecef;
  
 // Current_state.gps = gps;
  
  Current_state.gps.time_since_reading = 0;
  //increase a zero at end
  //int32_t test = -181930228;
  v_r_ecef[0] = ((double)Current_state.gps.x)/100;//((float)test/100);//
  v_r_ecef[1] = ((double)Current_state.gps.y) / 100;//((float)test/100);//
  v_r_ecef[2] = ((double)Current_state.gps.z) / 100;//((float)test/100);//
  
  v_v_ecef[0] = ((double)Current_state.gps.v_x) / 1000;
  v_v_ecef[1] = ((double)Current_state.gps.v_y) / 1000;
  v_v_ecef[2] = ((double)Current_state.gps.v_z) / 1000;
   
  get_seconds_since_equinox();//check
   
  r_ecef_ash[0] = v_r_ecef[0];
  r_ecef_ash[1] = v_r_ecef[1];
  r_ecef_ash[2] = v_r_ecef[2];
  ecef2eci(v_r_ecef, v_r);
  ecef2eci(v_v_ecef, v_v);
  
  int32_t send;
  /*for (int i=0;i<3;i++)
  {
	  //if(send[i]<0)
	  //send[i]=(int16_t)(-1*(v_v_ecef[i]/1));
	  //else
	  send[i]=(int16_t)((abs(v_v[i])));
	  
  }*/
 /*
  uint8_t a,b,c,d;
  for (int i=0;i<3;i++)
  {
	  send=(int32_t)(v_r_ecef[i]*100);
	  a = (int8_t)send;
	  b = (int8_t)(send>>8);
	  c = (int8_t)(send>>16);
	  d = (int8_t)(send>>24);
	  transmit_UART0(a);
	  transmit_UART0(b);
	  transmit_UART0(c);
	  transmit_UART0(d);
  }
  */
  v_sat[0] = (((float)Current_state.gps.lat) / 10000000) * (M_PI / 180);// check scale factor
  v_sat[1] = (((float)Current_state.gps.lon) / 10000000) * (M_PI / 180);
  
  GPS_done = 0;
}

void sgp_get_acceleration(vector v_g)// only j2 perturbations taken
{
  vector v_r_ecef, v_g_ecef;
  double R, R2, R3, R4;// floatch
 
  //eci2ecef(v_r, v_r_ecef);//see change
  
  R = vector_norm(r_ecef_ash);  //
  R2 = pow(R, 2);
  R2 = (1.5 * J2 * R_E2) / R2;
  
  R3 = pow(R, 3);
  
  R4 = pow(R, 4);
  R4 = (7.5 * J2 * pow(r_ecef_ash[2],2) * R_E2) / R4; //
  
  v_g_ecef[0] = (-1 * GM * r_ecef_ash[0] * (1 + R2 - R4)) / R3;//
  v_g_ecef[1] = (-1 * GM * r_ecef_ash[1] * (1 + R2 - R4)) / R3;//
  v_g_ecef[2] = (-1 * GM * r_ecef_ash[2] * (1 + 3 * R2 - R4)) / R3;//
  
  ecef2eci(v_g_ecef, v_g);
/* uint8_t sent[3];
 for (int i=0;i<2;i=i+1)
 {
	 sent[i] = (uint8_t)((v_g_ecef[i]));
	 transmit_UART0(sent[i]);
 }*/
  
  
}

void sgp_orbit_propagator(void)
{
  vector v_g;
  vector v_v_old;
  float delta_t = ((float)FRAME_TIME) / 10;
  uint8_t i, j;
  
  for(i = 0; i < 10; i++)
  {
    sgp_get_acceleration(v_g);
    
    copy_vector(v_v, v_v_old);
    
    for(j = 0; j < 3; j++)
      v_v[j] += v_g[j] * delta_t;
    
    for(j = 0; j < 3; j++)
      v_r[j] += ((v_v[j] + v_v_old[j]) / 2) * delta_t;// this is also different from controls
  }
  
  /*uint16_t send[3];
  for (int i=0;i<3;i++)
  {
	  if(send[i]<0)
	  send[i]=(uint16_t)(-1*(v_v[i]));
	  else
	  send[i]=(uint16_t)(1*(v_v[i]));
	  
  }
  uint8_t a,b;
  for (int i=0;i<3;i++)
  {
	  a = (uint8_t)send[i];
	  b = (uint8_t)(send[i]>>8);
	  transmit_UART0(a);
	  transmit_UART0(b);
  }*/
  
}

void sun_vector_estimator(vector v_sun_o)
{
  double time_in_days = ((float)seconds_since_equinox  / 86400);///////////////////////////////////////////////// dekhna padega// floatch
  double lambda = (2 * M_PI * time_in_days) / 365.256363;// floatch
  double delta, alpha;// floatch
  vector v_sun_eci;
  delta =   asin(sin(EPSILON) * sin(lambda));
  alpha = acos(cos(lambda)/cos(delta));
  
  if (delta>=0)
  {
	  alpha=alpha;
  }
  else
  alpha= 2*M_PI-alpha;
  
  v_sun_eci[0] = cos(delta) * cos(alpha);//cosdelta*cosalpha;//cos(delta) * cos(alpha);
  v_sun_eci[1] = cos(delta) * sin(alpha);//cosdelta*sinalpha;//cos(delta) * sin(alpha);
  v_sun_eci[2] = sin(delta);//sindelta;//sin(delta);
  frtm = frtm+0.1;
  eci2orbit(v_r, v_v, v_sun_eci, v_sun_o);
/* int8_t a,b;
 uint16_t sen;
 for (int i=0;i<3;i=i+1)
 {
	
	 sen = ((uint16_t)(v_sun_o[i]*10000));
	 a = (uint8_t)sen;
	 b = (uint8_t)(sen>>8);
	 transmit_UART0(a);
	 transmit_UART0(b);
	 //transmit_UART0(sen);
 }
 */
}

void magnetic_field_estimator(vector v_B_o)
{
  double time_in_years = 2016 + (float)seconds_since_pivot / SECONDS_IN_YEAR;// floatch
  vector v_temp, v_r_lla, v_B_ned, v_B_eci;
  
  eci2ecef(v_r, v_temp);
  ecef2lla(v_temp, v_r_lla);// LLA is need
  ///* Save LLA vector for use in communications check routine
  copy_vector(v_r_lla, v_sat);// why is this required when ,lat long alt coming from GPS
  
  igrf(v_r_lla, time_in_years, 8, v_B_ned);// need to check at the end
  
  ned2ecef(v_B_ned, v_r_lla, v_temp);
  ecef2eci(v_temp, v_B_eci);
  eci2orbit(v_r, v_v, v_B_eci, v_B_o); 
  //scalar_into_vector(v_B_o, 1e-9);     // igrf gives in nT
 /* int8_t sen,sen1;
  int16_t st;
  
  for (int i=0;i<3;i=i+1)
  {
	  //sen = ((int8_t)((lambda))/2);
	  st =(int16_t)(v_B_o[i]);
	  sen = (int8_t)st;
	  sen1 = (int8_t)(st>>8);
	  transmit_UART0(sen);
	  transmit_UART0(sen1);
  }*/
}
