#include <assert.h>
#include <vector>
#include <iostream>
#include <array>

using namespace std;

template <typename T>
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
    const char * get_datatype() {
        return datatype;
    }
    void print() 
    {
	// print the data in row major format with a copy pasta-able format
	// so you can put it straight into python or c++ if you want
        //
	// should make it so prints out a truncated view of the head if the
	// data structure is too big
    int column_index;
    int row_index;
	cout << "[";
        for (int column_index = 0; column_index <= data_size; column_index += stride[0]) 
	{
	    cout << "[";
	    for (int row_index = 0; row_index <= data_size; row_index += stride[1]) {
		    cout << *data_pointer[column_index + row_index];
		    if (row_index != data_size - stride[1]) {
			    cout << ", ";
		    }
            if (row_index > 1000) {
                cout << "]]";
                cout << endl;
                return void();
            }
	    }
	    cout << "]";
	    if (column_index != data_size) {
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

template <typename T>
T CPUDotProduct(T mat1, T mat2) 
{
    //should assert that the two vectors are the same size
    array<int, 2> dimensions_1 = mat1.get_dimensions();
    array<int, 2> dimensions_2 = mat2.get_dimensions();
    const int mat1_size = mat1.get_size();
    const int mat2_size = mat2.get_size();
    if (mat1_size != 1 and mat2_size != 1) {
    if (dimensions_1 != dimensions_2) { 
        throw invalid_argument("Dimensions of mat1 and mat2 do not match");
    }
    if (mat1_size != mat2_size) {
        throw invalid_argument("Size of mat1 and mat2 data do not match");
    }
    }
    int output_size;
    array<int, 2> output_dimensions;
    if (mat1_size == 1) {
        output_size = mat2_size;
        output_dimensions = dimensions_2;
    }
    else if (mat2_size == 1) {
        output_size = mat1_size;
        output_dimensions = dimensions_1;
    }
    else {
        //do compiler optimizations get this
        output_size = mat1_size;
        output_dimensions = dimensions_1;
    }
    array<T, mat1_size> output_data;
    int i;
    for (i = 0; i < output_size; i++)
    {
        output_data[i] = *mat1.get_data_pointer()[i] * *mat2.get_data_pointer()[i];
    }
    return smatrix(&output_data, output_size, mat1.get_datatype(), dimensions_1);
}

template <typename T>
smatrix<T> CPUMatMul(smatrix<T> mat1, smatrix<T> mat2)
{
    array<int, 2> dimensions_1 = mat1.get_dimensions();
    array<int, 2> dimensions_2 = mat2.get_dimensions();
    int output_size = mat1.get_size();
    array<T, output_size> output_data;
    array<int, 2> output_dimensions = [dimensions_1[0], dimensions_2[1]];
    //rows of mat1 should be same size of columns of mat2
    if (dimensions_1[0] != dimensions_2[1]) 
    {
	    throw invalid_argument("Rows of mat1 do not match cols of mat2");
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
            new_cell_value = *mat1.get_data_pointer()[mat1_abs_index] * *mat2.get_data_pointer()[mat2_abs_index];
            output_data[i + j] = new_cell_value;
	    }
        }
    }
    return smatrix(&output_data, output_size, mat1.get_datatype(), output_dimensions);
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
