namespace GeoGen.Studio.Utilities
{
	using System;
	using System.Windows.Input;
	using System.Windows.Threading;

	using GeoGen.Studio.Utilities.Extensions;

	using NUnit.Framework;

	// ReSharper disable InconsistentNaming

	/// <summary>
	/// Tests for the <see cref="RelayCommand"/> class.
	/// </summary>
	[TestFixture]
	public class RelayCommandTests
	{
		/// <summary>
		/// Tests that constructor throws <see cref="ArgumentNullException"/> when it gets null execute.
		/// </summary>
		[Test]
		[ExpectedException(typeof(ArgumentNullException))]
		public void Construct_NullExecute_ThrowsException()
		{
			new RelayCommand(null);
		}
		
		/// <summary>
		/// Tests that execute executes and the parameter gets passed.
		/// </summary>
		[Test]
		public void Execute_Execute_Fires()
		{
			bool fired = false;
			object parameter = new object();
			var command = new RelayCommand(delegate(object p)
			{
				if (p == parameter)
				{
					fired = true;	
				}				
			});

			command.Execute(parameter);

			Assert.IsTrue(fired);
		}

		/// <summary>
		/// Tests that <see cref="RelayCommand.CanExecuteChanged"/> fires after it was added .
		/// </summary>
		[Test]
		public void Constructor_CanExecuteChanged_Fires()
		{
			bool fired = false;
			object parameter = new object();
			var command = new RelayCommand(delegate{}, delegate(object p)
			{
				if (p == parameter)
				{
					fired = true;
				}
				return true;
			});

			command.CanExecute(parameter);

			Assert.IsTrue(fired);
		}

		/// <summary>
		/// Tests that <see cref="RelayCommand.CanExecuteChanged"/> handler gets fired by the <see cref="CommandManager"/> requery.
		/// </summary>
		[Test]
		public void CanExecuteChanged_CanExecuteChanged_Fires()
		{			
			var command = new RelayCommand(delegate { });

			bool fired = false;
			command.CanExecuteChanged += delegate
			{
				fired = true;
			};

			// Make WPF command manager call the handler
			CommandManager.InvalidateRequerySuggested();

			// Flush the requery from the dispatcher queue
			Dispatcher.CurrentDispatcher.DoEvents();

			Assert.IsTrue(fired);
		}

		/// <summary>
		/// Tests that <see cref="RelayCommand.CanExecuteChanged"/> handler doesn't get fired by the <see cref="CommandManager"/> requery after it was removed.
		/// </summary>
		[Test]
		public void CanExecuteChanged_RemovedCanExecuteChanged_DoesntFire()
		{
			var command = new RelayCommand(delegate { });

			bool fired = false;
			EventHandler handler = delegate
			{
				fired = true;
			};

			command.CanExecuteChanged += handler;
			command.CanExecuteChanged -= handler;

			// Make WPF command manager call the handler
			CommandManager.InvalidateRequerySuggested();

			// Flush the requery from the dispatcher queue
			Dispatcher.CurrentDispatcher.DoEvents();

			Assert.IsFalse(fired);
		}
	}
}
