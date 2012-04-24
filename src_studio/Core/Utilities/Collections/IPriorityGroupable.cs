namespace GeoGen.Studio.Utilities.Collections
{
    public interface IPriorityGroupable : IPriority
    {
        string Group { get; }
    }
}
