namespace GeoGen.Studio.Utilities
{
	using System.ComponentModel;

	using GeoGen.Studio.Utilities.Testing;

	using NUnit.Framework;

	// ReSharper disable InconsistentNaming

	/// <summary>
	/// Tests for the <see cref="ObservableObject"/> class.
	/// </summary>
	[TestFixture]
	public class ObservableObjectTests
	{
		/// <summary>
		/// Tests that <see cref="ObservableObject.PropertyChanged"/> fires (with correct property name).
		/// </summary>
		[Test]
		public void OnPropertyChanged_SomeProperty_PropertyChangedFires()
		{
			var testingObject = new TestingObservableObject();

			bool fired = false;
			testingObject.PropertyChanged += delegate(object sender, PropertyChangedEventArgs args)
			{
				if (args.PropertyName == "SomeProperty")
				{
					fired = true;
				}
			};

			testingObject.SomeProperty = 13;

			Assert.IsTrue(fired);
		}
	}
}
