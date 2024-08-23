pub struct Lokitrix {
    data: Vec<T>,
    stride: [i64; 2],
    dimensions: [i64; 2],
}
impl Lokitrix
{
    /// Create a Lokitrix by passing the data pointer and ownership of dimensions. This copies the dimensions
    /// array, but that's okay it's not that big. Note that if the input dimensions don't match up with the
    /// data size the created Lokitrix will have extra uninitialized values
    pub fn create(data: Vec<T>, dimensions: [i64; 2]) -> Result<Lokitrix, &'static str>
    {
        let size: i64 = dimensions[0] * dimensions[1];
        //Check that size is equal to or greater than size of data, if not throw an error
        if (data.size() > size)
        {
            return Err("Input data is larger than input size");
        }

        data.reserve(size)
        let stride: [i64, 2];
        stride[0] = (size / dimensions[0]);
        stride[1] = 1;


    }
    pub fn create_zeroes(dimensions: [i64, 2])
    {

    }
    pub fn create_identity(dimensions: [i64, 2])
    {

    }
    pub fn array(data: Vec<Vec<T>)
    {

    }
    pub fn transpose() -> Result<&'static str, &'static str>
    {
        new_dimensions: [i64, 2];
    }
}
#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn it_works() {
        let result = add(2, 2);
        assert_eq!(result, 4);
    }
}
