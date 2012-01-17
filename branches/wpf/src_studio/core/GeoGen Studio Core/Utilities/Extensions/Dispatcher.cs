namespace GeoGen.Studio.Utilities.Extensions
{
    using System.Windows.Threading;

    /// <summary>
    /// Contains extension methods for <see cref="Dispatcher"/>.
    /// </summary>
    static class DispatcherExtensions
    {
        /// <summary>
        /// Forces the <see cref="Dispatcher"/> to immediately handle all scheduled requests.
        /// </summary>
        /// <param name="dispatcher">The dispatcher.</param>
        public static void DoEvents(this Dispatcher dispatcher){
            var frame = new DispatcherFrame();

            dispatcher.BeginInvoke(
                DispatcherPriority.Background,
                new DispatcherOperationCallback(delegate(object o) {
                    ((DispatcherFrame)o).Continue = false;
                    return null;
                }),
                frame);

            Dispatcher.PushFrame(frame);
        }
    }
}
