using System;
using System.Collections.Generic;
using System.Diagnostics.Contracts;
using System.Linq;
using System.Reflection;
using System.Collections.ObjectModel;
using System.IO;
using System.Windows;
using GeoGen.Studio.Utilities.Persistence;
using GeoGen.Studio.Utilities.Messaging;

namespace GeoGen.Studio.PlugInLoader
{

    /// <summary>
    /// Loader class detects, analyzes and loads plug-ins.
    /// </summary>
    public static class Loader
    {   
        #region Fields
        private static List<Registrator> rootRegistrators;
        private static Dictionary<Type, List<Registrator>> registratorsByInterface;
        private static Dictionary<Registrator, List<Registrator>> registratorsByParent;
        private static Dictionary<Type, List<Registrator>> registratorsByPlugInType;
        private static Dictionary<Type, List<object>> instancesByInterface;
        private static Dictionary<Type, List<object>> instancesByPlugInType;
        private static List<Registrator> orderedRegistrators;        
        private static readonly LoaderConfig config = new LoaderConfig();
        private static List<PlugIn> plugIns;        
        private static bool isFinished;
        #endregion

        /// <summary>
        /// Occurs when all plug-ins are loaded and the application is ready to use.
        /// </summary>
        public static event EventHandler Ready;

        #region Properties
        /// <summary>
        /// Indicates whether all registrators were already executed.
        /// </summary>
        /// <value>
        /// 	<c>true</c> if this instance is finished; otherwise, <c>false</c>.
        /// </value>
        public static bool IsFinished
        {
            get
            {
                return Loader.isFinished;
            }
            set
            {
                // Build the plug-in list if the loader is being flagged as finished.
                if(Loader.isFinished == false && value)
                {
                    Loader.BuildPlugInsList();
                }
                else if (Loader.isFinished && value == false)
                {
                    throw new InvalidOperationException("Loader can't be flagged as unfinished once it was flagged as finished.");
                }

                Loader.isFinished = value;
            }
        }

        /// <summary>
        /// List of all <see cref="Registrator">registrators</see> registered with <see cref="Loader"/>.
        /// </summary>
        /// <value>The <see cref="Registrator" /> list.</value>
        public static ObservableCollection<Registrator> Registrators { get; private set; }

        /// <summary>
        /// List of all plug-in instances registered with <see cref="Loader"/>.
        /// </summary>
        /// <value>The instances.</value>
        public static ObservableCollection<object> Instances { get; private set; }

        /// <summary>
        /// List of all <see cref="Registrator">registrators</see> registered with <see cref="Loader"/> which don't depend on any other plug-ins.
        /// </summary>
        /// <remarks>
        /// There must always be at least one root registrator during program star-up. The <see cref=" Loader"/> will have to terminate the program otherwise.
        /// </remarks>
        /// <value>The root <see cref="Registrator"/> list.</value>        
        public static List<Registrator> RootRegistrators
        {
            get
            {
                if (rootRegistrators == null)
                {
                    Loader.RebuildRootRegistratorIndex();
                }
                return rootRegistrators;
            }
            private set { rootRegistrators = value; }
        }

        /// <summary>
        /// Gets a list of plug-ins registered with this <see cref="Loader"/>.
        /// </summary>
        /// <value>The plug-ins.</value>
        public static IEnumerable<PlugIn> PlugIns
        {
            get
            {
                if(Loader.plugIns == null)
                {
                    throw new InvalidOperationException("Plug-in list is not readable until all plug-ins were registered.");
                }

                return Loader.plugIns;
            }
        }

        /// <summary>
        /// List of all <see cref="Registrator">Registrators</see> registered with <see cref="Loader"/> arranged in their final execution order.
        /// </summary>
        /// <remarks>
        /// <see cref="Registrator">Registrators</see> which failed earlier in the loading process will be ignored.
        /// </remarks>
        /// <value>The ordered <see cref="Registrator"/> list.</value>                
        public static List<Registrator> OrderedRegistrators
        {
            get
            {
                if (orderedRegistrators == null)
                {
                    Loader.OrderRegistrators();
                }
                return orderedRegistrators;
            }
        }

        private static Dictionary<Type, List<Registrator>> RegistratorsByInterface
        {
            get
            {
                if (Loader.registratorsByInterface == null)
                {
                    Loader.RebuildRegistratorsByInterfaceIndex();
                }
                return Loader.registratorsByInterface;
            }
        }

        private static Dictionary<Registrator, List<Registrator>> RegistratorsByParent
        {
            get
            {
                if (registratorsByParent == null)
                {
                    Loader.RebuildRegistratorsByParentIndex();
                }
                return Loader.registratorsByParent;
            }
            set { registratorsByParent = value; }
        }

        private static Dictionary<Type, List<Registrator>> RegistratorsByPlugInType
        {
            get
            {
                if (registratorsByPlugInType == null)
                {
                    Loader.RebuildRegistratorsByPlugInTypeIndex();
                }
                return Loader.registratorsByPlugInType;
            }
            set { registratorsByPlugInType = value; }
        }

        private static Dictionary<Type, List<object>> InstancesByInterface
        {
            get
            {
                if (instancesByInterface == null)
                {
                    Loader.RebuildInstancesByInterfaceIndex();
                }
                return Loader.instancesByInterface;
            }
            set { instancesByInterface = value; }
        }

        private static Dictionary<Type, List<object>> InstancesByPlugInType
        {
            get
            {
                if (instancesByPlugInType == null)
                {
                    Loader.RebuildInstancesByPlugInTypeIndex();
                }
                return Loader.instancesByPlugInType;
            }
            set { instancesByPlugInType = value; }
        }
        #endregion

        /// <summary>
        /// Initializes a new instance of the <see cref="Loader"/> class.
        /// </summary>
        static Loader(){
            MainConfig.Register(config);

            Loader.Registrators = new ObservableCollection<Registrator>();
            Loader.Instances = new ObservableCollection<object>();

            // Indexes are made obsolete every time the main collection changes.
            Loader.Registrators.CollectionChanged += delegate
            {
                Loader.ForceRegistratorIndexRebuild();
            };
            Loader.Instances.CollectionChanged += delegate
            {
                Loader.ForceInstanceIndexRebuild();
            };
        }

        private static void ForceRegistratorIndexRebuild()
        {
            Loader.rootRegistrators = null;
            Loader.registratorsByInterface = null;
            Loader.registratorsByParent = null;
            Loader.orderedRegistrators = null;
        }

        private static void ForceInstanceIndexRebuild()
        {
            Loader.InstancesByInterface = null;
            Loader.InstancesByPlugInType = null;
        }

        /// <summary>
        /// Reads all <see cref="Assembly">assemblies</see> and scans them for plug-ins. All found <see cref="Registrator"/>s will be added to the <see cref="Loader.Registrators">Registrator list</see>.
        /// </summary>
        /// <param name="directory">The directory path.</param>
        public static void ParseDirectory(string directory)
        {
            Contract.Requires(directory != null);
            Contract.Requires(Directory.Exists(directory));

            string[] files = Directory.GetFiles(directory, "*.dll", SearchOption.AllDirectories);

            foreach(string file in files){
                // Reflection methods require full file path.
                FileInfo fileInfo = new FileInfo(file);

                AssemblyName assemblyName;

                // Test if the file is an .NET compatible assembly.
                try
                {
                    assemblyName = AssemblyName.GetAssemblyName(fileInfo.FullName);
                }
                catch
                {                    
                    // The file is not an assembly.
                    continue;
                }

                try
                {
                    // The file is an assembly.
                    Assembly assembly = Assembly.Load(assemblyName);

                    // Load plug-ins from the assembly.
                    Loader.ParseAssembly(assembly);
                }
                catch
                {
                    // For some reason, the assembly failed to load.
                    Messenger.ThrowMessage(new Message(assemblyName + " failed to load.", MessageType.Warning));                    
                }
            }            
        }

        /// <summary>
        /// Scans one <see cref="Assembly"/> for plug-ins. All found <see cref="Registrator">registrators</see> will be added to the <see cref="Loader.Registrators">Registrator list</see>.
        /// </summary>
        /// <param name="assembly">The assembly.</param>
        public static void ParseAssembly(Assembly assembly)
        {
            Contract.Requires(assembly != null);

            // Search the assembly for plug-in types.
            var methods = from type in assembly.GetTypes()
                where typeof (IPlugIn).IsAssignableFrom(type)
                where !type.IsAbstract 
                where !type.IsGenericType
                from method in type.GetMethods()
                where method.Name == "Register"
                select method;
            foreach (var method in methods)
            {                
                Loader.Registrators.Add(new Registrator(method));                            
            }
        }

        /// <summary>
        /// Checks the <see cref="Registrator.DependsOn">prerequisites</see> of all <see cref="Registrator">registrators</see>. <see cref="Registrator">Registrators</see> with one or more missing <see cref="Registrator.DependsOn">prerequisites</see> will be marked as failed with according <see cref="RegistratorFailureType">failure type</see>.
        /// </summary>
        /// <remarks>
        /// This method will automatically call <see cref="OrderRegistrators"/> method when necessary.
        /// </remarks>
        public static void CheckRegistratorPrequisites()
        {
            // A failure of one registrator can cause a chain of missing dependencies -> detection must be done multiple times.
            bool badRegistratorFound;
            do
            {
                badRegistratorFound = false;

                // Ignore registrators, which failed earlier in the process.
                var registrators = from registrator in Loader.Registrators
                    where registrator.FailureType > RegistratorFailureType.UnimplementedInterface
                    select registrator;

                foreach (Registrator registrator in registrators)
                {
                    // Check all dependencies - all the dependecies must be implemented for the registrator to load.
                    var errors = from dependency in registrator.DependsOn
                        where !Loader.RegistratorsByInterface.ContainsKey(dependency)
                        select dependency + " is not implemented by any valid registrator";

                    // Some dependency is not implemented...
                    if (errors.Count() > 0)
                    {
                        registrator.FailureType = RegistratorFailureType.UnimplementedInterface;
                        registrator.Exception = new Exception(String.Join(", ", errors));
                        Loader.ForceRegistratorIndexRebuild();
                        badRegistratorFound = true;
                    }
                }
            } while (badRegistratorFound);
        }

        /// <summary>
        /// Orders the <see cref="Registrator">registrators</see> topologically, so all <see cref="Registrator">registrators</see> know all their <see cref="Registrator.DependsOn">prerequisites</see> when executed.
        /// </summary>
        /// <remarks>
        /// <see cref="Loader"/> methods will automatically call this method when necessary.
        /// </remarks>
        private static void OrderRegistrators()
        {
            Loader.CheckRegistratorPrequisites();

            // Erase those errors, that happen during or after the ordering process.
            var registrators = from registrator in Loader.Registrators
                where registrator.FailureType > RegistratorFailureType.UnimplementedInterface
                select registrator;           
            foreach (Registrator registrator in registrators)
            {
                registrator.FailureType = RegistratorFailureType.Success;
            }

            // Some registrator could be missing from the indexes because of being marked as failed.
            Loader.ForceRegistratorIndexRebuild();

            // The sorting algorithm can't run without a starting point.
            if (Loader.RootRegistrators.Count == 0)
            {
                throw new InvalidOperationException("No root registrators found.");
            }
            
            // Calculate how many edges lead to each registrator in the dependency graph.
            Dictionary<Registrator, int> unresolvedDependencyCountByRegistrator = Loader.CalculateDependencyCountByRegistrator();

            /* List of registrators without incoming edges - these are the initial "free" nodes for the topological
             * sorting algorithm. */
            Queue<Registrator> currentRootRegistrators = new Queue<Registrator>(Loader.RootRegistrators);

            // Topological sorting algorithm.
            Loader.orderedRegistrators = new List<Registrator>();
            while (currentRootRegistrators.Count > 0)
            {
                Registrator currentRegistrator = currentRootRegistrators.Dequeue();

                Loader.orderedRegistrators.Add(currentRegistrator);

                // Remove any adges that go FROM the current node.
                if (Loader.RegistratorsByParent.ContainsKey(currentRegistrator))
                {                    
                    var children = from child in Loader.RegistratorsByParent[currentRegistrator]
                        where unresolvedDependencyCountByRegistrator.ContainsKey(child)
                        select child;
                    foreach (Registrator child in children)
                    {
                        // No more edges will lead to the child node in the dependency graph -> it becomes a root node.
                        if (unresolvedDependencyCountByRegistrator[child] == 1)
                        {
                            currentRootRegistrators.Enqueue(child);                            
                            unresolvedDependencyCountByRegistrator.Remove(child);
                        }
                        else
                        {
                            unresolvedDependencyCountByRegistrator[child]--;
                        }
                    }
                }
            }

            // Any nodes with remaining edges are part of one or more cycles (or depend on cycled nodes).
            foreach(Registrator registrator in unresolvedDependencyCountByRegistrator.Keys){
                registrator.FailureType = RegistratorFailureType.CyclicDependency;
            }
        }

        private static Dictionary<Registrator, int> CalculateDependencyCountByRegistrator()
        {
            Contract.Ensures(Contract.Result<Dictionary<Registrator, int>>() != null);
            
            return (from registrator in Loader.Registrators
                where !registrator.Failed
                where registrator.IsEnabled
                where registrator.DependsOn.Length > 0
                select registrator).ToDictionary(
                    registrator => registrator,
                    registrator => registrator.DependsOn.Sum(dependency => Loader.RegistratorsByInterface[dependency].Count));
        }

        /// <summary>
        /// Returns a plug-in instance for a specific <see cref="Registrator"/>. The instance will be created only if plug-in settings allow it, otherwise existing instannce will be used.
        /// </summary>
        /// <param name="registrator">The registrator.</param>
        /// <returns></returns>
        public static object CreatePlugInInstance(Registrator registrator)
        {
            Contract.Requires(registrator != null);
            Contract.Requires(Loader.Registrators.Contains(registrator)); // The Loader must know this Registrator.
            Contract.Requires(registrator.FailureType >= RegistratorFailureType.ExceptionInConstructor); // Failures in constructor (or later) don't matter.
            Contract.Ensures(registrator.FailureType == RegistratorFailureType.ExceptionInConstructor || Contract.Result<object>() != null);
            Contract.Ensures(registrator.FailureType == RegistratorFailureType.ExceptionInConstructor || Contract.Result<object>().GetType() == registrator.PluginType);
            Contract.Ensures(registrator.FailureType > RegistratorFailureType.ExceptionInConstructor || registrator.Exception != null);

            // If the plug-in/registrator is set to one instance mode (and an instance exists), do not create another instance.););
            if (registrator.InstanceCount == InstanceCount.One && Loader.InstancesByPlugInType.ContainsKey(registrator.PluginType) && Loader.InstancesByPlugInType[registrator.PluginType].Count > 0)
            {
                return Loader.InstancesByPlugInType[registrator.PluginType][0];
            }

            try
            {
                object instance = Activator.CreateInstance(registrator.PluginType);
                Loader.Instances.Add(instance);

                // Erase any previously thrown exception.
                registrator.Exception = null;

                return instance;
            }
            catch (Exception e)
            {
                registrator.FailureType = RegistratorFailureType.ExceptionInConstructor;
                registrator.Exception = e;
                return null;
            }            
        }

        /// <summary>
        /// Executes all <see cref="Registrator">registrators</see>, which are not in a failure state. New plug-in instances will be created when necessary. The <see cref="Registrator">registrators</see> will be executed on all possible valid combinations of parameters.
        /// </summary>
        public static void ExecuteAllRegistrators()
        {
            /* Execute the first registrator in the hierarchy and let it execute all the registrators dependent on it -> all 
             * valid registrators will be executed. */
            Loader.ExecuteRegistrator(Loader.OrderedRegistrators[0]);

            if (Loader.Ready != null)
            {
                Loader.Ready(Application.Current, new EventArgs());
            }
        }

        /// <summary>
        /// Executes a registrator. The plug-in instance and parameters will be determined automatically (the registrator will be called for each valid combination of parameters).
        /// </summary>
        /// <param name="registrator">The registrator.</param>
        /// <param name="executeDependentRegistrators">If set to <c>true</c> all <see cref="Registrator">Registrators</see> following in the <see cref="Loader.OrderedRegistrators">Registrator queue</see> will be executed as well.</param>
        private static void ExecuteRegistrator(Registrator registrator, bool executeDependentRegistrators = true)
        {
            Contract.Requires(registrator.FailureType >= RegistratorFailureType.ExceptionInConstructor);
            Contract.Requires(Loader.OrderedRegistrators.Contains(registrator));

            /* Generate all combinations of available parameter instances.
             * Total count of combination is product of numbers of options.
             * Individual parameter sets are then for objects which takes parameters A, B and C where A has 2 options, B 2 as well and C 3 
             * generated like Loader (where on X axis are param. sets and on Y axis parameters):
             * A 1 2 1 2 1 2 1 2 1 2 1 2
             * B 1 1 2 2 1 1 2 2 1 1 2 2
             * C 1 1 1 1 2 2 2 2 3 3 3 3
             */

            int combinationCount = 1;
            int[] indexShifts = new int[registrator.DependsOn.Length];
            int i = 0;
            foreach (Type dependency in registrator.DependsOn)
            {
                /* There are no instances of a type Loader registrator depends on (but it was ready to instantiate and register) -> it failed 
                 * either in constructor or in registrator */
                if (!Loader.InstancesByInterface.ContainsKey(dependency))
                {
                    combinationCount = 0;
                    registrator.FailureType = RegistratorFailureType.ExceptionInDependency;
                    break;
                } 

                combinationCount *= Loader.InstancesByInterface[dependency].Count;
                indexShifts[i] = combinationCount;
                i++;
            }

            List<object> currentRegistratorInstances = new List<object>();
            for (i = 0; i < combinationCount; i++)
            {
                object instance = Loader.CreatePlugInInstance(registrator);
                currentRegistratorInstances.Add(instance);

                // Instance creation might have failed.
                if(instance == null) continue;

                // Choose current combination of parameters.
                object[] parameters = new object[registrator.DependsOn.Length];
                
                int j = 0;
                foreach(Type interfaceType in registrator.DependsOn){
                    parameters[j] = Loader.InstancesByInterface[interfaceType][i % indexShifts[j]];
                    j++;
                }

                Loader.ExecuteRegistratorOnInstance(registrator, instance, parameters);
            }

            // If there are any remaining registrators in the queue (and execution of dependent registrators is requested).
            int registratorIndex = Loader.OrderedRegistrators.IndexOf(registrator);
            if(executeDependentRegistrators && registratorIndex + 1 < Loader.OrderedRegistrators.Count){
                Loader.ExecuteRegistrator(Loader.OrderedRegistrators[registratorIndex + 1]);
            }
            // All registrators were already executed -> flag the Loader as finished.
            else if(executeDependentRegistrators)
            {
                Loader.IsFinished = true;
            }
        }

        /// <summary>
        /// Executes one specific <see cref="Registrator"/> with specific a set of parameters on a specific plug-in instance.
        /// </summary>
        /// <param name="registrator">The registrator.</param>
        /// <param name="instance">The instance.</param>
        /// <param name="parameters">The parameters.</param>
        private static void ExecuteRegistratorOnInstance(Registrator registrator, object instance, object[] parameters)
        {
            PlugInAttribute attribute = Attribute.GetCustomAttribute(registrator.PluginType, typeof(PlugInAttribute)) as PlugInAttribute ?? new PlugInAttribute();            
            
            if(Loader.IsFinished)
            {
                throw new InvalidOperationException("Can't register any more plug-ins after the loader was flagged as finished.");
            }

            if (Loader.OrderedRegistrators.IndexOf(registrator) == -1)
            {
                throw new ArgumentException("Passed registrator does not belong to Loader Loader (or failed to be ordered).");
            }

            if(instance == null)
            {
                throw new ArgumentNullException("Instance must not be null.");
            }

            if(parameters == null)
            {
                throw new ArgumentNullException("Parameter array must not be null.");
            }

            if(registrator.FailureType < RegistratorFailureType.ExceptionInRegistrator)
            {
                throw new ArgumentException("Passed registrator is in failure state.");
            }

            if(instance.GetType() != registrator.PluginType)
            {
                throw new ArgumentException("Passed registrator and instance types do not match.");
            }

            if (attribute.InstanceCount == InstanceCount.One && Loader.Instances.IndexOf(instance) == -1)
            {
                throw new ArgumentException("One instance of passed plug-in type is already registered in Loader. Only one plug-in instance is allowed for Loader plug-in type.");
            }

            /* The instance might be coming from outside -> make sure it is registered within the PlugInLoader */
            if(Loader.Instances.IndexOf(instance) == -1){
                Loader.Instances.Add(instance);
            }

            try
            {
                registrator.Method.Invoke(instance, parameters);                
            }
            catch(TargetInvocationException e){
                registrator.FailureType = RegistratorFailureType.ExceptionInRegistrator;
                registrator.Exception = e.InnerException;
            }
        }

        /// <summary>
        /// Determines whether specific plug-in is enabled.
        /// </summary>
        /// <param name="plugInType">The plug-in type.</param>
        /// <returns>
        /// 	<c>true</c> if the plug-in; otherwise, <c>false</c>.
        /// </returns>
        public static bool IsPlugInTypeEnabled(Type plugInType)
        {
            if (Loader.config.DisabledPlugIns.Contains(plugInType))
            {
                return false;
            }

            return true;
        }

        /// <summary>
        /// Enables the the plug-in type.
        /// </summary>
        /// <param name="plugInType">The plug-in type.</param>
        public static void EnablePlugInType(Type plugInType)
        {
            Loader.config.DisabledPlugIns.Remove(plugInType);      
        }

        /// <summary>
        /// Disables the the plug-in type.
        /// </summary>
        /// <param name="plugInType">The plug-in type.</param>
        public static void DisablePlugInType(Type plugInType)
        {
            if(Loader.IsPlugInTypeEnabled(plugInType)){
                Loader.config.DisabledPlugIns.Add(plugInType);
            }
        }

        /// <summary>
        /// Determines whether the plug-in is enabled.
        /// </summary>
        /// <param name="plugIn">The plug in.</param>
        /// <returns>
        /// 	<c>true</c> if the plug-in is enabled; otherwise, <c>false</c>.
        /// </returns>
        public static bool IsPlugInEnabled(PlugIn plugIn)
        {
            return Loader.IsPlugInTypeEnabled(plugIn.Type);
        }

        /// <summary>
        /// Enables the plug-in.
        /// </summary>
        /// <param name="plugIn">The plug-in.</param>
        public static void EnablePlugIn(PlugIn plugIn)
        {
            Loader.EnablePlugInType(plugIn.Type);
        }

        /// <summary>
        /// Disables the plug-in.
        /// </summary>
        /// <param name="plugIn">The plug-in.</param>
        public static void DisablePlugIn(PlugIn plugIn)
        {
            Loader.DisablePlugInType(plugIn.Type);
        }

        private static void BuildPlugInsList()
        {
            Contract.Ensures(Loader.plugIns != null);

            var typesAlreadyAdded = new List<Type>();

            Loader.plugIns = new List<PlugIn>();
            var registrators = from registrator in Loader.Registrators
                where !typesAlreadyAdded.Contains(registrator.PluginType)
                select registrator;
            foreach(Registrator registrator in registrators)
            {
                List<object> currentTypeInstances;
                Loader.InstancesByPlugInType.TryGetValue(registrator.PluginType, out currentTypeInstances);

                List<Registrator> currentTypeRegistrators;
                Loader.RegistratorsByPlugInType.TryGetValue(registrator.PluginType, out currentTypeRegistrators);

                Loader.plugIns.Add(
                    new PlugIn(
                        plugInType: registrator.PluginType,
                        instances: currentTypeInstances ?? new List<object>(),
                        registrators: currentTypeRegistrators ?? new List<Registrator>()
                    )
                );

                typesAlreadyAdded.Add(registrator.PluginType);
            }
        }

        #region Index Rebuilders
        private static void RebuildRegistratorsByParentIndex()
        {
            Loader.RegistratorsByParent = new Dictionary<Registrator, List<Registrator>>();

            foreach (Registrator registrator in Loader.Registrators)
            {
                if(registrator.FailureType <= RegistratorFailureType.UnimplementedInterface) continue;

                foreach (Type interfaceType in registrator.DependsOn)
                {
                    foreach (Registrator parent in Loader.RegistratorsByInterface[interfaceType])
                    {
                        List<Registrator> childrenList;
                        if (Loader.registratorsByParent.TryGetValue(parent, out childrenList))
                        {
                            childrenList.Add(registrator);
                        }
                        else
                        {
                            childrenList = new List<Registrator> {registrator};
                            Loader.registratorsByParent.Add(parent, childrenList);
                        }
                    }
                }
            }
        }

        private static void RebuildRootRegistratorIndex()
        {
            Loader.RootRegistrators = new List<Registrator>();
            
            foreach (Registrator registrator in Loader.Registrators)
            {
                if (registrator.FailureType <= RegistratorFailureType.BadRegistratorTemplate) continue;

                if (registrator.IsRootRegistrator)
                {
                    Loader.rootRegistrators.Add(registrator);
                }
            }
        }

        private static void RebuildRegistratorsByInterfaceIndex()
        {
            Loader.registratorsByInterface = new Dictionary<Type, List<Registrator>>();

            foreach (Registrator registrator in Loader.Registrators)
            {
                if (registrator.FailureType <= RegistratorFailureType.UnimplementedInterface) continue;

                foreach (Type interfaceType in registrator.Implements)
                {
                    List<Registrator> implementingRegistrators;
                    if (Loader.registratorsByInterface.TryGetValue(interfaceType, out implementingRegistrators))
                    {
                        implementingRegistrators.Add(registrator);
                    }
                    else
                    {
                        implementingRegistrators = new List<Registrator> {registrator};
                        Loader.registratorsByInterface.Add(interfaceType, implementingRegistrators);
                    }
                }
            }
        }

        private static void RebuildRegistratorsByPlugInTypeIndex()
        {
            Loader.RegistratorsByPlugInType = new Dictionary<Type, List<Registrator>>();

            foreach (Registrator registrator in Loader.Registrators)
            {
                List<Registrator> registratorList;
                if (Loader.registratorsByPlugInType.TryGetValue(registrator.PluginType, out registratorList))
                {
                    registratorList.Add(registrator);
                }
                else
                {
                    registratorList = new List<Registrator> {registrator};
                    Loader.registratorsByPlugInType.Add(registrator.PluginType, registratorList);
                }
            }
        }

        private static void RebuildInstancesByPlugInTypeIndex()
        {
            Loader.InstancesByPlugInType = new Dictionary<Type, List<object>>();

            foreach (object instance in Loader.Instances)
            {
                List<object> instanceList;
                if (Loader.instancesByPlugInType.TryGetValue(instance.GetType(), out instanceList))
                {
                    instanceList.Add(instance);
                }
                else
                {
                    instanceList = new List<object> {instance};
                    Loader.instancesByPlugInType.Add(instance.GetType(), instanceList);
                }
            }
        }

        private static void RebuildInstancesByInterfaceIndex()
        {
            Loader.instancesByInterface = new Dictionary<Type, List<object>>();

            foreach (object instance in Loader.Instances)
            {
                foreach (Type interfaceType in instance.GetType().GetInterfaces())
                {
                    List<object> implementerList;
                    if (Loader.instancesByInterface.TryGetValue(interfaceType, out implementerList))
                    {
                        implementerList.Add(instance);
                    }
                    else
                    {
                        implementerList = new List<object> {instance};
                        Loader.instancesByInterface.Add(interfaceType, implementerList);
                    }
                }
            }
        }
        #endregion
    }
}
