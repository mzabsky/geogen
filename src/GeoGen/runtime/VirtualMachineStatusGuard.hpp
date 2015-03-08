#pragma once

#include "VirtualMachine.hpp"

namespace geogen
{
	namespace runtime
	{
		/// Guard that sets a status of a VirtualMachine to a specified value when it is destroyed. Used mark VM as failed when native execution leaves a block prematurely, for example because an exception was thrown.
		class VirtualMachineStatusGuard
		{
		private:
			VirtualMachineStatus guardStatus;
			VirtualMachineStatus& vmStatus;
		public:

			/// Constructor.
			/// @param vmStatus Reference to a field with VM status to be set when this object is destroyed.
			VirtualMachineStatusGuard(VirtualMachineStatus& vmStatus) : guardStatus(VIRTUAL_MACHINE_STATUS_FAULTED), vmStatus(vmStatus) {}
			~VirtualMachineStatusGuard()
			{
				this->vmStatus = this->guardStatus;
			}

			/// Sets status to which the VM is set when this object is destroyed. Unless this is called, VIRTUAL_MACHINE_STATUS_FAULTED will be used.
			/// @param status The status.
			void SetGuardStatus(VirtualMachineStatus status)
			{
				this->guardStatus = status;
			}
		};
	}
}