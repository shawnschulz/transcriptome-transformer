#include <array>
#include <assert.h>
#include <iomanip>
#include <iostream>
#include <vector>

using namespace std;

template <typename T> class lokitrix {
public:
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
    create(a, b, c);
  }
  lokitrix(size_type n, const T& value, array<int, 2> input_dimensions){
	  create(n, value, input_dimensions);
  }
  ~lokitrix() { delete_lokitrix(); } //this is the destructor for the class
  //not using const T& here for operator overload violates the assumption of class invariance and may not 
  //be worth it for convenience in writing the matmul function
  //Having a lot of trouble accessing data to use [] operator overload, lets come back to this
  //after fixing other compiler errors
  //T& operator[](size_type i) { 
  //  if (dimensions[0] > 1) {
  //    size_t output_size = dimensions[1];
  //    array<int, 2> output_dimensions = {1, dimensions[1]};
  //    lokitrix slice(output_size, 0, output_dimensions);
  //    int row_start_index = this.absolute_index(i,0);
  //    for (int i = 0; i < dimensions[1]; i++) {
  //        //how are you gonna use the index operator for this when you haven't
  //        //defined the index operator yet LMAO
  //           slice[i] = this.data_start()[row_start_index + i];
  //    } 
  //    return lokitrix(slice, output_size, output_dimensions);
  //  } else {
  //    return this.data_start()[i];
  //  }
  //}
  lokitrix<T>& operator=(const lokitrix& right_hand_side) {
    if (&right_hand_side != this) {
      delete_lokitrix();
      create(right_hand_side.begin(), right_hand_side.end());
    }
    return *this;
  }
  void shape() {
    cout << "row x cols: " << dimensions[0] << " " << dimensions[1] << endl;
  }
  array<int, 2> get_dimensions() { return dimensions; }
  data_pointer get_data() { return data; }
  int get_size() { return data_size; }
  //const char *get_datatype() { return datatype; }
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

private:
  size_type data_size;
  //the data_pointer should be a smart pointer
  data_pointer data;
  //eehhh we have strayed from god, no more dynamic mem allocation 
  //lets make 6 create functions that make a static array of size
  //1mb, 100 mb, 1gb, 8gb, 32gb and 64gb
  //ideally we use make_unique or something somehow to add some
  //attempt at memory safety
  //
  //create will default to an 8gb lokitrix, since that seems like
  //a reasonable amount for a neural network sized dataset
  //
  //data will be a smart pointer to an appropriately sized std::array
  void create(data_pointer, size_type, array<int, 2>);
  void create_4mb(data_pointer, size_type, array<int, 2>);
  void create_100mb(data_pointer, size_type, array<int, 2>);
  void create_1gb(data_pointer, size_type, array<int, 2>);
  void create_8gb(data_pointer, size_type, array<int, 2>);
  void create_32gb(data_pointer, size_type, array<int, 2>);
  void create_64gb(data_pointer, size_type, array<int, 2>);

  //data will be a smart pointer to std::vec
  void create_dyn(data_pointer, size_type, array<int, 2>);
  //all of these can be overloaded to make empty versions with repititous input
  //data
  //are we repeating ourselves? yes. should we really have a shared interface?
  //also yes. but i think this is the only way to do this without dynamic
  //mem allocation, and while it was a fun side thing to reimpliment std::vec
  //the point of this library is HPC and this will make that a lot easier
  //i think we don't need to store what type we made? can check it based on
  //the size of array since these are now gonna be std::arrays
  //
  //old dynamic mem allocation functions
  //void create();
  //void create(size_type, const T&, array<int, 2>);
  //void create_from_data(data_pointer, size_type, array<int, 2>);
  //void create(const_data_pointer, const_data_pointer);
  void add_rows(lokitrix);
  void delete_lokitrix();

  array<int, 2> dimensions;
  // stride is the amount to move to format columns and rows, can simply
  // swap stride around to go from row major to column major
  array<int, 2> stride;
};

//template <class T>
//initializes data of all same value
//void lokitrix<T>::create_same_data(size_type n, const T& value, array<int, 2> input_dimensions) {
//  data = alloc.allocate(n);
//  data_end_pointer = data + n;
//  uninitialized_fill(data, data_end_pointer, value);
//  dimensions = input_dimensions;
//  stride[0] = (data_size / input_dimensions[0]);
//  stride[1] = 1;
//  if (data_size % dimensions[1] != 0 ||
//      dimensions[0] != data_size / dimensions[1]) {
//    throw invalid_argument("Given data size and dimensions don't match");
//  }
//}
template <class T>
//initializes data from an input array with no shape, default allocates 
//roughly 1 gb worth of f32 array (e.g. if you choose to use double instead
//you'll actually use 2 gb of memory, if u use a f16 you use 500 mb of memory
//etc.
void lokitrix<T>::create(data_pointer input, size_type n, array<int, 2> input_dimensions) {
  array<T, 250000000> data;
  //this seems dangerous, could easily dereference a null ptr
  if (!(data.size() > n)) {
          throw invalid_argument("Given data exceeds size of allocated array");
          }
  std::copy(*input, data);
  dimensions = input_dimensions;
  stride[0] = (data_size / input_dimensions[0]);
  stride[1] = 1;
  if (data_size % dimensions[1] != 0 ||
      dimensions[0] != data_size / dimensions[1]) {
    throw invalid_argument("Given data size and dimensions don't match");
  }
}

template <class T>
void lokitrix<T>::delete_lokitrix()
{
  if (data) {
    delete_lokitrix(data);
    delete_lokitrix(dimensions);
    delete_lokitrix(stride);
  }
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
    // You gotta avoid copying this much, you will be slower than
    // a garbage collected language like python unless you avoid
    // as many copying steps as possible
  const array<int, 2> dimensions_1 = mat1.get_dimensions();
  const array<int, 2> dimensions_2 = mat2.get_dimensions();
  array<int, 2> output_dimensions;
  output_dimensions[0] = dimensions_1[0];
  output_dimensions[1] = dimensions_2[1];
  const size_t output_size = dimensions_1[0] * dimensions_2[1];
  
  lokitrix<T> output_data(output_size, 0, output_dimensions);
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
	int mat3_abs_index = output_data.absolute_index(mat1_abs_index, mat2_abs_index);
        new_cell_value = mat1.data_start()[mat1_abs_index] * mat2.data_start()[mat2_abs_index];
        output_data.data_start()[mat3_abs_index] = new_cell_value;
      }
    }
  }
  // it doesn't feel intuitive that this will work, will the actual data
  // output_data points to still be there once we leave this scope? be careful
  // about this
  return output_data;
}

int main() {
  array<float, 12> data1 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
  array<float, 12> data2 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
  array<int, 2> dims = {4, 3};
  lokitrix mat1(&input, 12, dims);
  lokitrix mat2(&input2, 12, dims);
  mat1.print();
  cout << mat1.absolute_index(1, 2) << endl;
  mat1.transpose();
  mat1.print();
  lokitrix mat3 = CPUMatMul(mat1, mat2);
  mat3.print();
}
