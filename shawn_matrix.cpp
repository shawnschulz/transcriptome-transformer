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
    smatrix(vector<vector<float>> columns){
        //smatrix must be supplied columns of equal length
        //or undefined behavior will occur
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
    array<int, 2> dimensions;
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
    smatrix mat1 = smatrix(vector<vector<float> vect{vector<float> vect{10,20,30}, vector<float> vect2{20,30,40}, vector<float> vect3{21,31,41}});
    smatrix mat2 = smatrix(vector<vector<float> vect{vector<float> vect{10,20,30}, vector<float> vect2{20,30,40}, vector<float> vect3{21,31,41}});
    smatrix output = NaiveMatMul(mat1, mat2);
    output.view();
    output.print();
}
