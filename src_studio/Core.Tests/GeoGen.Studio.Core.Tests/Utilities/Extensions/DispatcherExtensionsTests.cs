namespace GeoGen.Studio.Utilities.Extensions
{
    using System;
    using System.Collections.Generic;
    using System.Linq;
    using System.Text;
    using System.Windows.Threading;

    using NUnit.Framework;

    // ReSharper disable InconsistentNaming

    /// <summary>
    /// Tests for the <see cref="DispatcherExtensions"/>.
    /// </summary>
    public class DispatcherExtensionsTests
    {
        /// <summary>
        /// Tests that <see cref="DispatcherExtensions.DoEvents"/> forces invoked call to execute.
        /// </summary>
        [Test]
        public void DoEvents_InvokedCall_Executes()
        {
            var dispatcher = Dispatcher.CurrentDispatcher;

            bool executed = false;
            dispatcher.Invoke(DispatcherPriority.Background, (Action)delegate
            {
                executed = true;
            });

            dispatcher.DoEvents();

            Assert.IsTrue(executed);
        }
    }
}
