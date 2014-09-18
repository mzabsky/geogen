/// Declares the height map class.
class HeightMap
{
public:
	/// Creates a flat height map.
	/// @param height The height.
	/// @return A HeightMap.
	static HeightMap Flat(Number height);

	HeightMap Add(Number/HeightMap addend);
};