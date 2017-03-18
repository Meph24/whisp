#version 120

varying vec2 texCoord0;

uniform sampler2D sampler;

void main()
{
		float bx = -2.0 + texCoord0.x*2.0;
		float by = 1.0 - texCoord0.y*2.0; 
		float zx = 0;
		float zy = 0;
		int maxiter=16;
		int i = maxiter;
		float r1=0;
		float tempx;
		while (i>0)
		{
			tempx = zx*zx  + bx - r1; 
			zy = 2 * zx*zy + by;//3 (2)
			r1 = zy * zy;//1
			
			zx = tempx*tempx + bx - r1;
			zy = 2 * tempx*zy + by;
			r1 = zy*zy;

			if (zx*zx + r1>4) break;
			i--;
		}
	if(i==0)
	{
		gl_FragColor = vec4((0.8-zx)/1.6,(0.8-zy)/1.6,0.0,1);
	}
	else
	{
		gl_FragColor = vec4(0,0,0,1);//0.97*0.97,0.85*0.85,0.35*0.35,1);
		//gl_FragColor = vec4(i*1.0/maxiter,i*1.0/maxiter,i*1.0/maxiter,1);
		//gl_FragColor = vec4( (8-zx)/16.0 , (8-zy)/16.0 ,0 , 1 ); //= texture2D(sampler, texCoord0);  i*1.0/maxiter
	}
   
}
