#include <assert.h>
#include <vector>
#include <iostream>
#include <array>
using namespace std;

class smatrix {

public:
    smatrix(array * data, array<int,2> dimensions)
    {
	    //basic constructor should be able to get everything from a contiguous array
	    //and what the shape is supposed to be (do basic assertions that shape make sense)
	    //prob wanna make 2 additional constructors, one that takes in a formatted string
	    //and one that uses the formatted string construcor function but reads off of an
	    //uncompressed file
	    //
	    //note that this default constructor takes a reference to the contiguous array,
	    //not the data itself. 
	    //
	    //Also, lets start with just 2d arrays. this will make it somewhat inconvenient to
	    //refactor for higher dimensoinal arrays, but we can also compose a higher order
	    //array as multiple 2d smatrices in the future, so it should be okay to start with
	    //assuming 2d
	    data = data;
        dimensions = dimensions;
	    if (!data) {
		    throw invalid_argument("Did not receive pointer to data");
	    }
	    int data_size = *data.size();
	    if (data_size % dimensions[1] != 0) {
		    throw invalid_argument("Number of columns does not divide data evenly");
	    }
	    stride = data_size / dimensions[0];
	    // since c++ arrays are static should be able to infer data type from just one example
	    datatype = typeid(*data[0]);
    }
    void shape() 
    {
        cout << "row x cols: " << dimensions[0] << " " << dimensions[1] << endl;
    }
    array<int, 2> get_dimensions() 
    {
	    return dimensions;
    }
    void print() 
    {
	// print the data in row major format with a copy pasta-able format
    // so you can put it straight into python or c++ if you want
	cout << "[";
    [1, 3] [1,2,3,4,5,6,7,8,9]
        for (int column_index = 0; column_index <= *data.size(); column_index += stride[0]) 
	{
	    cout << "["
	    for (int row_index = 0; row_index < *data.size(); row_index += stride[1]) {
		    cout << *data[column_index + row_index];
		    if (row_index != *data.size() - stride[1]) {
			    cout << ", ";
		    }
	    }
	    cout << "]";
	    if (column_index != *data.size(){
	    	cout << ","
	    }
	    cout << "\n";
        }
	cout << "]";
        cout << endl;
    }
    
    void transpose() {
        //transpose mutates the matrix (compiler optimizations got me for da
        //swap)
        int temp = stride[0];
        stride[0] = stride[1];
        stride[1] = temp;
        temp = dimensions[0];
        dimensions[0] = dimensions[1];
        dimensions[1] = temp;
    }
    smatrix copy (){
        return smatrix(data, stride)
    }

private:
    //it makes more sense to go for a contiguous array vs vectors
    //since that's what numpy fortran and matlab do
    //its much less memory intensive and lets you do 
    //things like transpose and other matrix operations much faster
    int data_size;
    array * data;
    const char * datatype;
    array<int, 2> dimensions;
    //user just supplies dimensions, not stride. we set the stride based on user dimensions
    //also, i think less confusing to just do what numpy does for stride, even though i'm
    //not sure we need to implement storing a byte value.
    //
    
    //first element is number of array elements you have to go in
    array<int, 2> stride;
    //it's not hard to use other data types but lets start by assuming we are using floating point numbers
    //the amount to jump to get to the next row of data. since arrays are always NxM, you jump the same amount
    //to get to any row of data, so to get to the 20th row you just index to 20 * stride

};



//#include <stdio.h>
//#include "cuda_runtime.h"
float CPUDotProduct(vector<float> vec1, vector<float> vec2) 
{
    //should assert that the two vectors are the same size
    assert(vec1.size() == vec2.size());
    int vector_index;
    float sum = 0;
    for (vector_index = 0; vector_index < vec1.size(); vector_index++)
    {
        sum = sum + (vec1[vector_index] * vec2[vector_index]);
    }
    return sum;
}
smatrix NaiveMatMul(smatrix mat1, smatrix mat2)
{
    //rows of mat1 should be same size of columns of mat2
    assert(mat1.shape()[0] == mat2.shape()[1]);
    float new_cell_value;
    vector<vector<float>> all_rows = mat1.getrows();
    vector<vector<float>> all_columns = mat2.getcols();
    vector<vector<float>> output_matrix;

    int i; 
    int j;
    for (j = 0;  j < all_columns.size(); j++)
    {
        for (i = 0; i < all_rows.size(); i++)
        {
	    float sum = 0;
	    int dp_index;
	    for (dp_index = 0; dp_index < dp_index.size(); dp_index++) {
		sum = sum + (
	    }
            new_cell_value = CPUDotProduct(all_columns[j], all_rows[i]);
        }
        output_matrix[i][j] = new_cell_value;
    }
    return smatrix(output_matrix);
}
void GPUDotProduct(vector<float> vec1, vector<float> vec2)
{
}
void GPUMatMul(smatrix mat1, smatrix mat2) 
{
}
int main() {
    array<float, 9> input = [1,2,3,4,5,6,7,8,9, 10, 11, 12]
    array<int, 2> dims = [4,1]
    smatrix mat1 = smatrix(&input, dims);
    smatrix mat2 = smatrix(&input, dims);
    mat1.print();
    smatrix output = NaiveMatMul(mat1, mat2);
    output.print();
}
