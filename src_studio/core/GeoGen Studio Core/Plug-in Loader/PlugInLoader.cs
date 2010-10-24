using System;
using System.Collections.Generic;
using System.Text;
using System.Reflection;
using System.Collections.ObjectModel;
using System.Collections.Specialized;
using System.IO;

namespace GeoGen.Studio.PlugInLoader
{

    class Loader
    {   
        #region Fields
        protected List<Registrator> rootRegistrators;
        protected Dictionary<Type, List<Registrator>> registratorsByInterface = null;
        protected Dictionary<Registrator, List<Registrator>> registratorsByParent = null;
        protected Dictionary<Type, List<Type>> plugInTypesByInterface = null;
        protected Dictionary<Type, List<object>> instancesByInterface = null;
        protected Dictionary<Type, List<object>> instancesByPlugInType = null;
        protected List<Registrator> orderedRegistrators = null;
        #endregion

        #region Properties
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

        protected void ForceRegistratorIndexRebuild(){
            this.rootRegistrators = null;
            this.registratorsByInterface = null;
            this.registratorsByParent = null;
            this.orderedRegistrators = null;
        }

        protected void ForceInstanceIndexRebuild()
        {
            this.InstancesByInterface = null;
            this.InstancesByPlugInType = null;
        }        

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

        public void CheckRegistratorPrequisites()
        {
            /* A failure of one registrator can cause a chain of missing dependencies -> detection must be done multiple times */
            bool changed = true;
            while(changed == true){
                changed = false;
                foreach (Registrator registrator in this.Registrators)
                {
                    if (registrator.FailureType > RegistratorFailureType.UnimplementedInterface)
                    {

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

        public object CreatePlugInInstance(Registrator registrator)
        {
            if (this.Registrators.IndexOf(registrator) == -1)
            {
                throw new ArgumentException("Passed registrator does not belong to this PlugInLoader");
            }

            else if (registrator.FailureType < RegistratorFailureType.ExceptionInConstructor)
            {
                throw new ArgumentException("Passed registrator is in failure state");
            }

            /* If the plug-in/registrator is set to one instance mode, do not create another instance */
            if (registrator.InstanceCount == InstanceCount.One && this.InstancesByPlugInType.ContainsKey(registrator.PluginType) && this.InstancesByPlugInType[registrator.PluginType].Count > 0)
            {
                return this.InstancesByPlugInType[registrator.PluginType];
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

        public void ExecuteAllRegistrators(){
            /* Execute the first registrator in the hieararchy and let it execute all the registrators dependdent on it -> all 
             * valid registrators will be executed */
            this.ExecuteRegistrator(this.OrderedRegistrators[0], true);
        }

        public void ExecuteRegistrator(Registrator registrator, bool executeDependentRegistrators = true){
            int registratorIndex = this.OrderedRegistrators.IndexOf(registrator);

            /* The PlugInLoader must be able to place the registrator into its precalculated hierarchy */
            if (registratorIndex == -1)
            {
                throw new ArgumentException("Passed registrator does not belong to this PlugInLoader (or failed to be ordered)");
            }

            else if (registrator.FailureType < RegistratorFailureType.ExceptionInConstructor)
            {
                throw new ArgumentException("Passed registrator is in failure state");
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

                combinationCount *= this.InstancesByInterface[dependency].Count;//Math.Max(this.InstancesByInterface[dependency].Count, this.RegistratorsByInterface[dependency].Count);
                indexShifts[i] = combinationCount;
                i++;
            }

            Console.WriteLine(registrator + " " + combinationCount + "x");

            List<object> currentRegistratorInstances = new List<object>();
            for (i = 0; i < combinationCount; i++)
            {
                object instance = this.CreatePlugInInstance(registrator);
                currentRegistratorInstances.Add(instance);

                /* Instance creation might have failed */
                if(instance == null) continue;
                
                object[] parameters = new object[registrator.DependsOn.Length];

                Console.Write("    ");

                int j = 0;
                foreach(Type interfaceType in registrator.DependsOn){
                    parameters[j] = this.InstancesByInterface[interfaceType][i % indexShifts[j]];
                    Console.Write(parameters[j].GetHashCode() + " ");
                    j++;
                }

                Console.WriteLine();

                this.ExecuteRegistratorOnInstance(registrator, instance, parameters);
            }

            if(executeDependentRegistrators && registratorIndex + 1 < this.OrderedRegistrators.Count){
                this.ExecuteRegistrator(this.OrderedRegistrators[registratorIndex + 1], true);
            }
        }

        public void ExecuteRegistrator(Registrator registrator, object[] parameters)
        {
            object instance = this.CreatePlugInInstance(registrator);

            /* CreatePlugInInstance might have failed (failure is then described in the FailureType flag) */
            if(instance != null){
                this.ExecuteRegistratorOnInstance(registrator, instance, parameters);
            }
        }

        public void ExecuteRegistratorOnInstance(Registrator registrator, object instance, object[] parameters)
        {
            if (this.OrderedRegistrators.IndexOf(registrator) == -1)
            {
                throw new ArgumentException("Passed registrator does not belong to this PlugInLoader (or failed to be ordered)");
            }

            else if(instance == null){
                throw new ArgumentNullException("Instance must not be null");
            }

            else if(parameters == null){
                throw new ArgumentNullException("Parameter array must not be null");
            }

            else if(registrator.FailureType < RegistratorFailureType.ExceptionInRegistrator){
                throw new ArgumentException("Passed registrator is in failure state");
            }

            else if(instance.GetType() != registrator.PluginType){
                throw new ArgumentException("Passed registrator and instance types do not match");
            }

            /* The instance might be coming from outside -> make sure it is registered within the PlugInLoader */
            if(this.Instances.IndexOf(instance) == -1){
                this.Instances.Add(instance);
            }

            try
            {
                Console.WriteLine("before");
                registrator.Method.Invoke(instance, parameters);
                Console.WriteLine("after");
            }
            catch(TargetInvocationException e){
                registrator.FailureType = RegistratorFailureType.ExceptionInRegistrator;
                registrator.Exception = e.InnerException;
            }
        }

        #region Index Rebuilders
        protected void RebuildRegistratorsByParentIndex()
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

        protected void RebuildRootRegistratorIndex()
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

        protected void RebuildRegistratorsByInterfaceIndex()
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

        protected void RebuildPlugInTypesByInterfaceIndex()
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

        protected void RebuildInstancesByPlugInTypeIndex()
        {
            this.InstancesByPlugInType = new Dictionary<Type, List<object>>();

            foreach (object instance in this.Instances)
            {
                List<object> instanceList;
                if (this.instancesByInterface.TryGetValue(instance.GetType(), out instanceList))
                {
                    instanceList.Add(instance);
                }
                else
                {
                    instanceList = new List<object>();
                    instanceList.Add(instance);
                    this.instancesByInterface.Add(instance.GetType(), instanceList);
                }
            }
        }

         protected void RebuildInstancesByInterfaceIndex()
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
