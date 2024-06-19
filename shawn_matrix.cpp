#include <assert.h>
#include <vector>
#include <iostream>
using namespace std;

class smatrix {

public:
    void view() {
        cout << "row x cols: " << shape[0] << " " << shape[1] << endl;
    }
    array<int,2> shape(){
        return shape;
    }
    smatrix(initializer_list<vector<float>> list){
       int args_index;
       for (args_index = 0; args_index < list.size(); args_index++) {
            colsview.push_back(list[args_index])
            int cols_index;
            arr<vector<float> rows;
            for (cols_index = 0; cols_index < list[args_index].size(); cols_index++) {
                arr[cols_index].push_back(list[args_index][cols_index])
            }
            int rows_index;
            for (rows_index = 0; rows_index < arr.size(); rows_index++){
                rowsview.push_back(arr[rows_index[)
            }
       }
       shape[0] = colsview.size();
       shape[1] = rowsview.size();
    }

    void print() {
        for (int i = 0; i < colsview.size(); i++) {
            for (int j = 0; j < colsview[i].size(); j++) {
                cout << colsview[i][j];
            }
            cout << "\n";
        }
        cout << endl;
    }
    vector<vector<float> getcols() {
        return colsview
    }
    vector<vector<float> getrows() {
        return rowsview
    }
    void setcols(vector<vector<float>> input) {
        colsview = input
    }
    void setrows(vector<vector<float>> input) {
        rowsview = input
    }
private:
    array<int,2> shape;
    vector<vector<float>> colsview;
    vector<vector<float>> rowsview;
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
    assert(mat1.shape()[0] == mat2.shape())[1];
    int mat_1_col_index; 
    int mat_2_col_index;
    vector<float> row_multiple;
    vector<float> column_multiple;
    float new_cell_value;
    initializer_list<vector<float>> output_list
    vector<vector<float>> all_rows = mat1.getrows()
    vector<vector<float>> all_columns = mat2.getcols()

    for ( mat_2_col_index= 0;  mat_2_col_index < mat2.size(); mat_2_col_index++)
    {
        for (mat_1_col_index = 0; mat_1_col_index < mat1[0].size(); mat_1_col_index++)
        {
            column_multiple = all_columns[mat_2_col_index];
            row_multiple = all_rows[mat_1_col_index]
            new_cell_value = CPUDotProduct(column_multiple, row_multiple);
            new_column.push_back(new_cell_value);
        }
        output_list.push_back(new_column);
    }
    return smatrix(output_list);
}
void GPUDotProduct(vector<float> vec1, vector<float> vec2)
{
}
void GPUMatMul(smatrix mat1, smatrix mat2) 
{
}
int main() {
    smatrix mat1 = smatrix(vector<float> vect{10,20,30}, vector<float> vect2{20,30,40}, vector<float> vect3{21,31,41})
    smatrix mat2 = smatrix(vector<float> vect{10,20,30}, vector<float> vect2{20,30,40}, vector<float> vect3{21,31,41})
    smatrix output = NaiveMatMul(mat1, mat2);
    output.view();
    output.print();
}
