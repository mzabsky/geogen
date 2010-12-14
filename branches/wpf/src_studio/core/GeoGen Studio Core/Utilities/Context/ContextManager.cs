using System;
using System.Collections.ObjectModel;
using System.Collections.Generic;
using System.Linq;
using System.Windows.Threading;
using System.Windows;

namespace GeoGen.Studio.Utilities.Context
{
    /// <summary>
    /// Provides another mode of communication between independent plug-ins. Allows multiple listeners to enter or leave application-wide <see cref="Context">Contexts</see> and to query the presence of specific <see cref="Context"/> (given the caller knows the <see cref="Context"/>).
    /// </summary>
    public static class ContextManager
    {
        private static List<Context> contexts = new List<Context>();

        /// <summary>
        /// Occurs when the manager enter or leaves a <see cref="Context"/>.
        /// </summary>
        public static event EventHandler ContextChanged;

        /// <summary>
        /// Enters given <see cref="Context"/> (unless it already active).
        /// </summary>
        /// <param name="context">The <see cref="Context"/>.</param>
        /// <returns><c>true</c> if the <see cref="Context"/> was not active yet; otherwise <c>false</c>.</returns>
        public static bool EnterContext(Context context)
        {
            bool result;
            
            lock (ContextManager.contexts)
            {
                // Can't enter one context twice
                if (ContextManager.contexts.Contains(context))
                {
                    result = false;
                }
                else
                {
                    ContextManager.contexts.Add(context);
                    ContextManager.OnContextChanged();
                    result = true;
                }
            }

            if (result)
            {
                ContextManager.OnContextChanged();
            }

            return result;
        }

        /// <summary>
        /// Leaves given <see cref="Context"/>.
        /// </summary>
        /// <param name="context">The <see cref="Context"/>.</param>
        /// <returns><c>false</c> if the <see cref="Context"/> was not active; otherwise <c>true</c>.</returns>
        public static bool LeaveContext(Context context)
        {
            bool result;
            
            lock (ContextManager.contexts)
            {
                if (ContextManager.contexts.Contains(context))
                {
                    ContextManager.contexts.Remove(context);                    
                    result = true;
                }
                else
                {                    
                    // The manager was not in that context at all.
                    result = false;
                }
            }

            if(result)
            {
                ContextManager.OnContextChanged();
            }

            return result;
        }

        /// <summary>
        /// Gets a list of active <see cref="Context"/> (known to the caller).
        /// </summary>
        /// <param name="knownContexts">Known <see cref="Context">contexts</see>.</param>
        /// <returns>List of known active <see cref="Context">contexts</see>.</returns>
        public static IEnumerable<Context> GetKnownActiveContexts(IEnumerable<Context> knownContexts)
        {
            lock (ContextManager.contexts)
            {
                return Enumerable.Intersect(ContextManager.contexts as IEnumerable<Context>, knownContexts);
            }
        }

        /// <summary>
        /// Gets the most recently entered of active <see cref="Context">contexts</see> (known to the caller).
        /// </summary>
        /// <param name="knownContexts">Known <see cref="Context">contexts</see>.</param>
        /// <returns>List of known active <see cref="Context">contexts</see>.</returns>
        public static Context GetTopMostKnownActiveContext(IEnumerable<Context> knownContexts)
        {
            Context result;

            lock (ContextManager.contexts)
            {
                if (Enumerable.Any(ContextManager.contexts) && Enumerable.Any(knownContexts))
                {
                    result = Enumerable.Last(Enumerable.Intersect(ContextManager.contexts as IEnumerable<Context>, knownContexts));
                }
                else
                {
                    result = null;
                }
            }

            return result;
        }

        /// <summary>
        /// Determines whether the specified <see cref="Context"/> is active.
        /// </summary>
        /// <param name="context">The <see cref="Context"/>.</param>
        /// <returns>
        /// 	<c>true</c> if the <see cref="Context"/> is active; otherwise, <c>false</c>.
        /// </returns>
        public static bool IsContextActive(Context context)
        {
            lock (ContextManager.contexts){
                return ContextManager.contexts.Contains(context);
            }
        }        

        private static void OnContextChanged()
        {
            if(ContextManager.ContextChanged != null)
            {
                Application.Current.Dispatcher.Invoke(DispatcherPriority.Background, (Action)delegate()
                {
                    ContextManager.ContextChanged(null, new EventArgs());
                });
            }
        }
    }
}