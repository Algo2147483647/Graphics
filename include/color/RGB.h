#ifndef RGB_H
#define RGB_H

#include <math.h>

#define INT8U unsigned char
#define ARGB  unsigned int

class RGB {
public:
	INT8U R = 0, G = 0, B = 0;

	/*---------------- 基础函数 ----------------*/
	RGB() { }
	RGB(ARGB a) { *this = a; }			//构造函数
	RGB(INT8U R, INT8U G, INT8U B) : R(R), G(G), B(B) { ; }			//构造函数

	inline RGB& operator=(const RGB& a) {
		R = a.R; 
		G = a.G; 
		B = a.B; 
		return *this; 
	}

	inline RGB& operator=(ARGB& a) {
		R = a >> 16; 
		G = a >> 8; 
		B = a; 
		return *this; 
	}

	inline RGB& operator*=(double a) {
		R *= a; 
		G *= a; 
		B *= a; 
		return *this; 
	}

	inline RGB& operator+=(RGB a) {
		R += a.R; 
		G += a.G; 
		B += a.B; 
		return *this; 
	}
	
	inline INT8U& operator[](int index) {
		switch (index) {
		case 0: return R;
		case 1: return G;
		case 2: return B;
		}
	}

	inline ARGB to_ARGB() {
		return to_ARGB(R, G, B);
	}

	static ARGB to_ARGB(INT8U R, INT8U G, INT8U B, INT8U A = 0xFF) {
		return ARGB(B) << 0 |
			   ARGB(G) << 8 |
			   ARGB(R) << 16 |
			   ARGB(A) << 24;
	}

	static ARGB to_ARGB(double R, double G, double B, double A = 1) {
		return
			(ARGB)(B * 0xFF) << 0 |
			(ARGB)(G * 0xFF) << 8 |
			(ARGB)(R * 0xFF) << 16|
			(ARGB)(A * 0xFF) << 24;
	}

	static ARGB mul(ARGB a, double r) {
		return
			((ARGB)((a & 0x0000FF) * r) & 0x0000FF) |
			((ARGB)((a & 0x00FF00) * r) & 0x00FF00) |
			((ARGB)((a & 0xFF0000) * r) & 0xFF0000);
	}

	static ARGB Alpha(ARGB a, ARGB b, double r) {
		return 
			((ARGB)((a & 0x0000FF) * r + (b & 0x0000FF) * (1 - r)) & 0x0000FF) |
			((ARGB)((a & 0x00FF00) * r + (b & 0x00FF00) * (1 - r)) & 0x00FF00) |
			((ARGB)((a & 0xFF0000) * r + (b & 0xFF0000) * (1 - r)) & 0xFF0000);
	}


};

/*
 * Mixture of Colors 
 */
#define ChannelBlend_Normal(A,B)     ((INT8U)(A))
#define ChannelBlend_Lighten(A,B)    ((INT8U)((B > A) ? B:A))
#define ChannelBlend_Darken(A,B)     ((INT8U)((B > A) ? A:B))
#define ChannelBlend_Multiply(A,B)   ((INT8U)(((ARGB)A * B) / 0xFF))
#define ChannelBlend_Average(A,B)    ((INT8U)((A + B) / 2))
#define ChannelBlend_Add(A,B)        ((INT8U)(min(0xFF, (A + B))))
#define ChannelBlend_Subtract(A,B)   ((INT8U)((A + B < 0xFF) ? 0:(A + B - 0xFF)))
#define ChannelBlend_Difference(A,B) ((INT8U)(abs(A - B)))
#define ChannelBlend_Negation(A,B)   ((INT8U)(0xFF - abs(0xFF - A - B)))
#define ChannelBlend_Screen(A,B)     ((INT8U)(0xFF - (((0xFF - A) * (0xFF - B)) >> 8)))
#define ChannelBlend_Exclusion(A,B)  ((INT8U)(A + B - 2 * A * B / 0xFF))
#define ChannelBlend_Overlay(A,B)    ((INT8U)((B < 0x80) ? (2 * A * B / 0xFF):(0xFF - 2 * (0xFF - A) * (0xFF - B) / 0xFF)))
#define ChannelBlend_SoftLight(A,B)  ((INT8U)((B < 0x80)?(2*((A>>1)+64))*((float)B/0xFF):(0xFF-(2*(0xFF-((A>>1)+64))*(float)(0xFF-B)/0xFF))))
#define ChannelBlend_HardLight(A,B)  (ChannelBlend_Overlay(B,A))
#define ChannelBlend_ColorDodge(A,B) ((INT8U)((B == 0xFF) ? B:min(0xFF, ((A << 8 ) / (0xFF - B)))))
#define ChannelBlend_ColorBurn(A,B)  ((INT8U)((B == 0) ? B:max(0, (0xFF - ((0xFF - A) << 8 ) / B))))
#define ChannelBlend_LinearDodge(A,B)(ChannelBlend_Add(A,B))
#define ChannelBlend_LinearBurn(A,B) (ChannelBlend_Subtract(A,B))
#define ChannelBlend_LinearLight(A,B)((INT8U)(B < 0x80)?ChannelBlend_LinearBurn(A,(2 * B)):ChannelBlend_LinearDodge(A,(2 * (B - 0x80))))
#define ChannelBlend_VividLight(A,B) ((INT8U)(B < 0x80)?ChannelBlend_ColorBurn(A,(2 * B)):ChannelBlend_ColorDodge(A,(2 * (B - 0x80))))
#define ChannelBlend_PinLight(A,B)   ((INT8U)(B < 0x80)?ChannelBlend_Darken(A,(2 * B)):ChannelBlend_Lighten(A,(2 * (B - 0x80))))
#define ChannelBlend_HardMix(A,B)    ((INT8U)((ChannelBlend_VividLight(A,B) < 0x80) ? 0:0xFF))
#define ChannelBlend_Reflect(A,B)    ((INT8U)((B == 0xFF) ? B:min(0xFF, (A * A / (0xFF - B)))))
#define ChannelBlend_Glow(A,B)       (ChannelBlend_Reflect(B,A))
#define ChannelBlend_Phoenix(A,B)    ((INT8U)(min(A,B) - max(A,B) + 0xFF))
#define ChannelBlend_Alpha(A,B,O)    ((INT8U)(O * A + (1 - O) * B))
#define ChannelBlend_AlphaF(A,B,F,O) (ChannelBlend_Alpha(F(A,B),A,O))
#define ColorBlend_Buffer(T,A,B,M)   (T)[0] = ChannelBlend_##M((A)[0], (B)[0]),\
                                     (T)[1] = ChannelBlend_##M((A)[1], (B)[1]),\
                                     (T)[2] = ChannelBlend_##M((A)[2], (B)[2])
#define ColorBlend_Normal(T,A,B)        (ColorBlend_Buffer(T,A,B,Normal))
#define ColorBlend_Lighten(T,A,B)       (ColorBlend_Buffer(T,A,B,Lighten))
#define ColorBlend_Darken(T,A,B)        (ColorBlend_Buffer(T,A,B,Darken))
#define ColorBlend_Multiply(T,A,B)      (ColorBlend_Buffer(T,A,B,Multiply))
#define ColorBlend_Average(T,A,B)       (ColorBlend_Buffer(T,A,B,Average))
#define ColorBlend_Add(T,A,B)           (ColorBlend_Buffer(T,A,B,Add))
#define ColorBlend_Subtract(T,A,B)      (ColorBlend_Buffer(T,A,B,Subtract))
#define ColorBlend_Difference(T,A,B)    (ColorBlend_Buffer(T,A,B,Difference))
#define ColorBlend_Negation(T,A,B)      (ColorBlend_Buffer(T,A,B,Negation))
#define ColorBlend_Screen(T,A,B)        (ColorBlend_Buffer(T,A,B,Screen))
#define ColorBlend_Exclusion(T,A,B)     (ColorBlend_Buffer(T,A,B,Exclusion))
#define ColorBlend_Overlay(T,A,B)       (ColorBlend_Buffer(T,A,B,Overlay))
#define ColorBlend_SoftLight(T,A,B)     (ColorBlend_Buffer(T,A,B,SoftLight))
#define ColorBlend_HardLight(T,A,B)     (ColorBlend_Buffer(T,A,B,HardLight))
#define ColorBlend_ColorDodge(T,A,B)    (ColorBlend_Buffer(T,A,B,ColorDodge))
#define ColorBlend_ColorBurn(T,A,B)     (ColorBlend_Buffer(T,A,B,ColorBurn))
#define ColorBlend_LinearDodge(T,A,B)   (ColorBlend_Buffer(T,A,B,LinearDodge))
#define ColorBlend_LinearBurn(T,A,B)    (ColorBlend_Buffer(T,A,B,LinearBurn))
#define ColorBlend_LinearLight(T,A,B)   (ColorBlend_Buffer(T,A,B,LinearLight))
#define ColorBlend_VividLight(T,A,B)    (ColorBlend_Buffer(T,A,B,VividLight))
#define ColorBlend_PinLight(T,A,B)      (ColorBlend_Buffer(T,A,B,PinLight))
#define ColorBlend_HardMix(T,A,B)       (ColorBlend_Buffer(T,A,B,HardMix))
#define ColorBlend_Reflect(T,A,B)       (ColorBlend_Buffer(T,A,B,Reflect))
#define ColorBlend_Glow(T,A,B)          (ColorBlend_Buffer(T,A,B,Glow))
#define ColorBlend_Phoenix(T,A,B)       (ColorBlend_Buffer(T,A,B,Phoenix))

#endif