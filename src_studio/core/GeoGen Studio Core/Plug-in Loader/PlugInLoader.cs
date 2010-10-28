using System;
using System.Collections.Generic;
using System.Text;
using System.Reflection;
using System.Collections.ObjectModel;
using System.Collections.Specialized;
using System.IO;

namespace GeoGen.Studio.PlugInLoader
{

    /// <summary>
    /// This class detects, analyzes and loads plug-ins.
    /// </summary>
    public sealed class Loader
    {   
        #region Fields
        private List<Registrator> rootRegistrators;
        private Dictionary<Type, List<Registrator>> registratorsByInterface = null;
        private Dictionary<Registrator, List<Registrator>> registratorsByParent = null;
        private Dictionary<Type, List<Type>> plugInTypesByInterface = null;
        private Dictionary<Type, List<object>> instancesByInterface = null;
        private Dictionary<Type, List<object>> instancesByPlugInType = null;
        private List<Registrator> orderedRegistrators = null;
        #endregion

        #region Properties
        /// <summary>
        /// List of all <see cref="Registrator">Registrators<see/> registered with this <see cref="Loader"/>.
        /// </summary>
        /// <value>The registrators.</value>
        public ObservableCollection<Registrator> Registrators { get; protected set; }
        public ObservableCollection<object> Instances { get; protected set; }

        public List<Registrator> RootRegistrators
        {
            get
            {
                if (rootRegistrators == null)
                {
                    this.RebuildRootRegistratorIndex();
                }
                return rootRegistrators;
            }
            protected set { rootRegistrators = value; }
        }

        public List<Registrator> OrderedRegistrators
        {
            get
            {
                if (orderedRegistrators == null)
                {
                    this.OrderRegistrators();
                }
                return orderedRegistrators;
            }
            protected set { orderedRegistrators = value; }
        }

        protected Dictionary<Type, List<Registrator>> RegistratorsByInterface
        {
            get
            {
                if (this.registratorsByInterface == null)
                {
                    this.RebuildRegistratorsByInterfaceIndex();
                }
                return this.registratorsByInterface;
            }
            set { registratorsByInterface = value; }
        }

        protected Dictionary<Registrator, List<Registrator>> RegistratorsByParent
        {
            get
            {
                if (registratorsByParent == null)
                {
                    this.RebuildRegistratorsByParentIndex();
                }
                return this.registratorsByParent;
            }
            set { registratorsByParent = value; }
        }

        protected Dictionary<Type, List<Type>> PlugInTypesByInterface
        {
            get
            {
                if (plugInTypesByInterface == null)
                {
                    this.RebuildPlugInTypesByInterfaceIndex();
                }
                return this.plugInTypesByInterface;
            }
            set { plugInTypesByInterface = value; }
        }

        protected Dictionary<Type, List<object>> InstancesByInterface
        {
            get
            {
                if (instancesByInterface == null)
                {
                    this.RebuildInstancesByInterfaceIndex();
                }
                return this.instancesByInterface;
            }
            set { instancesByInterface = value; }
        }

        protected Dictionary<Type, List<object>> InstancesByPlugInType
        {
            get
            {
                if (instancesByPlugInType == null)
                {
                    this.RebuildInstancesByPlugInTypeIndex();
                }
                return this.instancesByPlugInType;
            }
            set { instancesByPlugInType = value; }
        }
        #endregion

        /// <summary>
        /// Initializes a new instance of the <see cref="Loader"/> class.
        /// <threadsafety static="true" instance="false"/>
        /// </summary>
        public Loader(){
            this.Registrators = new ObservableCollection<Registrator>();
            this.Instances = new ObservableCollection<object>();

            /* Indexes are made obsolete every time the main collection changes */
            this.Registrators.CollectionChanged += delegate(object sender, NotifyCollectionChangedEventArgs args)
            {
                this.ForceRegistratorIndexRebuild();
            };
            this.Instances.CollectionChanged += delegate(object sender, NotifyCollectionChangedEventArgs args)
            {
                this.ForceInstanceIndexRebuild();
            };
        }

        private void ForceRegistratorIndexRebuild(){
            this.rootRegistrators = null;
            this.registratorsByInterface = null;
            this.registratorsByParent = null;
            this.orderedRegistrators = null;
        }

        private void ForceInstanceIndexRebuild()
        {
            this.InstancesByInterface = null;
            this.InstancesByPlugInType = null;
        }

        /// <summary>
        /// Reads all <see cref="Assembly">assemblies</see> and scans them for plug-ins. All found <see cref="Registrator"/>s will be added to the <see cref="Loader.Registrators">Registrator list</see>.
        /// </summary>
        /// <param name="directory">The directory path.</param>
        public void ParseDirectory(string directory){
            if(!Directory.Exists(directory)){
                throw new ArgumentException("Directory \"" + directory + "\" does not exist or is not readable.");
            }

            string[] files = Directory.GetFiles(directory, "*.dll", SearchOption.AllDirectories);

            foreach(string file in files){
                /* Reflection methods require full file path */
                FileInfo fileInfo = new FileInfo(file);

                AssemblyName assemblyName = null;

                /* Test if the file is an .NET compatible assembly */
                try
                {
                    assemblyName = AssemblyName.GetAssemblyName(fileInfo.FullName);
                }
                catch
                {                    
                    /* The file is not an assembly */
                    continue;
                }

                try
                {
                    /* The file is an assembly */
                    Assembly assembly = Assembly.Load(assemblyName);

                    /* Load plug-ins from the assembly */
                    this.ParseAssembly(assembly);
                }
                catch
                {
                    /* For some reason, the assembly failed to load */
                }
            }            
        }

        /// <summary>
        /// Scans one <see cref="Assembly"/> for plug-ins. All found <see cref="Registrator"/>s will be added to the <see cref="Loader.Registrators">Registrator list</see>.
        /// </summary>
        /// <param name="assembly">The assembly.</param>
        public void ParseAssembly(Assembly assembly){                      
            /* Search the assembly for plug-in types */
            foreach (Type type in assembly.GetTypes())
            {
                if (typeof(IPlugIn).IsAssignableFrom(type) && !type.IsAbstract && !type.IsGenericType)
                {
                    /* The type is an plug-in, look for registrators */
                    MethodInfo[] methods = type.GetMethods();
                    foreach (MethodInfo method in methods)
                    {
                        if (method.Name == "Register")
                        {
                            this.Registrators.Add(new Registrator(method));
                        }
                    }
                }
            }
        }

        /// <summary>
        /// Checks the <see cref="Registrator.DependsOn">prerequisites</see> of all <see cref="Registrator">Registrators</see>. <see cref="Registrator"/>Registrators</see> with one or more missing <see cref="Registrator.DependsOn">prerequisites</see> will be marked as <see cref="Registrator.FailureTupe">failed</see> with according <see cref="RegistratorFailureType">failure type</see>.
        /// </summary>
        /// <remarks>
        /// <see cref="Registrator"/> execution methods will automatically call this method when necessary.
        /// </remarks>
        public void CheckRegistratorPrequisites()
        {
            /* A failure of one registrator can cause a chain of missing dependencies -> detection must be done multiple times */
            bool changed = true;
            while(changed == true){
                changed = false;
                foreach (Registrator registrator in this.Registrators)
                {
                    /* Ignore registrators, which failed earlier in the process. */
                    if (registrator.FailureType > RegistratorFailureType.UnimplementedInterface)
                    {
                        /* Check all dependencies. */
                        List<string> errors = new List<string>();
                        foreach (Type dependency in registrator.DependsOn)
                        {
                            if (!this.RegistratorsByInterface.ContainsKey(dependency))
                            {
                                errors.Add(dependency.ToString() + " is not implemented by any valid registrator");
                            }
                        }

                        if (errors.Count > 0)
                        {
                            registrator.FailureType = RegistratorFailureType.UnimplementedInterface;
                            registrator.Exception = new Exception(String.Join(", ", errors));
                            this.ForceRegistratorIndexRebuild();
                            changed = true;
                        }
                    }
                }
            }
        }

        /// <summary>
        /// Orders the <see cref="Registrator">Registrators</see> topologically, so all <see cref="Registrator">Registrators</see> know all their <see cref="Registrator.DependsOn">prerequisites</see> when executed.
        /// </summary>
        /// <remarks>
        /// <see cref="OrderedRegistrators"/> will automatically call this method when necessary.
        /// </remarks>
        public void OrderRegistrators(){
            this.CheckRegistratorPrequisites();

            /* Most errors could be fixed since last execution of this method */
            foreach (Registrator registrator in this.Registrators)
            {
                if(registrator.FailureType > RegistratorFailureType.UnimplementedInterface){
                    registrator.FailureType = RegistratorFailureType.Success;
                }
            }

            /* Some registrator could be missing from the indexes because of being marked as failed */
            this.ForceRegistratorIndexRebuild();
            
            /* Calculate how many edges lead to each registrator in the dependency graph */
            Dictionary<Registrator, int> numUnresolvedDependencies = new Dictionary<Registrator, int>();
            foreach (Registrator registrator in this.Registrators)
            {
                /* Ignore failed registrators. */
                if (registrator.Failed) continue;

                if (registrator.DependsOn.Length > 0)
                {
                    int numDependencies = 0;
                    foreach (Type dependency in registrator.DependsOn)
                    {
                        numDependencies += this.RegistratorsByInterface[dependency].Count;
                    }

                    numUnresolvedDependencies[registrator] = numDependencies;
                }
            }

            /* List of registrators without incoming edges */
            Queue<Registrator> currentRootRegistrators = new Queue<Registrator>(this.RootRegistrators);

            /* The sorting algorithm can't run without a starting point */
            if(currentRootRegistrators.Count == 0){
                throw new InvalidOperationException("No root registrators found");
            }

            /* Topological sorting algorithm */
            this.orderedRegistrators = new List<Registrator>();
            while (currentRootRegistrators.Count > 0)
            {
                Registrator registrator = currentRootRegistrators.Dequeue();

                this.orderedRegistrators.Add(registrator);

                /* Are there some edges going FROM the node? */
                if (this.RegistratorsByParent.ContainsKey(registrator))
                {
                    /* Remove all edges going from current node */
                    List<Registrator> children = this.RegistratorsByParent[registrator];
                    foreach (Registrator child in children)
                    {
                        if (!numUnresolvedDependencies.ContainsKey(child))
                        {
                            continue;
                        }

                        /* No more edges will lead to the child note in the dependency graph -> it becomes a root node */
                        if (numUnresolvedDependencies[child] == 1)
                        {
                            currentRootRegistrators.Enqueue(child);                            
                            numUnresolvedDependencies.Remove(child);
                        }

                        else
                        {
                            numUnresolvedDependencies[child]--;
                        }
                    }
                }
            }

            /* Any nodes with remaining edges are part of one or more cycles (or depend on cycled nodes) */
            foreach(Registrator registrator in numUnresolvedDependencies.Keys){
                registrator.FailureType = RegistratorFailureType.CyclicDependency;
            }
        }

        /// <summary>
        /// Returns a plug-in instance for a specific registrator. The instance will be created only if plug-in settings allow it, otherwise existing instannce will be used.
        /// </summary>
        /// <param name="registrator">The registrator.</param>
        /// <returns></returns>
        public object CreatePlugInInstance(Registrator registrator)
        {
            if (this.Registrators.IndexOf(registrator) == -1)
            {
                throw new ArgumentException("Passed registrator does not belong to this Loader.");
            }

            else if (registrator.FailureType < RegistratorFailureType.ExceptionInConstructor)
            {
                throw new ArgumentException("Passed registrator is in failure state.");
            }

            /* If the plug-in/registrator is set to one instance mode, do not create another instance */
            if (registrator.InstanceCount == InstanceCount.One && this.InstancesByPlugInType.ContainsKey(registrator.PluginType) && this.InstancesByPlugInType[registrator.PluginType].Count > 0)
            {
                return this.InstancesByPlugInType[registrator.PluginType][0];
            }
            else
            {
                try
                {
                    object instance = Activator.CreateInstance(registrator.PluginType);
                    this.Instances.Add(instance);

                    return instance;
                }
                catch (Exception e)
                {
                    registrator.FailureType = RegistratorFailureType.ExceptionInConstructor;
                    registrator.Exception = e;
                }
                return null;
            }
        }

        /// <summary>
        /// Executes all <see cref="Registrator">Registrators</see>, which are not in a failure state. New plug-in instances will be created when necessary. The <see cref="Registrator">Registrators</see> will be executed on all possible valid combinations of parameters.
        /// </summary>
        public void ExecuteAllRegistrators(){
            /* Execute the first registrator in the hierarchy and let it execute all the registrators dependent on it -> all 
             * valid registrators will be executed */

            this.ExecuteRegistrator(this.OrderedRegistrators[0], true);
        }

        /// <summary>
        /// Executes a registrator. The plug-in instance and parameters will be determined automatically (the registrator will be called for each valid combination of parameters).
        /// </summary>
        /// <param name="registrator">The registrator.</param>
        /// <param name="executeDependentRegistrators">If set to <c>true</c> all <see cref="Registrator">Registrators</see> following in the <see cref="Loader.OrderedRegistrators">Registrator queue</see> will be executed as well.</param>
        public void ExecuteRegistrator(Registrator registrator, bool executeDependentRegistrators = true){
            int registratorIndex = this.OrderedRegistrators.IndexOf(registrator);

            if (registrator.FailureType < RegistratorFailureType.ExceptionInConstructor)
            {
                throw new ArgumentException("Passed registrator is in failure state.");
            }
            /* The PlugInLoader must be able to place the registrator into its precalculated hierarchy */
            else if (registratorIndex == -1)
            {
                throw new ArgumentException("Passed registrator does not belong to this Loader (or failed to be ordered).");
            }

            /* Generate all combinations of available parameter instances.
             * Total count of combination is product of numbers of options.
             * Individual parameter sets are then for objects which takes parameters A, B and C where A has 2 options, B 2 as well and C 3 
             * generated like this (where on X axis are param. sets and on Y axis parameters):
             * A 1 2 1 2 1 2 1 2 1 2 1 2
             * B 1 1 2 2 1 1 2 2 1 1 2 2
             * C 1 1 1 1 2 2 2 2 3 3 3 3
             */

            int combinationCount = 1;
            int[] indexShifts = new int[registrator.DependsOn.Length];
            int i = 0;
            foreach (Type dependency in registrator.DependsOn)
            {
                /* There are no instances of a type this registrator depends on (but it was ready to instantiate and register) -> it failed 
                 * either in constructor or in registrator */
                if (!this.InstancesByInterface.ContainsKey(dependency))
                {
                    combinationCount = 0;
                    registrator.FailureType = RegistratorFailureType.ExceptionInDependency;
                    break;
                } 

                combinationCount *= this.InstancesByInterface[dependency].Count;
                indexShifts[i] = combinationCount;
                i++;
            }

            List<object> currentRegistratorInstances = new List<object>();
            for (i = 0; i < combinationCount; i++)
            {
                object instance = this.CreatePlugInInstance(registrator);
                currentRegistratorInstances.Add(instance);

                /* Instance creation might have failed. */
                if(instance == null) continue;

                /* Choose current combination of parameters. */
                object[] parameters = new object[registrator.DependsOn.Length];
                
                int j = 0;
                foreach(Type interfaceType in registrator.DependsOn){
                    parameters[j] = this.InstancesByInterface[interfaceType][i % indexShifts[j]];
                    j++;
                }

                this.ExecuteRegistratorOnInstance(registrator, instance, parameters);
            }

            /* If there are any remaining registrators in the queue (and execution of dependent registrators is requested). */
            if(executeDependentRegistrators && registratorIndex + 1 < this.OrderedRegistrators.Count){
                this.ExecuteRegistrator(this.OrderedRegistrators[registratorIndex + 1], true);
            }
        }

        /// <summary>
        /// Executes one specific registrator with specific a set of parameters. The plug-in will be instantiated depending on its settings. */
        /// </summary>
        /// <param name="registrator">The registrator.</param>
        /// <param name="parameters">The parameter set.</param>
        public void ExecuteRegistrator(Registrator registrator, object[] parameters)
        {
            object instance = this.CreatePlugInInstance(registrator);

            /* CreatePlugInInstance might have failed (failure is then described in the FailureType flag) */
            if(instance != null){
                this.ExecuteRegistratorOnInstance(registrator, instance, parameters);
            }
        }

        /// <summary>
        /// Executes one specific registrator with specific a set of parameters on a specific plug-in instance.
        /// </summary>
        /// <param name="registrator">The registrator.</param>
        /// <param name="instance">The instance.</param>
        /// <param name="parameters">The parameters.</param>
        public void ExecuteRegistratorOnInstance(Registrator registrator, object instance, object[] parameters)
        {
            PlugInAttribute attribute = Attribute.GetCustomAttribute(registrator.PluginType, typeof(PlugInAttribute)) as PlugInAttribute ?? new PlugInAttribute();            
            
            if (this.OrderedRegistrators.IndexOf(registrator) == -1)
            {
                throw new ArgumentException("Passed registrator does not belong to this Loader (or failed to be ordered).");
            }

            else if(instance == null)
            {
                throw new ArgumentNullException("Instance must not be null.");
            }

            else if(parameters == null)
            {
                throw new ArgumentNullException("Parameter array must not be null.");
            }

            else if(registrator.FailureType < RegistratorFailureType.ExceptionInRegistrator)
            {
                throw new ArgumentException("Passed registrator is in failure state.");
            }

            else if(instance.GetType() != registrator.PluginType)
            {
                throw new ArgumentException("Passed registrator and instance types do not match.");
            }

            else if (attribute.InstanceCount == InstanceCount.One && this.Instances.IndexOf(instance) > -1)
            {
                throw new ArgumentException("One instance of passed plug-in type is already registered in this Loader. Only one plug-in instance is allowed for this plug-in type.");
            }

            /* The instance might be coming from outside -> make sure it is registered within the PlugInLoader */
            if(this.Instances.IndexOf(instance) == -1){
                this.Instances.Add(instance);
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

        #region Index Rebuilders
        private void RebuildRegistratorsByParentIndex()
        {
            this.RegistratorsByParent = new Dictionary<Registrator, List<Registrator>>();

            foreach (Registrator registrator in this.Registrators)
            {
                if(registrator.FailureType <= RegistratorFailureType.UnimplementedInterface) continue;

                foreach (Type interfaceType in registrator.DependsOn)
                {
                    foreach (Registrator parent in this.RegistratorsByInterface[interfaceType])
                    {
                        List<Registrator> childrenList;
                        if (this.registratorsByParent.TryGetValue(parent, out childrenList))
                        {
                            childrenList.Add(registrator);
                        }
                        else
                        {
                            childrenList = new List<Registrator>();
                            childrenList.Add(registrator);
                            this.registratorsByParent.Add(parent, childrenList);
                        }
                    }
                }
            }
        }

        private void RebuildRootRegistratorIndex()
        {
            this.RootRegistrators = new List<Registrator>();
            
            foreach (Registrator registrator in this.Registrators)
            {
                if (registrator.FailureType <= RegistratorFailureType.BadRegistratorTemplate) continue;

                if (registrator.IsRootRegistrator)
                {
                    this.rootRegistrators.Add(registrator);
                }
            }
        }

        private void RebuildRegistratorsByInterfaceIndex()
        {
            this.RegistratorsByInterface = new Dictionary<Type, List<Registrator>>();

            foreach (Registrator registrator in this.Registrators)
            {
                if (registrator.FailureType <= RegistratorFailureType.UnimplementedInterface) continue;

                foreach (Type interfaceType in registrator.Implements)
                {
                    List<Registrator> implementingRegistrators;
                    if (this.registratorsByInterface.TryGetValue(interfaceType, out implementingRegistrators))
                    {
                        implementingRegistrators.Add(registrator);
                    }
                    else
                    {
                        implementingRegistrators = new List<Registrator>();
                        implementingRegistrators.Add(registrator);
                        this.registratorsByInterface.Add(interfaceType, implementingRegistrators);
                    }
                }
            }
        }

        private void RebuildPlugInTypesByInterfaceIndex()
        {
            this.PlugInTypesByInterface = new Dictionary<Type, List<Type>>();

            foreach (Registrator registrator in this.Registrators)
            {
                foreach (Type interfaceType in registrator.Implements)
                {
                    List<Type> implementerList;
                    if (this.plugInTypesByInterface.TryGetValue(interfaceType, out implementerList))
                    {
                        implementerList.Add(registrator.PluginType);
                    }
                    else
                    {
                        implementerList = new List<Type>();
                        implementerList.Add(registrator.PluginType);
                        this.plugInTypesByInterface.Add(interfaceType, implementerList);
                    }
                }
            }
        }

        private void RebuildInstancesByPlugInTypeIndex()
        {
            this.InstancesByPlugInType = new Dictionary<Type, List<object>>();

            foreach (object instance in this.Instances)
            {
                List<object> instanceList;
                if (this.instancesByPlugInType.TryGetValue(instance.GetType(), out instanceList))
                {
                    instanceList.Add(instance);
                }
                else
                {
                    instanceList = new List<object>();
                    instanceList.Add(instance);
                    this.instancesByPlugInType.Add(instance.GetType(), instanceList);
                }
            }
        }

        private void RebuildInstancesByInterfaceIndex()
        {
            this.instancesByInterface = new Dictionary<Type, List<object>>();

            foreach (object instance in this.Instances)
            {
                foreach (Type interfaceType in instance.GetType().GetInterfaces())
                {
                    List<object> implementerList;
                    if (this.instancesByInterface.TryGetValue(interfaceType, out implementerList))
                    {
                        implementerList.Add(instance);
                    }
                    else
                    {
                        implementerList = new List<object>();
                        implementerList.Add(instance);
                        this.instancesByInterface.Add(interfaceType, implementerList);
                    }
                }
            }
        }
        #endregion
    }
}
