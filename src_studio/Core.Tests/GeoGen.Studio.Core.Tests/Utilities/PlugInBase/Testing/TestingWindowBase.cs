namespace GeoGen.Studio.Utilities.PlugInBase.Testing
{
	using GeoGen.Studio.Utilities.PlugInBase;

	/// <summary>
	/// Helper class for <see cref="WindowBaseTests"/>.
	/// </summary>
	public class TestingWindowBase : WindowBase
	{
		private int someProperty;

		public int SomeProperty
		{
			get
			{
				return someProperty;
			}

			set
			{
				this.someProperty = value;
				this.OnPropertyChanged("SomeProperty");
			}
		}
	}
}
