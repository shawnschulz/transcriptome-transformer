#include <assert.h>
#include <vector>
#include <iostream>
#include <array>
using namespace std;

class smatrix {

public:
    void view() {
        cout << "row x cols: " << dimensions[0] << " " << dimensions[1] << endl;
    }
    array<int, 2> shape(){
        return dimensions;
    }
    smatrix(array * data, array<int,2> shape){
	    //basic constructor should be able to get everything from a contiguous array
	    //and what the shape is supposed to be (do basic assertions that shape make sense)
	    //prob wanna make 2 additional constructors, one that takes in a formatted string
	    //and one that uses the formatted string construcor function but reads off of an
	    //uncompressed file
	    //
	    //note that this default constructor takes a reference to the contiguous array,
	    //not the data itself
	    data = data
	    if (!data) {
		    throw invalid_argument("Did not receive pointer to data")
	    }
	    data_size = *data.size()
	    if (data_size % shape[1] != 0) {
		    throw invalid_argument("Number of columns does not divide data evenly")
	    }
	    stride = data_size / shape[1]
	    // since c++ arrays are static should be able to infer data type from just one example
	    datatype = typeid(*data[0])
    }

    void print() {
        for (int i = 0; i < rowsview.size(); i++) {
            for (int j = 0; j < rowsview[i].size(); j++) {
                cout << rowsview[i][j];
            }
            cout << "\n";
        }
        cout << endl;
    }
    vector<vector<float>> getcols() {
        return colsview;
    }
    vector<vector<float>> getrows() {
        return rowsview;
    }
    void setcols(vector<vector<float>> columns) {
       colsview = columns;
       vector<vector<float>> rows;
       int i;
       for (i = 0; i < columns[0].size(); i++){
           int j;
           for (j = 0; j < columns.size(); j++) {
               rows[i].push_back(columns[j][i]);
           }
       }
       rowsview = rows;
       dimensions[0] = rowsview.size();
       dimensions[1] = colsview.size();
    }
    void setrows(vector<vector<float>> rows) {
       rowsview = rows;
       vector<vector<float>> columns;
       int j;
       for (j = 0; j < rows[0].size(); j++){
           int i;
           for (i = 0; i < rows.size(); i++) {
               columns[j].push_back(rows[i][j]);
           }
       }
       colsview = columns;
       dimensions[0] = rowsview.size();
       dimensions[1] = colsview.size();
    }
    void setcell(int i, int j, float new_value) {
       colsview[j][i] = new_value;
       rowsview[i][j] = new_value;
       dimensions[0] = rowsview.size();
       dimensions[1] = colsview.size();
    }
    vector<vector<float>> transpose() {
        return colsview;
    }
private:
    //it makes more sense to go for a contiguous array vs vectors
    //since that's what numpy fortran and matlab do
    //its much less memory intensive and lets you do 
    //things like transpose and other matrix operations much faster
    int data_size;
    array * data;
    const char * datatype;
    array<int, 2> shape;
    //user just supplies dimensions, not stride. we set the stride based on user dimensions
    int stride;
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
    vector<vector<float>> input{{10,20,30}, {20,30,40}, {21,31,41}};
    smatrix mat1 = smatrix(input);
    smatrix mat2 = smatrix(input);
    smatrix output = NaiveMatMul(mat1, mat2);
    output.view();
    output.print();
}
