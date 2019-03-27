#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

/*                                                                            *
 *  	  IZP - project 2 iterative calculations	 	  
 *    
 *  Author: David Oravec - xorave05 
 * 
 *  This programme can count logarithm with common basis from number X
 *  on N iterations by using Taylor polynomial and cotinued fractions.
 *
 *  Usage at the command line:
 *  ./proj2 --log X N
 * 
 *  Also this programme can count exponential function with common basis X
 *  from number Y on N iterations by using Taylor polynomial and
 *  continued fractions.
 *
 * Usage at the command line:
 * ./proj2 --pow X Y N
 *                                                                           */

/* function which controls the value of parameter x in --log
   if the value is none of the below values, returning 42
   else returning special value
*/
double control_value_of_x_log(double x){
	if (isinf(x)){
		return INFINITY;
	} else if (x == 1.0){
		return 0.0;
	} else if (x < 0){
		return NAN;
	} else if (isnan(x)){
		return NAN;
	} else if (fabs(x) == 0.0){
		return -INFINITY;
	}
	return 42;
}

/* function which controls the value of parameter x and y in --pow
   if the value of x or y is none of the below values, returning 42
   else ruturning special value
*/
double control_value_of_xy_pow(double x,double y){
	if (x < 0.0){
		return NAN;
	} else if (x == 1.0){
		return 1.0;
	} else if (isnan(x) || isnan(y)){
		return NAN;
	} else if (fabs(y) == 0.0){
		return 1.0;
	} else if (x == INFINITY){
		if (y < 0.0){
			return 0.0;
		}
		return INFINITY;
	} else if (y == INFINITY){
		if (fabs(x) < 1.0){
			return 0.0;
		}
		return INFINITY;
	}
	return 42;
}

/* function for counting a logarithm using a Taylor polynomial
 
   parameter x is number from which programme calculate the logarithm
   parameter n is number of polynomial members which is greater than 0
   returning logarithm of x
*/
double taylor_log(double x, unsigned int n){
	// controls the value of x
	double control = control_value_of_x_log(x);
	if (control != 42){
		return control;
	}

	double sum = 0.0, numerator = 1.0, fraction;		
	// if x is from (0,1) period
	if (x<1.0){
		x = 1.0 - x;
		for (unsigned int i = 1; i<=n; i++){
			numerator *= x;
			fraction = numerator / i;
			if (isinf(x)){
				break;
			}
			sum -= fraction;
		}
	// if x is from (1,INFINITY) period	
	} else {
		for (unsigned int i = 1; i<=n; i++){
			numerator *= (x - 1.0) / x;
			fraction = numerator / i;
			if (isinf(x)){
				break;
			}
			sum += fraction;
		}
	}
	return sum;
}

/* function for counting a logarithm using continued fractions
 
   parameter x is number from which we calculate the logarithm
   parameter n is number of continued fractions which is greater than 0
   returning logarithm of x
*/
double cfrac_log(double x, unsigned int n){

	double control = control_value_of_x_log(x);
	if (control != 42){
		return control;
	}
	x = (x - 1.0) / (x + 1.0);
	unsigned int i = n;
	double c = (2*n - 1);
	double sum = c, fraction, doublex = x * x;
	while (i > 1){
		i--;
		c -= 2;
		fraction = i * i * doublex / sum;
		if (isinf(fraction)){
			break;
		}
		sum = c - fraction;
	}
	return 2.0 * x / sum;
}

/* function for counting an exponential function using taylor polynomial
   for calculation of natural logarithm we use previous function taylor_log
   
   parameter x is the base value
   parameter y is the power value
   parameter n is number of polynomial members which is greater than 0
   returning exponential function from number x
*/
double taylor_pow(double x, double y, unsigned int n){

	double control = control_value_of_xy_pow(x,y);
	if (control != 42){
		return control;
	}
	double log_x = taylor_log(x,n);
	double sum = 1.0, help_y = 1.0, ln_x = 1.0, fraction;
	double denominator = 1.0;
	for (unsigned int i = 1; i < n; i++){
		help_y *= y;
		ln_x *= log_x;
		denominator *= i;
		fraction = help_y * ln_x / denominator;
		if (isinf(fraction)){
			break;
		}
		sum += fraction;
	}
	return sum;
}

/* function for counting an exponential function using continued fractions
   for calculation of natural logarithm we use previous function cfrac_log
   
   parameter x is the base value
   parameter y is the power value
   parameter n is number of polynomial members which is greater than 0
   returning exponential function from number x
*/
double taylorcf_pow(double x, double y, unsigned int n){

	double control = control_value_of_xy_pow(x,y);
	if (control != 42){
		return control;
	}
	double log_x = cfrac_log(x,n);
	double sum = 1.0,ln_x = 1.0,help_y = 1.0,fraction;
	double denominator = 1.0;
	for (unsigned int i = 1; i < n; i++){
		help_y *= y;
		ln_x *= log_x;
		denominator *= i;
		fraction = help_y * ln_x / denominator;
		if (isinf(fraction)){
			break;
		}
		sum += fraction;
	}
	return sum;
}

// function which prints the results of logarithms to stdout
void print_logs(double x, unsigned int n){
	printf("       log(%g) = %.12g\n", x,log(x));
	printf(" cfrac_log(%g) = %.12g\n", x, cfrac_log(x, n));
	printf("taylor_log(%g) = %.12g\n", x, taylor_log(x, n));
}

// function which prints the results of exponential function to stdout
void print_pows(double x, double y, unsigned int n){
	printf("         pow(%g,%g) = %.12g\n", x,y,pow(x,y));
	printf("  taylor_pow(%g,%g) = %.12g\n", x,y,taylor_pow(x,y,n));
	printf("taylorcf_pow(%g,%g) = %.12g\n", x,y,taylorcf_pow(x,y,n));
}

int main(int argc, char *argv[]){
	if (argc < 2){
		fprintf(stderr,"Usage 1: ./proj2 --log X N\nUsage 2: ./proj2 --pow X Y N\n");
		return 1;
	}
	if (strcmp(argv[1],"--log") == 0){
		if (argc !=	4){
			fprintf(stderr,"Bad usage of content log\nUsage: ./proj2 --log X N\n");
			return 1;
		}
		char *endptr = NULL;
		double x = strtod(argv[2],&endptr);
		if (*endptr){
			fprintf(stderr,"You have entered %s on %d. argument which is invalid.\n"
				"Try to fix your input and repeat.\n",endptr,argc - 1);
			return 1;
		}
		unsigned int n = strtoul(argv[3],&endptr,10);
		if (*endptr){
			fprintf(stderr,"You have entered %s on %d. argument which is invalid.\n"
				"Try to fix your input and repeat.\n",endptr,argc);
			return 1;
		}
		if (strchr(argv[3],'-') || n == 0){
			fprintf(stderr,"You entered an invalid number %d on %d. argument\n"
				"Number must be greater than 0\n",n,argc);
			return 1;
		}
		print_logs(x,n);
	} else if (strcmp(argv[1],"--pow") == 0){
		if (argc != 5){
			fprintf(stderr,"Bad usage of content pow\nUsage: ./proj2 --pow X Y N\n");
			return 1;
		}
		char *endptr = NULL;
		double x = strtod(argv[2],&endptr);
		if (*endptr){
			fprintf(stderr,"You have entered %s on %d. argument which is invalid.\n"
				"Try to fix your input and repeat.\n",endptr,argc - 2);
			return 1;
		}
		double y = strtod(argv[3],&endptr);
		if (*endptr){
			fprintf(stderr,"You have entered %s on %d. argument which is invalid.\n"
				"Try to fix your input and repeat.\n",endptr,argc - 1);
			return 1;
		}
		unsigned int n = strtoul(argv[4],&endptr,10);
		if (*endptr){
			fprintf(stderr,"You have entered %s on %d. argument which is invalid.\n"
				"Try to fix your input and repeat.\n",endptr,argc);
			return 1;
		}
		if (strchr(argv[4],'-') || n == 0){
			fprintf(stderr,"You entered an invalid number %d on %d. argument\n"
				"Number must be greater than 0\n",n,argc);
			return 1;
		}
		print_pows(x,y,n);
	} else {
		fprintf(stderr,"Usage 1: ./proj2 --log X N\nUsage 2: ./proj2 --pow X Y N\n");
		return 1;
	}
return 0;
}
