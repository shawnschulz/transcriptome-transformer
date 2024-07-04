#include <array>
#include <assert.h>
#include <iomanip>
#include <iostream>
#include <vector>

using namespace std;

template <typename T> class lokitrix {
public:
  //PUBLIC FUNCTIONS MUST NOT CHANGE data or data_end_pointer!!!!
  //new idea: use this matrix for multi thread intensive operations
  //where eeking out more performance matters alot more, and use
  //a diff, simpler version with a vec<> as the underlying
  //data storage for matrix operations that can be done quickly
  //single threaded. make a shared interface that uses both appropriately
  //(or just repeat yourself LOLOL)
  typedef T* data_pointer;
  typedef const T* const_data_pointer;
  typedef size_t size_type;
  typedef T datatype;

  lokitrix(T *a, int b, array<int, 2> c) {
    data = a;
    data_size = b;
    dimensions = c;
    // actually might just not be possible to check if array is null lol

    stride[0] = (data_size / dimensions[0]);
    stride[1] = 1;
  }
  lokitrix() { create(); } //the empty constructor
  lokitrix(const lokitrix& l) { create(l.data_start(), l.data_end()); } //this is the copy constructor
  ~lokitrix() { delete_lokitrix(); } //this is the destructor for the class
  const T& operator[](size_type i) { 
    if (dimensions[0] > 1) {
      size_t output_size = dimensions[1];
      array<int, 2> output_dimensions = {1, dimensions[1]};
      lokitrix slice(output_size, output_dimensions)
      data_pointer slice = allocate.alloc(output_size);
      data_pointer slice_end = slice + dimensions[1];
      int row_start_index = *this.absolute_index(i,0);
      for (int i = 0; i < dimensions[1]; i++) {
	     slice[i] = *this[row_start_index + i];
      } 
      return lokitrix(slice, output_size, output_dimensions);
    } else {
      return data[i];
    }
  }
  lokitrix<T>& operator=(const lokitrix& right_hand_side) {
    if (&right_hand_side != this) {
      uncreate();
      create(right_hand_side.begin(), right_hand_side.end())
    }
    return *this;
  }
  void shape() {
    cout << "row x cols: " << dimensions[0] << " " << dimensions[1] << endl;
  }
  array<int, 2> get_dimensions() { return dimensions; }
  T get_data() { return data; }
  int get_size() { return data_size; }
  const char *get_datatype() { return datatype; }
  void print(int precision = 2, int width = 5) {
    // idk we change defaults for the width that feel right
    int column_index;
    cout << fixed << setprecision(precision);
    cout << "[";
    for (int row_index = 0; row_index <= dimensions[0] - 1; row_index++) {
      if (row_index != 0) {
        cout << " ";
      }
      cout << "[";
      cout << setw(width);
      for (int column_index = 0; column_index <= dimensions[1] - 1;
           column_index += stride[1]) {
        int abs_index = row_index * stride[0] + column_index;
        auto output = data[abs_index];
        cout << output;
        if (abs_index != data_size - 1 && column_index != dimensions[1] - 1) {
          cout << ", ";
        }
        if (row_index > 1000) {
          cout << "...";
          cout << "]]";
          cout << endl;
          return void();
        }
      }
      cout << "]";
      if (row_index * stride[0] < data_size - stride[0]) {
        cout << ",";
        cout << "\n";
      }
    }
    cout << "]";
    cout << endl;
    return void();
  }
  void transpose() {
    int temp2 = dimensions[0];
    dimensions[0] = dimensions[1];
    dimensions[1] = temp2;
    stride[0] = data_size / dimensions[0];
  }
  int absolute_index(int i, int j) {
    int absolute_index = i * stride[0] + j;
    return absolute_index;
  }
  data_pointer data_start() { return data; };
  const_data_pointer data_start() const { return data; };
  data_pointer data_end() { return data_end_pointer; };
  const_data_pointer data_end() const { return data_end_pointer; };

private:
  const int data_size;
  data_pointer data;
  data_pointer data_end_pointer;
  allocator<T> alloc;
  void create();
  void create(size_type, const T&);
  void create(const_data_pointer, const_data_pointer);
  void delete_lokitrix();

  array<int, 2> dimensions;
  // stride is the amount to move to format columns and rows, can simply
  // swap stride around to go from row major to column major
  array<int, 2> stride;
};

template <class T> 
void lokitrix<T>::create() {
  data = data_end_pointer = 0;
}
template <class T>
void lokitrix<T>::create(size_type n, const T& value, array<int, 2> input_dimensions) {
  data = alloc.allocate(n);
  data_end_pointer = data + n;
  uninitialized_fill(data, data_end_pointer, value);
  dimensions = input_dimensions
  stride[0] = (data_size / input_dimensions[0]);
  stride[1] = 1;
  if (data_size % dimensions[1] != 0 ||
      dimensions[0] != data_size / dimensions[1]) {
    throw invalid_argument("Given data size and dimensions don't match");
  }
}
template <class T>
void lokitrix<T>::create(const_data_pointer i, const_data_pointer j)
{
  data = alloc.allocate(j - i);
  data_end_pointer = uninitialized_copy(i, j, data);
}
template <class T>
void lokitrix<T>::delete_lokitrix()
{
  if (data) {
      data_pointer it = data_end_pointer;
      while (it != data)
          alloc.destroy(--it);
      alloc.deallocate(data, data_end_pointer - data);
  }
  data = data_end_pointer = 0;
}
template <class T, size_t SIZE>
void lokitrix<T>::add_rows(lokitrix<T> new_rows) {
  //because we don't have a limit and used pointer, this method incurs a 
  //memory overhead, since we allocate an entirely new matrix every time.
  //thus it should be used sparingly (as I would expect)
  //
  //if this memory overhead is a lot, create a second matrix representation
  //that uses std::vector and incurs the performance hit to syncing across
  //threads in favor of making this process faster
  if (new_row.get_stride[0] != dimensions[1]) {
    throw invalid_argument("New row sizes do not match matrix dimensions")
  }
  size_type new_size = data_end_pointer + new_rows.data_end();
  data_pointer new_data = alloc.allocate(new_size);
  data_pointer new_end_pointer = uninitialized_copy(data, data_end_pointer, new_data);
  delete_lokitrix();
  data = new_data;
  data_end_pointer = new_end_pointer;
}
// template <typename T>
// T CPUDotProduct(T mat1, T mat2)
//{
//     //should assert that the two vectors are the same size
//     array<int, 2> dimensions_1 = mat1.get_dimensions();
//     array<int, 2> dimensions_2 = mat2.get_dimensions();
//     const int mat1_size = mat1.get_size();
//     const int mat2_size = mat2.get_size();
//     if (mat1_size != 1 and mat2_size != 1) {
//     if (dimensions_1 != dimensions_2) {
//         throw invalid_argument("Dimensions of mat1 and mat2 do not match");
//     }
//     if (mat1_size != mat2_size) {
//         throw invalid_argument("Size of mat1 and mat2 data do not match");
//     }
//     }
//     const int output_size;
//     array<int, 2> output_dimensions;
//     if (mat1_size == 1) {
//         output_size = mat2_size;
//         output_dimensions = dimensions_2;
//     }
//     else if (mat2_size == 1) {
//         output_size = mat1_size;
//         output_dimensions = dimensions_1;
//     }
//     else {
//         //do compiler optimizations get this
//         output_size = mat1_size;
//         output_dimensions = dimensions_1;
//     }
//     array<T, mat1_size> output_data;
//     int i;
//     for (i = 0; i < output_size; i++)
//     {
//         output_data[i] = *mat1.get_data()[i] * *mat2.get_data()[i];
//     }
//     return lokitrix(&output_data, output_size, mat1.get_datatype(),
//     dimensions_1);
// }
//

template <class T>
lokitrix<T> CPUMatMul(lokitrix<T> mat1, lokitrix<T> mat2) {
  const array<int, 2> dimensions_1 = mat1.get_dimensions();
  const array<int, 2> dimensions_2 = mat2.get_dimensions();
  array<int, 2> output_dimensions;
  output_dimensions[0] = dimensions_1[0];
  output_dimensions[1] = dimensions_2[1];
  const size_t output_size = dimensions_1[0] * dimensions_2[1];
  T *output_data = new T[output_size];
  // the output dimensions are the rows of mat1 x the columns of mat2
  // rows of mat1 should be same size of columns of mat2
  if (dimensions_1[0] != dimensions_2[1]) {
    throw invalid_argument("Rows of mat1 do not match cols of mat2");
  }
  T new_cell_value;
  int i;
  int j;
  for (j = 0; j < dimensions_1[0]; j++) {
    for (i = 0; i < dimensions_2[1]; i++) {
      int shared_index;
      for (shared_index = 0; shared_index < dimensions_1[0]; shared_index++) {
        int mat1_abs_index = mat1.absolute_index(shared_index, j);
        int mat2_abs_index = mat2.absolute_index(i, shared_index);
        new_cell_value =
            *mat1.get_data()[mat1_abs_index] * *mat2.get_data()[mat2_abs_index];
        output_data[i + j] = new_cell_value;
      }
    }
  }
  // it doesn't feel intuitive that this will work, will the actual data
  // output_data points to still be there once we leave this scope? be careful
  // about this
  lokitrix return_value(output_data, output_size, mat1.get_datatype(),
                       output_dimensions);
  return return_value;
}

int main() {
  float input[12] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
  array<int, 2> dims = {4, 3};
  lokitrix mat1(input, 12, "float", dims);
  lokitrix mat2(input, 12, "float", dims);
  mat1.print();
  cout << mat1.absolute_index(1, 2) << endl;
  mat1.transpose();
  mat1.print();
  lokitrix mat3 = CPUMatMul(mat1, mat2);
  mat3.print();
  free(mat3);
}
