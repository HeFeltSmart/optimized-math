#include <stdio.h>
#include <time.h>

// Forward declaration of the standard double sqrt function
double sqrt(double x);

void run_test(double value)
{
    clock_t start, end;
    double cpu_time_used;

    // Start tracking CPU clock cycles
    start = clock();
    
    // Execute enhanced sqrt function
    double result = sqrt( value );
    
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

    printf( "Input: %10.2f  ->  Calculated Root: %15.10f (Time: %f seconds)\n", 
            value, result, cpu_time_used );
}

int main()
{
    printf( "==================================================\n" );
    printf( "        Testing the enhanced sqrt function        \n" );
    printf( "==================================================\n" );

    // 1. Test Perfect Squares (Triggers your Early-Stop Optimization!)
    printf( "\n[Testing Perfect Squares - Early Exit Check]\n" );
    run_test( 4.0 );
    run_test( 100.0 );
    run_test( 144.0 );
    run_test( 1000000.0 );

    // 2. Test Inexact / Irrational Roots (Runs to full 53-bit resolution)
    printf("\n[Testing Irrational Roots - Full Precision Check]\n");
    run_test( 2.0 );
    run_test( 3.0 );
    run_test( 12.5 );
    run_test( 0.5 );

    printf( "==================================================\n" );
    return 0;
}
