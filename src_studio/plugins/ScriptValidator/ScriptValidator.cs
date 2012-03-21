namespace GeoGen.Studio.PlugIns
{
    using System.ComponentModel;
    using System.Windows.Media;

    using GeoGen.Studio.Utilities.PlugInBase;

    /// <summary>
    /// Validates stript opened in <see cref="IEditor">code editor</see> in real-time by executing it in the <see cref="IGenerator">generator</see>
    /// whenever it changes.
    /// </summary>
    public sealed class ScriptValidator : ObjectBase
    {
        /// <summary>
        /// The generator.
        /// </summary>
        private IGenerator generator;

        /// <summary>
        /// The code editor.
        /// </summary>
        private IEditor editor;

        /// <summary>
        /// True if syntax check will be executed next time the generator is not busy.
        /// </summary>
        private bool isSyntaxCheckScheduled;

        /// <summary>
        /// Initializes code validation for given code editor with given generator.
        /// </summary>
        /// <param name="generator">The generator.</param>
        /// <param name="editor">The editor.</param>
        public void Register(IGenerator generator, IEditor editor)
        {
            this.editor = editor;
            this.generator = generator;

            // Validate the code every time it changes.
            editor.PropertyChanged += delegate(object o, PropertyChangedEventArgs args)
            {
                if (args.PropertyName != "Text")
                {
                    return;
                }

                this.ScheduleSyntaxCheck();
            };

            // Whenever a script was executed (reason doesn't matter), it can be used for validation.
            generator.Started += delegate
            {
                this.isSyntaxCheckScheduled = false;
            };

            // The script header is valid - the syntax error state can be removed.
            generator.HeaderLoaded += delegate
            {
                // The header was executed successfuly - it is valid
                this.HideError();

                // ... and recheck if the script text changed in the meantime
                this.StartCheckIfScheduled();
            };

            // The generator finished - look if there are any scheduled syntax checks and execute them.
            generator.Finished += delegate
            {
                // The generator might have run for considerable amount of time, the script might
                // have to be rechecked.
                this.StartCheckIfScheduled();
            };

            // If the generator failed while executing the header, announce the error.
            generator.Failed += delegate(object o, GenerationFailedEventArgs args)
            {
                if (!args.IsHeaderLoaded)
                {
                    this.DisplayError();
                }
            };
        }

        /// <summary>
        /// Validates current contents of the editor.
        /// </summary>
        public void StartCheck()
        {
            this.generator.Start(this.editor.Text, true);
        }

        /// <summary>
        /// Look if there is an check scheduled and execute it if there is one.
        /// </summary>
        public void StartCheckIfScheduled()
        {
            if (!this.isSyntaxCheckScheduled)
            {
                return;
            }

            this.StartCheck();
        }

        /// <summary>
        /// Executes a syntax check if the generator is not busy, otherwise schedules it for later.
        /// </summary>
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

        /// <summary>
        /// Displays an error to the user.
        /// </summary>
        private void DisplayError()
        {
            this.editor.Control.BorderBrush = new SolidColorBrush(Colors.Red);
            this.editor.Control.BorderThickness = new System.Windows.Thickness(0, 0, 0, 5);
        }

        /// <summary>
        /// Hides an error from the user (because presumably the script is now valid).
        /// </summary>
        private void HideError()
        {
            this.editor.Control.BorderBrush = null;
            this.editor.Control.BorderThickness = new System.Windows.Thickness(0);
        }
    }
}
