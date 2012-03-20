namespace GeoGen.Studio.Utilities.PlugInBase
{
	using System;
	using System.ComponentModel;

	using GeoGen.Studio.PlugIns.Interfaces;
	using GeoGen.Studio.Utilities.PlugInBase.Testing;

	using NUnit.Framework;

	// ReSharper disable InconsistentNaming

	/// <summary>
	/// Tests for the <see cref="WindowBase"/> class.
	/// </summary>
	[TestFixture]
	public class WindowBaseTests
	{
		/// <summary>
		/// Tests that <see cref="WindowBase.Window"/> returns itself.
		/// </summary>
		[Test]
		[STAThread]
		public void Window_ReturnsItself()
		{
			var testingObject = new TestingWindowBase();

			Assert.AreSame(testingObject, testingObject.Window);
		}

		/// <summary>
		/// Tests that <see cref="ControlBase.Control"/> returns itself.
		/// </summary>
		[Test]
		[STAThread]
		public void Control_ReturnsItself()
		{
			var testingObject = new TestingWindowBase();

			Assert.AreSame(testingObject, ((IControl)testingObject).Control);
		}

		/// <summary>
		/// Tests that <see cref="WindowBase.PropertyChanged"/> fires (with correct property name).
		/// </summary>
		[Test]
		[STAThread]
		public void OnPropertyChanged_SomeProperty_PropertyChangedFires()
		{
			var testingObject = new TestingWindowBase();

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
