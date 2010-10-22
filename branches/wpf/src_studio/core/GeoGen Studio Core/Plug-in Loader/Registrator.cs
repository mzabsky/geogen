using System;
using System.Collections.Generic;
using System.Text;
using System.Reflection;

namespace GeoGen.Studio.PlugInLoader
{
    enum RegistratorFailureType{        
        UnreadableAssembly = 0,
        MissingAssemblyDependency = 1,
        BadRegistratorTemplate = 2,
        UnimplementedInterface = 3,
        CyclicDependency = 4,
        ExceptionInConstructor = 6,
        ExceptionInRegistrator = 7,
        ExceptionInDependency = 8,
        Success = 9,
    }

    class Registrator
    {
        protected RegistratorFailureType failureType;

        public Type PluginType { get; private set; }
        public MethodInfo Method { get; private set; }
        public RegistratorFailureType FailureType { 
            get{
                return this.failureType;
            }
            set{
                /* Bad template or lower errors can't be fixed on runtime */
                if(this.FailureType <= RegistratorFailureType.BadRegistratorTemplate && value > RegistratorFailureType.BadRegistratorTemplate){
                    value = RegistratorFailureType.BadRegistratorTemplate;
                }

                this.failureType = value;
            } 
        }
        public Exception Exception { get; set; }
        public bool Failed
        {
            get
            {
                return this.FailureType != RegistratorFailureType.Success;
            }
        }
        public Type[] DependsOn { get; private set; }
        public Type[] Implements { get; private set; }
        public bool IsRootRegistrator
        {
            get
            {
                return DependsOn.Length == 0;
            }
        }
        public bool IsRequired { get; private set; }
        public InstanceCount InstanceCount { get; private set; }

        public Registrator(MethodInfo method)
        {
            this.Method = method;
            this.failureType = RegistratorFailureType.Success;
            this.Exception = null;
            this.PluginType = method.DeclaringType;
            this.Implements = method.DeclaringType.GetInterfaces();

            PlugInAttribute PlugInAttribute = (PlugInAttribute)Attribute.GetCustomAttribute(this.PluginType, typeof(PlugInAttribute)) ?? new PlugInAttribute();
            
            this.InstanceCount = PlugInAttribute.InstanceCount;

            

            try
            {
                if (method.IsPublic && !method.IsStatic && method.ReturnType == typeof(void))
                {
                    ParameterInfo[] parameters = method.GetParameters();

                    List<string> failureMessages = new List<string>();

                    this.DependsOn = new Type[parameters.Length];
                    int i = 0;
                    foreach (ParameterInfo parameter in parameters)
                    {
                        bool subclass = typeof(IPlugInInterface).IsAssignableFrom(parameter.ParameterType);
                        if (parameter.IsIn || parameter.IsOut || parameter.IsRetval || !typeof(IPlugInInterface).IsAssignableFrom(parameter.ParameterType))
                        {
                            failureMessages.Add("Incorrect Register parameter " + i + " " + parameter.ToString());
                        }

                        /* No two dependencies must be of the same type */
                        foreach(Type dependency in this.DependsOn){
                            if(dependency == parameter.ParameterType){
                                failureMessages.Add("Repeating parameter " + parameter.ToString() + ", two or more parameters of the same type are not allowed");
                            }
                        }
                        
                        this.DependsOn[i] = parameter.ParameterType;

                            //pluginGraph.Add(parameter.ParameterType, method);
                            /*List<MethodInfo> dependentMethods;
                            if (registratorsByDependency.TryGetValue(parameter.ParameterType, out dependentMethods))
                            {
                                // one method cannot depend on two interfaces of the same type
                                if (dependentMethods.IndexOf(method) == -1)
                                {
                                    dependentMethods.Add(method);
                                }
                                else
                                {
                                    Console.WriteLine(method.ToString() + " depends on one interface twice");
                                    breakRequested = true;
                                    break;
                                }

                            }
                            else
                            {
                                dependentMethods = new List<MethodInfo>();
                                dependentMethods.Add(method);
                                registratorsByDependency.Add(parameter.ParameterType, dependentMethods);
                            }*/
     
                        /*else
                        {
                            throw new InvalidOperationException("Incorrect Register parameter " + parameter.ToString());
                        }*/

                        i++;
                    }

                    if(failureMessages.Count > 0){
                        throw new InvalidOperationException(String.Join(", ", failureMessages));
                    }
                }
                else
                {
                    throw new InvalidOperationException("Incorrect Register template");
                }
            }
            catch(Exception e){
                this.FailureType = RegistratorFailureType.BadRegistratorTemplate;
                this.Exception = e;
            }
        }               

        public override string ToString()
        {
            string s = this.PluginType.Name + ".Register(";
            
            if (this.DependsOn != null)
            {
                string[] parameters = new string[this.DependsOn.Length];

                int i = 0;
                foreach (Type dependency in this.DependsOn)
                {
                    parameters[i] = dependency.ToString();
                    i++;
                }

                s += String.Join(", ", parameters);
            }

            return s + ")";
        }
    }
}
