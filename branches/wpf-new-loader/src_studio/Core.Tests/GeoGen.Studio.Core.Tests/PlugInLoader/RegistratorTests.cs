namespace GeoGen.Studio.PlugInLoader
{
    using System;
    using System.Linq;

    using GeoGen.Studio.PlugInLoader.Testing;

    using NUnit.Framework;

    using Rhino.Mocks;

    // ReSharper disable InconsistentNaming

    /// <summary>
    /// Tests for the <see cref="Registrator"/> class.
    /// </summary>
    [TestFixture]
    public class RegistratorTests
    {
        // These can't be handled via Action.
        public delegate void RefParameterTestDelegate(ref int a);

        public delegate void OutParameterTestDelegate(out int a);

        /// <summary>
        /// Tests that contructor throws an exception on a method with parameter not implementing <see cref="IPlugInInterface"/>.
        /// </summary>
        [Test]
        [ExpectedException(typeof(LoaderException))]
        public void Ctor_NonPlugInMethodParameter_ThrowsException()
        {
            Action<string> method = delegate { };

            new Registrator(method.Method);
        }        

        /// <summary>
        /// Tests that contructor throws an exception on a method with ref parameter.
        /// </summary>
        [Test]
        [ExpectedException(typeof(LoaderException))]
        public void Ctor_RefMethodParameter_ThrowsException()
        {
            RefParameterTestDelegate method = delegate { };

            new Registrator(method.Method);
        }

        /// <summary>
        /// Tests that contructor throws an exception on a method with out parameter.
        /// </summary>
        [Test]
        [ExpectedException(typeof(LoaderException))]
        public void Ctor_OutMethodParameter_ThrowsException()
        {
            OutParameterTestDelegate method = delegate(out int a) { a = 1; };

            new Registrator(method.Method);
        }

        /// <summary>
        /// Tests that contructor throws an exception on a method with parameter which implements <see cref="IPlugInInterface"/>, but is not a plug-in itself.
        /// </summary>
        [Test]
        [ExpectedException(typeof(LoaderException))]
        public void Ctor_NonInterfaceParameter_ThrowsException()
        {
            Action<SimpleTestingPlugIn> method = delegate { };

            new Registrator(method.Method);
        }

        /// <summary>
        /// Tests that contructor throws an exception on a method with parameter which is just <see cref="IPlugInInterface"/>.
        /// </summary>
        [Test]
        [ExpectedException(typeof(LoaderException))]
        public void Ctor_IPlugInInterfaceParameter_ThrowsException()
        {
            Action<IPlugInInterface> method = delegate { };

            new Registrator(method.Method);
        }

        /// <summary>
        /// Tests that contructor throws an exception on a method with two parameters implementing the same interface.
        /// </summary>
        [Test]
        [ExpectedException(typeof(LoaderException))]
        public void Ctor_RepeatedInterfaceParameter_ThrowsException()
        {
            Action<ITestingPlugInInterface1, ITestingPlugInInterface1> method = delegate { };

            new Registrator(method.Method);
        }
        
        /// <summary>
        /// Tests that Invoke invokes the registrator method.
        /// </summary>
        [Test]
        public void Invoke_ValidMethod_Invokes()
        {
            var instance = MockRepository.GenerateMock<ITestingPlugInInterface1>();
            bool executed = false;
            Action<ITestingPlugInInterface1> method = delegate(ITestingPlugInInterface1 a)
            {
                if (a == instance)
                {
                    executed = true;
                }
            };

            var registrator = new Registrator(method.Method);

            registrator.Invoke(method.Target, new object[] { instance });

            Assert.IsTrue(executed);
        }

        /// <summary>
        /// Tests that IsRootRegistrator returns true on parameter-less registrator.
        /// </summary>
        [Test]
        public void IsRootRegistrator_RootRegistrator_ReturnsTrue()
        {
            Action method = delegate { };

            var registrator = new Registrator(method.Method);

            Assert.IsTrue(registrator.IsRootRegistrator);
        }

        /// <summary>
        /// Tests that IsRootRegistrator returns true on parameterized registrator.
        /// </summary>
        [Test]
        public void IsRootRegistrator_NonRootRegistrator_ReturnsFalse()
        {
            Action<ITestingPlugInInterface1> method = delegate { };

            var registrator = new Registrator(method.Method);

            Assert.IsFalse(registrator.IsRootRegistrator);
        }

        /// <summary>
        /// Tests that DependsOn correctly contains the method's parameters.
        /// </summary>
        [Test]
        public void DependsOn_Parameters_Correct()
        {
            Action<ITestingPlugInInterface1, ITestingPlugInInterface2> method = delegate { };

            var registrator = new Registrator(method.Method);

            Assert.AreEqual(typeof(ITestingPlugInInterface2), registrator.DependsOn.ToArray()[1]);
        }

        /// <summary>
        /// Tests that ImplementedInterfaces correctly detects implemented plug-in interface.
        /// </summary>
        [Test]
        public void ImplementedInterfaces_SimplePlugIn_FindsPlugInInterface()
        {
            var method = typeof(SimpleTestingPlugIn).GetMethod("Register");

            var registrator = new Registrator(method);

            Assert.Contains(typeof(ITestingPlugInInterface1), registrator.ImplementedInterfaces.ToList());
        }

        /// <summary>
        /// Tests that ImplementedInterfaces correctly ignores non-plug-in interface.
        /// </summary>
        [Test]
        public void ImplementedInterfaces_SimplePlugIn_IgnoresNonPlugInInterface()
        {
            var method = typeof(SimpleTestingPlugIn).GetMethod("Register");

            var registrator = new Registrator(method);
            
            /* IPlugInInterface and ITestingPlugInInterface1. */
            Assert.AreEqual(2, registrator.ImplementedInterfaces.Count());
        }

        /// <summary>
        /// Tests that ToString provides correct text representation of the registrator.
        /// </summary>
        [Test]
        public void ToString_Registrator_Correct()
        {
            var method = typeof(SimpleTestingPlugIn).GetMethod("Register");

            var registrator = new Registrator(method);

            Assert.AreEqual("SimpleTestingPlugIn.Register(ITestingPlugInInterface2)", registrator.ToString());
        }
    }
}