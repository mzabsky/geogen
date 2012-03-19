namespace GeoGen.Studio.Utilities.Collections
{
    using System;
    using System.Collections.Generic;

    /// <summary>
    /// Compares two object using a delegate.
    /// </summary>
    /// <typeparam name="T">Type of the compared objects.</typeparam>
    public class DelegateComparer<T> : IComparer<T>
    {
        private readonly Func<T, T, int> comparison;

        /// <summary>
        /// Initializes a new instance of the <see cref="DelegateComparer{T}"/> class.
        /// </summary>
        /// <param name="comparison">The comparison.</param>
        public DelegateComparer(Func<T, T, int> comparison)
        {
            this.comparison = comparison;
        }

        public int Compare(T x, T y)
        {
            return comparison(x, y);
        }
    }
}
