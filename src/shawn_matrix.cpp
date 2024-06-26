#include <assert.h>
#include <vector>
#include <iostream>
#include <array>

using namespace std;

template<typename T>
class smatrix {
public:
    smatrix(T data_pointer[], int data_size, const char * datatype,  array<int,2> dimensions)
    {
	    //we are gonna do polymorphic arrays bc it makes it easy to lower
	    //precision of numbers stored in the array but it might be
	    //really annoying
	    //Also, lets start with just 2d arrays. this will make it somewhat inconvenient to
	    //refactor for higher dimensoinal arrays, but we can also compose a higher order
	    //array as multiple 2d smatrices in the future, so it should be okay to start with
	    //assuming 2d

	    data_pointer = data_pointer;
	    data_size = data_size;
	    datatype = datatype;
	    dimensions = dimensions;
	    
	    if (!data_pointer) {
		    throw invalid_argument("Did not receive pointer to data");
	    }
	   
	    if (data_size % dimensions[1] != 0) {
		    throw invalid_argument("Given data size and dimensions don't match");
	    }
	  
	    stride = data_size / dimensions[0];
    }
    void shape() 
    {
        cout << "row x cols: " << dimensions[0] << " " << dimensions[1] << endl;
    }
    array<int, 2> get_dimensions() 
    {
	    return dimensions;
    }
    T* get_data_pointer() {
	return data_pointer;
    }
    int get_size() {
	    return data_size;
    }
    void print() 
    {
	// print the data in row major format with a copy pasta-able format
	// so you can put it straight into python or c++ if you want
        //
	// should make it so prints out a truncated view of the head if the
	// data structure is too big
	cout << "[";
        for (int column_index = 0; column_index <= data_size; column_index += stride[0]) 
	{
	    cout << "[";
	    for (int row_index = 0; row_index =< data_size; row_index += stride[1]) {
		    cout << *data[column_index + row_index];
		    if (row_index != data_size - stride[1]) {
			    cout << ", ";
		    }
	    }
	    cout << "]";
	    if (column_index != data_size {
	    	cout << ",";
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
    int absolute_index(int i, int j) {
	int absolute_index = (j * stride[0]) + i;
	return absolute_index;
    }
    smatrix copy (){
    	return smatrix(data_pointer, data_size, datatype,  dimensions);
    }

private:
    //it makes more sense to go for a contiguous array vs vectors
    //since that's what numpy fortran and matlab do
    //its much less memory intensive and lets you do 
    //things like transpose and other matrix operations much faster
    //
    //we do need the data size since we're taking a generic pointer
    int data_size;
    T* data_pointer;
    const char * datatype;
    array<int, 2> dimensions;
    //stride is the amount to move to format columns and rows, can simply
    //swap stride around to go from row major to column major
    array<int, 2> stride;
    
    //first element is number of array elements you have to go in
    //it's not hard to use other data types but lets start by assuming we are using floating point numbers
    //the amount to jump to get to the next row of data. since arrays are always NxM, you jump the same amount
    //to get to any row of data, so to get to the 20th row you just index to 20 * stride

};


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

template <typename T>
smatrix NaiveMatMul(smatrix mat1, smatrix mat2)
{
    array<int, 2> dimensions_1 = mat1.get_dimensions()
    array<int, 2> dimensions_2 = mat2.get_dimensions()
    int output_size = mat1.get_size()
    array<T, output_size> output_data;
    //rows of mat1 should be same size of columns of mat2
    if (dimensions_1[0] == dimensions_2[1]) 
    {
	    throw invalid_argument("Rows of mat1 do not match cols of mat2")
    }
    T new_cell_value;
    int i; 
    int j;
    for (j = 0;  j < dimensions_1[0]; j++)
    {
        for (i = 0; i < dimensions_2[1]; i++)
        {
	    int shared_index;
	    for (shared_index = 0; shared_index < dimensions_1[0]); shared_index++) {
		mat1_abs_index = mat1.absolute_index(shared_index, j);
		mat2_abs_index = mat2.absolute_index(i, shared_index);
			
		new_cell_value = *mat1.get_data_pointer()[mat1_abs_index] * *mat2.get_data_pointer()[mat2_abs_index]
		output_data[i + j] = new_cell_value
	    }
        }
    }
    return smatrix(&output_data, data_size, T, dimensions);
}
void GPUDotProduct(vector<float> vec1, vector<float> vec2)
{
}
void GPUMatMul(smatrix mat1, smatrix mat2) 
{
}
int main() {
    array<float, 9> input = [1,2,3,4,5,6,7,8,9, 10, 11, 12];
    array<int, 2> dims = [4,1];
    smatrix mat1 = smatrix(&input, dims);
    smatrix mat2 = smatrix(&input, dims);
    mat1.print();
    smatrix output = NaiveMatMul(mat1, mat2);
    output.print();
}
