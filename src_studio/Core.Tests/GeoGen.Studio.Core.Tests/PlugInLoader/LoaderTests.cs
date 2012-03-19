namespace GeoGen.Studio.PlugInLoader
{
    using System.Collections.Generic;
    using System.Linq;

    using GeoGen.Studio.PlugInLoader.Testing;
    using GeoGen.Studio.PlugInLoader;

    using NUnit.Framework;

    using Rhino.Mocks;

    // ReSharper disable InconsistentNaming

    /// <summary>
    /// Tests for the <see cref="Loader"/>.
    /// </summary>
    [TestFixture]
    public class LoaderTests
    {
        /// <summary>
        /// Tests that there are no plug-ins in clean <see cref="Loader"/>.
        /// </summary>
        [Test]
        public void PlugIns_Clean_Empty()
        {
            var loader = new Loader();
            
            Assert.IsFalse(loader.PlugIns.Any());
        }

        /// <summary>
        /// Tests that plug-in found with a locator is correctly added into the <see cref="Loader"/>.
        /// </summary>
        [Test]
        public void AddPlugIn_ValidLocator_AddsInstance()
        {
            var plugIn = new PlugIn(typeof(EmptyTestingPlugIn), new EmptyTestingRegistratorLocator());

            var plugIns = new List<PlugIn> { plugIn };

            var plugInLocator = MockRepository.GenerateStub<IPlugInLocator>();
            plugInLocator.Stub(p => p.GetPlugIns()).Return(plugIns);

            var loader = new Loader();
            loader.AddPlugIns(plugInLocator);

            bool result = loader.PlugIns.Contains(plugIn) && loader.PlugIns.Count() == 1;
            Assert.IsTrue(result);
        }

        /// <summary>
        /// Tests that GetInstancesOfTypes with no instaces returns no instances on a signature.
        /// </summary>
        [Test]
        public void GetInstancesOfTypes_NoInstances_ReturnsEmptyCollection()
        {
            var loader = new Loader();

            var result = loader.GetInstancesOfTypes(new[] { typeof(int), typeof(string) });
            Assert.IsFalse(result.Any());
        }

        /// <summary>
        /// Tests that GetInstancesOfTypes with no instaces returns no instances on a signature.
        /// </summary>
        [Test]
        public void GetInstancesOfTypes_OneInstanceSimpleSignature_ReturnsOneSet()
        {
            var plugIn = new PlugIn(typeof(EmptyTestingPlugIn), new EmptyTestingRegistratorLocator());

            var plugIns = new List<PlugIn> { plugIn };

            var plugInLocator = MockRepository.GenerateStub<IPlugInLocator>();
            plugInLocator.Stub(p => p.GetPlugIns()).Return(plugIns);
            
            var loader = new Loader();
            loader.AddPlugIns(plugInLocator);

            var result = loader.GetInstancesOfTypes(new[] { typeof(EmptyTestingPlugIn)});

            Assert.That(result, Is.EquivalentTo(new[] { new[] { loader.Instances.First() } }));
        }

        /// <summary>
        /// Tests that GetInstancesOfTypes with three instaces of types A, A and B returns two correct sets.
        /// </summary>
        [Test]
        public void GetInstancesOfTypes_TwoInstancesWithOneSharedType_ReturnsTwoPlugInsOfCorrectTypes()
        {
            var plugIn1 = new PlugIn(typeof(EmptyTestingPlugIn), new EmptyTestingRegistratorLocator());
            var plugIn2 = new PlugIn(typeof(SimpleTestingPlugIn), new EmptyTestingRegistratorLocator());

            var plugIns = new List<PlugIn> { plugIn1, plugIn2 };

            var plugInLocator = MockRepository.GenerateStub<IPlugInLocator>();
            plugInLocator.Stub(p => p.GetPlugIns()).Return(plugIns);

            var loader = new Loader();
            loader.AddPlugIns(plugInLocator);

            var result = loader.GetInstancesOfTypes(new[] { typeof(EmptyTestingPlugIn), typeof(SimpleTestingPlugIn) });

            var resultTypes = 
                from paramSet in result
                select (
                    from oneParam in paramSet
                    select oneParam.GetType());
                
            var correctTypes = new[]
                {
                    new[]
                        {
                            typeof(EmptyTestingPlugIn), typeof(SimpleTestingPlugIn)
                        },
                    new[]
                        {
                            typeof(SimpleTestingPlugIn), typeof(SimpleTestingPlugIn)
                        } 
                };

            Assert.That(resultTypes, Is.EquivalentTo(correctTypes));
        }

        /// <summary>
        /// Tests that plug-in found with a locator has its instance correctly created.
        /// </summary>
        [Test]
        public void AddPlugIn_ValidPlugIn_CreatesInstance()
        {
            var plugIn = new PlugIn(typeof(EmptyTestingPlugIn), new EmptyTestingRegistratorLocator());

            var plugIns = new List<PlugIn> { plugIn };

            var plugInLocator = MockRepository.GenerateStub<IPlugInLocator>();
            plugInLocator.Stub(p => p.GetPlugIns()).Return(plugIns);

            var loader = new Loader();
            loader.AddPlugIns(plugInLocator);

            bool result = loader.Instances.OfType<EmptyTestingPlugIn>().Count() == 1;
            Assert.IsTrue(result);
        }

        /// <summary>
        /// Tests that plug-in found with a constructor.
        /// </summary>
        [Test]
        public void AddPlugIn_PlugInWithThrowingCtor_ThrowsException()
        {
            var plugIn = new PlugIn(typeof(EmptyTestingPlugIn), new EmptyTestingRegistratorLocator());

            var plugIns = new List<PlugIn> { plugIn };

            var plugInLocator = MockRepository.GenerateStub<IPlugInLocator>();
            plugInLocator.Stub(p => p.GetPlugIns()).Return(plugIns);

            var loader = new Loader();
            loader.AddPlugIns(plugInLocator);

            bool result = loader.Instances.OfType<EmptyTestingPlugIn>().Count() == 1;
            Assert.IsTrue(result);
        }

        /// <summary>
        /// Tests that ExecuteRegistrators creates an isntance of the plug-in.
        /// </summary>
        [Test]
        public void ExecuteRegistrators_SingleSimpleRegistrator_Executes()
        {
            /*var registratorLocator = MockRepository.GenerateMock<IRegistratorLocator>();
              registratorLocator.Stub(p => p.GetRegistrators(typeof()))*/
            
            var plugIn = new PlugIn(typeof(SimpleTestingPlugIn), new ReflectionRegistratorLocator());

            var plugIns = new List<PlugIn> { plugIn };

            var plugInLocator = MockRepository.GenerateStub<IPlugInLocator>();
            plugInLocator.Stub(p => p.GetPlugIns()).Return(plugIns);

            var loader = new Loader();
            loader.AddPlugIns(plugInLocator);

            loader.ExecuteRegistrators();

            Assert.IsTrue(plugIn.Instances.Cast<SimpleTestingPlugIn>().First().RegistratorExecuted);
        }
    }
}
