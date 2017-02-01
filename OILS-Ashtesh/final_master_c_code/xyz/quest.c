#include "common.h"
#include "peripherals.h"
#include "mathutil.h"
#include "quest.h"
static quaternion q_B_old;
	
static vector v_w_old;

uint8_t light_cal()
{
	int i;
	 uint8_t light=1,num_dark_sensors = 0;
	 float * sun_adc = (float *)Current_state.ss.read;
	  for(i = 0; i < N_SS; i++)
	  {
		  //Confirm SS_GAIN value and whether we have to use SS_MAX_ANGLE
		  if(sun_adc[i] < (0.17 * SS_GAIN)) // cos(85) = 0.087; earlier it was 0.5, changed by amolika
		  num_dark_sensors++;
		  
	  }
	  if(num_dark_sensors == N_SS)
	  light = 0;
	 return light;
}
uint8_t quest(vector v_B_c, vector v_sun_c, quaternion q_triad, uint8_t * p_w_ctrl)
{
  //uint8_t w_ctrl = &(p_w_ctrl);
  static uint16_t time_since_light = 0;
  static uint8_t light_prev = 1;
  double * sun_adc = (double *)Current_state.ss.read;// floatch
  uint8_t light = 1, num_dark_sensors = 0, i, j;
  vector v_sun_m, v_B_m, v_cross_m, v_cross_c, v_mc_cross, v_mc_add;
  vector v_temp1, v_temp2;
  vector v_triad;
  double mu, nu, rho, k, triad;// floatch
  
  
  for(i = 0; i < N_SS; i++)
  {
    //Confirm SS_GAIN value and whether we have to use SS_MAX_ANGLE
    if(sun_adc[i] < (0.17 * SS_GAIN)) // cos(85) = 0.087; earlier it was 0.5, changed by amolika
      num_dark_sensors++;
  }
  if(num_dark_sensors == N_SS)
    light = 0;
  
  if(light)
  {
    if(!*p_w_ctrl)
    {
      time_since_light += FRAME_TIME;
      if(time_since_light == 300)
        *p_w_ctrl = 1;
    }
    
    if(light_prev == 0)
    {
      *p_w_ctrl = 0;
      time_since_light = 0;
    }
	
    //uint8_t sen;
    for(i = 0; i < (N_SS / 2); i++)
    {
      j = i * 2;
      if(sun_adc[j] > sun_adc[j + 1])
        {v_sun_m[i] = (double)sun_adc[j];}
      else
        {v_sun_m[i] = -1.0 * (double)sun_adc[j + 1];}
    }
	

	double temp=0,ang=0,angle=0;
	// added recently- Sun sensor calibration eq
	for(i = 0; i < N_SS/2; i++)
	{
		temp = (fabs(v_sun_m[i])-1.05)/1.514;
		ang = acos(temp)*180.0/3.14159;
		angle = (ang+9.9)/1.65*3.14159/180.0;
		if (v_sun_m[i] != 0) 
		v_sun_m[i] = cos(angle)*(fabs(v_sun_m[i])/v_sun_m[i]);
		else
		v_sun_m[i] = cos(angle);
		
	}
	
    /*
	int8_t sen,sen1,sen2,sen3;
	int32_t st;
	//convert_unit_vector(v_sun_m);
	for (int i=0;i<3;i=i+1)
	{
		//sen = ((int8_t)((lambda))/2);
		st =(int32_t)(v_sun_m[i]*65535000);
		sen = (int8_t)st;
		sen1 = (int8_t)(st>>8);
		sen2 = (int8_t)(st>>16);
		sen3 = (int8_t)(st>>24);
		transmit_UART0(sen);
		transmit_UART0(sen1);
		transmit_UART0(sen2);
		transmit_UART0(sen3);
	}*/
	convert_unit_vector(v_sun_m);
	  v_B_m[2] = Current_state.mm.B_x;
	  v_B_m[1] = -1*Current_state.mm.B_y;
	  v_B_m[0] = Current_state.mm.B_z;
	 /*
	 int8_t sen,sen1;
	 int16_t st;
	 
	 for (int i=0;i<3;i=i+1)
	 {
		 //sen = ((int8_t)((lambda))/2);
		 st =(int16_t)(v_B_m[i]*100000000);
		 sen = (int8_t)st;
		 sen1 = (int8_t)(st>>8);
		 transmit_UART0(sen);
		 transmit_UART0(sen1);
	 }*/
	/*
    uint8_t sent[3];
    for (int i=0;i<3;i=i+1)
    {
	    sent[i] = (uint8_t)((v_sun_m[i]*100));
	    transmit_UART0(sent[i]);
    }*/
	/*v_B_m[0]=4.1887e-5;
	v_B_m[1]=-2.0011e-5;
	v_B_m[2]=1.4286e-6;
	v_B_c[0]=3.2723e3;
	v_B_c[1]=3.1313e3;
	v_B_c[2]=4.6221e4;
	v_sun_c[0]=-0.9385;
	v_sun_c[1]=0.3340;
	v_sun_c[2]=0.0873;
	v_sun_m[0]=-0.221;
	v_sun_m[1]=-0.6178;
	v_sun_m[2]=-0.7543;
	
	int8_t sen,sen1;
	int16_t st;
	/*
	for (int i=0;i<3;i=i+1)
	{
		//sen = ((int8_t)((lambda))/2);
		st =(int16_t)(v_B_m[i]*100000000);
		sen = (int8_t)st;
		sen1 = (int8_t)(st>>8);
		transmit_UART0(sen);
		transmit_UART0(sen1);
	}*/
    vector_cross_product(v_B_m, v_sun_m, v_cross_m);
    convert_unit_vector(v_cross_m);
    
    vector_cross_product(v_B_c, v_sun_c, v_cross_c);
    convert_unit_vector(v_cross_c);
    /*uint8_t sent[3];
    for (int i=0;i<3;i=i+1)
    {
	    sent[i] = (uint8_t)((v_cross_c[i]*100));
	    transmit_UART0(sent[i]);
	}*/
    mu = (1 + vector_dot_product(v_cross_m, v_cross_c)) * (MAG_WEIGHT * vector_dot_product(v_B_m, v_B_c) + (1 - MAG_WEIGHT) * vector_dot_product(v_sun_m, v_sun_c));
    
    vector_cross_product(v_B_m, v_B_c, v_temp1);
    vector_cross_product(v_sun_m, v_sun_c, v_temp2);
    for(i = 0; i < 3; i++)
      v_temp2[i] = v_temp1[i] * MAG_WEIGHT + (1 - MAG_WEIGHT) * v_temp2[i];
    vector_cross_product(v_cross_m, v_cross_c, v_mc_cross);
    
    mu += vector_dot_product(v_mc_cross, v_temp2);
    
    add_vectors(v_cross_m, v_cross_c, v_mc_add);
    
    nu = vector_dot_product(v_mc_add, v_temp2);
    //mu = 0.1606;
	//nu = 0.3671;
    rho = sqrt(mu * mu + nu * nu);
    /*
	uint8_t sent[3];
    for (int i=0;i<3;i=i+1)
    {
	    sent[i] = (uint8_t)((mu*100));
	    transmit_UART0(sent[i]);
    }*/
	
    if(mu > 0)
    {
      k = 1 / (2 * sqrt(rho * (rho + mu) * (1 + vector_dot_product(v_cross_m, v_cross_c))));
      
      for(i = 0; i < 3; i++)
        v_triad[i] = v_mc_cross[i] * (rho + mu) + v_mc_add[i] * nu;
      
      triad = (rho + mu) * (1 + vector_dot_product(v_cross_m, v_cross_c));
      
    }
    else
    {
      k = 1 / (2 * sqrt(rho * (rho - mu) * (1 + vector_dot_product(v_cross_m, v_cross_c))));
      
      for(i = 0; i < 3; i++)
        v_triad[i] = v_mc_cross[i] * nu + v_mc_add[i] * (rho - mu);
      
      triad = nu * (1 + vector_dot_product(v_cross_m, v_cross_c));
      
    }
    for(i = 0; i < 3; i++)
      q_triad[i] = v_triad[i];
    q_triad[3] = triad;
      
    scalar_into_quaternion(q_triad, k);
  }
  else
  {
    for(i = 0; i < 3; i++)
      q_triad[i] = 0;
    q_triad[3] = 1;
  }
  light_prev = light;
 
  return light;
   
}

void omega_estimation(quaternion q_B, vector v_w)
{
//static quaternion q_B_old;
  quaternion dq, q;
  vector e, de, v_w_temp;
  //static vector v_w_old = { 0.0, 0.0, 0.0 };
  matrix m_temp;
  double n;// floatch
  uint8_t i, j;
  
  for(i = 0; i < 4; i++)
  {
    dq[i] = (q_B[i] - q_B_old[i]) / FRAME_TIME;
    q[i] = (q_B[i] + q_B_old[i]) / 2;
  }
    
  for(i = 0; i < 3; i++)
  {
    de[i] = dq[i];
    e[i] = q[i];
  }
  n = q[3];

  matrix m_ex = { { 0, -2 * e[2], 2 * e[1] },
  { 2 * e[2], 0, -2 * e[0] },
  { -2 * e[1], 2 * e[0], 0 } };                              //{ { 0, -2 * e[2], 2 * e[1] },
                                              //{ 2 * e[2], 0, -2 * e[0] },
                                              //{ -2 * e[1], 2 * e[0], 0 } };
  matrix m_I = { { 2 * n, 0, 0 },
                 { 0, 2 * n, 0 },
                 { 0, 0, 2 * n } };
  for(i = 0; i< 3; i++)
  {
    for(j = 0; j < 3; j++)
      m_temp[j][i] = m_I[i][j] - m_ex[i][j] + ((2 * e[i] * e[j]) / n);
  }
  
  vector_into_matrix(de, m_temp, v_w_temp);
  
  for(i = 0; i < 3; i++)
    v_w[i] = A_F * v_w_temp[i] + (1 - A_F) * v_w_old[i];
  
  copy_quaternion(q_B, q_B_old);
  copy_vector(v_w, v_w_old);// order of 10000
  /*  uint8_t sent[3];
for (int i=0;i<3;i=i+1)
{
	sent[i] = (uint8_t)(abs(v_w[i]*10000));
	transmit_UART0(sent[i]);
	/*if(v_m[i]<0)
	transmit_UART0(0x01);
	else
	transmit_UART0(0x00);
} */ 
}
