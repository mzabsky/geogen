namespace GeoGen.Studio.PlugIns.Viewers
{
	using System;
	using SlimDX;
	using SlimDX.Direct3D9;
	using SlimDX.Wpf;

	public class RenderEngine : IRenderEngine
	{
		private Sprite sprite;
		private Texture texture;
		private DataModel dataModel;

		public Func<bool> EnableRenderCondition { get; set; }

		public RenderEngine(DataModel dataModel) :
			base()
		{
			this.dataModel = dataModel;
		}

		public void OnDeviceCreated(object sender, EventArgs e)
		{
			return;
		}

		public void OnDeviceDestroyed(object sender, EventArgs e)
		{
			if (sprite != null && !sprite.Disposed)
			{
				sprite.Dispose();
			}
			if (texture != null && !texture.Disposed)
			{
				texture.Dispose();
				texture = null;
			}
		}

		public void OnDeviceLost(object sender, EventArgs e)
		{
			if (sprite != null && !sprite.Disposed)
			{
				sprite.Dispose();
			}
			if (texture != null && !texture.Disposed)
			{
				texture.Dispose();
				texture = null;
			}
		}

		public void OnDeviceReset(object sender, EventArgs e)
		{
			SlimDXControl control = sender as SlimDXControl;
			if (control == null)
				throw new ArgumentNullException("sender");

			if (sprite != null)
			{
				sprite.Dispose();
			}
			sprite = new Sprite(control.Device);

			if (texture == null)
				texture = Texture.FromStream(control.Device, dataModel.Stream, Usage.None, Pool.Default);

			return;
		}

		public void OnMainLoop(object sender, EventArgs e)
		{
			if (this.EnableRenderCondition()) {
				sprite.Begin(SpriteFlags.AlphaBlend);
				sprite.Draw(texture, Vector3.Zero, Vector3.Zero, dataModel.Color4);
				sprite.End();
			}
		}
	}
}
