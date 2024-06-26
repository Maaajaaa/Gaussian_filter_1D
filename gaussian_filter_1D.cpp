// g++ gaussian_filter.cpp -std=c++11 -Wall -o gaussian_filter -I/usr/include/python2.7 -lpython2.7
/* C++ Related */
#include <iostream>
#include <stdio.h>
#include <math.h> 
#include "matplotlib-cpp/matplotlibcpp.h"

namespace plt = matplotlibcpp;

//prottype function
double applyKernel(int n_points, int x_position, double kernel[], double y_values[]);

double fwhm2sigma(float fwhm)
{
    double sigma = fwhm / (sqrt(8*log(2)));
    return sigma;
}

double sigma2fwhm(float sigma)
{
    double fwhm = sigma * (sqrt(8*log(2)));
    return fwhm;
}

double computeAndApplyKernel(int n_points, int x_position, double sigma, double y_values[])
{
    //Compute the kernel for the given x point
    double kernel[n_points] = {};
    double sum_kernel = 0;
    //calculate sigma² once to speed up calculation
    double twoSigmaSquared = (2*pow(sigma,2));
    for (int i =0; i<n_points;i++)
    {
        //Compute gaussian kernel
        kernel[i] = exp(-(pow(i- x_position,2) / twoSigmaSquared));
        //compute a weight for each kernel position
        sum_kernel += kernel[i];
    }
    //apply weight to each kernel position to give more important value to the x that are around ower x
    for(int i = 0;i<n_points;i++)
        kernel[i] = kernel[i] / sum_kernel;
    return applyKernel(n_points, x_position, kernel, y_values);
}

double applyKernel(int n_points, int x_position, double kernel[], double y_values[])
{
    double y_filtered = 0;
    //apply filter to all the y values with the weighted kernel
    for(int i = 0;i<n_points;i++) 
        y_filtered += kernel[i] * y_values[i];

    return y_filtered;
}

int main()
{
    int n_points = 50; //number of points
    //double fwhm = 2; //FWHM
    double sigma = 0.8493218003; //can be calculated dfrom FWHM

    double y_values[n_points] = {};
    //Initiate y array with random values
    for (int i = 0;i<n_points;i++)
        y_values[i] = std::rand() % 10;

    double y_values_filtered[n_points] = {};

    //apply filter to every points
    for (int x_position=0;x_position<n_points;x_position++)
    {
        double y_filtered = computeAndApplyKernel(n_points,x_position,sigma,y_values);
        y_values_filtered[x_position] = y_filtered;
    }

    std::vector<int> x_vector(n_points);
    for(int i = 0; i<n_points; i++){
        x_vector[i] = i;
    }

    // convert arrays to vectors for plotting
    std::vector<int> y_vector(y_values, y_values + n_points);
    std::vector<int> y_filt_vector(y_values_filtered, y_values_filtered + n_points);

    plt::figure_size(1200, 780);
    plt::plot(x_vector, y_vector, "r");
    plt::plot(x_vector, y_filt_vector);
    plt::show();

}

