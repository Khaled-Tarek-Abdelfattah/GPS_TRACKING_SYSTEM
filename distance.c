#include "distance.h"



long double distance(long double long1,long double long2,long double lat1,long double lat2)
	{
		long double res=0;
		long double a=0;
		long double b=0;
		long double c=0;
		long double d=0;
		// calculating the distance between two coordinates using Haversine formula
		//rad_cairo is calculated using Geocentric radius.
		// 6378.135229km of cairo   6378137 mean radius of earth in meters
		long double rad_cairo = 6378137;
		long double lat_difference_rad = (lat2-lat1)*(3.141592654/180);
		long double long_difference_rad = (long2-long1)*(3.141592654/180);
		lat1 = lat1 *(3.141592654/180);
		lat2 = lat2 *(3.141592654/180);
		long1 = long1 *(3.141592654/180);
		long2 = long2 * (3.141592654/180);

		a = pow(sin(lat_difference_rad/2),2);
		b = cos(lat1)*cos(lat2);
		c = pow(sin(long_difference_rad/2),2);
		d = sqrt(a+(b*c));
		res = 2 * rad_cairo * asin(d);//rad_cairo is the distance from the center of the earth to cairo in meters.
		return res;
	}