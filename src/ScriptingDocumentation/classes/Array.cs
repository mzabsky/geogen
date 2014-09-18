/// @brief Ordered map associating values to keys.
/// It can be used to emulate many data structures commonly used in other programming languages – lists/vectors, hash tables, dictionaries, stacks, queues and can also be used to emulate structures.
/// 
/// A new array can be created by calling Array.Empty. Operator [] can then be used to read and write items into/from an array.
/// 
/// @code{.cs}
/// var array = Array.Empty();
/// array["hello"] = "world";
/// var value = array["hello"]; // value is now "hello"
/// @endcode
/// 
/// Additionally, an array can be created using the array literal notation:
///
/// @code{.cs}
/// var array = { "hello": "world" };
/// var value = array["hello"]; // value is now "hello"
/// @endcode
/// 
/// An attempt to read from a key that is not present in the array will result in a runtime error.
/// 
/// Key in an array can be any object. Value in an array can also be any object.
/// 
/// Operator [] can also be used to add items into an array without specifying the key. In such case a number from integer sequence starting at 0 will be used (the key for the newly added value will be the greatest integer key ever added into that particular array plus one).
/// 
/// @code{.cs}
/// var array = Array.Empty();
/// array[] = "first"; // added with key 0
/// array[] = "second"; // added with key 1
/// array[5] = "third"; // added with key 5
/// array[] = "fourth"; // added with key 6
/// @endcode
///         
/// The array literal can also be used without specifying the key. Following code will result in the same array as the above example:
/// @code{.cs}
/// var array = {"first", "second", 5: "third", "fourth"};
/// @endcode
/// 
/// The array is ordered by the order in which the items were added into it (this becomes relevant with functions like Array.Front).
/// 
/// Multidimensional arrays are not supported, however arrays can be arbitrarily nested:
/// 
/// @code{.cs}
/// var array = 
/// {
///     {"item 0 - 0", "item 0 - 1"}.
///     2: {"item 2 - 0", 3: "item 2 - 3"}
/// };
/// 
/// var value = array[2][0]; // value is now "item 2-0"
/// @endcode
/// @ingroup standard_library
class Array
{
private:
public:

    /// Creates an empty array.
    /// @return An empty array.
    static Array Empty();

    /// Gets the number of items in the array.
    /// @code{.cs}
    /// var array = {"a", "b"};
    /// var count = array.Count(); // count is now 2
    /// @endcode
    /// @return The number of items in the array.
    Number Count();

    /// Adds @a object to the end of the array. Its key will be the greatest integral key that was ever added to the array plus one (or 0 if the array is empty). This is the equivalent of using the [] operator without a key.
    /// @code{.cs}
    /// var array = Array.Empty();
    /// array.PushBack(1);
    /// array.PushBack(2);
    /// var value = array[1]; // value is now 2
    /// @endcode
    /// @param object The object.
    PushBack(Object object);

    /// Gets key of @a index-th key-value pair in the array. Triggers runtime error if the index is greater than or equal to size of the array.
    /// @code{.cs}
    /// var array = {"a": "valueA", "b": "valueB"};
    /// for(var i = 0; i < array.Count(); i++)
    /// {
    ///     var key = array.GetKeyByIndex(i);
    ///     Print("{0}th item has key {1} and value {2}", i, key, array[key]);
    /// }
    /// @endcode
    /// @param index Zero-based index.
    /// @return The key of @a index-th pair.
    Object GetKeyByIndex(Number index);

    /// Gets value of @a index-th key-value pair in the array. Triggers runtime error if the index is greater than or equal to size of the array.
    /// @code{.cs}
    /// var array = {"a": "valueA", "b": "valueB"};
    /// for(var i = 0; i < array.Count(); i++)
    /// {
    ///     Print("{0}th item has value {1}", i, array.GetValueByIndex(i););
    /// }
    /// @endcode
    /// @param index Zero-based index.
    /// @return The key of @a index-th pair.
    Object GetValueByIndex(Number index);

    /// Gets the first item in the array. Triggers runtime error if the array is empty.
    /// @code{.cs}
    /// var array = Array.Empty();
    /// array.PushBack(1);
    /// array.PushBack(2);
    /// var value = array.Front(); // value is now 1
    /// @endcode
    /// @return The item.
    Object Front();

    /// Gets the last item in the array. Triggers runtime error if the array is empty.
    /// @code{.cs}
    /// var array = Array.Empty();
    /// array.PushBack(1);
    /// array.PushBack(2);
    /// var value = array.Back(); // value is now 2
    /// @endcode
    /// @return The item.
    Object Back();

    /// Determines whether the array contains key-value pair with given @a key.
    /// @code{.cs}
    /// var array = {"a": "itemA", "b": "itemB"};
    /// var value = array.ContainsKey("a"); // value is now true
    /// @endcode
    /// @param key The key.
    /// @return True if the array contains @a key, false otherwise.
    Boolean ContainsKey(Object key);

    /// Determines whether the array contains key-value pair with given @a value.
    /// @code{.cs}
    /// var array = {"a": "itemA", "b": "itemB"};
    /// var value = array.ContainsValue("itemC"); // value is now false
    /// @endcode
    /// @param value The value.
    /// @return True if the array contains @a value, false otherwise.
    Boolean ContainsValue(Object value);

    /// Removes key-value pair with given @a key from the array. Triggers runtime error if the array doesn't contain @a key.
    /// @code{.cs}
    /// var array = {"a": "itemA", "b": "itemB"};
    /// array.RemoveKey("b"); // the array now contains only a single pair, "a": "itemA"
    /// @endcode.
    /// @param key The key.
    RemoveKey(Object key);

    /// Removes key-value pair with given @a value from the array. Does nothing if the array doesn't contain @a value.
    /// @code{.cs}
    /// var array = {"a": "itemA", "b": "itemB"};
    /// array.RemoveValue("itemA"); // the array now contains only a single pair, "b": "itemB"
    /// @endcode.
    /// @param value The value.
    RemoveValue(Object value);

    /// Orders the key-value pairs by the keys. Only changes the order of the pairs, not the pairs themselves.
    /// @code{.cs}
    /// var array = {"b": "itemB", "a": "itemA"};
    /// array.SortByKeys(); // the order is now "a", "b"
    /// @endcode.
    SortByKeys();

    /// Orders the key-value pairs by the values. Only changes the order of the pairs, not the pairs themselves.
    /// @code{.cs}
    /// var array = {"a": "itemB", "b": "itemA"};
    /// array.SortByValues(); // the order is now "b", "a"
    /// @endcode.
    SortByValues();

    /// Randomizes the order of the key-value pairs. Only changes the order of the pairs, not the pairs themselves.
    /// 
    /// @param randomSeed (optional) The random seed. If not specified, it is generated using the common random sequence.
    Shuffle(Number randomSeed);
};
