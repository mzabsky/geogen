using GeoGen.Studio.Utilities.Collections;

namespace GeoGen.Studio.PlugIns
{
    public class MenuSeparator: IPriority
    {
        public double Priority { get; protected set; }

        public MenuSeparator(double priority = 0)
        {
            this.Priority = priority;
        }
    }
}
