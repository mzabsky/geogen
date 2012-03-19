namespace GeoGen.Studio.PlugIns
{
	using System.ComponentModel;
	using System.Windows.Media;

	using GeoGen.Studio.Utilities.PlugInBase;

	public sealed class ScriptValidator : ObjectBase
	{
		private IGenerator generator;
		private IEditor editor;

		private bool isSyntaxCheckScheduled;

		public void Register(IGenerator generator, IEditor editor)
		{
			this.editor = editor;
			this.generator = generator;

			editor.PropertyChanged += delegate(object o, PropertyChangedEventArgs args)
			{
				if (args.PropertyName != "Text")
				{
					return;
				}

				this.ScheduleSyntaxCheck();
			};

			generator.Started += delegate
			{
				this.isSyntaxCheckScheduled = false;
			};

			generator.HeaderLoaded += delegate
			{
				// The header was executed successfuly - it is valid
				this.HideError();

				// ... and recheck if the script text changed in the meantime
				this.StartCheckIfScheduled();
			};

			generator.Finished += delegate
			{
				// The generator might have run for considerable amount of time, the script might
				// have to be rechecked.
				this.StartCheckIfScheduled();
			};

			generator.Failed += delegate(object o, GenerationFailedEventArgs args)
			{
				if (!args.IsHeaderLoaded)
				{
					this.DisplayError();
				}
			};
		}

		public void StartCheck(){
			this.generator.Start(this.editor.Text, true);
		}

		public void StartCheckIfScheduled()
		{
			if (!this.isSyntaxCheckScheduled)
			{
				return;
			}

			this.StartCheck();
		}

		public void ScheduleSyntaxCheck()
		{
			if (this.isSyntaxCheckScheduled)
			{
				// A check is already waiting to be performed.
				return;
			}
			else if (!this.generator.IsReady)
			{
				// The generator is not available right now, do it later.
				this.isSyntaxCheckScheduled = true;
			}
			else
			{
				// The generator is free to go without waiting.
				this.StartCheck();
			}
		}

		private void DisplayError()
		{
			this.editor.Control.BorderBrush = new SolidColorBrush(Colors.Red);
			this.editor.Control.BorderThickness = new System.Windows.Thickness(0, 0, 0, 5);
		}

		private void HideError()
		{
			this.editor.Control.BorderBrush = null;
			this.editor.Control.BorderThickness = new System.Windows.Thickness(0);
		}
	}
}
