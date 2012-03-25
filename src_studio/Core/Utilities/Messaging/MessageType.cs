namespace GeoGen.Studio.Utilities.Messaging
{
    /// <summary>
    /// Severity of a <see cref="Message"/>.
    /// </summary>
    public enum MessageType
    {
        /// <summary>
        /// Indicates a state that has negative impact.
        /// </summary>
        Error, 

        /// <summary>
        /// Indicates a state that could potentially have a negative impact.
        /// </summary>
        Warning,

        /// <summary>
        /// Information without any negative impact.
        /// </summary>
        Message,

        /// <summary>
        /// Information not significant enough for normal user.
        /// </summary>
        Trace
    }
}
