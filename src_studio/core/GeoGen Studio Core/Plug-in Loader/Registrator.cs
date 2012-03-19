namespace GeoGen.Studio.PlugInLoader
{
    using System;
    using System.Collections.Generic;
    using System.Diagnostics;
    using System.Linq;
    using System.Reflection;

    /// <summary>
    /// Types of <see cref="Registrator"/> failure causes.
    /// </summary>
    public enum RegistratorState
    {
        Parsed,

        Validated,

        Ordered,

        InstanceConstructed,

        Executed,

        /// <summary>
        /// The <see cref="Registrator"/> has incorrect modifier, return type or parameter.
        /// </summary>
        BadRegistratorTemplate = 0,

        UnimplementedInterface = 1,

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
    /// </summary>     
    /// <remarks>
    /// Registrator represents one <c>Register()</c> method of an plug-in. Each registrator is executed on all 
    ///     possible combinations of valid input parameters during <see cref="GeoGen.Studio.App"/> startup.    
    /// </remarks>
    public sealed class Registrator : IRegistrator
    {
        private readonly List<string> errorMessages = new List<string>();
        private readonly List<Type> dependsOn = new List<Type>();
        private readonly List<Type> implementedInterfaces = new List<Type>();

        /// <summary>
        /// Initializes a new instance of the <see cref="Registrator"/> class.
        /// </summary>
        /// <param name="method">The <c>Register()</c> method to be represented by the registrator.</param>
        public Registrator(MethodInfo method)
        {
            Debug.Assert(method.DeclaringType != null, "Declaring type must not be null");

            this.Method = method;
            this.State = RegistratorState.Success;
            this.Exception = null;
            this.PluginType = method.DeclaringType;
            this.implementedInterfaces = method.DeclaringType.GetInterfaces().Where(p => typeof(IPlugInInterface).IsAssignableFrom(p)).ToList();
            this.IsRequired = Attribute.GetCustomAttribute(this.Method, typeof(OptionalRegistratorAttribute)) == null;

            /* Use thee attribute defined by the plug-in (or default one if none is defined). */
            /* PlugInAttribute plugInAttribute = Attribute.GetCustomAttribute(this.PluginType, typeof(PlugInAttribute)) as PlugInAttribute ?? new PlugInAttribute();
               this.InstanceCountModeMode = plugInAttribute.InstanceCountMode; */
            this.ParseParameters(method.GetParameters());
        }

        public IEnumerable<Type> DependsOn
        {
            get
            {
                return this.dependsOn;
            }
        }

        public IEnumerable<string> ErrorMessages
        {
            get
            {
                return this.errorMessages;
            }
        }
 
        public Exception Exception { get; set; }
 
        public bool Failed
        {
            get
            {
                return this.State != RegistratorState.Success;
            }
        }

        public IEnumerable<Type> ImplementedInterfaces
        { 
            get
            {
                return this.implementedInterfaces;
            }		
        }

        // public InstanceCountMode InstanceCountModeMode { get; private set; }
        public bool IsEnabled
        {
            get
            {
                return true; //return OldLoader.IsPlugInTypeEnabled(this.PluginType);
            }
        }

        public bool IsRequired { get; private set; }

        public bool IsRootRegistrator
        {
            get
            {
                return !this.DependsOn.Any();
            }
        }

        /// <summary>
        /// Gets the <see ctype="Registrator"/> method object.         
        /// </summary>
        public MethodInfo Method { get; private set; }

        public Type PluginType { get; private set; }
     
        public RegistratorState State { get; internal set; }

        public void Invoke(object instance, IEnumerable<object> parameters)
        {
            this.Method.Invoke(instance, parameters.ToArray());
        }

        public override string ToString()
        {
            string s = this.PluginType.Name + ".Register(";

            s += string.Join(", ", this.DependsOn.Select(t => t.Name));

            return s + ")";
        }

        /// <summary>
        /// Parses one method parameter to calculate dependenies for a registrator.
        /// </summary>
        /// <param name="parameter">The parameters.</param>
        private void ParseParameter(ParameterInfo parameter)
        {
            // Only plain parameters are allowed. Also, all parameters must be an interface descendant of IPlugInInterface.
            if (!this.IsParameterValid(parameter))
            {
                string message = "Incorrect Register parameter " + parameter;

                this.errorMessages.Add(message);
                throw new LoaderException(message);
            }

            this.dependsOn.Add(parameter.ParameterType);
        }

        /// <summary>
        /// Parses method parameters to calculate dependenies for a registrator.
        /// </summary>
        /// <param name="parameters">The parameters.</param>
        private void ParseParameters(IEnumerable<ParameterInfo> parameters)
        {
            foreach (var parameter in parameters)
            {
                this.ParseParameter(parameter);
            }
        }

        /// <summary>
        /// Determines whether parameter is valid as a registrator parameter.
        /// </summary>
        /// <param name="parameter">The parameter.</param>
        /// <returns>
        ///   <c>true</c> if parameter is valid; otherwise, <c>false</c>.
        /// </returns>
        private bool IsParameterValid(ParameterInfo parameter)
        {
            /* No parameter is allowed twice. */
            bool isRepeated =
                ((MethodInfo)parameter.Member).GetParameters().Count(p => p.ParameterType.IsAssignableFrom(parameter.ParameterType))
                > 1;
          
            return
                !isRepeated &&
                !parameter.IsIn &&
                !parameter.IsOut &&
                typeof(IPlugInInterface).IsAssignableFrom(parameter.ParameterType) &&
                parameter.ParameterType.IsInterface &&
                parameter.ParameterType != typeof(IPlugInInterface);
        }
    }
}
