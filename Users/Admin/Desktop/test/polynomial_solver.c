#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <json-c/json.h>  

int decode_base(const char* encoded_value, int base) {
    int decoded_value = 0;
    while (*encoded_value) {
        decoded_value = decoded_value * base + (*encoded_value - '0');
        encoded_value++;
    }
    return decoded_value;
}

// Function to perform Gaussian elimination to solve the system of equations
void gaussian_elimination(double matrix[][10], double* results, int n) {
    for (int i = 0; i < n; i++) {
        // Make the diagonal element 1
        double divisor = matrix[i][i];
        for (int j = 0; j <= n; j++) {
            matrix[i][j] /= divisor;
        }

        // Eliminate the i-th variable from the other equations
        for (int k = 0; k < n; k++) {
            if (k != i) {
                double factor = matrix[k][i];
                for (int j = 0; j <= n; j++) {
                    matrix[k][j] -= factor * matrix[i][j];
                }
            }
        }
    }

    // Extract the results (the polynomial coefficients)
    for (int i = 0; i < n; i++) {
        results[i] = matrix[i][n];
    }
}

int main() {
    // Open and read the JSON file
    FILE *file = fopen("input.json", "r");
    if (!file) {
        printf("Error: Unable to open the JSON file.\n");
        return 1;
    }

    // Load the JSON data into a buffer
    char buffer[1024];
    fread(buffer, sizeof(char), 1024, file);
    fclose(file);

    // Parse the JSON data
    struct json_object *parsed_json;
    struct json_object *points_array;
    struct json_object *base_obj;
    struct json_object *point_obj;
    struct json_object *x_obj;
    struct json_object *y_obj;

    parsed_json = json_tokener_parse(buffer);

    // Get the base for decoding
    json_object_object_get_ex(parsed_json, "base", &base_obj);
    int base = json_object_get_int(base_obj);

    // Get the points array
    json_object_object_get_ex(parsed_json, "points", &points_array);
    int num_points = json_object_array_length(points_array);

    // Allocate arrays for x and y
    double x[num_points];
    double y[num_points];

    // Decode the points from the JSON array
    for (int i = 0; i < num_points; i++) {
        point_obj = json_object_array_get_idx(points_array, i);

        // Extract x and y values
        json_object_object_get_ex(point_obj, "x", &x_obj);
        json_object_object_get_ex(point_obj, "y", &y_obj);

        x[i] = json_object_get_double(x_obj);  // Get x value as a double
        const char* y_encoded = json_object_get_string(y_obj);  // Get y value as a string

        // Decode y from the given base
        y[i] = decode_base(y_encoded, base);
    }

    // Degree of the polynomial is num_points - 1
    int degree = num_points - 1;

    // Construct the matrix for Gaussian elimination
    double matrix[10][10];  // 10x10 matrix to hold the coefficients and constants
    double results[10];     // Array to store the solution (polynomial coefficients)

    // Fill the matrix with the powers of x
    for (int i = 0; i < num_points; i++) {
        for (int j = 0; j <= degree; j++) {
            matrix[i][j] = pow(x[i], j);
        }
        matrix[i][degree + 1] = y[i];  // Set the constants (y values) on the right-hand side
    }

    // Solve the system using Gaussian elimination
    gaussian_elimination(matrix, results, num_points);

    // Output the constant term 'c'
    printf("The constant term c of the polynomial is: %lf\n", results[0]);

    return 0;
}
