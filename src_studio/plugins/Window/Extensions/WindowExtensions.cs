namespace GeoGen.Studio.PlugIns.Extensions
{
	using System.Windows.Input;

	using GeoGen.Studio.PlugIns.Interfaces;

	/// <summary>
	/// Contains extension methods for IWindow interface.
	/// </summary>
	public static class WindowExtensions
	{
		/// <summary>
		/// Registers a window-wide <see cref="InputGesture"/>.
		/// </summary>
		/// <param name="window">The window.</param>
		/// <param name="gesture">The gesture.</param>
		/// <param name="command">The command.</param>
		public static void RegisterInputGesture(this IWindow window, InputGesture gesture, ICommand command)
		{
			var inputBinding = new InputBinding(command, gesture);
			window.Window.InputBindings.Add(inputBinding);
		}
	}
}
