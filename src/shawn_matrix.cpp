#include <array>
#include <assert.h>
#include <iomanip>
#include <iostream>
#include <vector>

using std::vector, std::array, std::string, std::cout, std::invalid_argument;

template <typename T> class lokitrix {
public:
  //new idea: use this matrix for multi thread intensive operations
  //where eeking out more performance matters alot more, and use
  //a diff, simpler version with a vec<> as the underlying
  //data storage for matrix operations that can be done quickly
  //single threaded. make a shared interface that uses both appropriately
  //(or just repeat yourself LOLOL)
  typedef vector<T> data_vector;
  typedef const vector<T> const_data_vector;
  typedef size_t size_type;
  typedef T datatype;
  
  //CONSTRUCTORS AND DESTRUCTORS
  //These should just call other functions that outline creation, rather than
  //being complex creation functions themselves 
  lokitrix(data_vector a, array<int, 2> b) {
    create(a, b);
  }
  lokitrix(size_type n, const T& value, array<int, 2> input_dimensions){
	  create(n, value, input_dimensions);
  }
  ~lokitrix() { delete_lokitrix(); } //this is the destructor for the class
  
  //OPERATOR OVERLOADS
  //These should not violate class invariance, instead create an UNSAFE funciton
  //or method if you want to do a mutative behavior
  vector<T>& operator[](size_type i) { 
    if (dimensions[0] > 1) {
      size_t output_size = dimensions[1];
      array<int, 2> output_dimensions = {1, dimensions[1]};
      lokitrix slice(output_size, 0, output_dimensions);
      int row_start_index = this.absolute_index(i,0);
      for (int i = 0; i < dimensions[1]; i++) {
          //how are you gonna use the index operator for this when you haven't
          //defined the index operator yet LMAO
             slice[i] = this.get_const_data()[row_start_index + i];
      } 
      return lokitrix(slice, output_size, output_dimensions);
    } else {
      return this.get_const_data()[i];
    }
  }
  lokitrix<T>& operator=(const lokitrix& right_hand_side) {
    if (&right_hand_side != this) {
      delete_lokitrix();
      create(right_hand_side.get_const_data(), right_hand_side.get_const_dimensions());
    }
    return this;
  }

  //SAFE GETTERS
  //The SAFE getters should all be const and provide read only access to data, protecting
  //class invariance
  const_data_vector get_const_data() const { return data; };
  array<int, 2> get_const_dimensions() const { return dimensions; }
  int get_const_size() const { return get_const_data.size(); }
  array<int, 2> get_const_stride() const { return stride; }

  //UNSAFE GETTERS 
  //The UNSAFE getter functions will violate guarantee of class invariance, which is necessary
  //for functions that mutate lokitrix. You can look to these functions if you have
  //unexpected changes to data (which hopeflly shouldn't happen)
  //Not all data needs an unsafe getter, for example the size is tracked by the data vector
  array<int, 2> UNSAFE_get_stride() {return stride; }
  array<int, 2> UNSAFE_get_dimensions() { return dimensions; }
  data_vector UNSAFE_get_data() { return data; }

  //SAFE METHODS
  //Safe methods should be able to be freely used without editing class attributes
  //Some safe methods return copies instead of mutating underlying data
  void print(int precision = 2, int width = 5) {
    // idk we change defaults for the width that feel right
    int column_index;
    array<int, 2> temp_stride = get_const_stride();
    array<int, 2> temp_dimensions = get_const_dimensions();
    int temp_data_size = get_const_size();
    cout << std::fixed << std::setprecision(precision);
    cout << "[";
    for (int row_index = 0; row_index <= temp_dimensions[0] - 1; row_index++) {
      if (row_index != 0) {
        cout << " ";
      }
      cout << "[";
      cout << std::setw(width);
      for (int column_index = 0; column_index <= temp_dimensions[1] - 1;
           column_index += temp_stride[1]) {
        int abs_index = row_index * temp_stride[0] + column_index;
        auto output = get_const_data()[abs_index];
        cout << output;
        if (abs_index != temp_data_size - 1 && column_index != temp_dimensions[1] - 1) {
          cout << ", ";
        }
        if (row_index > 1000) {
          cout << "...";
          cout << "]]";
          cout << "\n";
          return void();
        }
      }
      cout << "]";
      if (row_index * temp_stride[0] < temp_data_size - temp_stride[0]) {
        cout << ",";
        cout << "\n";
      }
    }
    cout << "]";
    cout << "\n";
    return void();
  }
  void shape() {
    cout << "row x cols: " << dimensions[0] << " " << dimensions[1] << '\n';
  }
  int absolute_index(int i, int j) {
    int absolute_index = i * get_const_stride()[0] + j;
    return absolute_index;
  }

  //this transpose function returns a copy, and so should not be unsafe
  lokitrix<T> transpose() {
    array<int, 2> new_dimensions;
    array<int, 2> old_dimensions = this.get_const_dimensions();
    new_dimensions[0] = old_dimensions[1];
    new_dimensions[1] = old_dimensions[0];
    //this should deep copy the data
    vector<T> new_data = this.get_const_data();
    return lokitrix(new_data, new_dimensions);
  }
  
  //UNSAFE METHODS
  //Unsafe methods mutate the underlying data, because the user is trying to do something
  //that they know they want to mutate the underlying data.
  //While unsafe methods violate class invariance by mutating underlying data, should still
  //avoid mutating the data vector pointer unless there is reason to do so. Methods like
  //transpose should be tested so they are safe for users
  void UNSAFE_mut_transpose() {
    array<int, 2> temp_dimensions = this.UNSAFE_get_dimensions();
    array<int, 2> temp_stride = this.UNSAFE_get_stride();
    int temp2 = temp_dimensions[0];
    temp_dimensions[0] = temp_dimensions[1];
    temp_dimensions[1] = temp2;
    temp_stride[0] = data_size / temp_dimensions[0];
  }

private:
  size_type data_size;
  data_vector data;
  void create(data_vector, array<int, 2>);
  void add_rows(lokitrix);
  void delete_lokitrix();
  array<int, 2> dimensions;
  // stride is the amount to move to format columns and rows, can simply
  // swap stride around to go from row major to column major
  array<int, 2> stride;
};

//CREATOR FUNCTIONS
template <class T>
void lokitrix<T>::create(data_vector input, array<int, 2> input_dimensions) {
  //This should deep copy the data, just a note
  data = input;
  //This should shallow copy which doesn't matter if type is int
  dimensions = input_dimensions;

  stride[0] = (data_size / input_dimensions[0]);
  stride[1] = 1;
  if (input.size() % dimensions[1] != 0 ||
      dimensions[0] != data_size / dimensions[1]) {
    throw invalid_argument("Given data size and dimensions don't match");
  }
}

//literally the same just you can preallocate a really large matrix if you know the size in advance
template <class T>
void lokitrix<T>::create(data_vector input, array<int, 2> input_dimensions, size_type size) {
  //This should deep copy the data, just a note
  data = input;
  data.reserve(size);
  //This should shallow copy which doesn't matter if type is int
  dimensions = input_dimensions;

  stride[0] = (data_size / input_dimensions[0]);
  stride[1] = 1;
  if (input.size() % dimensions[1] != 0 ||
      dimensions[0] != data_size / dimensions[1]) {
    throw invalid_argument("Given data size and dimensions don't match");
  }
}

template <class T>
void lokitrix<T>::create(size_type size, T value, array<int, 2> input_dimensions) {
  //This should deep copy the data, just a note
  vector data;
  data.reserve(size);
  data.fill(value);
  //This should shallow copy which doesn't matter if type is int
  dimensions = input_dimensions;

  stride[0] = (data_size / input_dimensions[0]);
  stride[1] = 1;
  if (input.size() % dimensions[1] != 0 ||
      dimensions[0] != data_size / dimensions[1]) {
    throw invalid_argument("Given data size and dimensions don't match");
  }
}

template <class T>
void lokitrix<T>::create(size_type size, array<int, 2> input_dimensions) {
  //This should deep copy the data, just a note
  vector data;
  data.reserve(size);
  //This should shallow copy which doesn't matter if type is int
  dimensions = input_dimensions;

  stride[0] = (data_size / input_dimensions[0]);
  stride[1] = 1;
  if (input.size() % dimensions[1] != 0 ||
      dimensions[0] != data_size / dimensions[1]) {
    throw invalid_argument("Given data size and dimensions don't match");
  }
}

template <class T>
void lokitrix<T>::delete_lokitrix()
{
  if (data) {
    delete(data);
    delete(dimensions);
    delete(stride);
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
        new_cell_value = mat1.get_data()[mat1_abs_index] * mat2.get_data()[mat2_abs_index];
        output_data.get_data()[mat4_abs_index] = new_cell_value;
      }
    }
  }
  return output_data;
}

//CONVENIENCE FUNCTIONS
//Functions to create matrices. Right now just get 0s and identity
lokitrix<T> identity(typename T, array<int, 2> dimensions) {
	if (dimensions[0] != dimensions[1]) {
		throw invalid_argument("Can only make identity matrix for square matrix");
	}
	vector<T> data;
	int size = dimensions[0] * dimensions[1];
	data.reserve(size);
	data.fill(0);
	for (i = 0; i < diemnsions[0]; i++) {
		data[i]
	}
}

int main() {
  vector<float> data1 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
  vector<float> data2 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
  array<int, 2> dims = {4, 3};
  lokitrix mat1(data1, dims);
  lokitrix mat2(data2, dims);
  mat1.print();
  cout << mat1.absolute_index(1, 2) << "\n";
  mat1.transpose();
  mat1.print();
  lokitrix mat3 = CPUMatMul(mat1, mat2);
  mat3.print();
}
