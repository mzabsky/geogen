namespace GeoGen.Studio.Utilities.Collections
{
    using System;
    using System.Collections.Generic;

    /// <summary>
    /// Compares two object using a delegate.
    /// </summary>
    /// <typeparam name="T">Type of the compared objects.</typeparam>
    public class DelegateEqualityComparer<T> : IEqualityComparer<T>
    {
        private readonly Func<T, T, bool> comparison;

        /// <summary>
        /// Initializes a new instance of the <see cref="DelegateEqualityComparer{T}"/> class.
        /// </summary>
        /// <param name="comparison">The comparison.</param>
        public DelegateEqualityComparer(Func<T, T, bool> comparison)
        {
            this.comparison = comparison;
        }

        public bool Equals(T x, T y)
        {
            return comparison(x, y);
        }

        public int GetHashCode(T obj)
        {
            return obj.GetHashCode();
        }
    }
}
