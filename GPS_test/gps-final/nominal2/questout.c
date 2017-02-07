#include "common.h"
#include "peripherals.h"
#include "mathutil.h"
#include "quest.h"


uint8_t quest(vector v_B_c, vector v_sun_c, quaternion q_triad, uint8_t * p_w_ctrl)
{
  uint8_t w_ctrl = *p_w_ctrl;
  static uint16_t time_since_light;
  static uint8_t light_prev = 1;
  float * sun_adc = (float *)Current_state.ss.read;
  uint8_t light = 1, num_dark_sensors = 0, i, j;
  vector v_sun_m, v_B_m, v_cross_m, v_cross_c, v_mc_cross, v_mc_add;
  vector v_temp1, v_temp2;
  vector v_triad;
  float mu, nu, rho, k, triad;
  
  for(i = 0; i < N_SS; i++)
  {
    //Confirm SS_GAIN value and whether we have to use SS_MAX_ANGLE
    if(sun_adc[i] < (0.5 *SS_GAIN ))// SS_gain was earlier 5
      num_dark_sensors++;
  }
  if(num_dark_sensors == N_SS)
    light = 0;
	
  
  if(light)
  {
    if(w_ctrl==0)//if(!w_ctrl)
    {
      time_since_light += FRAME_TIME;
      if(time_since_light == 300)
        w_ctrl = 1;// it was earlier 0 w_ctrl is zero for 300seconds after light region begins, else 1
    }
    
    if(light_prev == 0)
    {
      w_ctrl = 0;
      time_since_light = 0;
    }
	
	float ssvectors[3][3];
	float ssvalues[3];
	float vs[6][3]={{1,0,0},{-1,0,0},{0,1,0},{0,-1,0},{0,0,1},{0,0,-1}}; 
	float m_suntot[5][6]={{1,-1,0,0,0,0},{0,0,1,-1,0,0},{0,0,0,0,1,-1},{0,0,0,0,0,0},{sun_adc[0],sun_adc[1],sun_adc[2],sun_adc[3],sun_adc[4],sun_adc[5]}};
	float a_const = cos(SS_MAX_ANGLE*M_PI/180);
	float t_s[6]={a_const,a_const,a_const,a_const,a_const,a_const};		
	for (int i =0;i<N_SS;i=i+1)
	{
		if(m_suntot[4][i]>t_s[i])
		m_suntot[3][i]=1;
		else
		m_suntot[3][i]=0;
	}
	//sen
	
	
	
	
	float min;
	float ascending[6],temp;
	int ind=0;
	
	for (int i=0;i<N_SS;i=i+1)
	{
		ascending[i]=sun_adc[i];
	}
	//sorting begins
	for(int i=0;i<N_SS;i=i+1)
	{
		min=ascending[i];
		for (int j=i+1;j<N_SS;j=j+1)
		{
			if(ascending[j]<min)
			{
				min=ascending[j];
				ind=j;
			}
		}
		temp = ascending[i];
		ascending[i] = min;
		ascending[ind] = temp;
	}
	//sorting ends
	//storing index
	
	float search,temindex[6];
    for(int i=0;i<N_SS;i=i+1)
    {
		search = ascending[i];
		for (int j=0;j<N_SS;j=j+1)
		{
			if (sun_adc[j]==search)
			{
				temindex[i]=j;
			}
		}
	}
			
	for (int i=0;i<N_SS/2;i=i+1)
	{
		for (int j=0;j<N_SS/2;j=j+1)
		{
			ssvectors[i][j]=vs[(int)temindex[N_SS-1-i]][j];
			
		}
		ssvalues[i]=ascending[N_SS-1-i];
	}
	float result[3][3];
	//inverse_sun(&ssvectors[0][0],&result[0][0]);
	float A[3][3];
	for (i=0;i<3;i++){
		for (j=0;j<3;j++){
			A[i][j]=ssvectors[i][j];
		}
	}
	//inverse of 3*3 matrix
	float determinant =  A[0][0]*(A[1][1]*A[2][2]-A[2][1]*A[1][2])-A[0][1]*(A[1][0]*A[2][2]-A[1][2]*A[2][0])+A[0][2]*(A[1][0]*A[2][1]-A[1][1]*A[2][0]);
	float invdet = 1/determinant;
	result[0][0] =  (A[1][1]*A[2][2]-A[2][1]*A[1][2])*invdet;
	result[1][0] = (-1)*(A[0][1]*A[2][2]-A[0][2]*A[2][1])*invdet;
	result[2][0] =  (A[0][1]*A[1][2]-A[0][2]*A[1][1])*invdet;
	result[0][1] = (-1)*(A[1][0]*A[2][2]-A[1][2]*A[2][0])*invdet;
	result[1][1] =  (A[0][0]*A[2][2]-A[0][2]*A[2][0])*invdet;
	result[2][1] = (-1)*(A[0][0]*A[1][2]-A[1][0]*A[0][2])*invdet;
	result[0][2] =  (A[1][0]*A[2][1]-A[2][0]*A[1][1])*invdet;
	result[1][2] = (-1)*(A[0][0]*A[2][1]-A[2][0]*A[0][1])*invdet;
	result[2][2] =  (A[0][0]*A[1][1]-A[1][0]*A[0][1])*invdet;
	
	for (int i =0;i<N_SS/2;i=i+1)
	{
		v_sun_m[i]=0;
		for (int j=0;j<N_SS/2;j=j+1)
		{
			v_sun_m[i]= v_sun_m[i]+ result[i][j] * ssvalues[j];
		}
		
	}
	
	
	
    /*for(i = 0; i < (N_SS / 2); i++)//?????????? kuch bhi random he
    {
      j = i * 2;
      if(sun_adc[j] > sun_adc[j + 1])
        v_sun_m[i] = (float)sun_adc[j];
      else
        v_sun_m[i] = -1 * (float)sun_adc[j + 1];
    }*/
    convert_unit_vector(v_sun_m);
    
    v_B_m[0] = Current_state.mm.B_x;
    v_B_m[1] = Current_state.mm.B_y;
    v_B_m[2] = Current_state.mm.B_z;
    
    vector_cross_product(v_B_m, v_sun_m, v_cross_m);
    convert_unit_vector(v_cross_m);
    
    vector_cross_product(v_B_c, v_sun_c, v_cross_c);
    convert_unit_vector(v_cross_c);
    
    mu = (1 + vector_dot_product(v_cross_m, v_cross_c)) * (MAG_WEIGHT * vector_dot_product(v_B_m, v_B_c) + (1 - MAG_WEIGHT) * vector_dot_product(v_sun_m, v_sun_c));
    
    vector_cross_product(v_B_m, v_B_c, v_temp1);
    vector_cross_product(v_sun_m, v_sun_c, v_temp2);
    for(i = 0; i < 3; i++)
      v_temp2[i] = v_temp1[i] * MAG_WEIGHT + (1 - MAG_WEIGHT) * v_temp2[i];
    vector_cross_product(v_cross_m, v_cross_c, v_mc_cross);
    
    mu += vector_dot_product(v_mc_cross, v_temp2);
    
    add_vectors(v_cross_m, v_cross_c, v_mc_add);
    
    nu = vector_dot_product(v_mc_add, v_temp2);
    
    rho = sqrt(mu * mu + nu * nu);
    
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
  static quaternion q_B_old;
  quaternion dq, q;
  vector e, de, v_w_temp;
  static vector v_w_old = { 0.0, 0.0, 0.0 };
  matrix m_temp;
  float n;
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
                  { -2 * e[1], 2 * e[0], 0 } };
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
  copy_vector(v_w, v_w_old);
  
}
