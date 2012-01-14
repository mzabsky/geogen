namespace GeoGen.Studio.Utilities.PlugInBase.Testing
{
	using GeoGen.Studio.Utilities.PlugInBase;

	/// <summary>
	/// Helper class for <see cref="ControlBaseTests"/>.
	/// </summary>
	public class TestingControlBase : ControlBase
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
