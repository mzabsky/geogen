namespace GeoGen.Studio.Utilities.Testing
{
	/// <summary>
	/// Helper class for <see cref="ObservableObjectTests"/>.
	/// </summary>
	public class TestingObservableObject : ObservableObject
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
