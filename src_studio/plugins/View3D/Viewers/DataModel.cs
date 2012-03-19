namespace GeoGen.Studio.PlugIns.Viewers
{
	using System;
	using System.Drawing;
	using System.IO;
	using System.Windows;
	using SlimDX;
	using SlimDX.Wpf;

	public class DataModel
	{
		private byte[] _buffer;
		private SlimDXControl control;

		public DataModel(SlimDXControl slimDXControl, string imageName, Color color)
		{
			control = slimDXControl;

			Name = imageName;
			Uri uri = new Uri("pack://application:,,,/GGenStudio.PlugIn.View3D;component/Images/" + imageName + ".png");

			using (Stream s = Application.GetResourceStream(uri).Stream)
			{
				_buffer = new byte[s.Length];
				s.Read(_buffer, 0, (int)s.Length);
			}

			Color = color;
		}

		public string Name
		{
			private set;
			get;
		}

		public Stream Stream
		{
			get
			{
				MemoryStream ms = new MemoryStream(_buffer);
				return ms;
			}
		}

		private Color color;
		public Color Color
		{
			get
			{
				return color;
			}
			set
			{
				if (color != value)
				{
					color = value;
					Color4 = new Color4(color);
					control.ForceRendering();

				}
			}
		}

		public Color4 Color4
		{
			get;
			private set;
		}
	}

}
