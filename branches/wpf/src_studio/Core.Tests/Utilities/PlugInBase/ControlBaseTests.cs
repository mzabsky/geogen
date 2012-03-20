namespace GeoGen.Studio.Utilities.PlugInBase
{
	using System;
	using System.ComponentModel;

	using GeoGen.Studio.Utilities.PlugInBase.Testing;

	using NUnit.Framework;

	// ReSharper disable InconsistentNaming

	/// <summary>
	/// Tests for the <see cref="ControlBase"/> class.
	/// </summary>
	[TestFixture]
	public class ControlBaseTests
	{
		/// <summary>
		/// Tests that <see cref="ControlBase.Control"/> returns itself.
		/// </summary>
		[Test]
		[STAThread]
		public void Control_ReturnsItself()
		{
			var testingObject = new TestingControlBase();

			Assert.AreSame(testingObject, testingObject.Control);
		}
		
		/// <summary>
		/// Tests that <see cref="ControlBase.PropertyChanged"/> fires (with correct property name).
		/// </summary>
		[Test]
		[STAThread]
		public void OnPropertyChanged_SomeProperty_PropertyChangedFires()
		{
			var testingObject = new TestingControlBase();

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
