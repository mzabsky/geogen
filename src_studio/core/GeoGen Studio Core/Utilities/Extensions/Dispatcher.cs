namespace GeoGen.Studio.Utilities.Extensions
{
    using System.Windows.Threading;

    static class DispatcherExtensions
    {
        public static void DoEvents(this Dispatcher dispatcher){
            DispatcherFrame frame = new DispatcherFrame();

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
