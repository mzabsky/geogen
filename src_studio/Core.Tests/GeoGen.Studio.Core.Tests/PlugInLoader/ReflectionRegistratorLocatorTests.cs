namespace GeoGen.Studio.PlugInLoader
{
	// ReSharper disable InconsistentNaming

	using System;

	using GeoGen.Studio.PlugInLoader.Testing;

	using NUnit.Framework;

	[TestFixture]
	public class ReflectionRegistratorLocatorTests
	{
		/// <summary>
		/// Tests if constructor correctly sets the path.
		/// </summary>
		[Test]
		public void Directory_String_Equals()
		{
			var locator = new ReflectionPlugInLocator("foo/bar");
			Assert.AreEqual(locator.Directory, "foo/bar");
		}

		/// <summary>
		/// Tests if a non-plug-in type is not reported as a plug-in.
		/// </summary>
		[Test]
		public void IsTypePlugIn_NotPlugIn_False()
		{
			var locator = new ReflectionPlugInLocator("");
			bool result = locator.IsTypePlugIn(typeof(Type));

			Assert.IsFalse(result);
		}

		/// <summary>
		/// Tests if a plug-in type is reported as a plug-in.
		/// </summary>
		[Test]
		public void IsTypePlugIn_PlugIn_True()
		{
			var locator = new ReflectionPlugInLocator("");
			bool result = locator.IsTypePlugIn(typeof(EmptyTestingPlugIn));

			Assert.IsTrue(result);
		}
	}
}
