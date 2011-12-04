using System;
using System.IO;
using Microsoft.Win32;

namespace GeoGen.Studio.Utilities.IO
{
	/// <summary>
	/// Provides easy access to open and save file dialogs.
	/// </summary>
	public static class FileDialog
	{
		private static string ShowBase(string path, string filter, bool isSaveDialog)
		{
			Microsoft.Win32.FileDialog dialog;

			if (isSaveDialog) dialog = new SaveFileDialog();
			else dialog = new OpenFileDialog();

			dialog.Filter = filter;

			// add Examples shortcut, if the directory exists
			/*if (Directory.Exists(@"../examples"))
			{
				dialog.CustomPlaces.Add(new FileDialogCustomPlace(@"../examples"));
			}*/

			try
			{
				FileInfo info = new FileInfo(path);
				dialog.InitialDirectory = info.DirectoryName;
				dialog.FileName = info.Name;
			}
			// path  is incorrect -> let the dialog reset on defaults
			catch { };

			bool? result = dialog.ShowDialog();

			if (result == true)
			{
				path = dialog.FileName;

				return path;
			}

			// user pressed cancel
			throw new OperationCanceledException(@"User closed the dialog by pressing ""Cancel"".");
		}

		/// <summary>
		/// Shows the Open File dialog.
		/// </summary>
		/// <param name="path">The initial path.</param>
		/// <param name="filter">File type filter.</param>
		/// <returns>Selected file.</returns>
		/// <exception cref="Exception">Thrown when the user presses "Cancel" button in the dialog.</exception>
		public static string ShowOpen(string path, string filter)
		{
			return FileDialog.ShowBase(path, filter, false);
		}

		/// <summary>
		/// Shows the Save File dialog.
		/// </summary>
		/// <param name="path">The initial path.</param>
		/// <param name="filter">File type filter.</param>
		/// <returns>Selected file.</returns>
		/// <exception cref="Exception">Thrown when the user presses "Cancel" button in the dialog.</exception>
		public static string ShowSave(string path, string filter)
		{
			return FileDialog.ShowBase(path, filter, true);
		}
	}
}
