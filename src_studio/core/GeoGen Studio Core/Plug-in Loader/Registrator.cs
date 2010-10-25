using System;
using System.Collections.Generic;
using System.Text;
using System.Reflection;

namespace GeoGen.Studio.PlugInLoader
{
    /// <summary>
    /// Types of <see cref="Registrator"/> failure causes.
    /// </summary>
    public enum RegistratorFailureType{
        /// <summary>
        /// The <see cref="Registrator"/> has incorrect modifier, return type or parameter.
        /// </summary>
        BadRegistratorTemplate = 0,
        /// <summary>
        /// One or more of of the <see cref="Registrator"/>'s <see cref="Registrator.DependsOn">dependencies</see> are not implemented by any other plug-in.
        /// </summary>
        UnimplementedInterface = 1,
        /// <summary>
        /// The <see cref="Registrator"/> depends either directly or indirectly on itself and prevents the plug-in graph from being resolved.
        /// </summary>
        CyclicDependency = 2,
        /// <summary>
        /// Constructor of the <see cref="Registrator"/>'s defining plug-in has thrown an exception.
        /// </summary>
        ExceptionInConstructor = 3,
        /// <summary>
        /// The <see cref="Registrator"/> has thrown an exception.
        /// </summary>
        ExceptionInRegistrator = 5,
        /// <summary>
        /// One or more of the <see cref="Registrator"/>'s <see cref="Registrator.DependsOn">dependencies</see> have failed with an exception before the current <see cref="Registrator"/> could be called.
        /// </summary>
        ExceptionInDependency = 6,
        /// <summary>
        /// The <see cref="Registrator"/> has not failed yet (this is the only non-failure state).
        /// </summary>
        Success = 7,
    }

    /// <summary> 
    ///    Represents one plug-in registrator. 
    ///    <threadsafety static="true" instance="false"/> 
    /// </summary> 
    /// <remarks>
    ///     Registrator represents one <c>Register()</c> method of an plug-in. Each registrator is executed on all 
    ///     possible combinations of valid input parameters during <see cref="GeoGen.Studio.App"/> startup.    
    /// </remarks>
    public sealed class Registrator
    {
        private RegistratorFailureType failureType;

        /// <summary>
        ///    Type of the plug-in defining the current <see cref="Registrator"/>.         
        /// </summary>
        public Type PluginType { get; private set; }

        /// <summary>
        ///    The <see ctype="Registrator"/> method object.         
        /// </summary>
        public MethodInfo Method { get; private set; }

        /// <summary>
        ///    If the <see cref="Registrator"/> failed, this details reason of the failure. Otherwise this is <see cref="RegistratorFailureType.Success"/>.
        /// </summary>        
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

        /// <summary>
        ///    The exception which caused failure of the current <see cref="Registrator"/> (if any).
        /// </summary>     
        public Exception Exception { get; set; }

        /// <summary>
        ///    True if the <see cref="Registrator"/> failed, otherwise false.
        /// </summary>    
        public bool Failed
        {
            get
            {
                return this.FailureType != RegistratorFailureType.Success;
            }
        }

        /// <summary>
        ///    List of <see cref="IPlugInInterface"/> types on which the current depends.
        /// </summary>
        public Type[] DependsOn { get; private set; }

        /// <summary>
        ///    List of <see cref="IPlugInInterface"/> types implemented by the current <see cref="Registrator"/>'s defining plug-in.
        /// </summary>
        public Type[] Implements { get; private set; }

        /// <summary>
        ///    True if the <see ctype="Registrator"/> has no <see cref="Registrator.DependsOn">dependencies</see>, otherwise false.
        /// </summary>
        public bool IsRootRegistrator
        {
            get
            {
                return DependsOn.Length == 0;
            }
        }

        /// <summary>
        ///    Specifies whether creation of multiple plug-in instances is allowed in case the <see cref="Registrator"/> is called with multiple parameter combinations.
        /// </summary>
        public InstanceCount InstanceCount { get; private set; }

        /// <summary>
        /// Initializes a new instance of the <see cref="Registrator"/> class.
        /// </summary>
        /// <param name="method">The <c>Register()</c> method to be represented by the registrator.</param>
        public Registrator(MethodInfo method)
        {
            this.Method = method;
            this.failureType = RegistratorFailureType.Success;
            this.Exception = null;
            this.PluginType = method.DeclaringType;
            this.Implements = method.DeclaringType.GetInterfaces();

            /* Use thee attribute defined by the plug-in (or default one if none is defined). */
            PlugInAttribute PlugInAttribute = Attribute.GetCustomAttribute(this.PluginType, typeof(PlugInAttribute)) as PlugInAttribute ?? new PlugInAttribute();

            this.InstanceCount = PlugInAttribute.InstanceCount;

            try
            {
                /* Only methods like "public void Registrator(...)" are allowed. */
                if (method.IsPublic && !method.IsStatic && method.ReturnType == typeof(void))
                {
                    ParameterInfo[] parameters = method.GetParameters();

                    /* If an failure message is added in the process, the registrator has an incorrect template. */
                    List<string> failureMessages = new List<string>();

                    /* Check and save the list of parameters as list of dependencies. */
                    this.DependsOn = new Type[parameters.Length];
                    int i = 0;
                    foreach (ParameterInfo parameter in parameters)
                    {
                        /* Only plain parameters are allowed. Also, all parameters must be an interface descendant of IPlugInInterface. */
                        if (parameter.IsIn || parameter.IsOut || parameter.IsRetval || !typeof(IPlugInInterface).IsAssignableFrom(parameter.ParameterType) || !parameter.ParameterType.IsInterface)
                        {
                            failureMessages.Add("Incorrect Register parameter " + i + " " + parameter.ToString());
                        }

                        /* No two dependencies must be of the same type. */
                        foreach(Type dependency in this.DependsOn){
                            if(dependency == parameter.ParameterType){
                                failureMessages.Add("Repeating parameter " + parameter.ToString() + ", two or more parameters of the same type are not allowed");
                            }
                        }
                        
                        this.DependsOn[i] = parameter.ParameterType;

                        i++;
                    }
                    
                    if(failureMessages.Count > 0){
                        /* This will be catched and the registrator status will be marked as failed. */
                        throw new InvalidOperationException(String.Join(", ", failureMessages));
                    }
                }
                else
                {
                    /* This will be catched and the registrator status will be marked as failed. */
                    throw new InvalidOperationException("Incorrect Register template");
                }
            }
            catch(Exception e){
                this.FailureType = RegistratorFailureType.BadRegistratorTemplate;
                this.Exception = e;
            }
        }               

        /// <summary>
        /// Returns a <see cref="string" /> representation of the current <see cref="Registrator" />.
        /// </summary>
        /// <returns></returns>
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
