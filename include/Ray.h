#pragma once
#include <vec3.h>

class ray {
	
	public:
		ray() {}
		ray(const point3& origin , const point3& direction): pt(origin) , dir(direction) {}

		const point3& point() const { return pt; }

		const vec3& direction() const { return dir; } // here we are using trailing const  

		const point3 moveBy(double t) const{

			return pt + t * dir ;
			
		}

		/* 
		   the reason we are using that  trailing const is that, mostly across the program we will treat
		   ray as a constant object, so in such case when the compiler tries to inject the this pointer 
		   into the function argument it will, it will
		   inject immutable ray object with an immutable pointer. Else it will inject a mutable ray object with an immutable pointer.
		   So in such case if we try to invoke this function, we would essentially be contradicting our own definition of the ray object 
		   which we are passing.
		 */
	private:
		point3 pt;
		vec3 dir;
};
